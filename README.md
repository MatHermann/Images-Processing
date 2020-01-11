# Image processing algorithms
## List of implemented algorithms
- [Linear transformation](#linear-transformation)  
- [Linear transformation with saturation](#linear-transformation-with-saturation)  
- [Histogram equalization](#histogram-equalization)  
- [Arithmetic operations (ADDITION, SUBTRACTION, MULTIPLICATION)](#arithmetic-operations-addition-subtraction-multiplication) 
- [Nearest neighbor interpolation (zoom)](#nearest-neighbor-interpolation-zoom)  
- [Convolution product](#convolution-product)  
- [Gaussian filter](#gaussian-filter)  
- [Averaging filter](#averaging-filter)  
- [Median filter](#median-filter)  
- [Robert's filters](#roberts-filters)  
- [Prewitt's filters](#prewitts-filters)  
- [Sobel's filters](#sobels-filters)  
- [Laplace's Filters](#laplace-filters)  
- [Segmentation in 2 classes: Otsu's algorithm](#segmentation-in-2-classes-otsus-algorithm)  
- [Logical operations (NOT, AND, OR, XOR)](#logical-operations-not-and-or-xor)  
- [Erosion](#erosion)  
- [Expansion](#expansion)  
- [Opening](#opening)  
- [Closing](#closing)  
- [Internal gradient  ](#internal-gradient)
- [External gradient  ](#external-gradient)
- [morphological gradient](#morphological-gradient)

## Quick example

``` c++
#include <iostream>  
#include "Image.h"  
#include "Filter.h"  
#include "helpers.h"  
  
int main()  
{  
    Image image = Image::fromFile("image.pgm");  
    Filter gFilter = Filter::GAUSS(5); // Gaussian filter 5x5  
    Image filteredImage = image*gFilter;  
  
    image.transformLinear().save("image-linear.pgm");  
    image.binarize().save("image-binary.pgm");  
    image.equalizeHistogram().save("image-equalized.pgm");  
    filteredImage.save("image-filtered(gauss 5x5).pgm");  
  
    std::cout << "Done !" << std::endl;  
  
    return 0;  
}
```
## Note
All this was made to work on grayscale images in PGM (version P2) format.
If you want to use this on images of other format, you must rewrite these methods:

 - `Image::fromFile(std::string filename);`
 - `Image::save(std::string filename);`

## Setting
Before working with these files, you must change the prefix (defined in **helpers.cpp** file. 9th line) to add in images paths.
``` c++
std::string e(const std::string &filename)  
{  
    return R"(C:\images\)" + filename;  
}
```
You can replace the **"C:\images\\"** by any other prefix you want, or just return **filename** if you want to specify full path while loading an image.

## Usage
### Linear transformation
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.transformLinear();
result.save("image-transformLinear.pgm");
```

### Linear transformation with saturation
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.transformLinear(100, 200);
result.save("image-transformLinear.pgm");
```

### Histogram equalization
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.equalizeHistogram();
result.save("image-equalized.pgm");
```

### Arithmetic operations (ADDITION, SUBTRACTION, MULTIPLICATION)  
``` c++
Image image1 = Image::fromFile("image1.pgm");
Image image2 = Image::fromFile("image2.pgm");
Image _double = 2 * image1;
Image sum = image1 + image2;
Image result = _double - sum;
result.save("arithmetic.pgm");
```

### Nearest neighbor interpolation (zoom)  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.nearestNeighbourInterpolation(8); // zoom x 8
result.save("image-zoom x 8.pgm");
```

### Convolution product  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter = Filter::GAUSS(5); // Gaussian filter 5x5
Image result = image * filter; // Convolution product
result.save("image-filtered(gauss 5x5).pgm");
```

### Gaussian filter  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter = Filter::GAUSS(5); // Gaussian filter 5x5
Image result = image * filter; // Convolution product
result.save("image-filtered(gauss 5x5).pgm");
```

### Averaging filter  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter = Filter::AVERAGING(5); // Averaging filter 5x5
Image result = image * filter; // Convolution product
result.save("image-filtered(averaging 5x5).pgm");
```

### Median filter  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.median(5); // Median filter 5x5
result.save("image-median.pgm");
```

### Robert's filters  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter1 = Filter::ROBERT_X();
Filter filter2 = Filter::ROBERT_Y();
Image vImage = image * filter1;
Image hImage = image * filter2;
Image result = hImage + vImage;
result.save("image-filtered(robert).pgm");
```

### Prewitt's filters  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter1 = Filter::PREWITT_X();
Filter filter2 = Filter::PREWITT_Y();
Image vImage = image * filter1;
Image hImage = image * filter2;
Image result = hImage + vImage;
result.save("image-filtered(prewitt).pgm");
```

### Sobel's filters  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter1 = Filter::SOBEL_X();
Filter filter2 = Filter::SOBEL_Y();
Image vImage = image * filter1;
Image hImage = image * filter2;
Image result = hImage + vImage;
result.save("image-filtered(sobel).pgm");
```

### Laplace's Filters  
``` c++
Image image = Image::fromFile("image.pgm");
Filter filter1 = Filter::LAPLACE1();
Filter filter2 = Filter::LAPLACE2();
Image result1 = image * filter1;
Image result2 = image * filter2;
result.save("image-filtered(laplace1).pgm");
result.save("image-filtered(laplace2).pgm");
```

### Segmentation in 2 classes: Otsu's algorithm  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.binarize();
result.save("image-binary.pgm");
```

### Logical operations (NOT, AND, OR, XOR)  
``` c++
Image image1 = Image::fromFile("image1.pgm");
Image image2 = Image::fromFile("image2.pgm");
Image _not = !image1; // Not
Image _and = _not && image2; // And
Image _or = _and || image1; // Or
Image result = _or ^ image2; // Xor
result.save("logical.pgm");
```

### Erosion  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.erode(CONNEXITY_4()); // Erode using 4-connexity
result.save("image-eroded.pgm");
```

### Expansion  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.erode(CONNEXITY_8()); // Expand using 8-connexity
result.save("image-expanded.pgm");
```

### Opening  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.erode(CONNEXITY_8()); // Open using 8-connexity
result.save("image-open.pgm");
```

### Closing  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.close(CONNEXITY_4()); // Close using 4-connexity
result.save("image-closed.pgm");
```

### Internal gradient  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.internalGradient(CONNEXITY_8());
result.save("image-internalGradient.pgm");
```

### External gradient  
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.externalGradient(CONNEXITY_4());
result.save("image-externalGradient.pgm");
```

### Morphological gradient
``` c++
Image image = Image::fromFile("image.pgm");
Image result = image.morphologicalGradient(CONNEXITY_8());
result.save("image-morphologicalGradient.pgm");
```
