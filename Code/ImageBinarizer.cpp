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
    assert( pBuffer );
    
    m_buffer.CheckOrCreate( sizeX, sizeY );
    MakeGreyscale( pBuffer, bpp );
    
    if( m_pFilter )
        m_pFilter->Process( m_buffer.GetBuffer(), sizeX, sizeY );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_OPEN_CV
cv::Mat CImageBinarizer::Process( const cv::Mat& mat )
{
	const int sizeX = mat.cols;
	const int sizeY = mat.rows;

	m_buffer.CheckOrCreate( sizeX, sizeY );
	MakeGreyscale( mat );

    if (m_pFilter)
	    m_pFilter->Process(m_buffer.GetBuffer(), sizeX, sizeY);

    cv::Mat result( sizeY, sizeX, CV_8U, m_buffer.GetBuffer() );

    return result;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::Clear()
{
    m_buffer.Clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t *CImageBinarizer::GetBuffer() const
{
    return m_buffer.GetBuffer();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageBinarizer::MakeGreyscale( const uint8_t *pBuffer, const int bpp )
{    
    const size_t sizeX = m_buffer.GetSizeX();
    const size_t sizeY = m_buffer.GetSizeY();

    uint8_t *pOut = m_buffer.GetBuffer();
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
#ifdef USE_OPEN_CV
void CImageBinarizer::MakeGreyscale( const cv::Mat& mat )
{   
    const int sizeX = mat.cols;
    const int sizeY = mat.rows;

    uint8_t *pOut = m_buffer.GetBuffer();
    for( int  y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            const cv::Vec3b color = mat.at< cv::Vec3b> ( cv::Point( x, y ) );
            const size_t offset = (y * sizeX + x);

            const float colR = static_cast< float >( color[0] );
            const float colG = static_cast< float >( color[1] );
            const float colB = static_cast< float >( color[2] );
            const float grey = colR * 0.2125f + colG * 0.7154f + colB * 0.0721f;
            pOut[offset] = static_cast< uint8_t >(grey);
        }
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////

