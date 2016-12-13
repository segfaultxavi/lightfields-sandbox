# $1 input file (Bush.raw.reorder.nv12)
# $2 output file (out/Bush.h264-I)
for i in 500 1000 2000 4000 6000 8000 10000 20000 50000
do
  gst-launch-1.0 filesrc location=$1 ! videoparse width=624 height=432 format=nv12 ! videoconvert ! video/x-raw,format=I420 ! x265enc bitrate=$i speed-preset=1 ! filesink location=$2-$i
  gst-launch-0.10 filesrc location=$2-$i ! fluh265dec ! ffmpegcolorspace ! "video/x-raw-yuv,format=(fourcc)NV12" ! filesink location=$2-$i.dec
done

ls -v -l $2* | grep -v "\.dec"
for i in 500 1000 2000 4000 6000 8000 10000 20000 50000
do
  ./psnr $1 $2-$i.dec
done

