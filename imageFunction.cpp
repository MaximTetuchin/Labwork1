/* Maxim Tetuchin tetuhin@inbox.ru | st128993@student.spbu.ru
Lab work 1
*/
#include "imageFunction.hpp"
//! Constructor
image::image(int width,int height, std::string path):
    _width(width),
    _height(height),   
    _path(path){
    _imageMatrix = new uint8_t[_width * _height]();
    }

//! Destructor
image::~image() {
    delete[] _imageMatrix;
}

//! Methods
void image::readImage(){ //* Reading the input image
    std::ifstream openedImage(_path, std::ios::binary);
    if (!openedImage){
        std::cerr << "Check your filepath" << std::endl; //! Error
    }
    openedImage.read(reinterpret_cast<char*>(_imageMatrix),_width*_height);
    openedImage.close();
}

void image::rotateImage(){ //* Rotating the image
    uint8_t* _stepMatrix = new uint8_t[_height * _width]; //& new block of memory
    //&  _imageMatrix right now is 1D array, so....
    for (int i = 0;i<_height;i++){ 
        for (int j = 0; j < _width; j++) {
            _stepMatrix[j*_height + (_height-1-i)] = _imageMatrix[i*_width+j];
        }
    }
    delete[] _imageMatrix;
    _imageMatrix = _stepMatrix;
    std::swap(_height,_width); //* Because of matrix rotation
}

void image::gauss() {
    uint8_t* tempMatrix = new uint8_t[_height * _width]();
    int kernelSize = 20;
    double sigma = _sigma;
    double pi = 3.14;
    
    int center = kernelSize / 2;
    std::vector<double> kernel1D(kernelSize);
    double sum = 0.0;
    //! Kernel creation
    for (int x = 0; x < kernelSize; ++x) {
        int offset = x - center;
        kernel1D[x] = exp(-(offset * offset) / (2 * sigma * sigma)) / (sigma * sqrt(2 * pi));
        sum += kernel1D[x];
    }
    //! kernel normalization
    for (int x = 0; x < kernelSize; ++x) {
        kernel1D[x] /= sum;
    }
    //! Usage of 20x1 kernel
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            double pixelValue = 0.0;
            for (int k = -center; k <= center; ++k) {
                int originalJ = std::max(0, std::min(j + k, _width - 1));
                pixelValue += kernel1D[k + center] * _imageMatrix[i * _width + originalJ];
            }
            tempMatrix[i * _width + j] = static_cast<uint8_t>(std::min(std::max(pixelValue, 0.0), 255.0));
        }
    }

    //! Usage of 1x20 kernel
    for (int j = 0; j < _width; ++j) {
        for (int i = 0; i < _height; ++i) {
            double pixelValue = 0.0;
            for (int k = -center; k <= center; ++k) {
                int originalI = std::max(0, std::min(i + k, _height - 1));
                pixelValue += kernel1D[k + center] * tempMatrix[originalI * _width + j];
            }
            _imageMatrix[i * _width + j] = static_cast<uint8_t>(std::min(std::max(pixelValue, 0.0), 255.0));
        }
    }
    delete[] tempMatrix; //! Clean the memory
}

void image::saveToRaw(std::string outputFileName){ //* Saving the RAW image
    std::ofstream matrixOutput (outputFileName,std::ios::binary);
    matrixOutput.write(reinterpret_cast<char*>(_imageMatrix),(_height*_width));
}

void image::checkMatrix(){ //* Debug function
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) { 
            std::cout << static_cast<int>(_imageMatrix[i * _width + j]) << " ";
        }
        std::cout << std::endl; 
    }
}

void image::add_pixels() {
    int newHeight = _height + 4;
    int newWidth = _width + 4; 
    uint8_t* _stepMatrix = new uint8_t[newHeight * newWidth]();

    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j) {
            if (i < 2 || i >= newHeight - 2 || j < 2 || j >= newWidth - 2) {
                //! is the pixel located near the border
                int originalI = std::max(0, std::min(i - 2, _height - 1)); 
                int originalJ = std::max(0, std::min(j - 2, _width - 1));
                _stepMatrix[i * newWidth + j] = _imageMatrix[originalI * _width + originalJ];
            } else {
                _stepMatrix[i * newWidth + j] = _imageMatrix[(i - 2) * _width + (j - 2)];
            }
        }
    }
    delete[] _imageMatrix;
    _imageMatrix = _stepMatrix;
    _width = newWidth;
    _height = newHeight;
}