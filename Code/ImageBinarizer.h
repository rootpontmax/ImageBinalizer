////////////////////////////////////////////////////////////////////////////////////////////////////
// Image binarizer class.                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include  <memory>

////////////////////////////////////////////////////////////////////////////////////////////////////
struct IBinaryFilter;
////////////////////////////////////////////////////////////////////////////////////////////////////
class CImageBinarizer
{
public:
    CImageBinarizer();
    CImageBinarizer( const IBinaryFilter *pFilter );
    
    void    Init( const IBinaryFilter *pFilter );
    void    Process( const uint8_t *pBufferIn, const int sizeX, const int sizeY, const int bpp );
    void    Clear();
    
    // Get binarize image data
    const uint8_t  *GetBuffer() const;
    
private:

    void    CheckBuffer( const int sizeX, const int sizeY );
    void    CreateBuffer( const int sizeX, const int sizeY );
    void    MakeGreyscale( const uint8_t *pBuffer, const int bpp );
    
    std::unique_ptr< uint8_t >  m_buffer;
    const IBinaryFilter        *m_pFilter;
    int                         m_bufferSizeX;
    int                         m_bufferSizeY;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
