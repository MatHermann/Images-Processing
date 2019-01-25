//
// Created by @mathermann on 23/01/2019.
//

#ifndef IMAGES_PROCESSING_FILTER_H
#define IMAGES_PROCESSING_FILTER_H

class Filter
{
    private:
        int size;
        double** matrix;

        static Filter MAKE_2(int matrix[2][2]);
        static Filter MAKE_3(int matrix[3][3]);

    public:
        Filter();
        Filter(int size);
        Filter(const Filter& filter);
        ~Filter();

        int getSize();
        double** getMatrix();

        std::string toString();

        double* operator[](int i); // returns matrix[i]

        friend std::ostream& operator<<(std::ostream &out, Filter& filter); // out << filter

        static Filter GAUSS(int size, float sigma);
        static Filter GAUSS(int size);
        static Filter AVERAGING(int size);

        static Filter ROBERT_X();
        static Filter ROBERT_Y();
        static Filter PREWITT_X();
        static Filter PREWITT_Y();
        static Filter SOBEL_X();
        static Filter SOBEL_Y();

        static Filter LAPLACE1();
        static Filter LAPLACE2();
};


#endif //IMAGES_PROCESSING_FILTER_H
