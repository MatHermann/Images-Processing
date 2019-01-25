//
// Created by @mathermann on 22/01/2019.
//

#ifndef IMAGES_PROCESSING_IMAGE_H
#define IMAGES_PROCESSING_IMAGE_H

class Image
{
    private:
        int row;
        int col;
        int maxGray;
        int** matrix;

    public:
        Image();
        Image(int row, int col, int maxGray);
        Image(const Image &image);
        ~Image();

        int getRow();
        int getCol();
        int getMaxGray();
        int** getMatrix();

        static Image fromFile(const std::string &filename);
        void save(const std::string &filename);
        Image& normalize();
        Image clone();
        int* histogram();
        std::string toString();

        int* operator[](int i); // returns matrix[i]
        Image operator!(); // returns !image1.binarize()

        friend std::ostream& operator<<(std::ostream &out, Image& image); // out << image
        friend Image operator*(double ratio, Image& image); // returns ratio x image
        friend Image operator*(Image& image, double ratio); // returns ratio x image
        friend Image operator/(Image& image, double ratio); // returns image / ratio
        friend Image operator+(Image& image1, Image& image2); // returns image1 + image2
        friend Image operator-(Image& image1, Image& image2); // returns image1 - image2

        int threshold(); // Otsu algorithm
        Image binarize(); // binarization
        Image transformLinear(int s_min=-1, int s_max=-1);
        Image equalizeHistogram();
        Image nearestNeighbourInterpolation(); // zoom x4
        Image nearestNeighbourInterpolation(int factor); // zoom x factor
        Image median(int size);
        Image erode(int** connexity);
        Image expand(int** connexity);
        Image open(int** connexity);
        Image close(int** connexity);
        Image internalGradient(int** connexity);
        Image externalGradient(int** connexity);
        Image morphologicalGradient(int** connexity);
};


#endif //IMAGES_PROCESSING_IMAGE_H
