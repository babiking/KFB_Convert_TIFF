#include <iostream>
#include "KFB.h"
#include "write_RGB_to_TIFF.h"

int main() {

    void *handle = NULL;

    const char* kfb_filename = "../data/test.kfb";

    // !Image Pointer...
    ImageInfoStruct* sImageInfo;



    // !Load dynamic link library libImageOperationLib.so (provided by http://www.kfbio.cn/)
    handle = dlopen("../lib/libImageOperationLib.so", RTLD_LAZY);
    if(!handle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }



    // !Get function pointer e.g. InitImageFileFunc from DLL handle
    *(void* *)&InitImageFile = dlsym(handle,"InitImageFileFunc");
    if(InitImageFile == NULL)
    {   std::cout<<"!Failed to get InitImageFile function pointer..."<<std::endl;
        exit(EXIT_FAILURE);}

    sImageInfo = (ImageInfoStruct*)malloc(sizeof(ImageInfoStruct));

    int initRet = InitImageFile(sImageInfo, kfb_filename);

    if(initRet == 0){
        std::cout<<"!Failure to initialize image file..."<<std::endl;
        exit(EXIT_FAILURE);
    }



    // !Get header information
    *(void* *)&GetHeaderInfo = dlsym(handle,"GetHeaderInfoFunc");
    KFB_INT32* khiImageHeight    = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    KFB_INT32* khiImageWidth     = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    KFB_INT32* khiScanScale      = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    float*     khiSpendTime      = (float*)malloc(sizeof(float));
    double*    khiScanTime       = (double*)malloc(sizeof(double));
    float*     khiImageCapRes    =(float*)malloc(sizeof(float));
    KFB_INT32* khiImageBlockSize = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    int getHeaderInfoRet = GetHeaderInfo(sImageInfo, khiImageHeight, khiImageWidth, khiScanScale, khiSpendTime, khiScanTime, khiImageCapRes, khiImageBlockSize);



    // !Get ROI area within the .kfb image
    *(void* *)&GetImageDataRoi = dlsym(handle,"GetImageDataRoiFunc");
    float fScale = 20.0;
    KFB_INT32 ROI_x = 10240;
    KFB_INT32 ROI_y = 10240;
    KFB_INT32 nRoiHeight = 256;
    KFB_INT32 nRoiWidth  = 256;
    BYTE** pRoiBuffer = (BYTE**)malloc(sizeof(BYTE*));
    int* roiDataLength = (int*)malloc(sizeof(int));
    bool roiFlag = true;
    int getImageDataRoiRet = GetImageDataRoi(sImageInfo, fScale, ROI_x, ROI_y, nRoiWidth, nRoiHeight, pRoiBuffer, roiDataLength, roiFlag);



    // !Write ROI Data into .jpg file...
    FILE* f_ROI;
    f_ROI = fopen("test_ROI.jpg", "wb+");
    printf("The data length of ROI image is: %d\n",*roiDataLength);
    fwrite( *pRoiBuffer, sizeof(BYTE), *roiDataLength, f_ROI);
    fclose( f_ROI );



    // !Get original RGB image data stream
    *(void* *)&GetImageRGBDataStream = dlsym(handle,"GetImageRGBDataStreamFunc");
    KFB_INT32 nRgbPosX = 10240;
    KFB_INT32 nRgbPosY = 10240;
    KFB_INT32* rgbDataLength  = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    KFB_INT32* nRgbWidth  = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    KFB_INT32* nRgbHeight = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    BYTE** rgbStream = (BYTE**)malloc(sizeof(BYTE*));
    char* getImageRGBDataStreamRet = GetImageRGBDataStream(sImageInfo, fScale, nRgbPosX, nRgbPosY, rgbDataLength, nRgbWidth, nRgbHeight, rgbStream);
    printf("The data length of RGB image is: %d\n",*rgbDataLength);


    _write_RGB_to_TIFF(*rgbStream, "test.tiff", *rgbDataLength, 256, 256, *nRgbHeight, *nRgbWidth, 0);



    // !Delete the image file stream
    *(void* *)&DeleteImageData = dlsym(handle, "DeleteImageDataFunc");
    DeleteImageData(rgbStream);



    // !Close the image file
    *(void* *)&UnInitImageFile = dlsym(handle, "UnInitImageFileFunc");
    UnInitImageFile(sImageInfo);

    return 0;

}