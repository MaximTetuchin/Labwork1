/* Maxim Tetuchin tetuhin@inbox.ru | st128993@student.spbu.ru
Lab work 1
*/
#include "imageFunction.hpp"
//! Constructor
image::image(int width,int height, std::string path):
    _width(width),
    _height(height),   
    _path(path){    
    _imageVector = std::vector<uint8_t>(_width * _height);
    _imageMatrix = std::vector<std::vector<uint8_t>>(_height, std::vector<uint8_t>(_width, 0));
    }

//! Destructor
image::~image() {
    _imageVector.clear();
    _imageMatrix.clear();
}

//^ Methods
//! 1D Array method
void image::readImage(){ //* Reading the input image
    std::ifstream openedImage(_path, std::ios::binary);
    if (!openedImage){
        std::cerr << "Check your filepath" << std::endl; //! Error
    }
    openedImage.read(reinterpret_cast<char*>(_imageVector.data()),_width*_height);
    openedImage.close();
}

//! 1D Array method
void image::saveToRaw(std::string outputFileName){ //* Saving the RAW image 
    std::ofstream matrixOutput (outputFileName,std::ios::binary);
    matrixOutput.write(reinterpret_cast<char*>(_imageVector.data()),(_height*_width));
    _imageVector.clear();
}

//! 1D -> 2D method
void image::vecToMat(){ 
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            _imageMatrix[i][j] = _imageVector[i * _width + j];
        }
    }
    _imageVector.clear();
    _imageVector.shrink_to_fit();
    //! Vector's memory clear
}

//! 2D -> 1D method
void image::matToVec(){
    _imageVector.resize(_height * _width); //! Adding mem.
     _imageVector.resize(_height * _width);
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            _imageVector[i * _width + j] = _imageMatrix[i][j];
        }
    }
}
//! Attention!!!!
//! After using the method above we have imageMatrix and imageVector in memory.

//! 2D method
void image::matRotate(){
    std::vector<std::vector<uint8_t>> _rotatedMat = 
        std::vector<std::vector<uint8_t>>(_width, std::vector<uint8_t>(_height, 0));
    for (int i = 0; i < _height; ++i){
        for (int j = 0; j < _width; ++j){
            _rotatedMat[j][_height - 1 - i] = _imageMatrix[i][j];
        }
    }
    _imageMatrix = std::move(_rotatedMat); 
    std::swap(_height,_width);
}

//! 2D method
void image::gauss(int kernelSize) {
    if (kernelSize % 2 == 0) {
        std::cerr << "Kernel size must be an odd number." << std::endl;
        return;
    }

    int halfSize = kernelSize / 2;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize, 0));
    double sum = 0.0;

    //! Kernel fill
    for (int x = -halfSize; x <= halfSize; ++x) {
        for (int y = -halfSize; y <= halfSize; ++y) {
            double value = exp(-(x * x + y * y) / (2 * _sigma * _sigma)) / (2 * M_PI * _sigma * _sigma);
            //^ M_Pi - constant for pi
            kernel[x + halfSize][y + halfSize] = value;
            sum += value;
        }
    }

    //! Kernel normalization
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    std::vector<std::vector<uint8_t>> blurredImage = 
        std::vector<std::vector<uint8_t>>(_height, std::vector<uint8_t>(_width, 0));

    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            double pixelValue = 0.0;

            for (int ki = -halfSize; ki <= halfSize; ++ki) {
                for (int kj = -halfSize; kj <= halfSize; ++kj) {
                    int ni = i + ki; 
                    int nj = j + kj;

                    if (ni >= 0 && ni < _height && nj >= 0 && nj < _width) {
                        pixelValue += _imageMatrix[ni][nj] * kernel[ki + halfSize][kj + halfSize];
                    }
                }
            }
            blurredImage[i][j] = static_cast<uint8_t>(std::round(pixelValue));
        }
    }

    _imageMatrix = std::move(blurredImage);
}


//* Debug methods
void image::getLen(){
    std::cout<<_imageMatrix.size()<<"\n";
}