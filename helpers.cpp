//
// Created by @mathermann on 23/01/2019.
//

#include <vector>
#include <algorithm>
#include "helpers.h"

std::string e(const std::string &filename)
{
    return R"(C:\images\)" + filename;
}

double gauss(int x, int y, float sigma)
{
    double sigma2 = sigma * sigma;
    return exp(-(x*x + y*y)/(2*sigma2))/(2*M_PI*sigma2);
}

int vector_median(std::vector<int> list)
{
    size_t size = list.size();
    size_t size_2 = size / 2;

    if (size == 0)
    {
        return 0;  // Undefined, really.
    }
    else
    {
        std::sort(list.begin(), list.end());
        return size % 2 == 0
               ? (list[size_2 - 1] + list[size_2]) / 2
               : list[size_2];
    }
}

Image operator*(Image &image, Filter &filter) // returns image * filter
{
    Image result = image.clone();

    for (int i = 0; i < image.getRow(); ++i) {
        for (int j = 0; j < image.getCol(); ++j) {
            result[i][j] = 0;

            for (int u = 0; u < filter.getSize() && u <= i; ++u) {
                for (int v = 0; v < filter.getSize() && v <= j; ++v) {
                    result[i][j] += (int)(image[i-u][j-v] * filter[u][v]);
                }
            }
        }
    }

    return result.normalize();
}

Image operator&&(Image &image1, Image &image2)
{
    Image img1 = image1.binarize(), img2 = image2.binarize();

    Image result = img1.clone();

    for (int i = 0; i < img1.getRow(); ++i) {
        for (int j = 0; j < img1.getCol(); ++j) {
            result[i][j] =  img1[i][j] == img1.getMaxGray() && img2[i][j] == img2.getMaxGray() ? 255 : 0;
        }
    }

    return result.normalize();
}

Image operator||(Image &image1, Image &image2)
{
    Image img1 = image1.binarize(), img2 = image2.binarize();

    Image result = img1.clone();

    for (int i = 0; i < img1.getRow(); ++i) {
        for (int j = 0; j < img1.getCol(); ++j) {
            result[i][j] =  img1[i][j] == img1.getMaxGray() || img2[i][j] == img2.getMaxGray() ? 255 : 0;
        }
    }

    return result.normalize();
}

Image operator^(Image &image1, Image &image2)
{
    Image img1 = image1.binarize(), img2 = image2.binarize();

    Image result = img1.clone();

    for (int i = 0; i < img1.getRow(); ++i) {
        for (int j = 0; j < img1.getCol(); ++j) {
            result[i][j] =  img1[i][j] == img1.getMaxGray() ^ img2[i][j] == img2.getMaxGray() ? 255 : 0;
        }
    }

    return result.normalize();
}

int** CONNEXITY_4()
{
    int row = 4, col = 2;
    int** matrix = new_matrix(row+1, 2);
    int data[][2] = {
            {4},    //length
            {-1, 0}, {1, 0},
            {0, -1}, {0, 1}
    };
    matrix[0][0] = data[0][0];
    for (int i = 1; i <= row; ++i) {
        for (int j = 0; j < col; ++j) {
            matrix[i][j] = data[i][j];
        }
    }
    return matrix;
}

int** CONNEXITY_8()
{
    int row = 8, col = 2;
    int** matrix = new_matrix(row+1, 2);
    int data[][2] = {
            {8},    //length
            {-1, 0}, {1, 0}, {0, -1}, {0, 1},
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    matrix[0][0] = data[0][0];
    for (int i = 1; i <= row; ++i) {
        for (int j = 0; j < col; ++j) {
            matrix[i][j] = data[i][j];
        }
    }
    return matrix;
}