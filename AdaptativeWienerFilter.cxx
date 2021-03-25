/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainT1Slice.png}
//    OUTPUTS: {NeighborhoodIterators1a.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example uses the \doxygen{NeighborhoodIterator} to implement a simple
// Sobel edge detection algorithm \cite{Gonzalez1993}.  The algorithm uses the
// neighborhood iterator to iterate through an input image and calculate a
// series of finite difference derivatives.  Since the derivative results
// cannot be written back to the input image without affecting later
// calculations, they are written instead to a second, output image.  Most
// neighborhood processing algorithms follow this read-only model on their
// inputs.
//
// We begin by including the proper header files.  The
// \doxygen{ImageRegionIterator} will be used to write the results of
// computations to the output image.  A const version of the neighborhood
// iterator is used because the input image is read-only.
//
// Software Guide : EndLatex

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

// Software Guide : BeginCodeSnippet
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
// Software Guide : EndCodeSnippet
using PixelType = float;
using ImageType = itk::Image<PixelType, 3>;
using ReaderType = itk::ImageFileReader<ImageType>;

using NeighborhoodIteratorType = itk::ConstNeighborhoodIterator<ImageType>;
using IteratorType = itk::ImageRegionIterator<ImageType>;


std::vector<PixelType> getPixelValuesFromNeighborhood(std::vector<NeighborhoodIteratorType::OffsetType> vector, NeighborhoodIteratorType it);
std::vector<NeighborhoodIteratorType::OffsetType> getDynamicNeighborhood(int radius);
PixelType getMedian(PixelType pix1, PixelType pix2, PixelType pix3, PixelType pix4, PixelType pix5, PixelType pix6, PixelType pix7, PixelType pix8);


void setZValues(std::vector<PixelType> vector, PixelType &med, PixelType &min, PixelType &max);
void setMeanAndVariance(std::vector<PixelType> vector, PixelType& mean, PixelType& variance);

int
main(int argc, char ** argv)
{
    if (argc < 5)
    {
        std::cerr << "Missing parameters. " << std::endl;
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " inputImageFile outputImageFile WindowRadio NoiseVariance" << std::endl;
        return EXIT_FAILURE;
    }

    // Software Guide : BeginLatex
    //
    // The finite difference calculations
    // in this algorithm require floating point values.  Hence, we define the
    // image pixel type to be \code{float} and the file reader will
    // automatically cast fixed-point data to \code{float}.
    //
    // We declare the iterator types using the image type as
    // the template parameter. The second template parameter of the
    // neighborhood iterator, which specifies
    // the boundary condition, has been omitted because the default condition is
    // appropriate for this algorithm.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet

    // Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    //
    // The following code creates and executes the ITK image reader.
    // The \code{Update}
    // call on the reader object is surrounded by the standard \code{try/catch}
    // blocks to handle any exceptions that may be thrown by the reader.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    try
    {
        reader->Update();
    }
    catch (const itk::ExceptionObject & err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    //  We can now create a neighborhood iterator to range over the output of
    //  the reader. For Sobel edge-detection in 2D, we need a square iterator
    //  that extends one pixel away from the neighborhood center in every
    //  dimension.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    int windowRadio = atoi(argv[3]);
    float noiseVariance = atof(argv[4]);
    NeighborhoodIteratorType::RadiusType radius;
    radius.Fill(windowRadio);
    NeighborhoodIteratorType it(
            radius, reader->GetOutput(), reader->GetOutput()->GetRequestedRegion());
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // The following code creates an output image and iterator.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    ImageType::Pointer output = ImageType::New();
    output->SetRegions(reader->GetOutput()->GetRequestedRegion());
    output->Allocate();

    IteratorType out(output, reader->GetOutput()->GetRequestedRegion());
    // Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    //
    // Sobel edge detection uses weighted finite difference calculations to
    // construct an edge magnitude image.  Normally the edge magnitude is the
    // root sum of squares of partial derivatives in all directions, but for
    // simplicity this example only calculates the $x$ component. The result is
    // a derivative image biased toward maximally vertical edges.
    //
    // The finite differences are computed from pixels at six locations in the
    // neighborhood.  In this example, we use the iterator \code{GetPixel()}
    // method to query the values from their offsets in the neighborhood.
    // The example in Section~\ref{sec:NeighborhoodExample2} uses convolution
    // with a Sobel kernel instead.
    //
    // Six positions in the neighborhood are necessary for the finite difference
    // calculations. These positions are recorded in \code{offset1} through
    // \code{offset6}.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // It is equivalent to use the six corresponding integer array indices
    // instead. For example, the offsets \code{(-1,-1)} and \code{(1, -1)} are
    // equivalent to the integer indices \code{0} and \code{2}, respectively.
    //
    // The calculations are done in a \code{for} loop that moves the input and
    // output iterators synchronously across their respective images.  The
    // \code{sum} variable is used to sum the results of the finite differences.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
    {
        PixelType mean, variance;
        std::vector<NeighborhoodIteratorType::OffsetType> neighborhood = getDynamicNeighborhood(windowRadio);
        std::vector<PixelType> pixelValues = getPixelValuesFromNeighborhood(neighborhood, it);
        setMeanAndVariance(pixelValues, mean, variance);
        PixelType currentPixel = it.GetCenterPixel();
        PixelType weiner = mean + ((variance - noiseVariance)/variance)*(currentPixel - mean);
        out.Set(weiner);
    }
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // The last step is to write the output buffer to an image file.  Writing is
    // done inside a \code{try/catch} block to handle any exceptions.  The
    // output is rescaled to intensity range $[0, 255]$ and cast to unsigned
    // char so that it can be saved and visualized as a PNG image.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    using WritePixelType = unsigned char;
    using WriteImageType = itk::Image<WritePixelType, 3>;
    using WriterType = itk::ImageFileWriter<WriteImageType>;

    using RescaleFilterType =
    itk::RescaleIntensityImageFilter<ImageType, WriteImageType>;

    RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    rescaler->SetInput(output);

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(argv[2]);
    writer->SetInput(rescaler->GetOutput());
    try
    {
        writer->Update();
    }
    catch (const itk::ExceptionObject & err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    // Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    //
    // The center image of Figure~\ref{fig:NeighborhoodExamples1} shows the
    // output of the Sobel algorithm applied to
    // \code{Examples/Data/BrainT1Slice.png}.
    //
    // \begin{figure} \centering
    // \includegraphics[width=0.3\textwidth]{BrainT1Slice}
    // \includegraphics[width=0.3\textwidth]{NeighborhoodIterators1a}
    // \includegraphics[width=0.3\textwidth]{NeighborhoodIterators1b}
    // \itkcaption[Sobel edge detection results]{Applying the Sobel operator in
    // different orientations to an MRI image (left) produces $x$ (center) and
    // $y$ (right) derivative images.} \protect\label{fig:NeighborhoodExamples1}
    // \end{figure}
    //
    // Software Guide : EndLatex


    return EXIT_SUCCESS;
}

PixelType substractMean (PixelType pixelValue, PixelType mean){
    return pixelValue - mean;
}

void setMeanAndVariance(std::vector<PixelType> vector, PixelType& mean, PixelType& variance){
    auto n = vector.size();
    std::vector<PixelType> meanSubstractedS2;

    if ( n != 0) {
        mean = accumulate( vector.begin(), vector.end(), 0.0) / n;
    }

    for (PixelType n : vector) {
        meanSubstractedS2.push_back(pow(n - mean, 2));
    }

    variance = accumulate( meanSubstractedS2.begin(), meanSubstractedS2.end(), 0.0) / n;

}

void setZValues(std::vector<PixelType> vector, PixelType& med, PixelType& min, PixelType& max) {
    size_t size = vector.size();

    if (size == 0)
    {
        return;  // Undefined, really.
    }
    else
    {
        sort(vector.begin(), vector.end());
        if (size % 2 == 0)
        {
            med = (vector[size / 2 - 1] + vector[size / 2]) / 2;
        }
        else
        {
            med = vector[size / 2];
        }
        min = vector.front();
        max = vector.back();
    }
}

std::vector<PixelType> getPixelValuesFromNeighborhood(std::vector<NeighborhoodIteratorType::OffsetType> vector, NeighborhoodIteratorType it) {
    std::vector<PixelType> pixelValues = { };
    for (NeighborhoodIteratorType::OffsetType n : vector) {
        pixelValues.push_back(it.GetPixel(n));
    }
    return pixelValues;
}

PixelType
getMedian(PixelType pix1, PixelType pix2, PixelType pix3, PixelType pix4, PixelType pix5, PixelType pix6, PixelType pix7, PixelType pix8){
    PixelType arr[] = { pix1, pix2, pix3, pix4, pix5, pix6, pix7, pix8 };
    int sizeArr = sizeof(arr);
    int n = sizeArr / sizeof(arr[0]);
    std::sort(arr, arr + n);


    PixelType median;
    if (n % 2 == 0)
        median = ((PixelType)arr[n/2] + (PixelType)arr[n/2 - 1])/2;
    else
        median = (PixelType) arr[n/2];

    return median;
}

std::vector<NeighborhoodIteratorType::OffsetType> getDynamicNeighborhood(int radius){
    std::vector<NeighborhoodIteratorType::OffsetType> neighbors = {  };
    for (int i = -1 * radius; i <= radius; ++i) {
        for (int j = -1 * radius; j <= radius; ++j) {
            neighbors.push_back({ { i, j } });
        }
    }
    return neighbors;
}


