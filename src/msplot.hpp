#ifndef msplot_H
#define msplot_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "../../simpler_svg/src/simpler_svg.hpp"

using namespace svg;

class MSPlot
{
    struct SubplotFrame
    {
        std::vector<Point> data;
        std::string label;
        Color color;
        int x_pos;
        int y_pos;
        int full_width;
        int full_height;

        // Default constructor
        SubplotFrame()
            : data(), label(), color(Color::Black), x_pos(0), y_pos(0), full_width(0), full_height(0) {}

        struct Plot
        {
            const int margin = 40;
            const int left_margin = 80;
            int x_pos;
            int y_pos;
            int width;
            int height;
            const std::vector<Point> &data;
            const std::string &label;
            const Color &color;

            Plot(int x, int y, int w, int h, const std::vector<Point> &d, const std::string &l, const Color &c)
                : x_pos(x + left_margin), y_pos(y + margin), width(w - left_margin - margin), height(h - 2 * margin), data(d), label(l), color(c) {}

            Group group() const
            {
                Group group;

                // Find min and max values for x and y
                double x_min = std::numeric_limits<double>::max();
                double x_max = std::numeric_limits<double>::lowest();
                double y_min = std::numeric_limits<double>::max();
                double y_max = std::numeric_limits<double>::lowest();

                for (const auto &point : data)
                {
                    x_min = std::min(x_min, point.x);
                    x_max = std::max(x_max, point.x);
                    y_min = std::min(y_min, point.y);
                    y_max = std::max(y_max, point.y);
                }

                // Add data polyline
                Polyline polyline(Stroke(1, color));
                for (const auto &point : data)
                {
                    double scaled_x = x_pos + (point.x - x_min) / (x_max - x_min) * width;
                    double scaled_y = y_pos + (point.y - y_min) / (y_max - y_min) * height;
                    polyline << Point(scaled_x, scaled_y);
                }
                group << polyline;

                // Add x and y axes
                group << Line(Point(x_pos, y_pos), Point(x_pos + width, y_pos), Stroke(1, Color::Black));                   // X-axis
                group << Line(Point(x_pos, y_pos + height), Point(x_pos + width, y_pos + height), Stroke(1, Color::Black)); // X-axis

                group << Line(Point(x_pos, y_pos), Point(x_pos, y_pos + height), Stroke(1, Color::Black));                 // Y-axis
                group << Line(Point(x_pos + width, y_pos), Point(x_pos + width, y_pos + height), Stroke(1, Color::Black)); // Y-axis

                group << xAxisTicks(x_min, x_max);
                group << yAxisTicks(y_min, y_max);

                // Add labels
                // Text x_label(Point(x_pos + width / 2, y_pos + height + 20), "X-axis", Fill(Color::Black), Font(12, "Arial"));
                // group << x_label;

                Text y_label(Point(x_pos - 50, y_pos + height / 2), "Y-axis", Fill(Color::Black), Font(12, "Arial"));
                y_label.setRotation(90);
                group << y_label;

                Text title(Point(x_pos + width / 2, y_pos + height + 10), label, Fill(Color::Black), Font(14, "Arial"));
                group << title;

                return group;
            }

        private:
            Group xAxisTicks(double x_min, double x_max) const
            {
                std::cerr << "x_min: " << x_min << " x_max: " << x_max << std::endl;

                Group ticksAndValues;
                const int n = 5;
                for (int i = 0; i <= n; i++)
                {
                    double x_val = x_min + (x_max - x_min) * i / n;
                    double x_pos_tick = x_pos + width * i / n;
                    // Add tick mark and value
                    ticksAndValues << Line(Point(x_pos_tick, y_pos),
                                           Point(x_pos_tick, y_pos - 5),
                                           Stroke(1, Color::Black));
                    ticksAndValues << Text(Point(x_pos_tick, y_pos - 15),
                                           std::format("{:.2f}", x_val),
                                           Fill(Color::Black),
                                           Font(10, "Arial"));
                }
                return ticksAndValues;
            }
            Group yAxisTicks(double y_min, double y_max) const
            {
                std::cerr << "y_min: " << y_min << " y_max: " << y_max << std::endl;
                Group ticksAndValues;
                const int n = 2;
                for (int i = 0; i <= n; i++)
                {
                    double y_val = y_min + (y_max - y_min) * i / n;
                    double y_pos_tick = y_pos + height * i / n;
                    // Add tick mark and value
                    ticksAndValues << Line(Point(x_pos, y_pos_tick),
                                           Point(x_pos - 5, y_pos_tick),
                                           Stroke(1, Color::Black));
                    ticksAndValues << Text(Point(x_pos - 30, y_pos_tick),
                                           std::format("{:.3g}", y_val),
                                           Fill(Color::Black),
                                           Font(10, "Arial"));
                    //   .setAnchor("end")
                    //   .setAlignment("middle");
                }
                return ticksAndValues;
            }
        };

        Group group() const
        {
            Group group;

            // Create and render the Plot
            Plot plot(x_pos, y_pos, full_width, full_height,
                      data, label, color);
            group << plot.group();

            // Add border around the entire subplot
            group << Rectangle(Point(x_pos, y_pos),
                               full_width,
                               full_height,
                               Fill(), Stroke(1, color));
            return group;
        }
    };

public:
    class Figure
    {
        std::vector<SubplotFrame> subplots;
        int width;
        int height;
        Document document;

    public:
        Figure(int w, int h)
            : width(w), height(h),
              document("", Layout(Size(w, h)))
        {
        }

        void addSubplot(int rows, int cols, int position)
        {
            if (position >= rows * cols)
            {
                throw std::out_of_range("Subplot position exceeds grid size");
            }

            SubplotFrame plot;
            plot.full_width = width / cols;
            plot.full_height = height / rows;
            plot.x_pos = (position % cols) * plot.full_width;
            plot.y_pos = (rows - 1 - position / cols) * plot.full_height; // Flip y-axis positions
            subplots.push_back(plot);
        }

        void plot(const std::vector<double> &x, const std::vector<double> &y,
                  const std::string &label = "", const Color &color = Color(Color::Blue))
        {
            if (subplots.empty())
            {
                throw std::runtime_error("No subplot available. Call addSubplot first.");
            }
            if (x.size() != y.size())
            {
                throw std::invalid_argument("X and Y vectors must have same size");
            }
            if (x.empty())
            {
                throw std::invalid_argument("Data vectors cannot be empty");
            }

            auto &subplot = getCurrentSubplot();
            subplot.data.clear(); // Clear any existing data
            for (size_t i = 0; i < x.size(); i++)
            {
                subplot.data.push_back(Point{x[i], y[i]});
            }
            subplot.label = label;
            subplot.color = color;
        }

        SubplotFrame &getCurrentSubplot()
        {
            return subplots.back();
        }

        std::string toString()
        {
            for (const auto &subplot : subplots)
            {
                document << subplot.group();
            }

            return document.toString();
        }

        bool save(const std::string &filename)
        {
            if (subplots.empty())
            {
                std::cerr << "No plots to save" << std::endl;
            }

            std::ofstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Failed to open file: " << filename << std::endl;
                return false;
            }

            //     std::cerr << "Saving:\n"
            //               << toString()
            //    << std::endl;

            file << toString();
            file.close();
            return true;
        }
    };
};
#endif // msplot_H
