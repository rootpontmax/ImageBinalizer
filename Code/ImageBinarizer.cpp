#include <cassert>

#include "ImageBinarizer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::Process( const uint8_t *pBufferIn, uint8_t *pBufferOut,
                               const int sizeX, const int sizeY, const int bpp )
{
    assert( pBufferIn && pBufferOut );
    
    // Make grayscale
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            const int offsetIn = ( y * sizeX + x ) * 4;
            const int offset = ( y * sizeX + x );
            
            const uint8_t colR = pBufferIn[offsetIn];
            const uint8_t colG = pBufferIn[offsetIn + 1];
            const uint8_t colB = pBufferIn[offsetIn + 2];
            const int grey = ( colR + colG + colB ) / 3;
            pBufferOut[offset] = grey;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////

