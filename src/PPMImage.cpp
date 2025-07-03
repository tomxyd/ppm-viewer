#include "PPMImage.h"


int PPMImage::rgb2bgr(int r, int g, int b) {
	return (255 << 24) | (b << 16) | (g << 8) | r; // AARRGGBB format with full alpha
}

void PPMImage::ParseHeaderData(std::ifstream& file)
{
	std::string line;
	int counter = 0;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss(line);
		if (counter == 0)
		{
			std::string container;
			iss >> container;
			if (container == "P3") h._isAscii = true;
			else
			{
				throw std::runtime_error("Unsupported file format");
			}
		}
		else if (counter == 1)
			iss >> h._width >> h._height;
		else if (counter == 2)
			iss >> h._maxColorValue;
		counter++;
	}
}

void PPMImage::LoadPixels(std::ifstream& file)
{
	int r, g, b = 0;
	int index = 0;
	while (file >> r >> g >> b)
	{
		if (index >= h._width * h._height) // Ensure index does not exceed buffer size  
		{
			throw std::runtime_error("Error: Buffer overrun detected while writing to pixelBuffer.");
			file.close();
		
		}

		r = std::min(r, h._maxColorValue);
		g = std::min(g, h._maxColorValue);
		b = std::min(b, h._maxColorValue);

		pixelBuffer[index] = rgb2bgr(r, g, b);
		index++;
	}
}

const std::vector<unsigned int>& PPMImage::GetPixelBuffer()
{
	return pixelBuffer;
}