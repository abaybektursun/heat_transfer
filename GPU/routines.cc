#include <string>
#include "utils.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cuda.h>
#include <cuda_runtime.h>

cv::Mat uImg_8U;

unsigned char *d_inWorld__;
unsigned char *d_outWorld__;

unsigned rowsDef;
unsigned colsDef;
unsigned numOfHeaters__;

void setRows(unsigned rowsIn) { rowsDef = rowsIn; }
void setCols(unsigned colsIn) { colsDef = colsIn; }
void setNumOfHeaters(unsigned numOfHeatersIn) { numOfHeaters__ = numOfHeatersIn; }

size_t numRows() { return rowsDef; }
size_t numCols() { return colsDef; }
size_t numHeaters() { return numOfHeaters__; }



unsigned char *MatToUnsignChar(cv::Mat inMat){
    int height = numRows();
    int width  = numCols();
    //unsigned char buffer[height * width];
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) *  numRows() * numCols());
    uchar* p;
    for (int i = 0; i < height; ++i) {
        p = inMat.ptr<uchar>(i);
        for (int j = 0; j < width; ++j) {
            buffer[i * width + j] = p[j];
        }
    }
    return buffer;
}

void save8UImage(unsigned char **ucharArr, std::string filename)
{

    cv::Mat inputImg  = cv::Mat(cv::Size(numCols(), numRows()),CV_8U, *ucharArr);
    cv::Mat coloredImg;
    cv::cvtColor(inputImg,coloredImg,CV_GRAY2BGR);
    cv::Mat outMats[3];
    cv::split(coloredImg,outMats);  
    
    if (outMats[1].empty()) {
        std::cerr << "Couldn save file: " << filename << std::endl;
        exit(1);
    }
    cv::imwrite( "imgs/" + filename, outMats[0] );

}

void preProcess(unsigned char **h_inWorld, unsigned char **h_outWorld, unsigned **heaters,
                const std::string &filename)
{
    *h_inWorld  =  (unsigned char *)malloc(sizeof(unsigned char) *  numRows() * numCols());
    *h_outWorld =  (unsigned char *)malloc(sizeof(unsigned char) *  numRows() * numCols());

    //Check the context initializes correctly
    checkCudaErrors(cudaFree(0));

    std::srand(std::time(0));

    *heaters = (unsigned*)malloc(sizeof(unsigned) * numOfHeaters__);

    for (unsigned i = 0; i < numOfHeaters__; i++)
    {
        (*heaters)[i] = std::rand() % (rowsDef*colsDef);
    }

    //----------------------------------------------------------------------------------------------
    for (unsigned i = 0; i < numOfHeaters__; i++)
    {
        (*h_inWorld)[(*heaters)[i]] = 255;
    }

    save8UImage(h_inWorld, filename);
    //----------------------------------------------------------------------------------------------
}

void memoryManagement(unsigned char **h_inWorld,  unsigned char **d_inWorld, unsigned char **d_outWorld, 
                      unsigned **heaters, unsigned **d_heaters)
{
    const size_t numPixels = numRows() * numCols();

    //allocate memory on the device for both input and output
    checkCudaErrors(cudaMalloc(d_heaters,      sizeof(unsigned)      * numOfHeaters__));
    checkCudaErrors(cudaMalloc(d_inWorld,      sizeof(unsigned char) * numPixels));
    checkCudaErrors(cudaMalloc(d_outWorld,     sizeof(unsigned char) * numPixels));
    checkCudaErrors(cudaMemset(*d_outWorld, 0, sizeof(unsigned char) * numPixels));
    //copy input array to the GPU
    checkCudaErrors(cudaMemcpy(*d_inWorld, *h_inWorld, sizeof(unsigned char) * numPixels,      cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(*d_heaters, *heaters,   sizeof(unsigned)      * numOfHeaters__, cudaMemcpyHostToDevice));
    d_inWorld__  = *d_inWorld;
    d_outWorld__ = *d_outWorld;

}


void cleanUp()
{
    cudaFree(d_inWorld__);
    cudaFree(d_outWorld__);
}
