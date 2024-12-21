#include "svg.hpp"
#include "plot.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <fstream>

double Plot::transform_x(double x)
{
    return padding + (x - x_min) / (x_max - x_min) * (width - 2 * padding);
}

double Plot::transform_y(double y)
{
    return height - padding - (y - y_min) / (y_max - y_min) * (height - 2 * padding);
}

void Plot::update_limits(const std::vector<double> &x, const std::vector<double> &y)
{
    if (x_data.empty())
    {
        x_min = *std::min_element(x.begin(), x.end());
        x_max = *std::max_element(x.begin(), x.end());
        y_min = *std::min_element(y.begin(), y.end());
        y_max = *std::max_element(y.begin(), y.end());
    }
    else
    {
        x_min = std::min(x_min, *std::min_element(x.begin(), x.end()));
        x_max = std::max(x_max, *std::max_element(x.begin(), x.end()));
        y_min = std::min(y_min, *std::min_element(y.begin(), y.end()));
        y_max = std::max(y_max, *std::max_element(y.begin(), y.end()));
    }
}

void Plot::draw_axes()
{
    // X-axis
    SVG::Line x_axis(
        padding, width - padding,
        height - padding, height - padding);
    x_axis.set_attr("stroke", "black");
    plot_area->operator<<(std::move(x_axis));

    // Y-axis
    SVG::Line y_axis(
        padding, padding,
        height - padding, padding);
    y_axis.set_attr("stroke", "black");
    plot_area->operator<<(std::move(y_axis));

    // X-axis ticks and labels
    int num_x_ticks = 5;
    for (int i = 0; i <= num_x_ticks; ++i)
    {
        double x_tick_val = x_min + i * (x_max - x_min) / num_x_ticks;
        double x_pos = transform_x(x_tick_val);

        // Tick line
        SVG::Line tick(
            x_pos, x_pos,
            height - padding, height - padding + 5);
        tick.set_attr("stroke", "black");
        plot_area->operator<<(std::move(tick));

        // Tick label
        SVG::Text label(
            x_pos, height - padding + 15,
            SVG::to_string(x_tick_val));
        label.set_attr("text-anchor", "middle")
            .set_attr("font-size", "10");
        plot_area->operator<<(std::move(label));
    }

    // Y-axis ticks and labels
    int num_y_ticks = 5;
    for (int i = 0; i <= num_y_ticks; ++i)
    {
        double y_tick_val = y_min + i * (y_max - y_min) / num_y_ticks;
        double y_pos = transform_y(y_tick_val);

        // Tick line
        SVG::Line tick(
            padding - 5, padding,
            y_pos, y_pos);
        tick.set_attr("stroke", "black");
        plot_area->operator<<(std::move(tick));

        // Tick label
        SVG::Text label(
            padding - 10, y_pos,
            SVG::to_string(y_tick_val));
        label.set_attr("text-anchor", "end")
            .set_attr("alignment-baseline", "middle")
            .set_attr("font-size", "10");
        plot_area->operator<<(std::move(label));
    }
}

Plot::Plot(int w, int h, int pad)
    : x_min(std::numeric_limits<double>::max()),
      x_max(std::numeric_limits<double>::lowest()),
      y_min(std::numeric_limits<double>::max()),
      y_max(std::numeric_limits<double>::lowest()),
      width(w), height(h), padding(pad)
{

    svg.set_attr("width", std::to_string(width))
        .set_attr("height", std::to_string(height));

    plot_area = svg.add_child<SVG::Group>();
}

void Plot::plot(const std::vector<double> &x, const std::vector<double> &y, std::string color)
{
    if (x.size() != y.size())
    {
        throw std::invalid_argument("X and Y vectors must be same length");
    }

    // Update data limits
    update_limits(x, y);
    x_data.push_back(x);
    y_data.push_back(y);

    // Choose color
    if (color.empty())
    {
        color = colors[x_data.size() % colors.size()];
    }

    // Draw line segments
    for (size_t i = 0; i < x.size() - 1; ++i)
    {
        double x1 = transform_x(x[i]);
        double y1 = transform_y(y[i]);
        double x2 = transform_x(x[i + 1]);
        double y2 = transform_y(y[i + 1]);

        SVG::Line line(x1, x2, y1, y2);
        line.set_attr("stroke", color)
            .set_attr("stroke-width", "2");
        plot_area->operator<<(std::move(line));
    }
}

void Plot::scatter(const std::vector<double> &x, const std::vector<double> &y, std::string color)
{
    if (x.size() != y.size())
    {
        throw std::invalid_argument("X and Y vectors must be same length");
    }

    // Update data limits
    update_limits(x, y);
    x_data.push_back(x);
    y_data.push_back(y);

    // Choose color
    if (color.empty())
    {
        color = colors[x_data.size() % colors.size()];
    }

    // Draw points
    for (size_t i = 0; i < x.size(); ++i)
    {
        double px = transform_x(x[i]);
        double py = transform_y(y[i]);

        SVG::Circle point(px, py, 3);
        point.set_attr("fill", color)
            .set_attr("stroke", "black")
            .set_attr("stroke-width", "1");
        plot_area->operator<<(std::move(point));
    }
}

void Plot::save(const std::string &filename)
{
    // Draw axes after all data has been added
    draw_axes();

    // Set viewBox to ensure proper scaling
    std::stringstream viewbox;
    viewbox << "0 0 " << width << " " << height;
    svg.set_attr("viewBox", viewbox.str());

    // Save the SVG
    std::ofstream file(filename);
    file << std::string(svg);
    file << std::endl;
    file.close();
}
