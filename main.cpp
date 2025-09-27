/**
 * @file main.cpp
 * @brief Main application for multi-threaded image processing
 * @author Maxim Tetuchin
 * @date 2025
 * @version 1.0
 * 
 * Lab work 1: Multithreaded version for processing multiple images
 */

#include "imageFunction.hpp"
#include <atomic>
#include <thread>
#include <vector>
#include <memory>
#include <iostream>

/**
 * @brief Atomic counter for processed images
 */
std::atomic<int> imageCounter(0);

/**
 * @brief Processes a single image with rotation and Gaussian blur
 * @param img Unique pointer to image object to process
 * 
 * Reads image, rotates it, applies Gaussian blur, and saves both results.
 * Each processed image gets a unique ID from the atomic counter.
 */
void processImage(std::unique_ptr<image> img) {
    int id = imageCounter.fetch_add(1);
    
    std::string rotatedPath = "Images/rotatedIMG_" + std::to_string(id) + ".raw";
    std::string gaussPath = "Images/gaussIMG_" + std::to_string(id) + ".raw";
    
    img->readImage();
    img->vecToMat();
    img->matRotate();
    img->matToVec();
    img->saveToRaw(rotatedPath);
    img->gauss(9);
    img->matToVec();
    img->saveToRaw(gaussPath);
    
    std::cout << "Processed image " << id << std::endl;
}

/**
 * @brief Main function for multi-image processing
 * @return int Exit status (0 for success)
 * 
 * Prompts user for number of images and their parameters, then processes
 * them concurrently using multiple threads.
 */
int main(){
    int numImages;
    std::cout << "Enter number of images to process: ";
    std::cin >> numImages;
    
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<image>> images;
    
    for (int i = 0; i < numImages; ++i) {
        int width, height;
        std::string path;
        
        std::cout << "Image " << i + 1 << ":\n";
        std::cout << "Enter width: ";
        std::cin >> width;
        std::cout << "Enter height: ";
        std::cin >> height;
        std::cout << "Enter path: ";
        std::cin >> path;
        
        images.push_back(std::make_unique<image>(width, height, path));
        threads.emplace_back(processImage, std::move(images.back()));
    }
    
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    std::cout << "All images processed successfully!" << std::endl;
    return 0;
}
