#include <string>
#include "utils.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cuda.h>
#include <cuda_runtime.h>

float* a_frame;

unsigned char *d_inWorld__;
unsigned char *d_outWorld__;

size_t numRows() { return a_frame.rows; }
size_t numCols() { return a_frame.cols; }


unsigned char *MatToUnsignChar(cv::Mat inMat){
    int height = numRows();
    int width  = numCols();
    unsigned char buffer[height * width];
    uchar* p;
    for (int i = 0; i < height; ++i) {
        p = inMat.ptr<uchar>(i);
        for (int j = 0; j < width; ++j) {
            buffer[i * width + j] = p[j];
        }
    }
    return buffer;
}

void preProcess(unsigned char **h_inWorld, unsigned char **h_outWorld,
                unsigned char **d_inWorld, unsigned char **d_outWorld,
                unsigned char **d_outWorld_swap)
{
    //Check the context initializes correctly
    checkCudaErrors(cudaFree(0));

    //----------------------------------------------------------------------------------------------
    a_frame.at<double>(0,0) = 0;
    cv::imwrite( "imgs/1.png",a_frame );
    //----------------------------------------------------------------------------------------------

    // A single interations will be stored here
    a_frame = cv::Mat(cv::Size(a_frame.cols, a_frame.rows),CV_8U);

    if (!a_frame.isContinuous()) {
        std::cerr << "a_frame is no continuous, closing the program" << std::endl;
        exit(1);
    }

     *h_inWorld  = MatToUnsignChar(a_frame);

     *h_outWorld      =  (unsigned char *)malloc(sizeof(unsigned char) *  numRows() * numCols());
     *d_outWorld_swap =  (unsigned char *)malloc(sizeof(unsigned char) *  numRows() * numCols());
}

void memoryManagement(unsigned char **h_inWorld,  unsigned char **d_inWorld,
                      unsigned char **d_outWorld, unsigned char **d_outWorld_swap)
{
    const size_t numPixels = numRows() * numCols();

    //allocate memory on the device for both input and output
    checkCudaErrors(cudaMalloc(d_inWorld,       sizeof(unsigned char) * numPixels));
    checkCudaErrors(cudaMalloc(d_outWorld,      sizeof(unsigned char) * numPixels));
    checkCudaErrors(cudaMalloc(d_outWorld_swap, sizeof(unsigned char) * numPixels));
    checkCudaErrors(cudaMemset(*d_outWorld,      0, numPixels * sizeof(unsigned char)));
    checkCudaErrors(cudaMemset(*d_outWorld_swap, 0, numPixels * sizeof(unsigned char)));
    //copy input array to the GPU
    checkCudaErrors(cudaMemcpy(*d_inWorld, *h_inWorld, sizeof(unsigned char) * numPixels, cudaMemcpyHostToDevice));
    d_inWorld__  = *d_inWorld;
    d_outWorld__ = *d_outWorld;

}

void save8UC1Image(unsigned char **ucharArr, std::string filename)
{
    cv::Mat outMat = cv::Mat(cv::Size(a_frame.cols, a_frame.rows),CV_8U, *ucharArr);
    cv::imwrite( "imgs/" + filename, outMat );
}

void cleanUp()
{
    cudaFree(d_inWorld__);
    cudaFree(d_outWorld__);
}
