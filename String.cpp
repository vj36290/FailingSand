#include "String.hpp"
#include <iostream>
#include <sstream>

namespace String {
    std::vector<std::string> split(std::string string, char delimiter) {
        std::vector<std::string> elements;
        std::istringstream ss(string);
        std::string element;
        size_t index = 0;

        // Had to look this up since there's no string split in C++ STL, kinda wacky
        while (std::getline(ss, element, delimiter)) {
            // Don't do anything else with empty spaces.
            if (element.empty())
                continue;
            elements.push_back(element);
        }

        return elements;
    }
}