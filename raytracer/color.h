#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "utility.h"
#include <iostream>     // std::cout


void write_color(std::ostream& out, color pixel_color) {
    //"./image.ppm"
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';

    
}

#endif