#include "Image.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <numbers>

// Reads the input file and remembers the output file.
Image::Image(std::string const & input_file, std::string const & output_file) : output(output_file) {
    std::ifstream file;
    file.open(input_file);
    file >> type;
    file >> width;
    file >> length;
    file >> limit;
    matrix.resize(length, std::vector<std::vector<int>> (width, std::vector<int> (3)));
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            for (int k = 0; k != 3; ++k) {
                file >> matrix[i][j][k];
            }
        }
    }
    file.close();
}

// Changes each color's value by hand.
void Image::manual_filter(int red_change, int green_change, int blue_change) const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            file << manual_value(matrix[i][j][0], red_change) << ' ';
            file << manual_value(matrix[i][j][1], green_change) << ' ';
            file << manual_value(matrix[i][j][2], blue_change) << '\n';
        }
    }
    file.close();
}

// Changes each color's value to its opposite.
void Image::inverse_filter() const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            file << 255 - matrix[i][j][0] << ' ';
            file << 255 - matrix[i][j][1] << ' ';
            file << 255 - matrix[i][j][2] << '\n';
        }
    }
    file.close();
}

// Changes each pixel to its respective shade of gray.
void Image::grayscale_filter() const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            int gray = multiply(matrix[i][j], grayscale_operator);
            file << gray << ' ';
            file << gray << ' ';
            file << gray << '\n';
        }
    }
    file.close();
}

// Changes each color's value based on a formula to add a reddish-brown hue.
void Image::sepia_filter() const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            int red = multiply(matrix[i][j], sepia_operator[0]);
            int green = multiply(matrix[i][j], sepia_operator[1]);
            int blue = multiply(matrix[i][j], sepia_operator[2]);
            file << std::min(red, 255) << ' ';
            file << std::min(green, 255) << ' ';
            file << std::min(blue, 255) << '\n';
        }
    }
    file.close();
}

// Changes each color's value to the maximal value among its neighbors.
void Image::dilation_filter() const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            file << dilation_value(i, j, 0) << ' ';
            file << dilation_value(i, j, 1) << ' ';
            file << dilation_value(i, j, 2) << '\n';
        }
    }
    file.close();
}

// Changes each color's value to the minimal value among its neighbors.
void Image::erosion_filter() const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            file << erosion_value(i, j, 0) << ' ';
            file << erosion_value(i, j, 1) << ' ';
            file << erosion_value(i, j, 2) << '\n';
        }
    }
    file.close();
}

// Changes each color's value to the median value among its neighbors.
void Image::median_filter() const {
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            file << median_value(i, j, 0) << ' ';
            file << median_value(i, j, 1) << ' ';
            file << median_value(i, j, 2) << '\n';
        }
    }
    file.close();
}

// Changes each color's value based on a formula to smooth or blur an image.
void Image::gaussian_filter(double sigma) const {
    update_operator(sigma);
    std::ofstream file;
    file.open(output);
    file << type << '\n';
    file << width << ' ';
    file << length << '\n';
    file << limit << '\n';
    for (int i = 0; i != length; ++i) {
        for (int j = 0; j != width; ++j) {
            update_array(i, j, 0);
            file << gaussian_value() << ' ';
            update_array(i, j, 1);
            file << gaussian_value() << ' ';
            update_array(i, j, 2);
            file << gaussian_value() << '\n';
        }
    }
    file.close();
}

// Gets the value of specific pixel's specific color.
int Image::get(int row, int column, int color) const {
    if (row >= 0 and row < length and column >= 0 and column < width) {
        return matrix[row][column][color];
    } else {
        return 0;
    }
}

// Calculates the scalar product of the given vectors.
double Image::multiply(std::vector<int> const & lhs, std::vector<double> const & rhs) const {
    double scalar_product = 0;
    for (int i = 0; i != 3; ++i) {
        scalar_product += lhs[i] * rhs[i];
    }
    return scalar_product;
}

// Increases or decreases the color value by a given number.
int Image::manual_value(int value, int change) const {
    if (change < 0) {
        return std::max(value + change, 0);
    } else {
        return std::min(value + change, 255);
    }
}

// Finds the maximal color value among the neighboring pixels.
int Image::dilation_value(int row, int column, int color) const {
    int maximal = 0;
    for (std::pair<int, int> const & shift : shifts) {
        maximal = std::max(maximal, get(row + shift.first, column + shift.second, color));
    }
    return maximal;
}

// Finds the minimal color value among the neighboring pixels.
int Image::erosion_value(int row, int column, int color) const {
    int minimal = 255;
    for (std::pair<int, int> const & shift : shifts) {
        minimal = std::min(minimal, get(row + shift.first, column + shift.second, color));
    }
    return minimal;
}

// Finds the median color value among the neighboring pixels.
int Image::median_value(int row, int column, int color) const {
    for (int i = 0; i != 9; ++i) {
        median_array[i] = get(row + shifts[i].first, column + shifts[i].second, color);
    }
    std::ranges::sort(median_array);
    return median_array[4];
}

// Finds the respective gaussian operator for the given sigma.
void Image::update_operator(double sigma) const {
    for (std::pair<int, int> const & shift : shifts) {
        gaussian_operator[shift.first + 1][shift.second + 1] = exp((shift.first * shift.first + shift.second * shift.second) / (-2 * sigma * sigma)) / (2 * std::numbers::pi * sigma * sigma);
    }
}

// Builds an array of color values from the neighboring pixels.
void Image::update_array(int row, int column, int color) const {
    for (std::pair<int, int> const & shift : shifts) {
        gaussian_array[shift.first + 1][shift.second + 1] = get(row + shift.first, column + shift.second, color);
    }
}

// Finds the sum of products of gaussian operator and array of color values.
int Image::gaussian_value() const {
    return multiply(gaussian_array[0], gaussian_operator[0]) + multiply(gaussian_array[1], gaussian_operator[1]) + multiply(gaussian_array[2], gaussian_operator[2]);
}