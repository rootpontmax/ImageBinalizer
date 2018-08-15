#include <cassert>

#include "ImageBinarizer.h"
#include "IBinaryFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CImageBinarizer::CImageBinarizer() :
    m_pFilter( nullptr ),
    m_bufferSizeX( 0 ),
    m_bufferSizeY( 0 )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
CImageBinarizer::CImageBinarizer( const IBinaryFilter *pFilter ) :
m_pFilter( pFilter ),
    m_bufferSizeX( 0 ),
    m_bufferSizeY( 0 )
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
    
    CheckBuffer( sizeX, sizeY );
    MakeGreyscale( pBuffer, bpp );
    m_pFilter->ProcessGreyscale( m_buffer.get(), sizeX, sizeY );
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::Clear()
{
    m_buffer.reset( nullptr );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t *CImageBinarizer::GetBuffer() const
{
    return m_buffer.get();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::CheckBuffer( const int sizeX, const int sizeY )
{
    if( m_buffer && sizeX == m_bufferSizeX && sizeY == m_bufferSizeY )
        return;
        
    Clear();
    CreateBuffer( sizeX, sizeY );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::CreateBuffer( const int sizeX, const int sizeY )
{
    const size_t size = sizeX * sizeY;
    std::unique_ptr< uint8_t > buffer( new uint8_t[size] );
    m_buffer = std::move( buffer );
    m_bufferSizeX = sizeX;
    m_bufferSizeY = sizeY;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::MakeGreyscale( const uint8_t *pBuffer, const int bpp )
{
    uint8_t *pOut = m_buffer.get();
    
    // Make grayscale
    for( int y = 0; y < m_bufferSizeY; ++y )
        for( int x = 0; x < m_bufferSizeX; ++x )
        {
            const int offsetIn = ( y * m_bufferSizeX + x ) * bpp;
            const int offset = ( y * m_bufferSizeX + x );
            
            const float colR = static_cast< float >( pBuffer[offsetIn] );
            const float colG = static_cast< float >( pBuffer[offsetIn + 1] );
            const float colB = static_cast< float >( pBuffer[offsetIn + 2] );
            const float grey = colR * 0.2125f + colG * 0.7154f + colB * 0.0721f;
            pOut[offset] = static_cast< uint8_t >( grey );
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////

