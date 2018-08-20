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

////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *g_pAverageFilterStr = "average";
static const char *g_pThresholdFilterStr = "threshold";
static const char *g_pBradleyFilterStr = "bradley";

////////////////////////////////////////////////////////////////////////////////////////////////////
static void ShowUsageRules()
{
	std::cout << "Usage\n";
	std::cout << "[inputFile] [outFile] -t:[filterType]\n";
	std::cout << "\t-t:" << g_pAverageFilterStr << " - for average value\n";
	std::cout << "\t-t:" << g_pThresholdFilterStr << " - for assigned threshold\n";
	std::cout << "\t-t:" << g_pBradleyFilterStr << " - for Bradley's algorithm\n";
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

	// Get filename
	const char *pFilenameIn = argv[1];
	const char *pFilenameOut = argv[2];
	const char *pType = argv[3];

	// Load file
	int imageSizeX = 0;
	int imageSizeY = 0;
	int actualComps = 0;
	const int reqComps = 4;
	uint8_t *pBufferIn = jpgd::decompress_jpeg_image_from_file(pFilenameIn, &imageSizeX, &imageSizeY, &actualComps, reqComps);

	if (!pBufferIn)
	{
		std::cout << "Can't open input file: " << pFilenameIn << std::endl;
		return 1;
	}

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
		return 1;
	}

	CImageBinarizer binalizer(pFilter);
	binalizer.Process(pBufferIn, imageSizeX, imageSizeY, reqComps);

	// Save binary result
	jpge::params outParam;
	outParam.m_quality = 100;
	outParam.m_subsampling = jpge::Y_ONLY;
	const int outChannels = 1;
	const uint8_t *pOut = binalizer.GetBuffer();
	if (pOut)
	{
		const bool res = jpge::compress_image_to_jpeg_file(pFilenameOut, imageSizeX, imageSizeY, outChannels, pOut, outParam);
		if (res)
			std::cout << "Save\n";
		else
		{
			std::cout << "Can't write out file " << pFilenameOut << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "There is no any filter\n";
	}

    return 0;
}

