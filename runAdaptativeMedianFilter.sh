transformPath=./cmake-build-debug/AdaptativeMedianFilter
folder=transformedImages
mkdir "$folder"
(
cd $folder
mkdir archivos
)
for file in archivos/*;
do
  newPath="$folder/$file\_3.nii"
  $transformPath $file $newPath 3
done
for file in archivos/*;
do
  newPath="$folder/$file\_5.nii"
  $transformPath $file $newPath 5
done
for file in archivos/*;
do
  newPath="$folder/$file\_7.nii"
  $transformPath $file $newPath 7
done