// !Created by babiking on May 10th, 2018 to write original RGB byte stream into .tiff file...

#include "write_RGB_to_TIFF.h"

void write_rgb_to_tiff(const char* tiffFilename, unsigned char* pRgbStream, int nRgbBytes, TileIdxStruct tileIdx){

    // !LIBTIFF image data is by default returned as ABGR pixels (A: alpha channel)
    //          packed into 32-bit words (8-bits per sample)

    /*
     *
     *  libtiff:
     *      1. Scaline-oriented Image I/O
     *      2. Strip-oriented Image I/O
     *      3. Tiles-oriented Image I/O [*]
     *          a. BlockSize: An image is broke up into a set of rectangle areas, tileWidth/tileHeight must be multiple of 16
     *          b. TileOrder: left-to-right and top-to-bottom
     *          c. SamplePackaged: Contiguously or Separately (all the tiles for sample_0 appear before that of sample_1)
     *
     */
    clock_t start, stop;
    double  runtime;

    start = clock();

    TIFF *fp = TIFFOpen(tiffFilename, "wb+");

    // !Samples per pixel
    int SPP = 3;

    // !Bits per sample
    int BPP = 8;

    // !Set .TIFF file attributes
    TIFFSetField(fp, TIFFTAG_IMAGEWIDTH,      tileIdx.imageWidth);
    TIFFSetField(fp, TIFFTAG_IMAGELENGTH,     tileIdx.imageHeight);
    TIFFSetField(fp, TIFFTAG_TILELENGTH,      tileIdx.tileHeight);
    TIFFSetField(fp, TIFFTAG_TILEWIDTH,       tileIdx.tileWidth);
    TIFFSetField(fp, TIFFTAG_PLANARCONFIG,    PLANARCONFIG_CONTIG);
    TIFFSetField(fp, TIFFTAG_BITSPERSAMPLE,   BPP);
    TIFFSetField(fp, TIFFTAG_SAMPLESPERPIXEL, SPP);
    TIFFSetField(fp, TIFFTAG_COMPRESSION,     COMPRESSION_NONE);

    // TIFFSetField(TIFF_file, TIFFTAG_ROWSPERSTRIP, nImageHeight);
    TIFFSetField(fp, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);
    TIFFSetField(fp, TIFFTAG_FILLORDER,    FILLORDER_MSB2LSB);
    // TIFFSetField(TIFF_file, TIFFTAG_XRESOLUTION, 1);
    // TIFFSetField(TIFF_file, TIFFTAG_YRESOLUTION, 1);
    // TIFFSetField(TIFF_file, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

    // check if imageDim % tileDim == 0
    assert(tileIdx.imageWidth % tileIdx.tileWidth == 0);
    assert(tileIdx.imageHeight % tileIdx.tileHeight == 0);

    int tileWidthDim = tileIdx.imageWidth / tileIdx.tileWidth;
    int tileHeightDim = tileIdx.imageHeight / tileIdx.tileHeight;

    int nTileIdx = tileIdx.x + tileIdx.y * tileWidthDim;

    // write into .tiff file in tile-orientation
    TIFFWriteRawTile(fp, nTileIdx, pRgbStream+nTileIdx*nRgbBytes, nRgbBytes);

    _TIFFfree(pRgbStream);
    TIFFClose(fp);

    stop = clock();
    runtime = double(stop - start) / (CLOCKS_PER_SEC);

    printf("Elapsed time to write No.%d tile: %f seconds.\n", nTileIdx, runtime);

}

