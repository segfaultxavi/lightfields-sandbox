# $1 Bush.raw.reorder
gst-launch-1.0 filesrc location=$1 ! videoparse width=625 height=434 format=rgb ! videoconvert ! videoscale ! video/x-raw,width=624,height=432,format=NV12 ! filesink location=$1.nv12

