#include "inc/bmp.hpp"

int main() {
    BMP image;
    image.readBMP("../in");

    image.inverseFilter();

    image.writeBMP("../out");

    return 0;
}
