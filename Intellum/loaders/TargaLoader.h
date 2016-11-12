#ifndef _TARGALOADER_H_
#define _TARGALOADER_H_

#include <d3d11.h>
#include <stdio.h>

#include "../error_handling/Exception.h"

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

static class TargaLoader
{
private:
	static FILE* OpenFile(char* filename);
	static void CloseFile(FILE* filePointer, char* filename);
	
	static TargaHeader GetFileHeaderInformation(FILE* filePointer, char* filename);
	static bool CheckTarga32BitsPerPixel(TargaHeader headerInfo);
	static unsigned char* ReadTargaImageData(FILE* filePointer, int imageSize, char* filename);
	static unsigned char* ReverseTargaData(unsigned char* rawTargaData, int imageSize, int width, int height);
public:
	static unsigned char* LoadTarga(char* filename, int& height, int& width);
};

#endif