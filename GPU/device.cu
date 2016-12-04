#include "utils.h"
#include <stdio.h>

#define material 0.2545

__global__
void heat(const unsigned char* const in_world,
                     unsigned char* const out_world,
                     unsigned*      const heaters,
                             unsigned int numRows,
                             unsigned int numCols,
                             unsigned int numOfHeaters)
{

    const int2 thread_2D_pos = make_int2( blockIdx.x * blockDim.x + threadIdx.x,
                                          blockIdx.y * blockDim.y + threadIdx.y);
    // Check for out of boundries
    if (thread_2D_pos.x >= numCols || thread_2D_pos.y >= numRows)
        return;

    const unsigned int thread_1D_pos  = thread_2D_pos.y * numCols + thread_2D_pos.x;
    // DEBUG
    /*if (thread_1D_pos == 0)
    {
        for (unsigned idx = 0; idx < numOfHeaters; idx++)
        {
          printf("Heater ID: %d\n", heaters[idx] ); 
          printf("\n\n\n"); 
        }

    }*/
    // DEBUG



    float current = (float)in_world[thread_1D_pos];
    float top     = (float)in_world[(thread_1D_pos-numCols)%(numRows*numCols)];
    float bottom  = (float)in_world[(thread_1D_pos+numCols)%(numRows*numCols)];
    float left    = (float)in_world[(thread_1D_pos-1)      %(numRows*numCols)];
    float right   = (float)in_world[(thread_1D_pos+1)      %(numRows*numCols)];

    float resultFloat = current+material*(top+bottom+left+right-4*current);

    if (resultFloat > 255.0)
    {
      out_world[thread_1D_pos] = 255;
    }
    else
    {
      out_world[thread_1D_pos] = (unsigned char)resultFloat;
    }

    for (unsigned idx = 0; idx < numOfHeaters; idx++)
    {
      if(heaters[idx] == thread_1D_pos){
        out_world[thread_1D_pos] = 255;
      }
    }

}

void heat_cuda(unsigned char* const d_inWorld,
               unsigned char* const d_outWorld,
               unsigned*      const d_heaters,
                              const size_t numRows,
                              const size_t numCols,
                              const size_t numOfHeaters)
{
    const dim3 blockSize ( 32,32 );

                              // Ceiling
    const dim3 gridSize ( 1 + ((numCols - 1) / blockSize.x),   1 + ((numRows - 1) / blockSize.y) );
    //const dim3 gridSize ( numCols,numRows,1 );
    // Launch a kernel
    heat <<<gridSize, blockSize>>>(d_inWorld, d_outWorld, d_heaters, numRows, numCols, numOfHeaters);

    // Make sure I didn't did not mess this up like I did with my last relationship
    cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

}
