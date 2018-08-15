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

#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"
#include "ImageBinarizer.h"
#include "Filters/ThresholdFilter.h"
#include "Filters/AverageFilter.h"
#include "Filters/BradleyFilter.h"



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
    
    // Filters
    CThresholdFilter thresholdFilter( 0x70 );
    CAverageFilter averageFilter;
    CBradleyFilter bradleyFilter;
        
    // Process image
    //CImageBinarizer binalizer( &thresholdFilter );
    //CImageBinarizer binalizer( &averageFilter );
    CImageBinarizer binalizer( &bradleyFilter );
    binalizer.Process( pBufferIn, imageSizeX, imageSizeY, reqComps );
    
    // Save binary result
    jpge::params outParam;
    outParam.m_quality = 100;
    outParam.m_subsampling = jpge::Y_ONLY;
    const int outChannels = 1;
    const uint8_t *pOut = binalizer.GetBuffer();
    if( pOut )
    {
        const bool res = jpge::compress_image_to_jpeg_file( pFilenameOut, imageSizeX, imageSizeY, outChannels, pOut, outParam );
        assert( res );
        std::cout << "Save\n";
    }
    else
    {
        std::cout << "There is no any filter\n";
    }
    
    
    
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
