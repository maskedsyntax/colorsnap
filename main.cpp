#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "csv_file_reader.cpp"
#include "rgb2hex.cpp"

int b, g, r;

struct ClickData
{
    bool clicked = false;
    cv::Mat *img = nullptr;
};

std::pair<std::string, std::int32_t>
getColor(int32_t r, int32_t g, int32_t b, std::vector<std::vector<std::string>> &dataset)
{
    /*
        0 -> color
        1 -> color_name
        2 -> hex
        3 -> R
        4 -> G
        5 -> B
    */
    int minimum = INT_MAX;
    std::string colorFound;
    for (int i = 0; i < dataset.size(); ++i)
    {
        int distance = abs(r - std::stoi(dataset[i][3])) + abs(g - std::stoi(dataset[i][4])) + abs(b - std::stoi(dataset[i][5]));
        if (distance <= minimum)
        {
            minimum = distance;
            colorFound = dataset[i][1];
        }
    }

    return std::make_pair(colorFound, minimum);
}

int main()
{
    std::string image_path = "colorpic.jpg";
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

    std::vector<std::vector<std::string>> dataset = read_csv("colors.csv");

    /*
    // Print the loaded data
    for (const auto &r : dataset)
    {
        for (const auto &c : r)
        {
            std::cout << c << "\t";
        }
        std::cout << "\n";
    }
    */

    if (img.empty())
    {
        return -1;
    }

    cv::Mat resized_img;
    cv::Size new_size(1000, 800);

    resize(img, resized_img, new_size, 0, 0, cv::INTER_LINEAR);

    ClickData data;
    data.img = &resized_img;

    cv::namedWindow("image");

    cv::setMouseCallback("image", [](int event, int x, int y, int flags, void *userdata)
                         {
        if (event == cv::EVENT_LBUTTONDBLCLK) {
            auto* d = reinterpret_cast<ClickData*>(userdata);
            d->clicked = true;

            cv::Vec3b pixel = d->img->at<cv::Vec3b>(y, x); // BGR order
            std::cout << pixel << std::endl;
            b = static_cast<int>(pixel[0]);
            g = static_cast<int>(pixel[1]);
            r = static_cast<int>(pixel[2]);

        } }, &data);

    while (1)
    {
        imshow("image", resized_img);

        if (data.clicked)
        {
            cv::Point p1(200, 20);
            cv::Point p2(880, 60);
            cv::Scalar color(b, g, r);
            cv::rectangle(resized_img, p1, p2, color, -1);

            std::pair<std::string, std::int32_t> result = getColor(r, g, b, dataset);
            std::string colorFound = result.first;
            int accuracy = ((double)(765 - result.second) / 765) * 100;

            cv::Point pntOrg(210, 45);
            std::string text = colorFound + "R=" + std::to_string(r) + " G=" + std::to_string(g) + " B=" + std::to_string(b) + " " + rgb2hex(r, g, b) + " " + std::to_string(accuracy) + "%";
            // cv::Scalar color;
            if (r + g + b >= 500)
            {
                color = cv::Scalar(0, 0, 0);
                cv::putText(resized_img, text, pntOrg, 2, 0.6, color, 1, cv::LINE_AA);
            }
            else
            {
                color = cv::Scalar(255, 255, 255);
                cv::putText(resized_img, text, pntOrg, 2, 0.6, color, 1, cv::LINE_AA);
            }

            data.clicked = false;
        }

        int key = cv::waitKey(20);
        if (key == 27 || key == 'q')
        {
            break;
        }
    }

    cv::destroyAllWindows();

    return 0;
}