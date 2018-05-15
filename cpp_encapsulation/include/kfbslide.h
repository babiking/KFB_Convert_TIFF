// !Created by babiking@sensetime on May 13th, 2018 to process .kfb file by following openslide pipeline

#ifndef KFBSLIDE_CPP_KFBSLIDE_H
#define KFBSLIDE_CPP_KFBSLIDE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <map>
#include <iostream>

/*
 *     Preparation: declare KFB API function type and pointer
 */
typedef void* LPVOID;
typedef unsigned char BYTE;
typedef signed int KFB_INT32;

#define DLL_API extern "C"

struct kfbslide_t;
struct kfbslide_info;
struct kfbslide_ops;

extern "C"{

    // !Dynamic Link Library exposure to users...
    DLL_API kfbslide_t* kfbslide_open(const char* kfb_filename);

    DLL_API bool kfbslide_read_region_JPEG(kfbslide_t* pKFBSlide, float fScale, KFB_INT32 nPosW, KFB_INT32 nPosH, KFB_INT32 nWidth, KFB_INT32 nHeight);

    // DLL_API bool kfbslide_read_region_RAW(kfbslide_t* pKFBSlide, float fScale, KFB_INT32 nPosW, KFB_INT32 nPosH, KFB_INT32* nDataLength, KFB_INT32* nWidth, KFB_INT32* nHeight, BYTE** pBuffer);

    DLL_API bool kfbslide_close(kfbslide_t* pKFBSlide);

};





#endif //KFBSLIDE_CPP_KFBSLIDE_H
