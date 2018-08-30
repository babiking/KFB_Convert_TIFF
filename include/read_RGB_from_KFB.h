// [2018-08-30 03:06] babiking: create read_RGB_from_KFB.h

#ifndef KFB_CONVERT_TIFF_READ_RGB_FROM_KFB_H
#define KFB_CONVERT_TIFF_READ_RGB_FROM_KFB_H

#include "KFB.h"
#include "write_RGB_to_TIFF.h"
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"
#include <iostream>

using namespace std;

void read_rgb_from_kfb(const char* dllPath, const char* kfbFilename, const char* savePath, PosIdxStruct roiPosIdx, PosIdxStruct tileIdx, float fScale, int mode);

#endif
