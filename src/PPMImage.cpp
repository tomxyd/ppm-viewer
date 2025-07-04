#include "PPMImage.h"


int PPMImage::rgb2bgr(int r, int g, int b) {
	return (255 << 24) | (b << 16) | (g << 8) | r; // AARRGGBB format with full alpha
}

void PPMImage::ParseHeaderData(std::ifstream& file)
{
    std::string line;
    int count = 0;

    while (count < 3 && std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        if (count == 0) {
            std::string magic;
            iss >> magic;
            if (magic != "P3")
                throw std::runtime_error("Unsupported format: only P3 is supported");
            h._isAscii = true;
        }
        else if (count == 1)
            iss >> h._width >> h._height;
        else if (count == 2)
            iss >> h._maxColorValue;

        count++;
    }

    if (count < 3)
        throw std::runtime_error("Incomplete");
}

void PPMImage::LoadPixels(std::ifstream& file)
{
	pixelBuffer.resize(h._width * h._height);
	int r, g, b;
	int index = 0;

	while (file >> r >> g >> b)
	{
		if (index >= pixelBuffer.size()) {
			throw std::runtime_error("Too many pixels in PPM image.");
		}

		r = std::min(r, h._maxColorValue);
		g = std::min(g, h._maxColorValue);
		b = std::min(b, h._maxColorValue);

		pixelBuffer[index++] = rgb2bgr(r, g, b);
	}

	if (index < pixelBuffer.size()) {
		std::cerr << "Warning: Not enough pixel data in file.\n";
	}
}

const std::vector<unsigned int>& PPMImage::GetPixelBuffer()
{
	return pixelBuffer;
}