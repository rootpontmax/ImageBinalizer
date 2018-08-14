#include "ThresholdFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CThresholdFilter::CThresholdFilter( const uint8_t threshold ) :
    m_threshold( threshold )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CThresholdFilter::ProcessGreyscale( uint8_t *pBuffer, const int sizeX, const int sizeY ) const
{
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            const int offset = y * sizeX + x;
            pBuffer[offset] = ( pBuffer[offset] > m_threshold ) ? 0xFF : 0;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
