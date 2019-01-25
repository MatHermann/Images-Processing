//
// Created by @mathermann on 22/01/2019.
//

#ifndef IMAGES_PROCESSING_HELPERS_H
#define IMAGES_PROCESSING_HELPERS_H

#include <iostream>
#include <cmath>
#include <vector>
#include "Image.h"
#include "Filter.h"

template <typename T=int> T** new_matrix(int row, int col)
{
    auto ret_val = new T*[row];

    for (int i = 0; i < row; ++i) {
        ret_val[i] = new T[col];
    }

    return ret_val;
}

template <typename T=int> T** new_matrix(int size)
{
    return new_matrix<T>(size, size);
}

template <typename T=int> T** clone_matrix(T** matrix, int row, int col)
{
    T** ret_val = new_matrix<T>(row, col);

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            ret_val[i][j] = matrix[i][j];
        }
    }

    return ret_val;
}

template <typename T=int> T** clone_matrix(T** matrix, int size)
{
    return clone_matrix<T>(matrix, size, size);
}

template <typename T=int> void free_matrix(T** matrix, int row)
{
    for (int i = 0; i < row; ++i) {
        delete matrix[i];
    }
    delete matrix;
}

int** CONNEXITY_4(); // returns 4-connexity mask

int** CONNEXITY_8(); // returns 8-connexity mask

Image operator*(Image &image, Filter &filter); // returns image * filter
Image operator&&(Image &image1, Image &image2); // returns image1 and image2
Image operator||(Image &image1, Image &image2); // returns image1 or image2
Image operator^(Image &image1, Image &image2); // returns image1 xor image2

std::string e(const std::string &filename);

double gauss(int x, int y, float sigma); // returns exp(-(x²+y²)/2*sigma²)/2*pi*sigma²
int vector_median(std::vector<int> list); // returns median value

#endif //IMAGES_PROCESSING_HELPERS_H
