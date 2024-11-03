#pragma once

#include <string>
#include <vector>

struct Image {
public:
    // Reads the input file and remembers the output file.
    Image(std::string const & input_file, std::string const & output_file);

    // Changes each color's value by hand.
    void manual_filter(int red_change, int green_change, int blue_change) const;

    // Changes each color's value to its opposite.
    void inverse_filter() const;

    // Changes each pixel to its respective shade of gray.
    void grayscale_filter() const;

    // Changes each color's value based on a formula to add a reddish-brown hue.
    void sepia_filter() const;

    // Changes each color's value to the maximal value among its neighbors.
    void dilation_filter() const;

    // Changes each color's value to the minimal value among its neighbors.
    void erosion_filter() const;

    // Changes each color's value to the median value among its neighbors.
    void median_filter() const;

    // Changes each color's value based on a formula to smooth or blur an image.
    void gaussian_filter(double sigma) const;
private:
    // Disables copy constructor for our class.
    Image(Image const & other);

    // Disables assignment operator for our class.
    Image & operator = (Image const & other);

    // Returns the respective color value of the given pixel.
    int get(int row, int column, int color) const;

    // Returns the scalar product of the given vectors.
    double multiply(std::vector<int> const & lhs, std::vector<double> const & rhs) const;

    // Changes the color value by a given number.
    int manual_value(int value, int change) const;

    // Finds the maximal color value among the neighboring pixels.
    int dilation_value(int row, int column, int color) const;

    // Finds the minimal color value among the neighboring pixels.
    int erosion_value(int row, int column, int color) const;

    // Finds the median color value among the neighboring pixels.
    int median_value(int row, int column, int color) const;

    // Builds the respective gaussian operator for the given sigma.
    void update_operator(double sigma) const;

    // Builds an array of color values from the neighboring pixels.
    void update_array(int row, int column, int color) const;

    // Makes calculations using gaussian operator and array of color values.
    int gaussian_value() const;

    std::string type;
    int length;
    int width;
    int limit;
    std::vector<std::vector<std::vector<int>>> matrix;
    std::string const output;
    inline static std::vector<std::pair<int, int>> const shifts = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    inline static std::vector<double> const grayscale_operator = {0.299, 0.587, 0.114};
    inline static std::vector<std::vector<double>> const sepia_operator = {{0.393, 0.769, 0.189}, {0.349, 0.686, 0.168}, {0.272, 0.534, 0.131}};
    inline static std::vector<int> median_array = std::vector<int> (9);
    inline static std::vector<std::vector<double>> gaussian_operator = std::vector<std::vector<double>> (3, std::vector<double> (3));
    inline static std::vector<std::vector<int>> gaussian_array = std::vector<std::vector<int>> (3, std::vector<int> (3));
};
