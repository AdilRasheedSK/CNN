#include<iostream>
#include<vector>
#include "cnpy.h"
using namespace std;


int main(){

cnpy::NpyArray kernelData = cnpy::npy_load("conv1_wt.npy");

    // Check the shape
    std::vector<size_t> shape = kernelData.shape;
    std::cout << "Shape: ";
    for (size_t dim : shape) {
        std::cout << dim << " ";
    }
    std::cout << std::endl;

    // Extract data as float (since it's float32 in the .npy file)
    float* data = kernelData.data<float>();

    // Print the first few elements of the raw data
    std::cout << "First few elements in raw data:" << std::endl;
    for (size_t i = 0; i < 10; ++i) {
        std::cout << data[i] << std::endl;  // Print the first few values
    }

    // Ensure the array is 4D
    if (shape.size() != 4) {
        std::cerr << "Error: Expected 4D array but got " << shape.size() << " dimensions." << std::endl;
        return -1;
    }

    // Get the dimensions
    size_t dim1 = shape[0];
    size_t dim2 = shape[1];
    size_t dim3 = shape[2];
    size_t dim4 = shape[3];

    // Print the first few values of the 4D array
    std::cout << "First few values in 4D array:" << std::endl;
    for (size_t i = 0; i < std::min(dim1, (size_t)1); ++i) {
        for (size_t j = 0; j < std::min(dim2, (size_t)1); ++j) {
            for (size_t k = 0; k < std::min(dim3, (size_t)5); ++k) {
                for (size_t l = 0; l < std::min(dim4, (size_t)5); ++l) {
                    size_t index = i * (dim2 * dim3 * dim4) + j * (dim3 * dim4) + k * dim4 + l;
                    std::cout << "kernelData[" << i << "][" << j << "][" << k << "][" << l << "] = " << data[index] << std::endl;
                }
            }
        }
    }

}