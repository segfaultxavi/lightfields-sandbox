# $1 input file (Bush.raw.reorder.nv12)
# $2 output file (out/Bush.h264-I)
for i in 500 1000 2000 4000 6000 8000 10000 20000 50000
do
  gst-launch-1.0 filesrc location=$1 ! videoparse width=624 height=432 format=nv12 ! x264enc bitrate=$i key-int-max=8 bframes=1 b-adapt=0 ! filesink location=$2-$i
  gst-launch-0.10 filesrc location=$2-$i ! fluh264dec ! ffmpegcolorspace ! "video/x-raw-yuv,format=(fourcc)NV12" ! filesink location=$2-$i.dec
done

ls -v -l $2* | grep -v "\.dec"
for i in 500 1000 2000 4000 6000 8000 10000 20000 50000
do
  ./psnr $1 $2-$i.dec
done

