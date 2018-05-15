#include <iostream>
#include "kfbslide.h"

int main() {

    clock_t startTime, endTime;
    double  runTime;

    startTime = clock();

    const char* kfb_filename = "../data/test.kfb";
    kfbslide_t* slide = kfbslide_open(kfb_filename);

    bool readRoiRet = kfbslide_read_region_JPEG(slide, 20.0, 10240, 10240, 2560, 2560);

    bool closeSlideRet = kfbslide_close(slide);

    endTime = clock();
    runTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    printf("[RUNTIME]: %f seconds\n", runTime);

    return 0;

}