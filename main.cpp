#include "inc/bmp.hpp"

int main() {
    BMP image;
    image.readBMP("../in");

    std::string word;
    std::cin >> word;
    image.encrypt(word);
    image.decrypt();

    image.writeBMP("../out");

    return 0;
}
