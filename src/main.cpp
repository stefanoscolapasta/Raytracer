#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {
    //Image
    int nx = 200;
    int ny = 100;
    
    //Render
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";


    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            color pixel_color(double(i) / (nx-1), double(j) / (ny-1), 0.25);
            write_color(std::cout, pixel_color);
        }
    }
}