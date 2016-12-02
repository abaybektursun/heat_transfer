#include <iostream>
#include "utils.h"
#include <string>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <string>
#include "routines.cc"

#define ITERS 100

// device.cu  ----------------------------------------------------//
void heat_transfer(unsigned char* const d_inWorld,
                       unsigned char* const d_outWorld,
                       const size_t numRows,
                       const size_t numCols);
// device.cu  ----------------------------------------------------//

int main()
{
    unsigned char *h_inWorld,  *d_inWorld;
    unsigned char *h_outWorld, *d_outWorld;
    unsigned char *d_outWorld_swap;

    //load the image and get the pointers
    preProcess(&h_inWorld, &h_outWorld,
               &d_inWorld, &d_outWorld,
                      &d_outWorld_swap);

    memoryManagement(&h_inWorld, &d_inWorld, &d_outWorld, &d_outWorld_swap);


    /*size_t numPixels = numRows()*numCols();
    // CUDA
    heat_transfer(d_inWorld, d_outWorld, numRows(), numCols());
    // Get yo ass back to host
    checkCudaErrors(cudaMemcpy(h_outWorld, d_outWorld__, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));
    unsigned i;
    for(i = 2; i < ITERS; i++){
        std::thread writer([&](){
            save8UC1Image(&h_outWorld, std::to_string(i) + ".png");
        });
        std::swap(d_inWorld, d_outWorld);

        heat_transfer(d_inWorld, d_outWorld, numRows(), numCols());
        // Get yo ass back to host
        checkCudaErrors(cudaMemcpy(h_outWorld, d_outWorld__, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));

        writer.join();
    }
    cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
    save8UC1Image(&h_outWorld, std::to_string(i) + ".png");*/

    free(h_outWorld);
    cleanUp();
    return 0;
}
