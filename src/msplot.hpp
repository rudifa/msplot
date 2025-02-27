#ifndef msplot_H
#define msplot_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "../../simpler_svg/src/simpler_svg.hpp"

using namespace svg;

class MSPlot
{
    struct SubplotFrame
    {
        // Default constructor
        SubplotFrame() : x_pos(0), y_pos(0), full_width(0), full_height(0) {}

        struct CurveData
        {
            std::vector<Point> data;
            std::string label;
            Color color;

            CurveData(const std::vector<double> &x,
                      const std::vector<double> &y,
                      const std::string &label = "",
                      const Color &color = Color(Color::Blue))
                : label(label), color(color)
            {
                if (x.size() != y.size())
                {
                    throw std::invalid_argument(
                        "X and Y vectors must have same size");
                }
                if (x.empty())  // Empty data vectors
                {
                    throw std::invalid_argument("Data vectors cannot be empty");
                }
                for (size_t i = 0; i < x.size(); i++)
                {
                    data.push_back(Point{x[i], y[i]});
                }
                this->label = label;
                this->color = color;
            }

            std::tuple<double, double, double, double> getMinMax() const
            {
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

                return {x_min, x_max, y_min, y_max};
            }
        };

        struct Plot
        {
            const int margin = 40;
            const int left_margin = 80;
            int x_pos;
            int y_pos;
            int width;
            int height;
            const std::vector<CurveData> &curves;

            Plot(int x, int y, int w, int h, const std::vector<CurveData> &cd)
                : x_pos(x + left_margin),
                  y_pos(y + margin),
                  width(w - left_margin - margin),
                  height(h - 2 * margin),
                  curves(cd)
            {
            }

            Group group() const
            {
                // Find min and max values for x and y
                double x_min = std::numeric_limits<double>::max();
                double x_max = std::numeric_limits<double>::lowest();
                double y_min = std::numeric_limits<double>::max();
                double y_max = std::numeric_limits<double>::lowest();

                for (const auto &curve : curves)
                {
                    auto [curve_x_min, curve_x_max, curve_y_min, curve_y_max] =
                        curve.getMinMax();
                    x_min = std::min(x_min, curve_x_min);
                    x_max = std::max(x_max, curve_x_max);
                    y_min = std::min(y_min, curve_y_min);
                    y_max = std::max(y_max, curve_y_max);
                }

                Group group;
                std::string title;
                // Add data polylines to the group
                for (const auto &curve : curves)
                {
                    Polyline polyline(Stroke(1, curve.color));
                    for (const auto &point : curve.data)
                    {
                        double scaled_x =
                            x_pos + (point.x - x_min) / (x_max - x_min) * width;
                        double scaled_y = y_pos + (point.y - y_min) /
                                                      (y_max - y_min) * height;
                        polyline << Point(scaled_x, scaled_y);
                    }
                    group << polyline;

                    title += curve.label + "  ";
                }
                group << Text(Point(x_pos + width / 2, y_pos + height + 10),
                              title, Font(14, "Arial"), Fill(Color::Black),
                              Stroke(), 0, "middle");

                // Add x and y axes
                group << Line(Point(x_pos, y_pos), Point(x_pos + width, y_pos),
                              Stroke(1, Color::Black));  // X-axis
                group << Line(Point(x_pos, y_pos + height),
                              Point(x_pos + width, y_pos + height),
                              Stroke(1, Color::Black));  // X-axis

                group << Line(Point(x_pos, y_pos), Point(x_pos, y_pos + height),
                              Stroke(1, Color::Black));  // Y-axis
                group << Line(Point(x_pos + width, y_pos),
                              Point(x_pos + width, y_pos + height),
                              Stroke(1, Color::Black));  // Y-axis

                group << xAxisTicks(x_min, x_max);
                group << yAxisTicks(y_min, y_max);

                // Add labels
                // Text x_label(Point(x_pos + width / 2, y_pos + height + 20),
                // "X-axis", Fill(Color::Black), Font(12, "Arial")); group <<
                // x_label;

                Text y_label(Point(x_pos - 50, y_pos + height / 2), "Y-axis",
                             Font(12, "Arial"), Fill(Color::Black));
                y_label.setRotation(90);
                group << y_label;

                return group;
            }

           private:
            Group xAxisTicks(double x_min, double x_max) const
            {
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
                    ticksAndValues
                        << Text(Point(x_pos_tick, y_pos - 15),
                                std::format("{:.2f}", x_val), Font(10, "Arial"),
                                Fill(Color::Black), Stroke(), 0, "middle");
                }
                return ticksAndValues;
            }
            Group yAxisTicks(double y_min, double y_max) const
            {
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
                    ticksAndValues << Text(
                        Point(x_pos - 10, y_pos_tick),
                        std::format("{:.3g}", y_val), Font(10, "Arial"),
                        Fill(Color::Black), Stroke(), 0, "end", "middle");
                    //   .setAnchor("end")
                    //   .setAlignment("middle");
                }
                return ticksAndValues;
            }
        };

        std::vector<CurveData> curves;
        int x_pos;
        int y_pos;
        int full_width;
        int full_height;

        Group group() const
        {
            Group group;

            // Create and render the Plot
            Plot plot(x_pos, y_pos, full_width, full_height, curves);
            group << plot.group();

            // Add border around the entire subplot
            group << Rectangle(Point(x_pos, y_pos), full_width, full_height,
                               Fill(), Stroke(1, Color(Color::Silver)));
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
            : width(w), height(h), document("", Layout(Size(w, h)))
        {
        }

        void addSubplot(int rows, int cols, int position)
        {
            if (position >= rows * cols)
            {
                throw std::out_of_range("Subplot position exceeds grid size");
            }

            SubplotFrame subplot;
            subplot.full_width = width / cols;
            subplot.full_height = height / rows;
            subplot.x_pos = (position % cols) * subplot.full_width;
            subplot.y_pos = (rows - 1 - position / cols) *
                            subplot.full_height;  // Flip y-axis positions
            subplots.push_back(subplot);
        }

        void plot(const std::vector<double> &x, const std::vector<double> &y,
                  const std::string &label = "",
                  const Color &color = Color(Color::Blue))
        {
            if (subplots.empty())
            {
                throw std::runtime_error(
                    "No subplot available. Call addSubplot first.");
            }
            if (x.size() != y.size())
            {
                throw std::invalid_argument(
                    "X and Y vectors must have same size");
            }
            if (x.empty())
            {
                throw std::invalid_argument("Data vectors cannot be empty");
            }

            auto &subplot = getCurrentSubplot();
            auto curveData = SubplotFrame::CurveData(x, y, label, color);
            subplot.curves.push_back(curveData);
        }

        SubplotFrame &getCurrentSubplot() { return subplots.back(); }

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
#endif  // msplot_H
