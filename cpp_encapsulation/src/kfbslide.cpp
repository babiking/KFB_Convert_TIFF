// !Created by babiking@sensetime on May 13th, 2018 to process .kfb file by following openslide pipeline

/*
 *  Step 1: Define struct openslide_t as Base DataType of openslide image
 *
 *  Step 2: Implement openslide_open function to open .tiff/.svs/... files or a directory including these files
 *
 *  Step 3: Implement read_region function to extract arbitrary ROI within the openslide image
 *
 *  Step 4: Implement openslide_close function
 *
 */
#include "kfbslide.h"


/*
 *  Step 1: Define struct openslide_t as Main Structure
 *
 *  Step 2: Implement openslide_open function to open .tiff/.svs/... files or a directory including these files
 *
 *  Step 3: Implement read_region function to extract arbitrary ROI within the openslide image
 *
 *  Step 4: Implement openslide_close function
 *
 */



/*
 *     Module 1: declare openslide main structure
 */
typedef struct IMAGE_INFO_STRUCT{

    // !Linux Version
    int DataFilePTR;

    // !Windows Version
    // LPARAM DataFilePTR;

} ImageInfoStruct;

struct kfbslide_info {

    KFB_INT32 khiImageWidth;
    KFB_INT32 khiImageHeight;
    KFB_INT32 khiScanScale;
    float     khiSpendTime;
    double    khiScanTime;
    float     khiImageCapRes;
    KFB_INT32 khiImageBlockSize;

    KFB_INT32 nDataLength;

};

struct kfbslide_ops{
    // bool (*paint_region)(kfbslide_t* osr, KFB_INT32 posW, KFB_INT32 posH, KFB_INT32 fScale, KFB_INT32 width, KFB_INT32 height);
    // void (*destroy)(kfbslide_t* osr);
};

struct kfbslide_t {

    const struct kfbslide_ops  ops;
    struct kfbslide_info       infos;
    ImageInfoStruct            sImageInfo;
    BYTE*                      pImageData;

};




/*
 *      Module 2: declare the kfbslide image operations
 */
#define CHECK_NULL(ptr) if(!ptr) return false;

class kfbslideLib {

    // !Single-instance reference: https://blog.csdn.net/hackbuteer1/article/details/7460019
    // !Copy_and_Assign reference: https://blog.csdn.net/zcyzsy/article/details/52132936

        protected:

            /*
             *  Function:
             *      InitImageFileFunc(ImageInfoStruct* sImageInfo,
             *                        const char*      path_to_kfb_file), i.e. DLLInitImageFileFunc- 'abstract' function pointer
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

            /*
             *  Function:
             *      int GetHeaderInfoFunc(ImageInfoStruct* sImageInfo,
             *                            KFB_INT32        khiImageHeight,
             *                            KFB_INT32        khiImageWidth,
             *                            KFB_INT32        khiScanScale,
             *                            float*           khiSpendTime,
             *                            float*           khiImageCapRes,
             *                            KFB_INT32        khiImageBlockSize)
             *  Input:
             *      [1] <ImageInfoStruct*> sImageInfo: pointer to input digital image
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

            /*
             *  Function:
             *      int GetImageDataRoiFunc(ImageInfoStruct* sImageInfo,
             *                              float            fScale,
             *                              int              sp_x,
             *                              int              sp_y,
             *                              int              nWidth,
             *                              int              nHeight,
             *                              BYTE**           pBuffer,
             *                              int*             DataLength,
             *                              bool             flag)
             *  Input:
             *      [1] <ImageInfoStruct*> sImageInfo: pointer to input digital image
             *      [2] <float>            fScale:     target scaling/resampling factor
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

            /*
             *  Function:
             *      char* GetImageStreamFunc(ImageInfoStruct* sImageInfo,
             *                               float            fScale,
             *                               int              nImagePosX,
             *                               int              nImagePosY,
             *                               int*             nDataLength,
             *                               unsigned char**  ImageStream)
             *  Input:
             *      [1] <ImageInfoStruct*> sImageInfo
             *      [2] <float>            fScale
             *      [3] <int>              nImagePosX
             *      [4] <int>              nImagePosY
             *  Output:
             *      [1] <int*>             nDataLength
             *      [2] <BYTE**>           ImageStream
             *
             */
            typedef char* (*DLLGetImageStreamFunc)(ImageInfoStruct*, float, KFB_INT32, KFB_INT32, KFB_INT32*, BYTE**);

            /*
             *  Function:
             *      char* GetImageRGBDataStreamFunc(ImageInfoStruct* sImageInfo,
             *                                      float fScale,
             *                                      int   nImagePosX,
             *                                      int   nImagePosY,
             *                                      int*  nDataLength,
             *                                      int*  nImageWidth,
             *                                      int*  nImageHeight,
             *                                      BYTE** ImageStream)
             *  Input:
             *      [1] <ImageInfoStruct*> sImageInfo
             *      [2] <float>            fScale
             *      [3] <int>              nImagePosX
             *      [4] <int>              nImagePosY: nImagePosX and nImagePosY should be the multiple of 256
             *      [5] <int*>             nImageWidth
             *      [6] <int*>             nImageHeight
             *  Output:
             *      [1] <int*>            nDataLength
             *      [2] <BYTE**>          ImageStream: the size of ImageStream is related to khiImageBlockSize e.g. 256
             *
             */
            typedef char* (*DLLGetImageRGBDataStreamFunc)(ImageInfoStruct*, float, KFB_INT32, KFB_INT32, KFB_INT32*, KFB_INT32*, KFB_INT32*, BYTE**);

            /*
             *  Function:
             *      int DeleteImageDataFunc(LPVOID pImageData)
             *  Input:
             *      [1] <LPVOID> pImageData, i.e. the pointer of image block got by GetImageStreamFunc
             *
             */
            typedef int (*DLLDeleteImageDataFunc)(LPVOID);

            /*
             *  Function:
             *      int UnInitImageFileFunc(ImageInfoStruct* sImageInfo)
             *  Input:
             *      [1] <ImageInfoStruct*> sImageInfo
             *
             */
            typedef int (*DLLUnInitImageFileFunc)(ImageInfoStruct*);

        protected:
                DLLInitImageFileFunc         m_InitImageFile;
                DLLGetHeaderInfoFunc         m_GetHeaderInfo;
                DLLGetImageDataRoiFunc       m_GetImageDataRoi;
                DLLGetImageStreamFunc        m_GetImageStream;
                DLLGetImageRGBDataStreamFunc m_GetImageRGBDataStream;
                DLLDeleteImageDataFunc       m_DeleteImageData;
                DLLUnInitImageFileFunc       m_UnInitImageFile;





        private:

            kfbslideLib():m_kfbHandle(NULL){};
            static kfbslideLib* m_pInstance;

            // !Instance Garbo to destroy m_pInstance at the end of program
            class KGarbo{
            public:
                ~KGarbo(){
                        if(kfbslideLib::m_pInstance)
                                delete kfbslideLib::m_pInstance;
                }
            };

            // !Define a static member variable, at the end of program,
            //     Garbo.~KGarbo() will be automatically called
            static KGarbo Garbo;

            void* m_kfbHandle;



        public:

                ~kfbslideLib(){
                        if(m_kfbHandle){
                                dlclose(m_kfbHandle);         // Linux
                                // FreeLibrary(m_kfbHandle);  // Windows
                        }
                }            
    
                static kfbslideLib* m_GetInstance(){
                    if(m_pInstance == NULL)
                            m_pInstance = new kfbslideLib();
                    return m_pInstance;
                } // close m_GetInstance()

        
                bool m_Initialize(){

                        m_kfbHandle = dlopen("../lib/libImageOperationLib.so", RTLD_LAZY);
                        if(!m_kfbHandle){
                                fprintf(stderr, "%s\n", dlerror());
                                // exit(EXIT_FAILURE);
                                return false;
                        }

                        dlerror();

                        // !Get function pointer of InitImageFile
                        m_InitImageFile = (DLLInitImageFileFunc)dlsym(m_kfbHandle, "InitImageFileFunc");
                        CHECK_NULL(m_InitImageFile);

                        m_GetHeaderInfo = (DLLGetHeaderInfoFunc)dlsym(m_kfbHandle, "GetHeaderInfoFunc");
                        CHECK_NULL(m_GetHeaderInfo);

                        m_GetImageDataRoi = (DLLGetImageDataRoiFunc)dlsym(m_kfbHandle, "GetImageDataRoiFunc");
                        CHECK_NULL(m_GetImageDataRoi);

                        m_GetImageStream = (DLLGetImageStreamFunc)dlsym(m_kfbHandle, "GetImageStreamFunc");
                        CHECK_NULL(m_GetImageStream);

                        m_GetImageRGBDataStream = (DLLGetImageRGBDataStreamFunc)dlsym(m_kfbHandle, "GetImageRGBDataStreamFunc");
                        CHECK_NULL(m_GetImageRGBDataStream);

                        m_DeleteImageData = (DLLDeleteImageDataFunc)dlsym(m_kfbHandle, "DeleteImageDataFunc");
                        CHECK_NULL(m_DeleteImageData);

                        m_UnInitImageFile = (DLLUnInitImageFileFunc)dlsym(m_kfbHandle, "UnInitImageFileFunc");
                        CHECK_NULL(m_UnInitImageFile);

                        printf("[kfbslide.so]: Success to initialize KFB library...\n");
                        return true;

                } // close _initialize()

                bool InitImageFile(ImageInfoStruct* sImageInfo, const char* kfb_filename){
                        int initRet = m_InitImageFile(sImageInfo, kfb_filename);
                        return true;
                };

                int GetHeaderInfo(ImageInfoStruct* sImageInfo, KFB_INT32* khiImageWidth, KFB_INT32* khiImageHeight, KFB_INT32* khiScanScale, float* khiSpendTime, double* khiScanTime, float* khiImageCapRes, KFB_INT32* khiImageBlockSize){
                    return m_GetHeaderInfo(sImageInfo, khiImageWidth, khiImageHeight, khiScanScale, khiSpendTime, khiScanTime, khiImageCapRes, khiImageBlockSize);
                };

                char* GetImageStream(ImageInfoStruct* sImageInfo, float fScale, KFB_INT32 nImagePosW, KFB_INT32 nImagePosH, KFB_INT32* nDataLength, BYTE** pImageStream){
                        return m_GetImageStream(sImageInfo, fScale, nImagePosW, nImagePosH, nDataLength, pImageStream);
                };

                char* GetImageRGBDataStream(ImageInfoStruct* sImageInfo, float fScale, KFB_INT32 nRgbPosW, KFB_INT32 nRgbPosH, KFB_INT32* nRgbDataLength, KFB_INT32* nRgbWidth, KFB_INT32* nRgbHeight, BYTE** pRgbStream){
                        return m_GetImageRGBDataStream(sImageInfo, fScale, nRgbPosW, nRgbPosH, nRgbDataLength, nRgbWidth, nRgbHeight, pRgbStream);
                };

                int GetImageDataRoi(ImageInfoStruct* sImageInfo, float fScale, KFB_INT32 nRoiPosW, KFB_INT32 nRoiPosH, KFB_INT32 nRoiWidth, KFB_INT32 nRoiHeight, BYTE** pROI, KFB_INT32* nRoiDataLength, bool roiFlag){
                        return m_GetImageDataRoi(sImageInfo, fScale, nRoiPosW, nRoiPosH, nRoiWidth, nRoiHeight, pROI, nRoiDataLength, roiFlag);
                };

                int DeleteImageData(BYTE* pBuffer){
                        return m_DeleteImageData(pBuffer);
                };

                int UnInitImageFile(ImageInfoStruct* sImageInfo){
                        return m_UnInitImageFile(sImageInfo);
                };


}; // close class lib_kfbslide

// !m_pInstance is a static member variable, whose memory is allocated at Definition phase
    // other member variables at Initialization phase
kfbslideLib* kfbslideLib::m_pInstance = NULL;


kfbslide_t* kfbslide_open(const char* kfb_filename){

    kfbslideLib* pKFBLib = kfbslideLib::m_GetInstance();
    if(!pKFBLib){
        printf("[kfbslide.so]: Error KFBSlide Library instance is NULL\n");
        return NULL;
    }

    pKFBLib->m_Initialize();

    kfbslide_t* pKFBSlide = new kfbslide_t();
    if(!(&pKFBSlide)){
        printf("[kfbslide.so]: !Fail to allocate kfbslide_t\n");
        return NULL;
    }

    ImageInfoStruct* sImageInfo = (ImageInfoStruct*)malloc(sizeof(ImageInfoStruct));
    printf("[kfbslide.so]: KFB Filename is %s\n", kfb_filename);
    if(!pKFBLib->InitImageFile(&(pKFBSlide->sImageInfo),kfb_filename)){
        printf("[kfbslide.so]: Fail to InitImageFile\n");
        delete(pKFBSlide);
        return NULL;
    }

    // !Header information of .kfb file
    /*
    KFB_INT32* khiImageWidth  = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    KFB_INT32* khiImageHeight = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    KFB_INT32* khiScanScale   = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    float*     khiSpendTime   = (float*)malloc(sizeof(float));
    double*    khiScanTime    = (double*)malloc(sizeof(double));
    float*     khiImageCapRes = (float*)malloc(sizeof(float));
    KFB_INT32* khiImageBlockSize = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    */
    int getHeaderInfoRet = pKFBLib->GetHeaderInfo(&(pKFBSlide->sImageInfo),
                                                  &(pKFBSlide->infos.khiImageWidth),
                                                  &(pKFBSlide->infos.khiImageHeight),
                                                  &(pKFBSlide->infos.khiScanScale),
                                                  &(pKFBSlide->infos.khiSpendTime),
                                                  &(pKFBSlide->infos.khiScanTime),
                                                  &(pKFBSlide->infos.khiImageCapRes),
                                                  &(pKFBSlide->infos.khiImageBlockSize));

    return pKFBSlide;
} // close kfbslide_open()



bool kfbslide_read_region_JPEG(kfbslide_t* pKFBSlide, float fScale, KFB_INT32 nPosW, KFB_INT32 nPosH, KFB_INT32 nWidth, KFB_INT32 nHeight){

    kfbslideLib* pKFBLib = kfbslideLib::m_GetInstance();
    if(!pKFBLib){
        printf("[kfbslide.so]: Error KFBSlide Library instance is NULL\n");
        return NULL;
    }

    pKFBLib->m_Initialize();

    int getImageDataRoiRet = pKFBLib->GetImageDataRoi(&(pKFBSlide->sImageInfo), fScale, nPosW, nPosH, nWidth, nHeight, &(pKFBSlide->pImageData), &(pKFBSlide->infos.nDataLength), true);


    // !Write ROI Data into .jpg file...
    FILE* f_ROI;
    f_ROI = fopen("test_ROI.jpg", "wb+");
    // printf("The data length of ROI image is: %d\n",*roiDataLength);
    fwrite( pKFBSlide->pImageData, sizeof(BYTE), pKFBSlide->infos.nDataLength, f_ROI);
    fclose( f_ROI );


    return true;


} // close kfbslide_read_region_JPEG()

/*
bool kfbslide_read_region_RAW(kfbslide_t* pKFBSlide, float fScale, KFB_INT32 nPosW, KFB_INT32 nPosH, KFB_INT32* nDataLength, KFB_INT32* nWidth, KFB_INT32* nHeight, BYTE** pBuffer){

} // close kfbslide_read_region_RAW()
*/

bool kfbslide_close(kfbslide_t* pKFBSlide){

    kfbslideLib* pKFBLib = kfbslideLib::m_GetInstance();
    if(!pKFBLib){
        printf("[kfbslide.so]: Error KFBSlide Library instance is NULL\n");
        return NULL;
    }

    pKFBLib->m_Initialize();

    int deleteImageDataRet = pKFBLib->DeleteImageData(pKFBSlide->pImageData);

    int unInitImageFileRet = pKFBLib->UnInitImageFile(&(pKFBSlide->sImageInfo));

    return true;

} // close kfbslide_close()