/**
 * @file imageFunction.cpp
 * @brief Image processing class implementation
 * @author Maxim Tetuchin
 * @date 2025
 * @version 1.0
 */

#include "imageFunction.hpp"

/**
 * @brief Constructs an image object with specified parameters
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param path Path to source image file
 */
image::image(int width, int height, std::string path):
    _width(width),
    _height(height),   
    _path(path){    
    _imageVector = std::vector<uint8_t>(_width * _height);
    _imageMatrix = std::vector<std::vector<uint8_t>>(_height, std::vector<uint8_t>(_width, 0));
}

/**
 * @brief Destructor clears allocated memory
 */
image::~image() {
    _imageVector.clear();
    _imageMatrix.clear();
}

/**
 * @brief Reads image data from binary file into 1D vector
 * @throws std::runtime_error if file cannot be opened
 */
void image::readImage(){
    std::ifstream openedImage(_path, std::ios::binary);
    if (!openedImage){
        std::cerr << "Check your filepath" << std::endl;
        throw std::runtime_error("Failed to open image file");
    }
    openedImage.read(reinterpret_cast<char*>(_imageVector.data()),_width*_height);
    openedImage.close();
}

/**
 * @brief Saves processed image data to RAW binary file
 * @param outputFileName Path for output file
 */
void image::saveToRaw(std::string outputFileName){
    std::ofstream matrixOutput (outputFileName,std::ios::binary);
    matrixOutput.write(reinterpret_cast<char*>(_imageVector.data()),(_height*_width));
    _imageVector.clear();
}

/**
 * @brief Converts 1D vector representation to 2D matrix
 * 
 * After conversion, the vector memory is cleared to optimize memory usage.
 */
void image::vecToMat(){ 
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            _imageMatrix[i][j] = _imageVector[i * _width + j];
        }
    }
    _imageVector.clear();
    _imageVector.shrink_to_fit();
}

/**
 * @brief Converts 2D matrix representation back to 1D vector
 * 
 * Resizes the vector to accommodate all pixel data before conversion.
 */
void image::matToVec(){
    _imageVector.resize(_height * _width);
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            _imageVector[i * _width + j] = _imageMatrix[i][j];
        }
    }
}

/**
 * @brief Rotates image 90 degrees clockwise using matrix transformation
 * 
 * Swaps image dimensions after rotation. Uses move semantics for efficiency.
 */
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

/**
 * @brief Applies Gaussian blur filter with specified kernel size (single-threaded)
 * @param kernelSize Size of Gaussian kernel (must be odd number)
 * 
 * Creates Gaussian kernel, normalizes it, and applies convolution to image matrix.
 * Uses sigma value stored in class member _sigma.
 */
void image::gauss(int kernelSize) {
    if (kernelSize % 2 == 0) {
        std::cerr << "Kernel size must be an odd number." << std::endl;
        return;
    }

    int halfSize = kernelSize / 2;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize, 0));
    double sum = 0.0;

    // Fill kernel with Gaussian values
    for (int x = -halfSize; x <= halfSize; ++x) {
        for (int y = -halfSize; y <= halfSize; ++y) {
            double value = exp(-(x * x + y * y) / (2 * _sigma * _sigma)) / (2 * M_PI * _sigma * _sigma);
            kernel[x + halfSize][y + halfSize] = value;
            sum += value;
        }
    }

    // Normalize kernel
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

/**
 * @brief Debug method to print matrix dimensions
 */
void image::getLen(){
    std::cout << _imageMatrix.size() << "\n";
}