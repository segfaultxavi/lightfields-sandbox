# $1 input file (Bush.raw.reorder.nv12)
# $2 output file (out/Bush.jpg)
for i in 10 20 30 40 50 60 70 80 90 100
do
  gst-launch-0.10 filesrc location=$1 ! videoparse width=624 height=432 format=nv12 ! ffmpegcolorspace ! jpegenc quality=$i ! filesink location=$2-$i
  gst-launch-0.10 filesrc location=$2-$i ! typefind ! jpegdec ! ffmpegcolorspace ! "video/x-raw-yuv,format=(fourcc)NV12" ! filesink location=$2-$i.dec
done

ls -v -l $2* | grep -v "\.dec"
for i in 10 20 30 40 50 60 70 80 90 100
do
  ./psnr $1 $2-$i.dec
done

