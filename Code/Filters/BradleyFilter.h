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
    CBradleyFilter( const size_t patchSize, const float coef );
    virtual void Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const override;
    
private:
    void        CalcIntegralSum( uint8_t *pBuffer ) const;
    void        Binarize( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const; 
    
    mutable CImageBuffer< float >   m_integralSumBuffer;
    const size_t                    m_patchSize;
    const float                     m_coef;
    
};
////////////////////////////////////////////////////////////////////////////////////////////////////
