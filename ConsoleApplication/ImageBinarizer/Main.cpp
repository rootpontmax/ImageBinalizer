////////////////////////////////////////////////////////////////////////////////////////////////////
// Test console application for demontration IgameBinarizer static library.						  //
//																								  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "ImageBinarizer.h"

#include "Filters/AverageFilter.h"
#include "Filters/ThresholdFilter.h"
#include "Filters/BradleyFilter.h"

#include "../Code/jpeg/jpgd.h"
#include "../Code/jpeg/jpge.h"

//#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *g_pAverageFilterStr = "average";
static const char *g_pThresholdFilterStr = "threshold";
static const char *g_pBradleyFilterStr = "bradley";
////////////////////////////////////////////////////////////////////////////////////////////////////
static void ShowUsageRules()
{
	std::cout << "Usage\n";
	std::cout << "[inputFile] [outFile] [filterType]\n";
	std::cout << "Filter types:" << std::endl;
	std::cout << "\t" << g_pAverageFilterStr << " - for average value\n";
	std::cout << "\t" << g_pThresholdFilterStr << " - for assigned threshold\n";
	std::cout << "\t" << g_pBradleyFilterStr << " - for Bradley's algorithm\n";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static bool CheckCommandLine(int argc, const char * argv[])
{
	if (argc < 4)
	{
		ShowUsageRules();
		return false;
	}

	const char *pType = argv[3];
	if (strcmp(pType, g_pAverageFilterStr) != 0 &&
		strcmp(pType, g_pThresholdFilterStr) != 0 &&
		strcmp(pType, g_pBradleyFilterStr) != 0)
	{
		ShowUsageRules();
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, const char * argv[] )
{
	std::cout << "Image binalizer. Michael Scherbakov. 2018.\n";

	if (!CheckCommandLine(argc, argv))
		return 1;

	// Get filenames and filter type
	const char *pFilenameIn = argv[1];
	const char *pFilenameOut = argv[2];
	const char *pType = argv[3];

    // Load input image
    cv::Mat imageIn;
    imageIn = cv::imread( pFilenameIn, CV_LOAD_IMAGE_COLOR );
    const int imageSizeX = imageIn.rows;
    const int imageSizeY = imageIn.cols;
    if( 0 == imageSizeX || 0 == imageSizeY )
    {
        std::cout << "Can't load image" << std::endl;
        return 2;
    }

    // Choose filter
    IBinaryFilter *pFilter = nullptr;
	if (strcmp(pType, g_pAverageFilterStr) == 0)
	{
		CThresholdFilter filter(0x70);
		pFilter = &filter;
	}
	else if (strcmp(pType, g_pThresholdFilterStr) == 0)
	{
		CAverageFilter filter;
		pFilter = &filter;
	}
	else if (strcmp(pType, g_pBradleyFilterStr) == 0)
	{
		CBradleyFilter filter(4, 15);
		pFilter = &filter;
	}

	if (nullptr == pFilter)
	{
		std::cout << "Unknown filter type" << std::endl;
		return 3;
	}

    CImageBinarizer binalizer(pFilter);
    binalizer.Process( imageIn );
    cv::Mat imageOut( imageSizeX, imageSizeY, CV_8U, binalizer.GetBuffer() );
    const bool res = cv::imwrite( pFilenameOut, imageOut );
    if( res )
    {
        std::cout << "Done.\n";
    }
    else
    {
        std::cout << "Can't write out file " << pFilenameOut << std::endl;
        return 4;
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
