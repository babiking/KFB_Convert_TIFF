// !Created by babiking@sensetime on May 10th, 2018 to write original RGB byte stream to .tiff file...

#ifndef KFB_CONVERT_WRITE_RGB_TO_TIFF_H
#define KFB_CONVERT_WRITE_RGB_TO_TIFF_H

#include "tiffio.h"
#include <stdio.h>
#include <iostream>
#include "tiff.h"

using namespace std;

int _write_RGB_to_TIFF(unsigned char* RGB_stream, const char*  TIFF_filename, int nDataLength, int nHeightTiles, int nWidthTiles, int nTileHeight, int nTileWidth);

#endif //KFB_CONVERT_WRITE_RGB_TO_TIFF_H
