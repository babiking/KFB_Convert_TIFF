#include "read_RGB_from_KFB.h"



void get_dll_handle(const char *dllPath, void *handle)
{	
	handle = dlopen(dllPath, RTLD_LAZY);
    if(!handle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}



ImageInfoStruct* init_kfb_image(const char* kfbFilename, void *handle)
{	
	ImageInfoStruct* sImageInfo = (ImageInfoStruct*)malloc(sizeof(ImageInfoStruct));

	DLLInitImageFileFunc InitImageFile;

	*(void* *)&InitImageFile = dlsym(handle,"InitImageFileFunc");
	if(InitImageFile == NULL)
    {   
    	printf("%s\n", "Error: InitImageFile dlsym failed.");
        exit(EXIT_FAILURE);
    }
    else
    {
    	int initRtn = InitImageFile(sImageInfo, kfbFilename);
    	assert(initRtn == 1);
    }

    return sImageInfo;
}



HeaderInfoStruct get_header_info(ImageInfoStruct *sImageInfo, void *handle)
{
	HeaderInfoStruct headerInfo;
	DLLGetHeaderInfoFunc GetHeaderInfo;

	*(void* *)&GetHeaderInfo = dlsym(handle,"GetHeaderInfoFunc");
	if(GetHeaderInfo == NULL)
    {   
    	printf("%s\n", "Error: GetHeaderInfo dlsym failed.");
        exit(EXIT_FAILURE);
    }
    else
    {
    	int getHeaderInfoRtn = GetHeaderInfo(sImageInfo, 
    										 &(headerInfo.Height), 
    										 &(headerInfo.Width), 
    										 &(headerInfo.ScanScale), 
    										 &(headerInfo.SpendTime),
    										 &(headerInfo.ScanTime),
    										 &(headerInfo.CapRes),
    										 &(headerInfo.BlockSize));
    	assert(getHeaderInfoRtn == 1);
    }

    return headerInfo;
}



void get_roi_jpeg(ImageInfoStruct *sImageInfo, float fScale, PosIdxStruct roiPosIdx, BYTE **pRoiBuffer, KFB_INT32 *nRoiBytes, void *handle)
{
	DLLGetImageDataRoiFunc GetImageDataRoi;
	*(void* *)&GetImageDataRoi = dlsym(handle,"GetImageDataRoiFunc");
	if(GetImageDataRoi == NULL)
	{
		printf("%s\n", "Error: GetImageDataRoi dlsym failed.");
        exit(EXIT_FAILURE);
	}
	else
	{
		int getImageDataRoiRtn = GetImageDataRoi(sImageInfo, fScale, roiPosIdx.x, roiPosIdx.y, roiPosIdx.width, roiPosIdx.height, pRoiBuffer, nRoiBytes, true);
		assert(getImageDataRoiRtn == 1);
	}
}	



void write_to_jpeg(const char* jpgFilename, BYTE *pRoiBuffer, KFB_INT32 nRoiBytes)
{
	clock_t start, stop;
    double  runtime;

    start = clock();

	FILE *fp;
	fp = fopen(jpgFilename, "wb+");

	fwrite(pRoiBuffer, sizeof(BYTE), nRoiBytes, fp);
	fclose(fp);

	stop = clock();
    runtime = double(stop - start) / (CLOCKS_PER_SEC);

    printf("Elapsed time to write %s: %f seconds.\n", jpgFilename, runtime);
}


void get_rgb_stream(ImageInfoStruct *sImageInfo, float fScale, TileIdxStruct tileIdx, KFB_INT32 *nRgbBytes, BYTE **pRgbStream, void *handle)
{	
	DLLGetImageRGBDataStreamFunc GetImageRGBDataStream;
	*(void* *)&GetImageRGBDataStream = dlsym(handle,"GetImageRGBDataStreamFunc");
	if(GetImageRGBDataStream == NULL)
	{
		printf("%s\n", "Error: GetImageRGBDataStream dlsym failed.");
        exit(EXIT_FAILURE);
	}
	else
	{
		KFB_INT32* nTileWidth  = (KFB_INT32*)malloc(sizeof(KFB_INT32));
    	KFB_INT32* nTileHeight = (KFB_INT32*)malloc(sizeof(KFB_INT32));
		char* getImageRGBDataStreamRtn = GetImageRGBDataStream(sImageInfo, fScale, tileIdx.x*tileIdx.tileWidth, tileIdx.y*tileIdx.tileHeight, nRgbBytes, nTileWidth, nTileHeight, pRgbStream);
	}
}



void read_rgb_from_kfb(const char* dllPath, const char* kfbFilename, const char* savePath, PosIdxStruct roiPosIdx, PosIdxStruct tileIdx, float fScale, int mode)
{
	if (access(savePath, F_OK) == -1)
		mkdir(savePath, S_IRWXU);

	// get dll_handle of KFB_LIBS
	void *handle = NULL;
	// get_dll_handle(dllPath, handle);
	handle = dlopen(dllPath, RTLD_LAZY);
    if(!handle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

	string kfbFilenameCopy(kfbFilename);
	vector<string> pathnames = split_str(kfbFilenameCopy, "/");
	string basename = *(pathnames.end()-1);
	vector<string> extnames = split_str(basename, ".");
	const char* filename = extnames[0].c_str();

	ImageInfoStruct* sImageInfo = init_kfb_image(kfbFilename, handle);

	HeaderInfoStruct headerInfo = get_header_info(sImageInfo, handle);
	KFB_INT32 blockSize = headerInfo.BlockSize;

	switch(mode)
	{
		case 0:
		{
			// get jpg bytes stream of specific ROI
			BYTE** pRoiBuffer = (BYTE**)malloc(sizeof(BYTE*));
			int* nRoiBytes = (int*)malloc(sizeof(int));
			get_roi_jpeg(sImageInfo, fScale, roiPosIdx, pRoiBuffer, nRoiBytes, handle);

			char* jpgFilename = (char*)malloc(300*sizeof(char));
			sprintf(jpgFilename, "%s/%s_ROI_x%d_y%d_w%d_h%d.jpg", savePath, filename, roiPosIdx.x, roiPosIdx.y, roiPosIdx.width, roiPosIdx.height);
			write_to_jpeg(jpgFilename, *pRoiBuffer, *nRoiBytes);
			free(jpgFilename);
			free(pRoiBuffer);
			break;	
		}
			
		case 1:
		{	
			// get raw jpg bytes of specific ROI
			TileIdxStruct tileExtIdx = {tileIdx.x / blockSize, tileIdx.y / blockSize, blockSize, blockSize, blockSize, blockSize};

			BYTE** pRgbStream = (BYTE**)malloc(sizeof(BYTE*));
			int* nRgbBytes = (int*)malloc(sizeof(int));
			get_rgb_stream(sImageInfo, fScale, tileExtIdx, nRgbBytes, pRgbStream, handle);

			char* tiffFilename = (char*)malloc(300*sizeof(char));
			sprintf(tiffFilename, "%s/%s_ROI_x%d_y%d_w%d_h%d.tiff", savePath, filename, roiPosIdx.x, roiPosIdx.y, roiPosIdx.width, roiPosIdx.height);
			tileExtIdx.x = 0;
			tileExtIdx.y = 0;
			write_rgb_to_tiff(tiffFilename, *pRgbStream, *nRgbBytes, tileExtIdx);
			free(tiffFilename);
			free(pRgbStream);
			break;
		}

		case 2:
		{
			break;
		}

		default:
		{
			break;
		}
	}

	DLLUnInitImageFileFunc UnInitImageFile;
	*(void* *)&UnInitImageFile = dlsym(handle, "UnInitImageFileFunc");
    UnInitImageFile(sImageInfo);

}		
