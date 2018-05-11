// !Created by babiking@sensetime on May 10th, 2018 for .kfb file format conversion...
//  reference: [1]https://blog.csdn.net/ameyume/article/details/6326278
//             [2]https://blog.csdn.net/loveyaqin1990/article/details/40015731

#ifndef KFB_CONVERT_KFB_H
#define KFB_CONVERT_KFB_H

#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

// !Create IMAGE_INFO_STRUCT to store pointer to file...
typedef struct IMAGE_INFO_STRUCT{

    int DataFilePTR;

}ImageInfoStruct;



typedef void *LPVOID;
typedef unsigned char BYTE;
typedef signed int KFB_INT32;



// !Define the functionType of InitImageFileFunc
/*
 *  Function:
 *      InitImageFileFunc(ImageInfoStruct sImageInfo,
 *                        const char*     path_to_kfb_file), i.e. DLLInitImageFileFunc- 'abstract' function pointer
 *  Input:
 *      [1] <const char*> path_to_kfb_file
 *  Output:
 *      [1] <ImageInfoStruct> sImageInfo: pointer to input digital image
 *  Return:
 *      [1] <int> returnFlag
 *
 *  note: usage of TYPEDEF refer to https://blog.csdn.net/ameyume/article/details/6326278...
 *
 */
typedef int (*DLLInitImageFileFunc)(ImageInfoStruct*, const char*);
// !Create a function pointer to InitImageFileFunc...
DLLInitImageFileFunc InitImageFile;



// !Define the functionType of GetHeaderInfoFunc
/*
 *  Function:
 *      int GetHeaderInfoFunc(ImageInfoStruct sImageInfo,
 *                            KFB_INT32 khiImageHeight,
 *                            KFB_INT32 khiImageWidth,
 *                            KFB_INT32 khiScanScale,
 *                            float* khiSpendTime,
 *                            float* khiImageCapRes,
 *                            KFB_INT32, khiImageBlockSize)
 *  Input:
 *      [1] <ImageInfoStruct> sImageInfo: pointer to input digital image
 *  Output:
 *      [1] <KFB_INT32> khiImageHeight: scanning height
 *      [2] <KFB_INT32> khiImageWidth:  scanning width
 *      [3] <KFB_INT32> khiScanScale:   scanning scale factor e.g. 40X
 *      [4] <float*>  khiSpendTime:  scanning acquisition time
 *      [5] <double*> khiScanTime:   scanning start time
 *      [6] <float*>  khiImageCapRes:      ratio of the pixel and actual size
 *      [7] <KFB_INT32> khiImageBlockSize: size of scanning block
 */
typedef int (*DLLGetHeaderInfoFunc)(ImageInfoStruct*, KFB_INT32*, KFB_INT32*, KFB_INT32*, float*, double*, float*, KFB_INT32*);
DLLGetHeaderInfoFunc GetHeaderInfo;



// !Create the functionType of GetImageDataRoiFunc
/*
 *  Function:
 *      int GetImageDataRoiFunc(ImageInfoStruct sImageInfo,
 *                              float           fScale,
 *                              int             sp_x,
 *                              int             sp_y,
 *                              int             nWidth,
 *                              int             nHeight,
 *                              BYTE**          pBuffer,
 *                              int*            DataLength,
 *                              bool            flag)
 *  Input:
 *      [1] <ImageInfoStruct> sImageInfo: pointer to input digital image
 *      [2] <float>           fScale:     target scaling/resampling factor
 *      [3] <int> sp_x:   x-coordinate of left-top corner
 *      [4] <int> sp_y:   y-coordinate of left-top corner
 *      [5] <int> nWidth
 *      [6] <int> nHeight
 *  Output:
 *      [1] <BYTE**> pBuffer:  pointer to return image block, format->.jpeg
 *      [2] <int*> DataLength: data length (BYTEs) of return image
 *      [3] <bool> flag:       result of GetImageDataRoiFunc, true-success/false-fail
 *
 */
typedef int (*DLLGetImageDataRoiFunc)(ImageInfoStruct*, float, KFB_INT32, KFB_INT32, KFB_INT32, KFB_INT32, BYTE**, KFB_INT32*, bool);
DLLGetImageDataRoiFunc GetImageDataRoi;




// !Create the functionType of GetRGBDataStreamFunc
/*
 *  Function:
 *      char* GetImageRGBDataStreamFunc(ImageInfoStruct sImageInfo,
 *                                      float fScale,
 *                                      int   nImagePosX,
 *                                      int   nImagePosY,
 *                                      int*  nDataLength,
 *                                      int*  nImageWidth,
 *                                      int*  nImageHeight,
 *                                      BYTE** ImageStream)
 *  Input:
 *      [1] <ImageInfoStruct> sImageInfo
 *      [2] <float>           fScale
 *      [3] <int>             nImagePosX
 *      [4] <int>             nImagePosY: nImagePosX and nImagePosY should be the multiple of 256
 *      [5] <int*>            nImageWidth
 *      [6] <int*>            nImageHeight
 *  Output:
 *      [1] <int*>            nDataLength
 *      [2] <BYTE**>          ImageStream: the size of ImageStream is related to khiImageBlockSize e.g. 256
 *
 */
typedef char* (*DLLGetImageRGBDataStreamFunc)(ImageInfoStruct*, float, KFB_INT32, KFB_INT32, KFB_INT32*, KFB_INT32*, KFB_INT32*, BYTE**);
DLLGetImageRGBDataStreamFunc GetImageRGBDataStream;




// !Create the functionType of DeleteImageDataFunc
/*
 *  Function:
 *      int DeleteImageDataFunc(LPVOID pImageData)
 *  Input:
 *      [1] <LPVOID> pImageData, i.e. the pointer of image block got by GetImageStreamFunc
 *
 */
typedef int (*DLLDeleteImageDataFunc)(LPVOID);
DLLDeleteImageDataFunc DeleteImageData;




// !Create the functionType of UnInitImageFileFunc
/*
 *  Function:
 *      int UnInitImageFileFunc(ImageInfoStruct* sImageInfo)
 *  Input:
 *      [1] <ImageInfoStruct*> sImageInfo
 *
 */
typedef int (*DLLUnInitImageFileFunc)(ImageInfoStruct*);
DLLUnInitImageFileFunc UnInitImageFile;

#endif //KFB_CONVERT_KFB_H
