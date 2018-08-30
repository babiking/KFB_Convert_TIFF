#include <iostream>
#include "read_RGB_from_KFB.h"
#include <gflags/gflags.h>


DEFINE_string(kfb, "path/data/test.kfb", "absolute path to .kfb file");
DEFINE_string(dll, "path/lib/libImageOperationLib.so", "path to KFB_LIBS");
DEFINE_string(save, "path/build/results", "path to save .jpg or .tiff file");
DEFINE_int32(mode, 0, "image format that .kfb file converted to: 0-jpg_roi, 1-tiff_roi, 2-tiff_whole");

using namespace std;

int main(int argc, char **argv) {

    google::ParseCommandLineFlags(&argc, &argv, false);

    float fScale = 20.0;
    PosIdxStruct roiPosIdx = {10240, 5120, 256, 256};
    PosIdxStruct tileIdx = {10240, 5120, 0, 0};
    
    read_rgb_from_kfb(FLAGS_dll.c_str(), FLAGS_kfb.c_str(), FLAGS_save.c_str(), roiPosIdx, tileIdx, fScale, FLAGS_mode);

    return 0;

}