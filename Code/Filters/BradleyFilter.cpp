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
    const int sumSizeX = static_cast< int >( m_integralSumBuffer.GetSizeX() );
    const int sumSizeY = static_cast< int >( m_integralSumBuffer.GetSizeY() );
    
#ifdef DEBUG
    for( size_t y = 0; y < sumSizeY; ++y )
        for( size_t x = 0; x < sumSizeX; ++x )
        {
            const size_t offset = y * sumSizeX + x;
            pSum[offset] = -1.0f;
        }
#endif

    // First row
    for( size_t x = 0; x < sumSizeX; ++x )
        pSum[x] = 0.0f;
        
    // First column except first pixel
    for( size_t y = 1; y < sumSizeY; ++y )
    {
        const size_t offset = y * sumSizeX;
        pSum[offset] = 0.0f;
    }
    
    // All other pixels exept first row and first column
    for( int y = 1; y < sumSizeY; ++y )
        for( int x = 1; x < sumSizeX; ++x )
        {
            const int offset = y * sumSizeX + x;
            const int offsetX = offset - 1;
            const int offsetY = offset - sumSizeX;
            const int offsetXY = offsetY - 1;
            
            assert( offset >= 0.0f && offset < sumSizeX * sumSizeY );
            assert( offsetX >= 0.0f && offsetX < sumSizeX * sumSizeY );
            assert( offsetY >= 0.0f && offsetY < sumSizeX * sumSizeY );
            assert( offsetXY >= 0.0f && offsetXY < sumSizeX * sumSizeY );
            
            assert( pSum[offsetX] >= 0.0f );
            assert( pSum[offsetY] >= 0.0f );
            assert( pSum[offsetXY] >= 0.0f );
            
            pSum[offset] = pBuffer[offset] + pSum[offsetX] + pSum[offsetY] - pSum[offsetXY];
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::Binarize( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    const float *pSum = m_integralSumBuffer.GetBuffer();
    const size_t bufferStride = m_integralSumBuffer.GetSizeX();
    
    // Make int to avoid warnings
    const int imageSizeX = static_cast< int >( sizeX );
    const int imageSizeY = static_cast< int >( sizeY );
    const int patchSize = static_cast< int >( m_patchSize );
    
    for( int y = 0; y < sizeY; ++y )
        for( int x = 0; x < sizeX; ++x )
        {
            int xL = x - patchSize;
            int xR = x + patchSize;
            int yT = y - patchSize;
            int yB = y + patchSize;
            
            if( xL < 0 )
                xL = 0;
            if( yT < 0 )
                yT = 0;
            if( xR >= imageSizeX )
                xR = imageSizeX - 1;
            if( yB >= imageSizeY )
                yB = imageSizeY - 1;
                
            xL -= 1;
            yT -= 1;
                
            assert( xL >= -1 && xL < imageSizeX );
            assert( xR >= 0 && xR < imageSizeX );
            assert( yT >= -1 && yT < imageSizeY );
            assert( yB >= 0 && yB < imageSizeY );
            assert( xL < xR );
            assert( yT < yB );
                
            const size_t offset = y * sizeX + x;
            const size_t offsetLT = ( yT + 1 ) * bufferStride + ( xL + 1 ); 
            const size_t offsetRB = ( yB + 1 ) * bufferStride + ( xR + 1 );
            const size_t offsetRT = ( yT + 1 ) * bufferStride + ( xR + 1 );
            const size_t offsetLB = ( yB + 1 ) * bufferStride + ( xL + 1 );
            
            assert( offset < sizeX * sizeY );
            assert( offsetLT < bufferStride * m_integralSumBuffer.GetSizeY() );
            assert( offsetRB < bufferStride * m_integralSumBuffer.GetSizeY() );
            assert( offsetRT < bufferStride * m_integralSumBuffer.GetSizeY() );
            assert( offsetLB < bufferStride * m_integralSumBuffer.GetSizeY() );
                
            const int count = ( xR - xL + 1 ) * ( yB - yT + 1 );

            const float sumA = pSum[offsetLT];
            const float sumB = pSum[offsetRT];
            const float sumC = pSum[offsetLB];
            const float sumD = pSum[offsetRB];
            const float sum = sumA + sumD - sumB - sumC;
            const float avgSum = sum / static_cast< float >( count );
            //assert( avgSum <= 255.0f );
            const float threshold = avgSum * m_coef;
            const uint8_t threshold8 = static_cast< uint8_t >( threshold );
            
            pBuffer[offset] = ( pBuffer[offset] > threshold8 ) ? 0xFF : 0;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
