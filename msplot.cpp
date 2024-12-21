class Plot
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

        void render(SVG &svg)
        {
            // Create subplot group
            SVGElement group = svg.createGroup();

            // Add path for data
            SVGElement path = group.createPath();
            path.setStroke(color);
            path.setData(generatePathData(data));

            // Add axes
            SVGElement xAxis = group.createLine();
            SVGElement yAxis = group.createLine();

            // Add labels
            SVGElement title = group.createText(label);
            // etc...
        }
    };

    class Figure
    {
        std::vector<Subplot> subplots;
        int width;
        int height;
        SVG svg;

    public:
        Figure(int w, int h) : width(w), height(h)
        {
            svg = SVG(width, height);
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

        void render()
        {
            for (auto &subplot : subplots)
            {
                subplot.render(svg);
            }
            svg.save("plot.svg");
        }
    };
};
