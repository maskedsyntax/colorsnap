g++ -std=c++20 \
    src/main.cpp \
    src/utils/csv_file_reader.cpp \
    src/utils/rgb2hex.cpp \
    -Iinclude \
    -lfmt \
    -o bin/colorsnap \
    `pkg-config --cflags --libs opencv4`
