#include <cassert>

#include "ImageBinarizer.h"
#include "IBinaryFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CImageBinarizer::CImageBinarizer() :
    m_pFilter( nullptr )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
CImageBinarizer::CImageBinarizer( const IBinaryFilter *pFilter ) :
    m_pFilter( pFilter )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::Init( const IBinaryFilter *pFilter )
{
    m_pFilter = pFilter;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::Process( const uint8_t *pBuffer, const int sizeX, const int sizeY, const int bpp )
{
    if( !m_pFilter )
        return;
        
    assert( pBuffer );
    
    m_buffer.CheckOrCreate( sizeX, sizeY );
    MakeGreyscale( pBuffer, bpp );
    
    m_pFilter->Process( m_buffer.GetBuffer(), sizeX, sizeY );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::Clear()
{
    m_buffer.Clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t *CImageBinarizer::GetBuffer() const
{
    return m_buffer.GetBuffer();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::MakeGreyscale( const uint8_t *pBuffer, const int bpp )
{
    uint8_t *pOut = m_buffer.GetBuffer();
    
    // Make grayscale
    const size_t sizeX = m_buffer.GetSizeX();
    const size_t sizeY = m_buffer.GetSizeY();
    for( size_t y = 0; y < sizeY; ++y )
        for( size_t x = 0; x < sizeX; ++x )
        {
            const size_t offsetIn = ( y * sizeX + x ) * bpp;
            const size_t offset = ( y * sizeX + x );
            
            const float colR = static_cast< float >( pBuffer[offsetIn] );
            const float colG = static_cast< float >( pBuffer[offsetIn + 1] );
            const float colB = static_cast< float >( pBuffer[offsetIn + 2] );
            const float grey = colR * 0.2125f + colG * 0.7154f + colB * 0.0721f;
            pOut[offset] = static_cast< uint8_t >( grey );
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////

