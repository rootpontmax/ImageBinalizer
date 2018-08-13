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
#include <cassert>

#include "ImageBinarizer.h"
#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, const char * argv[] )
{
    std::cout << "Image binalizer\n";
    
    // Get filename
    const char *pFilenameIn = argv[1];
    const char *pFilenameOut = argv[2];

    // Load file
    int imageSizeX = 0;
    int imageSizeY = 0;
    int actualComps = 0;
    const int reqComps = 4;
    uint8_t *pBufferIn = jpgd::decompress_jpeg_image_from_file( pFilenameIn, &imageSizeX, &imageSizeY, &actualComps, reqComps );
    
    // Make enough memory for grayscale picture
    const size_t outSize = imageSizeX * imageSizeY * 3; // Let's make regular grayscale picture
    std::unique_ptr< uint8_t > outBuffer( new uint8_t[outSize] );
    
    uint8_t *pBufferOut = outBuffer.get();
    
    // Process image
    CImageBinarizer binalizer;
    binalizer.Process( pBufferIn, pBufferOut, imageSizeX, imageSizeY, actualComps );
    
    // Save binary result
    jpge::params outParam;
    outParam.m_quality = 100;
    outParam.m_subsampling = jpge::H2V2;//Y_ONLY;
    const int outChannels = 3;
    const bool res = jpge::compress_image_to_jpeg_file( pFilenameOut, imageSizeX, imageSizeY, outChannels, pBufferOut, outParam );
    //const bool res = jpge::compress_image_to_jpeg_file( pFilenameOut, imageSizeX, imageSizeY, outChannels, pBufferOut );
    assert( res );
    
    std::cout << "Done\n";
    
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
