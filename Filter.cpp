//
// Created by @mathermann on 23/01/2019.
//

#include <sstream>
#include "Filter.h"
#include "helpers.h"

Filter::Filter()
{
    size = 0;
}

Filter::Filter(int size)
{
    this->size = size;
    this->matrix = new_matrix<double>(size);
}

Filter::Filter(const Filter& filter)
{
    size = filter.size;
    matrix = clone_matrix(filter.matrix, size);
}

Filter::~Filter()
{
    free_matrix(matrix, size);
}

int Filter::getSize()
{
    return size;
}

double** Filter::getMatrix()
{
    return matrix;
}

std::string Filter::toString()
{
    std::stringstream stream;

    stream << "-----[ Filter ]-----" << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            stream << matrix[i][j] << "\t";
        }
        stream << std::endl;
    }
    stream << "--------------------" << std::endl;

    return stream.str();
}

double* Filter::operator[](int i)
{
    return matrix[i];
}

std::ostream &operator<<(std::ostream &out, Filter& filter)
{
    out << filter.toString();

    return out;
}

Filter Filter::GAUSS(int size, float sigma)
{
    Filter filter(size);

    int t = size / 2;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            filter[i][j] = gauss(i-t, j-t, sigma);
        }
    }

    return Filter(filter);
}

Filter Filter::GAUSS(int size)
{
    return GAUSS(size, size / 5.0f);
}

Filter Filter::AVERAGING(int size)
{
    Filter filter(size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            filter[i][j] = 1.0/(size*size);
        }
    }

    return Filter(filter);
}

Filter Filter::MAKE_2(int matrix[][2])
{
    Filter filter(2);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            filter[i][j] = matrix[i][j];
        }
    }

    return filter;
}

Filter Filter::MAKE_3(int matrix[3][3])
{
    Filter filter(3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            filter[i][j] = matrix[i][j];
        }
    }

    return filter;
}

Filter Filter::ROBERT_X()
{
    int matrix[2][2] = {
            {1, 0},
            {0, -1},
    };
    return MAKE_2(matrix);
}

Filter Filter::ROBERT_Y()
{
    int matrix[2][2] = {
            {0, 1},
            {-1, 0},
    };
    return MAKE_2(matrix);
}

Filter Filter::PREWITT_X()
{
    int matrix[3][3] = {
            {-1, -1, -1},
            {0, 0, 0},
            {1, 1, 1},
    };
    return MAKE_3(matrix);
}

Filter Filter::PREWITT_Y()
{
    int matrix[3][3] = {
            {-1, 0, 1},
            {-1, 0, 1},
            {-1, 0, 1},
    };
    return MAKE_3(matrix);
}

Filter Filter::SOBEL_X()
{
    int matrix[3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1},
    };
    return MAKE_3(matrix);
}

Filter Filter::SOBEL_Y()
{
    int matrix[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1},
    };
    return MAKE_3(matrix);
}

Filter Filter::LAPLACE1()
{
    int matrix[3][3] = {
            {0, 1, 0},
            {1, -4, 1},
            {0, 1, 0},
    };
    return MAKE_3(matrix);
}

Filter Filter::LAPLACE2()
{
    int matrix[3][3] = {
            {1, 1, 1},
            {1, -8, 1},
            {1, 1, 1},
    };
    return MAKE_3(matrix);
}
