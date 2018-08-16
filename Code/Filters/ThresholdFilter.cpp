#include "ThresholdFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CThresholdFilter::CThresholdFilter( const uint8_t threshold ) :
    m_threshold( threshold )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CThresholdFilter::Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    for( size_t y = 0; y < sizeY; ++y )
        for( size_t x = 0; x < sizeX; ++x )
        {
            const size_t offset = y * sizeX + x;
            pBuffer[offset] = ( pBuffer[offset] > m_threshold ) ? 0xFF : 0;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
