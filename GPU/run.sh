./heat_cuda && ffmpeg -r 24 -f image2 -r 1/0.07 -i imgs/%d.png -vcodec mpeg4 -y heat_cuda.mp4 > ffmpeg.log 2> ffmpeg.err && echo "Success! Video 'heat_cuda.mp4' was generated"
