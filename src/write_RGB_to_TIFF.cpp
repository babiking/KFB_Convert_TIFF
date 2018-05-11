// !Created by babiking@sensetime on May 10th, 2018 to write original RGB byte stream into .tiff file...

#include "write_RGB_to_TIFF.h"

int _write_RGB_to_TIFF(unsigned char* RGB_stream, const char* TIFF_filename, int nDataLength, int nImageHeight, int nImageWidth, int nTileHeight, int nTileWidth, int nTileIndex){

    /*
     *  Function:
     *      int _write_RGB_to_TIFF(unsigned char** RGB_stream,
     *                             const char TIFF_filename,
     *                             int nDataLength,
     *                             int nHeight,
     *                             int nWidth)
     *  Input:
     *      [1] <unsigned char**> RGB_stream:    original RGB byte stream
     *      [2] <const char*>     TIFF_filename
     *      [3] <int>             nDataLength:   number of bytes in RGB byte stream
     *      [4] <int>             nImageHeight:  height of Image
     *      [5] <int>             nImageWidth:   width  of Image
     *      [6] <int>             nTileHeight:   height of Tile i.e. blockSize
     *      [7] <int>             nTileWidth:    width of Tile
     *  Output:
     *      [1] <int>             retFlag: 0-write success, 1-write fail
     *
     */

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


    TIFF* TIFF_file = TIFFOpen(TIFF_filename, "wb+");

    // !Samples per pixel
    int SPP = 3;

    // !Bits per sample
    int BPP = 8;

    // !Set .TIFF file attributes
    TIFFSetField(TIFF_file, TIFFTAG_IMAGEWIDTH,      nImageWidth);
    TIFFSetField(TIFF_file, TIFFTAG_IMAGELENGTH,     nImageHeight);
    TIFFSetField(TIFF_file, TIFFTAG_TILELENGTH,      nTileHeight);
    TIFFSetField(TIFF_file, TIFFTAG_TILEWIDTH,       nTileWidth);
    TIFFSetField(TIFF_file, TIFFTAG_PLANARCONFIG,    PLANARCONFIG_CONTIG);
    TIFFSetField(TIFF_file, TIFFTAG_BITSPERSAMPLE,   BPP);
    TIFFSetField(TIFF_file, TIFFTAG_SAMPLESPERPIXEL, SPP);
    TIFFSetField(TIFF_file, TIFFTAG_COMPRESSION,     COMPRESSION_NONE);

    // TIFFSetField(TIFF_file, TIFFTAG_ROWSPERSTRIP, nImageHeight);
    TIFFSetField(TIFF_file, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);
    TIFFSetField(TIFF_file, TIFFTAG_FILLORDER,    FILLORDER_MSB2LSB);
    // TIFFSetField(TIFF_file, TIFFTAG_XRESOLUTION, 1);
    // TIFFSetField(TIFF_file, TIFFTAG_YRESOLUTION, 1);
    // TIFFSetField(TIFF_file, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);


    // !Write into .tiff file in tile-orientation
    TIFFWriteRawTile(TIFF_file, nTileIndex, RGB_stream, nDataLength);


    _TIFFfree(RGB_stream);
    TIFFClose(TIFF_file);

    return 0;

}

