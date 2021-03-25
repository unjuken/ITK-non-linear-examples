transformPath=./cmake-build-debug/AdaptativeWienerFilter
folder=transformedImages
mkdir "$folder"
(
cd $folder
mkdir archivos
)
for file in archivos/*;
do
  newPath="$folder/$file_1.3.nii"
  $transformPath $file $newPath 1 100
done
for file in archivos/*;
do
  newPath="$folder/$file_1.5.nii"
  $transformPath $file $newPath 1 300
done
for file in archivos/*;
do
  newPath="$folder/$file_1.7.nii"
  $transformPath $file $newPath 1 500
done
for file in archivos/*;
do
  newPath="$folder/$file_2.3.nii"
  $transformPath $file $newPath 2 100
done
for file in archivos/*;
do
  newPath="$folder/$file_2.5.nii"
  $transformPath $file $newPath 2 300
done
for file in archivos/*;
do
  newPath="$folder/$file_2.7.nii"
  $transformPath $file $newPath 2 500
done