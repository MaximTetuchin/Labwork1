/* Maxim Tetuchin tetuhin@inbox.ru | st128993@student.spbu.ru
Lab work 1
*/
#include "imageFunction.hpp"

int main(){
    int width,height;
    std::cout<<"Enter width of the image: ";
    std::cin >> width;
    std::cout<<"Enter height of the image: ";
    std::cin >> height;
    std::cout << "Enter the path to the image (in raw format): ";
    std::string path;
    std::cin >> path;
    std::cout << "Raw files will be saved in the 'Images' folder\n";
    image IMG(width,height,path);
    std::string output_rotate, output_gauss;
    output_rotate = "Images/rotatedIMG.raw";
    output_gauss = "Images/gaussIMG.raw";
    IMG.readImage();
    IMG.rotateImage();
    IMG.saveToRaw(output_rotate);
    IMG.gauss();
    IMG.saveToRaw(output_gauss);
}
