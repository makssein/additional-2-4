#include "bmp.hpp"

BMP::BMP(){

}
BMP::~BMP(){
    for (int i = 0; i < m_bmpInfo.Height; i++)
        delete[] m_pixels[i];
    delete[] m_pixels;

}
void BMP::readBMP(std::string name) {
    std::ifstream in(name+".bmp", std::ios::binary);

    if(!(in.is_open())){
        std::cout << "Image not found" << std::endl;
        return;
    }

    in.read(reinterpret_cast<char*>(&m_bmpHeader), sizeof(BMPHEADER));
    in.read(reinterpret_cast<char*>(&m_bmpInfo), sizeof(BMPINFO));

    m_pixels = new Pixel * [m_bmpInfo.Height];
    for (int i = 0; i < m_bmpInfo.Height; i++)
        m_pixels[i] = new Pixel[m_bmpInfo.Width];

    for (int i = 0; i < m_bmpInfo.Height; i++){
        for (int j = 0; j < m_bmpInfo.Width; j++)
            in.read(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

        if((3 * m_bmpInfo.Width) % 4 != 0)
            for (int j = 0; j < 4 - (3*m_bmpInfo.Width) % 4; j++){
                char c;
                in.read(&c, 1);
            }
    }

}
void BMP::writeBMP(std::string name) {
    std::ofstream out(name+".bmp", std::ios::binary);

    int width = m_bmpInfo.Width;
    int height = m_bmpInfo.Height;

    m_bmpHeader_new.Type = 0x4D42;
    m_bmpHeader_new.Size = 14 + 40 + (3 * width * height);
    if (width % 4 != 0)
        m_bmpHeader_new.Size += (4 - (3 * width) % 4) * height;
    m_bmpHeader_new.OffBits = 54;
    m_bmpHeader_new.Reserved1 = 0;
    m_bmpHeader_new.Reserved2 = 0;

    out.write(reinterpret_cast<char*>(&m_bmpHeader_new), sizeof(BMPHEADER));

    m_bmpInfo_new.BitCount = 24;
    m_bmpInfo_new.ClrImportant = 0;
    m_bmpInfo_new.ClrUsed = 0;
    m_bmpInfo_new.Compression = 0;
    m_bmpInfo_new.Height = height;
    m_bmpInfo_new.Planes = 1;
    m_bmpInfo_new.Size = 40;
    m_bmpInfo_new.SizeImage = m_bmpHeader_new.Size - 54;
    m_bmpInfo_new.Width = width;
    m_bmpInfo_new.XPelsPerMeter = 0;
    m_bmpInfo_new.YPelsPerMeter = 0;

    out.write(reinterpret_cast<char*>(&m_bmpInfo_new), sizeof(BMPINFO));

    for (int i = 0; i < m_bmpInfo_new.Height; i++){
        for (int j = 0; j < m_bmpInfo_new.Width; j++)
            out.write(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

        if ((3 * m_bmpInfo_new.Width) % 4 != 0)
            for (int j = 0; j < 4 - (3*m_bmpInfo_new.Width) % 4; j++){
                char c = 0;
                out.write(&c, 1);
            }
    }
}

void BMP::inverseFilter() {
    for (int i = 0; i < m_bmpInfo.Height; i++)
        for (int j = 0; j < m_bmpInfo.Width; j++){
            m_pixels[i][j].b = - m_pixels[i][j].b;
            m_pixels[i][j].r = - m_pixels[i][j].r;
            m_pixels[i][j].g = - m_pixels[i][j].g;


        }
}

void BMP::encrypt(std::string word) {
    int space = 0;
    for(int i = 0; i < word.length(); i++){
        if(word[i] == 'A' || word[i] == 'a'){
            m_pixels[6][space + 3].r++;
            m_pixels[5][space + 2].r++;
            m_pixels[4][space + 2].r++;
            m_pixels[3][space + 1].r++;
            m_pixels[2][space + 1].r++;
            m_pixels[1][space + 0].r++;
            m_pixels[0][space + 0].r++;

            m_pixels[5][space + 4].r++;
            m_pixels[4][space + 4].r++;
            m_pixels[3][space + 5].r++;
            m_pixels[2][space + 5].r++;
            m_pixels[1][space + 6].r++;
            m_pixels[0][space + 6].r++;

            m_pixels[2][space + 2].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[2][space + 4].r++;
        }

        if(word[i] == 'B' || word[i] == 'b'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[3][space + 1].r++;

            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 3].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[0][space + 1].r++;
        }

        if(word[i] == 'C' || word[i] == 'c'){
            m_pixels[6][space + 3].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[5][space + 1].r++;
            m_pixels[4][space + 0].r++;
            m_pixels[3][space + 0].r++;
            m_pixels[2][space + 0].r++;
            m_pixels[1][space + 1].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[0][space + 3].r++;
        }

        if(word[i] == 'D' || word[i] == 'c'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[6][space + 1].r++;
            m_pixels[5][space + 2].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 2].r++;
            m_pixels[0][space + 1].r++;
        }

        if(word[i] == 'E' || word[i] == 'e'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[6][space + 3].r++;

            m_pixels[3][space + 1].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[3][space + 3].r++;

            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[0][space + 3].r++;
        }

        if(word[i] == 'F' || word[i] == 'f') {
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[6][space + 3].r++;

            m_pixels[3][space + 1].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[3][space + 3].r++;
        }

        if(word[i] == 'G' || word[i] == 'g'){
            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[1][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[2][space + 2].r++;
        }

        if(word[i] == 'H' || word[i] == 'h'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[3][space + 1].r++;
            m_pixels[3][space + 2].r++;

            m_pixels[6][space + 3].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 3].r++;
            m_pixels[0][space + 3].r++;
        }

        if(word[i] == 'I' || word[i] == 'i'){
            m_pixels[6][space].r++;
            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[6][space + 3].r++;

            m_pixels[5][space + 1].r++;
            m_pixels[4][space + 1].r++;
            m_pixels[3][space + 1].r++;
            m_pixels[2][space + 1].r++;
            m_pixels[1][space + 1].r++;

            m_pixels[0][space].r++;
            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[0][space + 3].r++;
        }

        if(word[i] == 'J' || word[i] == 'j'){
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[1][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[3][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[6][space + 3].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[6][space + 1].r++;
        }

        if(word[i] == 'K' || word[i] == 'k'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[3][space + 1].r++;
            m_pixels[2][space + 2].r++;
            m_pixels[1][space + 3].r++;
            m_pixels[0][space + 3].r++;

            m_pixels[4][space + 2].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[6][space + 3].r++;
        }

        if(word[i] == 'L' || word[i] == 'l'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[0][space + 3].r++;
        }

        if(word[i] == 'M' || word[i] == 'm'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[5][space + 1].r++;
            m_pixels[4][space + 2].r++;
            m_pixels[5][space + 3].r++;

            m_pixels[6][space + 4].r++;
            m_pixels[5][space + 4].r++;
            m_pixels[4][space + 4].r++;
            m_pixels[3][space + 4].r++;
            m_pixels[2][space + 4].r++;
            m_pixels[1][space + 4].r++;
            m_pixels[0][space + 4].r++;
        }

        if(word[i] == 'N' || word[i] == 'n'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[5][space + 1].r++;
            m_pixels[4][space + 2].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[2][space + 3].r++;

            m_pixels[6][space + 4].r++;
            m_pixels[5][space + 4].r++;
            m_pixels[4][space + 4].r++;
            m_pixels[3][space + 4].r++;
            m_pixels[2][space + 4].r++;
            m_pixels[1][space + 4].r++;
            m_pixels[0][space + 4].r++;
        }

        if(word[i] == 'O' || word[i] == 'o'){
            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;

            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;

            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;

            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 3].r++;
        }

        if(word[i] == 'P' || word[i] == 'p'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[3][space + 1].r++;
        }

        if(word[i] == 'Q' || word[i] == 'q'){
            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;

            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;

            m_pixels[1][space + 4].r++;
            m_pixels[0][space + 5].r++;

            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;

            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 3].r++;
        }

        if(word[i] == 'R' || word[i] == 'r'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;
            m_pixels[0][space].r++;

            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[3][space + 1].r++;

            m_pixels[2][space + 1].r++;
            m_pixels[1][space + 2].r++;
            m_pixels[0][space + 3].r++;
        }

        if(word[i] == 'S' || word[i] == 's'){
            m_pixels[6][space + 3].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[6][space + 1].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space + 2].r++;
            m_pixels[3][space + 1].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 3].r++;
            m_pixels[0][space + 2].r++;
            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 0].r++;
        }

        if(word[i] == 'T' || word[i] == 'T'){
            m_pixels[6][space].r++;
            m_pixels[6][space + 1].r++;
            m_pixels[6][space + 2].r++;
            m_pixels[6][space + 3].r++;

            m_pixels[5][space + 1].r++;
            m_pixels[4][space + 1].r++;
            m_pixels[3][space + 1].r++;
            m_pixels[2][space + 1].r++;
            m_pixels[1][space + 1].r++;
            m_pixels[0][space + 1].r++;
        }

        if(word[i] == 'U' || word[i] == 'u'){
            m_pixels[0][space + 1].r++;
            m_pixels[0][space + 2].r++;

            m_pixels[5][space].r++;
            m_pixels[4][space].r++;
            m_pixels[3][space].r++;
            m_pixels[2][space].r++;
            m_pixels[1][space].r++;

            m_pixels[5][space + 3].r++;
            m_pixels[4][space + 3].r++;
            m_pixels[3][space + 3].r++;
            m_pixels[2][space + 3].r++;
            m_pixels[1][space + 3].r++;
        }

        if(word[i] == 'V' || word[i] == 'v'){
            m_pixels[6][space].r++;
            m_pixels[5][space].r++;
            m_pixels[4][space + 1].r++;
            m_pixels[3][space + 1].r++;
            m_pixels[2][space + 2].r++;
            m_pixels[1][space + 2].r++;
            m_pixels[0][space + 3].r++;

            m_pixels[1][space + 4].r++;
            m_pixels[2][space + 4].r++;
            m_pixels[3][space + 5].r++;
            m_pixels[4][space + 5].r++;
            m_pixels[5][space + 6].r++;
            m_pixels[6][space + 6].r++;
        }
        space += 8;
    }
}

void BMP::decrypt() {
    for(int i = 0; i < m_bmpInfo.Height; i++)
        for(int j = 0; j < m_bmpInfo.Width; j++)
            m_pixels[i][j].r *= 50;
}


