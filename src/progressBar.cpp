#include "progressBar.h"

#include <iostream>

void DrawBar(f32 t, u32 width) {
    u32 cutoff = t * width;

    std::cout << "\r[";

    for(u32 i = 0; i < width; i++)
        if (i == cutoff)
            std::cout << ">";
        else if (i < cutoff)
            std::cout << "=";
        else
            std::cout << " ";

    u32 percent = 100 * t;
    std::cout << "] - " << percent << "%" << std::flush;
}

