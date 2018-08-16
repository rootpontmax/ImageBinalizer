#include "AverageFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void CAverageFilter::Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    // Find the average value
    uint64_t average = 0;
    for( size_t y = 0; y < sizeY; ++y )
        for( size_t x = 0; x < sizeX; ++x )
        {
            const size_t offset = y * sizeX + x;
            average += pBuffer[offset];
        }
    average /= ( sizeX * sizeY );
        
    const uint8_t threshold = static_cast< uint8_t >( average );
    for( size_t y = 0; y < sizeY; ++y )
        for( size_t x = 0; x < sizeX; ++x )
        {
            const size_t offset = y * sizeX + x;
            pBuffer[offset] = ( pBuffer[offset] > threshold ) ? 0xFF : 0;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
