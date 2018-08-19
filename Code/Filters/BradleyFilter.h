////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary filter that uses Bradlye's algorithm.                                                   //
// Algorthm can be found here                                                                     //
// http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.420.7883                               //
//                                                                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../IBinaryFilter.h"
#include "../ImageBuffer.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
class CBradleyFilter : public IBinaryFilter
{
public:
    CBradleyFilter( const size_t patchSize, const uint8_t coef );
    virtual void Process( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const override;
    
private:
    void        CalcIntegralSum( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const;
    void        Binarize( uint8_t *pBuffer, const size_t sizeX, const size_t sizeY ) const; 
    
    mutable CImageBuffer< uint32_t >    m_integralSumBuffer;
    const size_t                        m_patchSize;
    const uint8_t                       m_coef;
    
};
////////////////////////////////////////////////////////////////////////////////////////////////////
