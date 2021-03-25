transformPath=./cmake-build-debug/MedianFilter
folder=transformedImages
mkdir "$folder"
(
cd $folder
mkdir archivos
)
for file in archivos/*;
do
  newPath="$folder/$file"
  $transformPath $file $newPath
done