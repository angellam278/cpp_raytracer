#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "utility.h"
#include <iostream> 


// Write the translated [0,255] value of each color component.
void write_color(std::ostream& out, color pixel_color) {
    // clamp the values to (0, 1)
    for (int c = 0; c < 3; c++) {
        if (pixel_color[c] > 1.0) {
            pixel_color[c] = 1.0;
        }
        if (pixel_color[c] < 0.0) {
            pixel_color[c] = 0.0;
        }
    }

    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

#endif