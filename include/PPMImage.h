#pragma once  
#include <iostream>  
#include <string>  
#include <fstream>  
#include <sstream>  
#include <vector>  

class HeaderData  
{  
public:
	bool _isAscii = false;  
	int _height;  
	int _width;  
	int _maxColorValue;  
};  

class PPMImage  
{  
private:  

	std::vector<unsigned int> pixelBuffer;  

private:  
	int rgb2bgr(int r, int g, int b);  

public:   
	HeaderData h;
	void ParseHeaderData(std::ifstream& file);  
	void LoadPixels(std::ifstream& file);  
	const std::vector<unsigned int>& GetPixelBuffer();  
};
