/* Maxim Tetuchin tetuhin@inbox.ru | st128993@student.spbu.ru
Lab work 1
*/
//! Libs
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <memory>

#ifndef IMAGE
#define IMAGE
class image{
    //! Constructor and destructor
    public:
        image(int width, int height, std::string path);
        ~image();

    //! Methods
    public:
        void readImage();
        void rotateImage();
        void gauss();
        void add_pixels();
        void saveToRaw(std::string outputFileName);

        void checkMatrix(); //* Debug function

    //! Variables
    private:
        int _width;
        int _height;
        std::string _path;
        double _sigma = 10;
        uint8_t* _imageMatrix;
};  
#endif