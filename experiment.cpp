/**
 * @file experimental_main.cpp
 * @brief Experimental application for testing multi-threaded Gaussian blur performance
 * @author Maxim Tetuchin
 * @date 2025
 * @version 1.0
 * 
 * Lab work 1 - Experiment with multithreading: Performance comparison between
 * single-threaded and multi-threaded Gaussian blur implementation.
 */

#include "imageFunction.hpp"
#include <chrono>
#include <iostream>
#include <thread>

/**
 * @brief Multi-threaded Gaussian blur implementation
 * @param kernelSize Size of Gaussian kernel (must be odd)
 * @param numThreads Number of threads to use for processing
 * 
 * Divides image processing among specified number of threads and measures
 * performance compared to single-threaded version.
 */
void image::gauss(int kernelSize, unsigned int numThreads) {
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

    // Lambda function for processing image segments
    auto processPart = [&](int startRow, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
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
    };

    // Create and launch threads
    std::vector<std::thread> threads;
    int rowsPerThread = _height / numThreads;
    
    for (unsigned int t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? _height : (t + 1) * rowsPerThread;
        threads.emplace_back(processPart, startRow, endRow);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    _imageMatrix = std::move(blurredImage);
}

/**
 * @brief Experimental main function for performance testing
 * @return int Exit status (0 for success)
 * 
 * Compares processing time between single-threaded and multi-threaded
 * Gaussian blur implementations and calculates speedup ratio.
 */
int main() {
    int width, height;
    std::string path;
    
    std::cout << "Enter width of the image: ";
    std::cin >> width;
    std::cout << "Enter height of the image: ";
    std::cin >> height;
    std::cout << "Enter the path to the image (in raw format): ";
    std::cin >> path;
    
    // Create two image instances for clean experiment
    image img1(width, height, path);
    image img4(width, height, path);
    
    // Load and prepare images
    img1.readImage();
    img1.vecToMat();
    
    img4.readImage();
    img4.vecToMat();
    
    std::cout << "\n=== Experiment: Gaussian Blur Performance ===\n";
    
    // Test with 1 thread
    auto start1 = std::chrono::high_resolution_clock::now();
    img1.gauss(9, 1); // Explicitly use 1 thread
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    
    std::cout << "1 thread: " << duration1.count() << " ms\n";
    
    // Test with 4 threads
    auto start4 = std::chrono::high_resolution_clock::now();
    img4.gauss(9, 4); // Explicitly use 4 threads
    auto end4 = std::chrono::high_resolution_clock::now();
    auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(end4 - start4);
    
    std::cout << "4 threads: " << duration4.count() << " ms\n";
    
    // Calculate speedup
    double speedup = static_cast<double>(duration1.count()) / duration4.count();
    std::cout << "Speedup: " << speedup << "x\n";
    
    // Save results for verification
    img1.matToVec();
    img1.saveToRaw("Images/experiment_1thread.raw");
    
    img4.matToVec();
    img4.saveToRaw("Images/experiment_4threads.raw");
    
    std::cout << "Results saved to Images/experiment_1thread.raw and Images/experiment_4threads.raw\n";
    
    return 0;
}