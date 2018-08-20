#include "BradleyFilter.h"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////
CBradleyFilter::CBradleyFilter( const size_t patchSize, const uint8_t coef ) :
    m_patchSize( patchSize ),
    m_coef( 0 )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    assert( pBuffer );
    m_integralSumBuffer.CheckOrCreate( sizeX, sizeY );
    CalcIntegralSum( pBuffer, sizeX, sizeY );
    Binarize( pBuffer, sizeX, sizeY );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::CalcIntegralSum( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    uint32_t *pSum = m_integralSumBuffer.GetBuffer();
    for( size_t x = 0; x < sizeX; ++x )
    {
        uint32_t sum = 0;
        for( size_t y = 0; y < sizeY; ++y )
        {
            const size_t offset = y * sizeX + x;
            assert( offset < sizeX * sizeY );
            
            sum += pBuffer[offset];
            if( 0 == x )
                pSum[offset] = sum;
            else
            {
                assert( offset > 0 );
                pSum[offset] = pSum[offset - 1] + sum;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CBradleyFilter::Binarize( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const
{
    const uint32_t *pSum = m_integralSumBuffer.GetBuffer();
    
    const int patchSize = static_cast< int >( sizeX ) / 16; // 16 from white paper
    
    // Make int to avoid warnings
    const int imageSizeX = static_cast< int >( sizeX );
    const int imageSizeY = static_cast< int >( sizeY );
    
    for( int y = 0; y < imageSizeY; ++y )
        for( int x = 0; x < imageSizeX; ++x )
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
                
            const int count = ( xR - xL ) * ( yB - yT );
            assert( count > 0 && count <= ( patchSize * 2 ) * ( patchSize * 2 ) );
                
            assert( xL < xR );
            assert( yT < yB );
                
            const size_t offset = y * sizeX + x;
            const size_t offsetLT = yT * sizeX + xL; 
            const size_t offsetRB = yB * sizeX + xR;
            const size_t offsetRT = yT * sizeX + xR;
            const size_t offsetLB = yB * sizeX + xL;
            
            assert( offset < sizeX * sizeY );
            assert( offsetLT < sizeX * sizeY );
            assert( offsetRB < sizeX * sizeY );
            assert( offsetRT < sizeX * sizeY );
            assert( offsetLB < sizeX * sizeY );

            const uint32_t sumA = pSum[offsetLT];
            const uint32_t sumB = pSum[offsetRT];
            const uint32_t sumC = pSum[offsetLB];
            const uint32_t sumD = pSum[offsetRB];
            const uint32_t sum = sumA + sumD - sumB - sumC;
            const int thisInten = pBuffer[offset] * count;
            const int threshold = sum * ( 100 - m_coef ) / 100;
            if( thisInten <= threshold )
                pBuffer[offset] = 0;
            else
                pBuffer[offset] = 0xFF;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
