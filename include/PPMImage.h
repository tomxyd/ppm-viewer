#pragma once  
#include <iostream>  
#include <string>  
#include <fstream>  
#include <sstream>  
#include <vector>  

struct HeaderData  
{  
bool _isAscii = false;  
int _height;  
int _width;  
int _maxColorValue;  
};  

class PPMImage  
{  
private:  
	HeaderData h;  
	std::vector<unsigned int> pixelBuffer;  

private:  
	int rgb2bgr(int r, int g, int b);  

public:   
	void ParseHeaderData(std::ifstream& file);  
	void LoadPixels(std::ifstream& file);  
	const std::vector<unsigned int>& GetPixelBuffer();  
};
