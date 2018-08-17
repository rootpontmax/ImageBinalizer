#include "BradleyFilter.h"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////
CBradleyFilter::CBradleyFilter( const size_t patchSize, const float coef ) :
    m_patchSize( patchSize ),
    m_coef( coef )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    assert( pBuffer );
    m_integralSumBuffer.CheckOrCreate( sizeX + 1, sizeY + 1 );
    CalcIntegralSum( pBuffer );
    Binarize( pBuffer, sizeX, sizeY );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::CalcIntegralSum( uint8_t *pBuffer ) const
{
    float *pSum = m_integralSumBuffer.GetBuffer();
    const size_t sumSizeX = m_integralSumBuffer.GetSizeX();
    const size_t sumSizeY = m_integralSumBuffer.GetSizeY();

    // First row
    for( size_t x = 0; x < sumSizeX; ++x )
        pSum[x] = 0.0f;
        
    // First column
    for( size_t y = 1; y < sumSizeY; ++y )
    {
        const size_t offset = y * sumSizeX;
        pSum[offset] = 0.0f;
    }
    
    // For all pixels
    
    /*
    // Left-top pixel
    pSum[0] = pBuffer[0];
    
    // Top row except left-top pixel
    for( size_t x = 1; x < sizeX; ++x )
        pSum[x] = pBuffer[x] + pSum[x-1];
        
    // Left column except left-top pixel
    for( size_t y = 1; y < sizeY; ++y )
    {
        const size_t offset = y * sizeX;
        const size_t offsetY = offset - sizeX;
        pSum[offset] = pBuffer[offset] + pSum[offsetY];
    }
        
    for( size_t y = 1; y < sizeY; ++y )
        for( size_t x = 1; x < sizeX; ++x )
        {
            const size_t offset = y * sizeX + x;
            const size_t offsetX = offset - 1;
            const size_t offsetY = offset - sizeX;
            const size_t offsetXY = offsetY - 1;
            pSum[offset] = pBuffer[offset] + pSum[offsetX] + pSum[offsetY] + pSum[offsetXY];
        }
    */
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::Binarize( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    const float *pSum = m_integralSumBuffer.GetBuffer();
    const size_t bufferStride = m_integralSumBuffer.GetSizeX();
    
    const int imageSizeX = static_cast< int >( sizeX );
    const int imageSizeY = static_cast< int >( sizeY );
    
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            int xL = x - m_patchSize;
            int xR = x + m_patchSize;
            int yT = y - m_patchSize;
            int yB = y + m_patchSize;
            
            if( xL < 0 )
                xL = 0;
            if( yT < 0 )
                yT = 0;
            if( xR >= imageSizeX )
                xR = imageSizeX - 1;
            if( yB >= imageSizeY )
                yB = imageSizeY - 1;
                
            const size_t offset = y * sizeX + x;
            const size_t offsetLT = ( yT + 1 ) * bufferStride + ( xL + 1 ); 
            const size_t offsetRB = ( yB + 1 ) * bufferStride + ( xR + 1 );
            const size_t offsetRT = ( yT + 1 ) * bufferStride + ( xR + 1 );
            const size_t offsetLB = ( yB + 1 ) * bufferStride + ( xL + 1 );
                
            const int count = ( xR - xL + 1) * ( yB - yT + 1 );
            const float sum = pSum[offsetLT] + pSum[offsetRB] - pSum[offsetRT] - pSum[offsetLB];
            const float threshold = sum * m_coef / static_cast< float >( count );
            assert( threshold <= 255.0f );
            const uint8_t threshold8 = static_cast< uint8_t >( threshold );
            
            pBuffer[offset] = ( pBuffer[offset] > threshold8 ) ? 0xFF : 0;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
