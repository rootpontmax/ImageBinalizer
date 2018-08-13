////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  ImageBinarizer
//
//  Created by Michael Scherbakov on 13/08/2018.
//  Copyright © 2018 msSoft. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <memory>

#include "ImageBinarizer.h"
#include "jpeg/jpgd.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, const char * argv[] )
{
    // Get filename
    const char *pFilename = argv[1];

    // Load file
    int imageSizeX = 0;
    int imageSizeY = 0;
    int actualComps = 0;
    const int reqComps = 4;
    uint8_t *pBufferIn = jpgd::decompress_jpeg_image_from_file( pFilename, &imageSizeX, &imageSizeY, &actualComps, reqComps );
    
    // Make enough memory for grayscale picture
    const size_t outSize = imageSizeX * imageSizeY * 3; // Let's make regular grayscale picture
    std::unique_ptr< uint8_t > outBuffer( new uint8_t[outSize] );
    
    
    CImageBinarizer binalizer;
    
    binalizer.Process( pBufferIn, outBuffer.get(), imageSizeX, imageSizeY, actualComps );
    
    std::cout << "Hello, World!\n";
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
