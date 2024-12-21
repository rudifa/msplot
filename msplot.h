#ifndef msplot_H
#define msplot_H

#include <vector>

#include "../simple_svg/simple_svg_1.0.0.hpp"

using namespace svg;

class MSPlot
{
    struct Subplot
    {
        std::vector<Point> data;
        std::string label;
        std::string color;
        int x_pos;
        int y_pos;
        int width;
        int height;

        void render(Document &svg)
        {
            // Create subplot group
            auto group = Group();

            // Find min and max values for x and y
            double x_min = std::numeric_limits<double>::max();
            double x_max = std::numeric_limits<double>::lowest();
            double y_min = std::numeric_limits<double>::max();
            double y_max = std::numeric_limits<double>::lowest();

            for (const auto& point : data) {
                x_min = std::min(x_min, point.x);
                x_max = std::max(x_max, point.x);
                y_min = std::min(y_min, point.y);
                y_max = std::max(y_max, point.y);
            }

            // Add path for data
            Polyline polyline(Stroke(1, Color::Blue)); // TODO parametrize color
            for (const auto& point : data) {
                double scaled_x = x_pos + (point.x - x_min) / (x_max - x_min) * width;
                double scaled_y = y_pos + height - (point.y - y_min) / (y_max - y_min) * height;
                polyline << Point(scaled_x, scaled_y);
            }
            group << polyline;

            // Add axes
            group << Line(Point(x_pos, y_pos + height), Point(x_pos + width, y_pos + height), Stroke(1, Color::Black));  // X-axis
            group << Line(Point(x_pos, y_pos), Point(x_pos, y_pos + height), Stroke(1, Color::Black));  // Y-axis

            // Add labels
            Text x_label(Point(x_pos + width / 2, y_pos + height + 20), "X-axis", Fill(Color::Black), Font(12, "Arial"));
            group << x_label;

            Text y_label(Point(x_pos - 30, y_pos + height / 2), "Y-axis", Fill(Color::Black), Font(12, "Arial"), -90);
            group << y_label;

            Text title(Point(x_pos + width / 2, y_pos - 10), label, Fill(Color::Black), Font(14, "Arial"));
            group << title;

            // Add the group to the SVG document
            svg << group;
        }
    };

    public:

    class Figure
    {
        std::vector<Subplot> subplots;
        int width;
        int height;
        Document svg;

    public:
        Figure(int w, int h)
            : width(w), height(h),
              svg("", Layout(Dimensions(w, h), Layout::BottomLeft))
        {
        }

        void addSubplot(int rows, int cols, int position)
        {
            Subplot plot;
            // Calculate subplot dimensions and position
            plot.width = width / cols;
            plot.height = height / rows;
            plot.x_pos = (position % cols) * plot.width;
            plot.y_pos = (position / cols) * plot.height;
            subplots.push_back(plot);
        }

        Subplot &getCurrentSubplot()
        {
            return subplots.back();
        }

        void plot(const std::vector<double> &x, const std::vector<double> &y,
                  const std::string &label = "", const std::string &color = "blue")
        {
            auto &subplot = getCurrentSubplot();
            // Convert data to points and store
            for (size_t i = 0; i < x.size(); i++)
            {
                subplot.data.push_back({x[i], y[i]});
            }
            subplot.label = label;
            subplot.color = color;
        }

        std::string toString()
        {
            for (auto &subplot : subplots)
            {
                subplot.render(svg);
            }
            return svg.toString();
        }

        bool save(const std::string& filename)
        {
            std::ofstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << filename << std::endl;
                return false;
            }

        //     std::cerr << "Saving:\n"
        //               << toString()
        //    << std::endl;

            file << toString();
            file.close();
            std::cerr << filename << " saved successfully" << std::endl;
            return true;
        }
    };
};
#endif // msplot_H
