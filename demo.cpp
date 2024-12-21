#include "svg.hpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include "plot.h"
#include "svgex.h"

// Example usage
int main()
{
    // Sine wave example
    std::vector<double> x, y, y2;
    for (double i = 0; i <= 10; i += 0.1)
    {
        x.push_back(i);
        y.push_back(std::sin(i));
        y2.push_back(std::cos(i));
    }

    Plot plt;
    plt.plot(x, y, "blue");
    plt.scatter(x, y2, "red");
    plt.save("plot.svg");

    std::cerr << "plot.svg saved" << std::endl;

    // Basic SVG example
    svg_example();

    return 0;
}
