#include "AverageFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void CAverageFilter::ProcessGreyscale( uint8_t *pBuffer, const int sizeX, const int sizeY ) const
{
    // Find the average value
    uint64_t average = 0;
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            const int offset = y * sizeX + x;
            average += pBuffer[offset];
        }
    average /= ( sizeX * sizeY );
        
    const uint8_t threshold = static_cast< uint8_t >( average );
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            const int offset = y * sizeX + x;
            pBuffer[offset] = ( pBuffer[offset] > threshold ) ? 0xFF : 0;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
