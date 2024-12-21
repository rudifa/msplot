#ifndef PLOT_H
#define PLOT_H

#include "svg.hpp"
#include <vector>
#include <string>

// Forward declarations
#include <iosfwd>

/**
 * Class Plot creates SVG y=f(x) plots with lines and scatter points.
 * Supports multiple data series, automatic scaling, and axis drawing.
 */
class Plot
{
private:
    SVG::SVG svg;
    SVG::Group *plot_area;
    std::vector<std::vector<double>> x_data;
    std::vector<std::vector<double>> y_data;
    double x_min, x_max, y_min, y_max;
    int width, height;
    int padding;

    // Color palette
    std::vector<std::string> colors = {
        "blue", "red", "green", "purple", "orange"};

    /**
     * Transforms an x-coordinate from data space to SVG space.
     * @param x The x-coordinate in data space.
     * @return The transformed x-coordinate in SVG space.
     */
    double transform_x(double x);

    /**
     * Transforms a y-coordinate from data space to SVG space.
     * @param y The y-coordinate in data space.
     * @return The transformed y-coordinate in SVG space.
     */
    double transform_y(double y);

    /**
     * Updates the data limits (min and max values) for both x and y axes.
     * @param x Vector of x-coordinates.
     * @param y Vector of y-coordinates.
     */
    void update_limits(const std::vector<double> &x, const std::vector<double> &y);

    /**
     * Draws the x and y axes with ticks and labels.
     */
    void draw_axes();

public:
    /**
     * Constructor for the Plot class.
     * @param w Width of the SVG plot (default: 800).
     * @param h Height of the SVG plot (default: 600).
     * @param pad Padding around the plot area (default: 50).
     */
    Plot(int w = 800, int h = 600, int pad = 50);

    /**
     * Adds a line plot to the SVG.
     * @param x Vector of x-coordinates.
     * @param y Vector of y-coordinates.
     * @param color Color of the line (optional).
     */
    void plot(const std::vector<double> &x, const std::vector<double> &y, std::string color = "");

    /**
     * Adds a scatter plot to the SVG.
     * @param x Vector of x-coordinates.
     * @param y Vector of y-coordinates.
     * @param color Color of the points (optional).
     */
    void scatter(const std::vector<double> &x, const std::vector<double> &y, std::string color = "");

    /**
     * Saves the SVG plot to a file.
     * @param filename Name of the file to save the SVG plot.
     */
    void save(const std::string &filename);
};

#endif // PLOT_H
