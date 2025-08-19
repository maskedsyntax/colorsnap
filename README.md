# ColorSnap
ColorSnap is a simple interactive color picker and identifier built with C++ and OpenCV.

The program lets you open an image, double-click anywhere on it, and instantly see:

- The closest matching color name (from a dataset)
- The RGB values of the selected pixel
- The HEX representation of the color
- An approximate accuracy score

The selected color is displayed as an overlay rectangle at the top of the image, along with its details.

This project is meant as a basic experimentation project with C++, OpenCV, and image interaction.

### Usage
```bash
./bin/colorsnap -i <image_path>
```

- `-i` or `--image` : Path to the image you want to analyze.

Once the image opens, **double-click** on any pixel to view its color information. Press `Esc` or `q` to close the program/gui.