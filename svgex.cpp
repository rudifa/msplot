#include "svg.hpp"
#include <iostream>
#include <fstream>
#include "svgex.h"

/**
 * @brief Demonstrates the creation of various SVG elements and saves them to a file.
 *
 * This function creates an SVG object and adds different shapes (rectangle, circle, line, text, polygon)
 * to it. It also applies CSS styles, converts the SVG to a string, and saves it to a file named "example.svg".
 * The function provides console output at each step of the process.
 *
 * @return void
 *
 * @note This function will attempt to create a file named "example.svg" in the current directory.
 *       Ensure that the program has write permissions in the current directory.
 */
void svg_example()
{
    // std::cout << "Starting SVG creation..." << std::endl;

    // Create an SVG object
    SVG::SVG svg({{"width", "400"}, {"height", "300"}, {"xmlns", "http://www.w3.org/2000/svg"}});
    // std::cout << "SVG object created." << std::endl;

    // Add a rectangle
    svg.add_child<SVG::Rect>(10, 10, 100, 50)
        ->set_attr("fill", "blue")
        .set_attr("stroke", "black")
        .set_attr("stroke-width", "2");
    // std::cout << "Rectangle added." << std::endl;

    // Add a circle
    svg.add_child<SVG::Circle>(200, 100, 40)
        ->set_attr("fill", "red")
        .set_attr("stroke", "black")
        .set_attr("stroke-width", "2");
    // std::cout << "Circle added." << std::endl;

    // Add a line
    svg.add_child<SVG::Line>(50, 200, 350, 200)
        ->set_attr("stroke", "green")
        .set_attr("stroke-width", "4");
    // std::cout << "Line added." << std::endl;

    // Add text
    svg.add_child<SVG::Text>(150, 280, "SVG Example")
        ->set_attr("font-family", "Arial")
        .set_attr("font-size", "24")
        .set_attr("fill", "black");
    // std::cout << "Text added." << std::endl;

    // Add a polygon
    std::vector<SVG::Point> points = {{300, 50}, {350, 100}, {300, 150}, {250, 100}};
    svg.add_child<SVG::Polygon>(points)
        ->set_attr("fill", "yellow")
        .set_attr("stroke", "black")
        .set_attr("stroke-width", "2");
    // std::cout << "Polygon added." << std::endl;

    // Add some CSS styles
    svg.style(".shape").set_attr("fill", "none").set_attr("stroke", "black");
    svg.style(".text").set_attr("font-family", "Arial").set_attr("font-size", "24px");
    // std::cout << "CSS styles added." << std::endl;

    // Convert SVG to string
    // std::cout << "Converting SVG to string..." << std::endl;
    // std::string optional_decls = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    //                              "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
    //                              "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    std::string svg_string = std::string(svg);
    // std::cout << "SVG converted to string." << std::endl;

    // Save SVG to file
    // std::cout << "Saving SVG to file..." << std::endl;
    std::ofstream out_file("example.svg");
    if (out_file.is_open())
    {
        out_file << svg_string;
        out_file.close();
        std::cout << "example.svg saved" << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}
