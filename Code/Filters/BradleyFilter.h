////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary filter that uses Bradlye's algorithm.                                                   // 
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../IBinaryFilter.h"
#include "../ImageBuffer.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
class CBradleyFilter : public IBinaryFilter
{
public:
    virtual void Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const override;
    
private:
    void        CalcIntegralSum( uint8_t *pBuffer ) const;
    void        Binarize( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const;
    uint32_t    GetAvg( const size_t xLT, const size_t yLT, const size_t xRB, const size_t yRB ) const; 
    
    static const size_t PATCH_SIZE = 8;
    
    mutable CImageBuffer< float >   m_integralSumBuffer;
    
};
////////////////////////////////////////////////////////////////////////////////////////////////////
