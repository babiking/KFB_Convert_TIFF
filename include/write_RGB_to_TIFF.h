// !Created by babiking@sensetime on May 10th, 2018 to write original RGB byte stream to .tiff file...

#ifndef KFB_CONVERT_TIFF_WRITE_RGB_TO_TIFF_H
#define KFB_CONVERT_TIFF_WRITE_RGB_TO_TIFF_H

#include "tiffio.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "tiff.h"

using namespace std;

typedef struct Tile_IDX_STRUCT
{
	int x;  // start from 0
	int y;
	int tileWidth;
	int tileHeight;
	int imageWidth;    // should know the dimension before write a RGB stream into image
	int imageHeight;   // 	explicit define imageWidth and imageHeight 
}TileIdxStruct;

void write_rgb_to_tiff(const char* tiffFilename, unsigned char* pRgbStream, int nRgbBytes, TileIdxStruct tileIdx);

#endif //KFB_CONVERT_TIFF_WRITE_RGB_TO_TIFF_H
