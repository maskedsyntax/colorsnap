#include <fmt/core.h>
#include "utils/csv_file_reader.h"
#include "utils/rgb2hex.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

struct ClickData
{
    bool clicked = false;
    cv::Point position;
    RGB color;
    cv::Mat *img = nullptr;
};

struct ColorEntry
{
    std::string color;
    std::string name;
    std::string hex;
    int r, g, b;
};

std::vector<ColorEntry> load_colors(const std::string filename)
{
    auto raw = read_csv(filename);
    std::vector<ColorEntry> dataset;

    for (const auto &row : raw)
    {
        if (row.size() < 6)
            continue;

        dataset.push_back(
            {row[0], row[1], row[2], std::stoi(row[3]), std::stoi(row[4]), std::stoi(row[5])});
    }

    return dataset;
}

std::pair<std::string, int> getColor(const RGB &rgb, std::vector<ColorEntry> &dataset)
{
    int minDist = INT_MAX;
    std::string found;
    for (const auto &entry : dataset)
    {
        int distance = abs(rgb.r - entry.r) + abs(rgb.g - entry.g) + abs(rgb.b - entry.b);
        if (distance <= minDist)
        {
            minDist = distance;
            found = entry.color;
        }
    }

    return std::make_pair(found, minDist);
}

int main()
{
    std::string image_path = "assets/images/colorpic.jpg";
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

    if (img.empty())
    {
        return -1;
    }

    auto dataset = load_colors("assets/data/colors.csv");

    cv::Mat resized_img;
    cv::Size new_size(1000, 800);

    resize(img, resized_img, new_size, 0, 0, cv::INTER_LINEAR);

    ClickData data;
    data.img = &resized_img;

    cv::namedWindow("image");

    cv::setMouseCallback(
        "image",
        [](int event, int x, int y, int flags, void *userdata)
        {
            if (event == cv::EVENT_LBUTTONDBLCLK)
            {
                auto *d = reinterpret_cast<ClickData *>(userdata);
                d->clicked = true;

                cv::Vec3b pixel = d->img->at<cv::Vec3b>(y, x); // BGR order
                std::cout << pixel << std::endl;
                d->color.b = static_cast<int>(pixel[0]);
                d->color.g = static_cast<int>(pixel[1]);
                d->color.r = static_cast<int>(pixel[2]);
            }
        },
        &data);

    for (;;)
    {
        imshow("image", resized_img);

        if (data.clicked)
        {
            cv::Point p1(200, 20);
            cv::Point p2(880, 60);
            cv::Scalar color(data.color.b, data.color.g, data.color.r); // BGR order
            cv::rectangle(resized_img, p1, p2, color, -1);

            std::pair<std::string, int> result = getColor(data.color, dataset);
            std::string colorFound = result.first;
            double accuracy = ((double)(765 - result.second) / 765) * 100;
            std::string accuracy_value = fmt::format("{:.2f}%", accuracy);

            cv::Point pntOrg(210, 45);
            std::string text = colorFound + " R=" + std::to_string(data.color.r) +
                               " G=" + std::to_string(data.color.g) +
                               " B=" + std::to_string(data.color.b) + " " + rgb2hex(data.color) +
                               " " + accuracy_value + "%";

            if (data.color.r + data.color.g + data.color.b >= 500)
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