#include "BradleyFilter.h"

#include <cassert>

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
    
    
    const size_t patchCountX = sizeX / PATCH_SIZE + 1;
    const size_t patchCountY = sizeY / PATCH_SIZE + 1;
    
    for( size_t patchY = 0; patchY < patchCountY; ++patchY )
        for( size_t patchX = 0; patchX < patchCountX; ++patchX )
        {
            const size_t xLT = patchX * PATCH_SIZE;
            const size_t yLT = patchY * PATCH_SIZE;
            const size_t xRB = xLT + PATCH_SIZE;
            const size_t yRB = yLT + PATCH_SIZE;
            const uint32_t avg = GetAvg( xLT, yLT, xRB, yRB );
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CBradleyFilter::GetAvg( const size_t xLT, const size_t yLT, const size_t xRB, const size_t yRB ) const
{
    const float *pSum = m_integralSumBuffer.GetBuffer();
    const size_t sumSizeX = m_integralSumBuffer.GetSizeX();
    const size_t sumSizeY = m_integralSumBuffer.GetSizeY();
    
    const size_t sumLTx = xLT + 1;
    const size_t sumLTy = yLT + 1;
    const size_t sumRBx = xRB + 1;
    const size_t sumRBy = yRB + 1;
    
    const size_t offsetA = sumLTy * sumSizeY + sumLTx;
    
    uint32_t sum = 0;
    
    
    
    
    return sum;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
