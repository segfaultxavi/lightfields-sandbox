# $1 input file (Bush.skip.jpg)
# $2 original file (Bush.raw.reorder.nv12)
if [[ $1 == *"jpg"* ]]
then
  vals="10 20 30 40 50 60 70 80 90 100"
else
  vals="500 1000 2000 4000 6000 8000 10000 20000 50000"
fi

for i in $vals
do
  ./interp $1-$i.dec 624 432 15 15 $1-$i.dec.interp
done

ls -v -l $1* | grep -v "\.dec"
for i in $vals
do
  ./psnr $1-$i.dec.interp $2
done

