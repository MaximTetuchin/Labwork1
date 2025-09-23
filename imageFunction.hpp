/**
 * @file imageFunction.hpp
 * @brief Image processing class definition
 * @author Maxim Tetuchin
 * @date 2025
 * @version 1.0
 */

#ifndef IMAGE_FUNCTION_HPP
#define IMAGE_FUNCTION_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <memory>
#include <thread>

/**
 * @class image
 * @brief Class for image processing operations including rotation and Gaussian blur
 *
 * This class provides functionality to read, process, and save RAW format images.
 * It supports image rotation and Gaussian blur filtering with single or multi-threaded processing.
 */
class image {
public:
    /**
     * @brief Constructor for image class
     * @param width Width of the image in pixels
     * @param height Height of the image in pixels
     * @param path File path to the source image
     */
    image(int width, int height, std::string path);

    /**
     * @brief Destructor for image class
     */
    ~image();

    // Public methods
    /**
     * @brief Reads image data from file into vector
     */
    void readImage();

    /**
     * @brief Rotates the image 90 degrees clockwise
     */
    void matRotate();

    /**
     * @brief Applies Gaussian blur with default kernel size
     */
    void gauss();

    /**
     * @brief Saves processed image to RAW format file
     * @param outputFileName Output file path
     */
    void saveToRaw(std::string outputFileName);

    /**
     * @brief Converts 1D vector representation to 2D matrix
     */
    void vecToMat();

    /**
     * @brief Converts 2D matrix representation back to 1D vector
     */
    void matToVec();

    /**
     * @brief Applies Gaussian blur with specified kernel size (single-threaded)
     * @param kernelSize Size of the Gaussian kernel (must be odd)
     */
    void gauss(int kernelSize);

    /**
     * @brief Applies Gaussian blur with specified kernel size and thread count (multi-threaded)
     * @param kernelSize Size of the Gaussian kernel (must be odd)
     * @param numThreads Number of threads to use for processing
     */
    void gauss(int kernelSize, unsigned int numThreads);

    /**
     * @brief Debug method to print matrix dimensions
     */
    void getLen();

private:
    int _width;                                     ///< Image width in pixels
    int _height;                                    ///< Image height in pixels
    std::string _path;                              ///< Source image file path
    double _sigma = 10;                             ///< Sigma parameter for Gaussian blur
    std::vector<uint8_t> _imageVector;              ///< 1D vector representation of image data
    std::vector<std::vector<uint8_t>> _imageMatrix; ///< 2D matrix representation of image data
};

#endif // IMAGE_FUNCTION_HPP
