#include <iostream>
#include "utils.h"
#include <string>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <string>
#include "routines.cc"

// User defined ----------
#define ITERS 500
#define rowsDef 500
#define colsDef 500
#define numOfHeatersDef 50
// User defined ----------

// device.cu  ----------------------------------------------------//
void heat_cuda(unsigned char* const d_inWorld,
               unsigned char* const d_outWorld,
               unsigned*      const d_heaters,
                              const size_t numRows,
                              const size_t numCols,
                              const size_t numOfHeaters);
// device.cu  ----------------------------------------------------//

int main()
{
    unsigned char *h_inWorld,  *d_inWorld;
    unsigned char *h_outWorld, *d_outWorld;
	unsigned      *heaters,    *d_heaters;

    std::string in_file_name = "0.png";

    setRows(rowsDef); setCols(colsDef); setNumOfHeaters(numOfHeatersDef);

    //load the image and get the pointers
    preProcess(&h_inWorld, &h_outWorld, &heaters,
                          in_file_name);


    memoryManagement(&h_inWorld, &d_inWorld, &d_outWorld, &heaters, &d_heaters);

    size_t numPixels = numRows()*numCols();
    // CUDA
    heat_cuda(d_inWorld, d_outWorld, d_heaters, numRows(), numCols(), numOfHeatersDef);
    // Get yo ass back to host
    checkCudaErrors(cudaMemcpy(h_outWorld, d_outWorld, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));
    unsigned i;
    for(i = 1; i < ITERS; i++){
    	//printf("Iter: %d\n",i);

        std::thread writer([&](){
            save8UImage(&h_outWorld, std::to_string(i) + ".png");
        });
        std::swap(d_inWorld, d_outWorld); 

        heat_cuda(d_inWorld, d_outWorld, d_heaters, numRows(), numCols(), numOfHeatersDef);
        // Get yo ass back to host
        checkCudaErrors(cudaMemcpy(h_outWorld, d_outWorld, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));
        cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

        writer.join();
    }
    
    save8UImage(&h_outWorld, std::to_string(i) + ".png");


    free(h_inWorld);
    free(h_outWorld);
    cudaFree(d_heaters);
    cleanUp();
    return 0;
}
