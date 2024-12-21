#ifndef msplot_H
#define msplot_H

#include <vector>

#include "../simple_svg/simple_svg_1.0.0.hpp"

using namespace svg;

// #include "svg.hpp"

class MSPlot
{
    struct Subplot
    {
        // std::vector<SVG::Point> data;
        std::string label;
        std::string color;
        int x_pos;
        int y_pos;
        int width;
        int height;

        // void render(SVG::SVG &svg)
        // {
        //     // Create subplot group
        //     SVG::Element group = svg.createGroup();

        //     // Add path for data
        //     SVG::Element path = group.createPath();
        //     path.setStroke(color);
        //     path.setData(generatePathData(data));

        //     // Add axes
        //     SVG::Element xAxis = group.createLine();
        //     SVG::Element yAxis = group.createLine();

        //     // Add labels
        //     SVG::Element title = group.createText(label);
        //     // etc...
        // }
    };

    class Figure
    {
        std::vector<Subplot> subplots;
        int width;
        int height;
        // SVG::SVG svg;

    public:
        Figure(int w, int h) : width(w), height(h)
        {
            // svg = SVG::SVG(width, height);
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

        // void plot(const std::vector<double> &x, const std::vector<double> &y,
        //           const std::string &label = "", const std::string &color = "blue")
        // {
        //     auto &subplot = getCurrentSubplot();
        //     // Convert data to points and store
        //     // for (size_t i = 0; i < x.size(); i++)
        //     // {
        //     //     subplot.data.push_back({x[i], y[i]});
        //     // }
        //     subplot.label = label;
        //     subplot.color = color;
        // }

        // void render()
        // {
        //     for (auto &subplot : subplots)
        //     {
        //         subplot.render(svg);
        //     }
        //     svg.save("plot.svg");
        // }
    };
};
#endif // msplot_H
