#include <iostream>
#include "Image.h"
#include "Filter.h"
#include "helpers.h"

int main()
{
    Image image = Image::fromFile("image.pgm");
    Filter gFilter = Filter::AVERAGING(5); // Gaussian filter 5x5
    Image filteredImage = image*gFilter;

    image.transformLinear().save("image-linear.pgm");
    image.binarize().save("image-binary.pgm");
    image.equalizeHistogram().save("image-equalized.pgm");
    filteredImage.save("image-filtered(gauss 5x5).pgm");

    std::cout << "Done !" << std::endl;

    return 0;
}