//
// Created by @mathermann on 22/01/2019.
//

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "Image.h"
#include "helpers.h"

Image::Image()
{
    row = 0;
    col = 0;
    maxGray = 0;
    matrix = nullptr;
}

Image::Image(int row, int col, int maxGray)
{
    this->row = row;
    this->col = col;
    this->maxGray = maxGray;
    matrix = new_matrix(row, col);
}

Image::Image(const Image &image)
{
    row = image.row;
    col = image.col;
    maxGray = image.maxGray;
    matrix = clone_matrix(image.matrix, row, col);
}

Image::~Image()
{
    free_matrix(matrix, row);
}

int Image::getRow()
{
    return row;
}

int Image::getCol()
{
    return col;
}

int** Image::getMatrix()
{
    return matrix;
}

Image Image::fromFile(const std::string &filename)
{
    std::ifstream file(e(filename).c_str());
    if(!file)
    {
        std::cerr << "Failed to open '" << filename << " ' !" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::stringstream fileContent;

    while(getline(file, line))
    {
        if(line.at(0) != '#') // Ignoring comments
        {
            fileContent << line << std::endl;
        }
    }
    file.close();

    getline(fileContent, line);
    if(line != "P2")
    {
        std::cerr << "Wrong PGM version !" << std::endl << "Found " << line << " instead of P2." << std::endl;
        exit(EXIT_FAILURE);
    }

    Image image;
    fileContent >> image.col >> image.row >> image.maxGray;

    image.matrix = new_matrix(image.row, image.col);
    for (int i = 0; i < image.row; ++i) {
        for (int j = 0; j < image.col; ++j) {
            fileContent >> image.matrix[i][j];
        }
    }

    return Image(image);
}

void Image::save(const std::string &filename)
{
    std::ofstream file(e(filename).c_str());
    if(!file)
    {
        std::cerr << "Failed to open '" << filename << " ' !" << std::endl;
        exit(EXIT_FAILURE);
    }

    file << "P2" << std::endl;
    file << "# " << filename << std::endl;
    file << col << " " << row << std::endl;
    file << maxGray << std::endl;

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            file << matrix[i][j] << " ";
        }
        file << std::endl;
    }

    file.close();
}

Image& Image::normalize()
{
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if(matrix[i][j] < 0)
                matrix[i][j] = 0;
            else if(matrix[i][j] > maxGray)
                matrix[i][j] = maxGray;
        }
    }

    return *this;
}

Image Image::clone()
{
    return Image(*this);
}

int *Image::histogram()
{
    auto h = new int[256];

    for(int i=0; i<=255; ++i)
        h[i] = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            h[matrix[i][j]]++;
        }
    }
    return h;
}

std::string Image::toString()
{
    std::stringstream stream;

    stream << "-----[ Image ]-----" << std::endl;
    stream << "row: " << row << ", col: " << col << std::endl;
    stream << "max_gray: " << maxGray << std::endl;
    stream << "-------------------" << std::endl;

    return stream.str();
}

int* Image::operator[](int i)
{
    return matrix[i];
}

Image Image::operator!()
{
    Image result = binarize();

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result[i][j] = matrix[i][j] == 0 ? maxGray : 0;
        }
    }

    return result;
}

std::ostream &operator<<(std::ostream &out, Image& image)
{
    out << image.toString();

    return out;
}

Image operator*(double ratio, Image& image)
{
    Image result = image.clone();

    for (int i = 0; i < image.row; ++i) {
        for (int j = 0; j < image.col; ++j) {
            result.matrix[i][j] = (int)(ratio * image.matrix[i][j]);
        }
    }

    return result.normalize();
}

Image operator*(Image& image, double ratio)
{
    return ratio * image;
}

Image operator/(Image& image, double ratio)
{
    return (1/ratio) * image;
}

Image operator+(Image& image1, Image& image2)
{
    Image result = image1.clone();

    for (int i = 0; i < image1.row; ++i) {
        for (int j = 0; j < image1.col; ++j) {
            result.matrix[i][j] += image2.matrix[i][j];
        }
    }

    return result.normalize();
}

Image operator-(Image& image1, Image& image2)
{
    Image result = image1.clone();

    for (int i = 0; i < image1.row; ++i) {
        for (int j = 0; j < image1.col; ++j) {
            result.matrix[i][j] -= image2.matrix[i][j];
        }
    }

    return result.normalize();
}

Image Image::transformLinear(int s_min, int s_max)
{
    if(s_min < 0 || s_max < 0)
    {
        s_min=maxGray, s_max=0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if(matrix[i][j] > s_max)
                    s_max = matrix[i][j];
                if(matrix[i][j] < s_min)
                    s_min = matrix[i][j];
            }
        }
    }

    Image result = clone();
    int LUT[maxGray+1];

    for(int i=0; i<=maxGray; ++i)
    {
        LUT[i] = maxGray*(i - s_min)/(s_max - s_min);
    }

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.matrix[i][j] = LUT[matrix[i][j]];
        }
    }

    return result.normalize();
}

Image Image::equalizeHistogram()
{
    Image result = clone();
    int* h = histogram();
    double h_n[maxGray+1];
    double C[maxGray+1];

    for(int i=0; i<=maxGray; ++i)
    {
        h_n[i] = (1.0 * h[i]) / (row * col);

        C[i] = 0.0;
        for(int j=0; j<=i; ++j)
            C[i] += h_n[j];
    }

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result[i][j] = (int)(C[matrix[i][j]] * 255);
        }
    }

    delete h;

    return result.normalize();
}

Image Image::nearestNeighbourInterpolation()
{
    Image result = Image(2*row, 2*col, maxGray);

    for (int i = 0; i < 2*row; ++i) {
        for (int j = 0; j < 2*col; ++j) {
            result[i][j] = matrix[i/2][j/2];
        }
    }

    return result;
}

Image Image::nearestNeighbourInterpolation(int factor)
{
    if (factor == 1)
        return clone();

    return  nearestNeighbourInterpolation().nearestNeighbourInterpolation(factor / 2);
}

Image Image::median(int size)
{
    Image result = clone();

    const int t = size / 2;
    std::vector<int> values;

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            values.clear();
            for (int u = i-t; u < i+t; ++u)
            {
                for (int v = j-t; v < j+t; ++v)
                {
                    if(u>=0 && u<row && v>=0 && v<col)
                    {
                        values.push_back(matrix[u][v]);
                    }
                }
            }
            result[i][j] = vector_median(values);
        }
    }

    return result;
}

int Image::threshold() // Otsu algorithm
{
    int* h = histogram();
    double p[maxGray+1];
    double omega[maxGray+1];
    double mu[maxGray+1];
    double sigma[maxGray+1];

    for(int i=0; i<=maxGray; ++i)
    {
        p[i] = (1.0 * h[i]) / (row * col);
    }

    omega[0] = p[0];
    mu[0] = 0.0;

    for(int i=1; i<maxGray; ++i)
    {
        omega[i] = omega[i-1] + p[i];
        mu[i] = mu[i-1] + i*p[i];
    }

    int ret_value = 0;
    double max_sigma = 0.0;
    for (int i = 0; i < maxGray-1; i++) {
        if (omega[i] != 0.0 && omega[i] != 1.0)
            sigma[i] = pow(mu[maxGray-1]*omega[i] - mu[i], 2) / (omega[i]*(1.0 - omega[i]));
        else
            sigma[i] = 0.0;

        if (sigma[i] > max_sigma)
        {
            max_sigma = sigma[i];
            ret_value = i;
        }
    }

    delete h;

    return ret_value;
}

Image Image::binarize()
{
    Image result = clone();

    int _threshold = threshold();
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result[i][j] = matrix[i][j] <= _threshold ? 0 : maxGray;
        }
    }

    return result;
}

int Image::getMaxGray()
{
    return maxGray;
}

Image Image::erode(int** connexity)
{
    Image image = binarize();
    Image result = image.clone();

    int x, y;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            for (int k=1; k<=connexity[0][0]; ++k)
            {
                x = i+connexity[k][0];
                y = j+connexity[k][1];
                if (x>=0 && x<row && y>=0 && y<col)
                {
                    if (image[x][y] == 0)
                    {
                        result[i][j] = image[x][y];
                        break;
                    }
                }
            }
        }
    }

    return result;
}

Image Image::expand(int** connexity)
{
    Image image = binarize();
    Image result = image.clone();

    int x, y;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            for (int k=1; k<=connexity[0][0]; ++k)
            {
                x = i+connexity[k][0];
                y = j+connexity[k][1];
                if (x>=0 && x<row && y>=0 && y<col)
                {
                    if (image[x][y] > 0)
                    {
                        result[i][j] = image[x][y];
                        break;
                    }
                }
            }
        }
    }

    return result;
}

Image Image::open(int** connexity)
{
    return erode(connexity).expand(connexity);
}

Image Image::close(int** connexity)
{
    return expand(connexity).erode(connexity);
}

Image Image::internalGradient(int **connexity)
{
    Image image = binarize();
    Image erosion = erode(connexity);

    return image - erosion;
}

Image Image::externalGradient(int **connexity)
{
    Image image = binarize();
    Image expanded = expand(connexity);

    return expanded - image;
}

Image Image::morphologicalGradient(int **connexity)
{
    Image expanded = expand(connexity);
    Image erosion = erode(connexity);

    return expanded - erosion;
}
