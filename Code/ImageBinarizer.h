////////////////////////////////////////////////////////////////////////////////////////////////////
// Image binarizer class.                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define USE_OPEN_CV

#include <cstdint>
#include <memory>

#include "ImageBuffer.h"

#ifdef USE_OPEN_CV
#include <opencv2/core/mat.hpp>
#endif



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
#ifdef USE_OPEN_CV
	cv::Mat Process( const cv::Mat& mat );
#endif
    void    Clear();
    
    // Get binarize image data
    uint8_t *GetBuffer() const;
    
private:

    void    MakeGreyscale( const uint8_t *pBuffer, const int bpp );
#ifdef USE_OPEN_CV
    void    MakeGreyscale(const cv::Mat& mat );
#endif
    
    CImageBuffer< uint8_t >     m_buffer;
    const IBinaryFilter        *m_pFilter;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
