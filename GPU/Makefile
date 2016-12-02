
# Pick either small or large image ------------------------------------------------------------------
IMGS_FOLDER=imgs
# Set the command for nvcc compiler -----------------------------------------------------------------
NVCC=nvcc
CUDA_INCLUDEPATH=${CUDA_HOME}/include
#----------------------------------------------------------------------------------------------------
OPENCV_LIBPATH=/usr/lib
OPENCV_INCLUDEPATH=/usr/include
OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui
#----------------------------------------------------------------------------------------------------
#GCC_OPTS=-std=c++1y -O1 -Wall -Wextra -m64
NVCC_OPTS=-O3 -arch=sm_20 -Xcompiler -Wall -Xcompiler -Wextra -m64
GCC_OPTS=-O3 -Wall -Wextra -m64 -std=c++1y
#----------------------------------------------------------------------------------------------------
APPLICATION_NAME=heat_transfer
SPEED_VIDEO=1/0.07

all: imgs_folder master create_run

imgs_folder:
	mkdir -p $(IMGS_FOLDER)

master: main.o device.o Makefile
	$(NVCC) -o $(APPLICATION_NAME) main.o device.o -L $(OPENCV_LIBPATH) $(OPENCV_LIBS) $(NVCC_OPTS)

main.o: main.cc utils.h routines.cc
	g++ -c main.cc $(GCC_OPTS) -I $(OPENCV_INCLUDEPATH) -I $(CUDA_INCLUDEPATH)

device.o: device.cu utils.h
	nvcc -c device.cu $(NVCC_OPTS)

create_run:
	@echo "./game_of_life_cuda && \
	ffmpeg -r 24 -f image2 -r $(SPEED_VIDEO) -i $(IMGS_FOLDER)/%d.png -vcodec mpeg4 -y $(APPLICATION_NAME).mp4 && \
	echo \"Success! Video '$(APPLICATION_NAME).mp4' was generated\"" > run.sh \
	&& chmod +x run.sh

clean:
	rm -f run.sh *.o *.log $(IMGS_FOLDER)/*.png $(APPLICATION_NAME)