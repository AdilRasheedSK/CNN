#include<iostream>
#include<vector>
#include "cnpy.h"
#include "utils/utils.hpp"
using namespace std;


int main(){

    // loading data
    cnpy::NpyArray inputData = cnpy::npy_load("../weights/py_input.npy");
    cnpy::NpyArray kernelData = cnpy::npy_load("../weights/conv1_wt.npy");

    // Check the shape
    vector<size_t> ishape = inputData.shape;
    vector<size_t> kshape = kernelData.shape;
    
    //Storing data
    float* idata = inputData.data<float>();
    float* kdata = kernelData.data<float>();

    // Get the dimensions
    size_t batch_size = ishape[0];
    size_t input_channels = ishape[1];
    size_t input_height = ishape[2];
    size_t input_width = ishape[3];

    size_t out_channels = kshape[0];
    size_t in_channels = kshape[1];
    size_t k_height = kshape[2];
    size_t k_width = kshape[3];

    //check dimensions of the data
    cout<<batch_size<<" "<<input_channels<<" "<<input_height<<" "<<input_width<<endl;
    cout<<out_channels<<" "<<in_channels<<" "<<k_height<<" "<<k_width<<endl;

    // Creating input using a 4D vector
    vector<vector<vector<vector<float>>>> input(
        batch_size, vector<vector<vector<float>>>(input_channels,
            vector<vector<float>>(input_height, vector<float>(input_width))));

    // Creating kernel using a 4D vector
    vector<vector<vector<vector<float>>>> kernel(
    out_channels, vector<vector<vector<float>>>(in_channels,
        vector<vector<float>>(k_height, vector<float>(k_width))));

    //storing data from input file to input vector
    for (size_t i = 0; i < batch_size; ++i) {
        for (size_t j = 0; j < input_channels; ++j) {
            for (size_t k = 0; k < input_height; ++k) {
                for (size_t l = 0; l <input_width; ++l) {
                    size_t index = i * (input_channels * input_height * input_width) + j * (input_height * input_width) + k * input_width + l;
                    input[i][j][k][l] = idata[index];
                }
            }
        }
    }

    //storing data from input file to input vector
    for (size_t i = 0; i < out_channels; ++i) {
        for (size_t j = 0; j < in_channels; ++j) {
            for (size_t k = 0; k < k_height; ++k) {
                for (size_t l = 0; l <k_width; ++l) {
                    size_t index = i * (in_channels * k_height * k_width) + j * (k_height * k_width) + k * k_width + l;
                    kernel[i][j][k][l] = kdata[index];
                }
            }
        }
    }
    //printing input vector data
    // for (size_t i = 0; i < 1; ++i) {
    //     for (size_t j = 0; j < 1; ++j) {
    //         for (size_t k = 0; k < 2; ++k) {
    //             for (size_t l = 0; l <2; ++l) {
    //                 cout << "input[" << i << "][" << j << "][" << k << "][" << l << "] = " << input[i][j][k][l] << std::endl;
    //             }
    //         }
    //     }
    // }

    //printing kernel vector data
    // for (size_t i = 0; i < 1; ++i) {
    //     for (size_t j = 0; j < 1; ++j) {
    //         for (size_t k = 0; k < 2; ++k) {
    //             for (size_t l = 0; l <2; ++l) {
    //                 std::cout << "kernel[" << i << "][" << j << "][" << k << "][" << l << "] = " << kernel[i][j][k][l] << std::endl;
    //             }
    //         }
    //     }
    // }

    // height and width of output vector
    int output_height = input_height - k_height + 1;
    int output_width = input_width - k_width + 1;

    // creating output vector
    vector<vector<vector<vector<float>>>> output(batch_size,
    vector<vector<vector<float>>>(out_channels,
    vector<vector<float>>(output_height,
    vector<float> (output_width))));
    
    // performing convolution
    for(int b = 0; b<batch_size; b++){
        for(int o_c =0; o_c<out_channels; o_c++){
            for(int i =0; i<output_height; i++){
                for(int j = 0; j<output_width;j++){
                    float sum =0;
                    for(int i_c = 0; i_c<in_channels; i_c++){
                        for(int k_i = 0; k_i<k_height; k_i++){
                            for(int k_j=0; k_j<k_width; k_j++){
                                int output_i = k_i +i;
                                int output_j = k_j +j;
                                sum+=input[b][i_c][output_i][output_j] * kernel[o_c][i_c][k_i][k_j];
                            }
                        }
                    }
                    output[b][o_c][i][j] = sum;
                }
            }
        }
    }


    cout<<endl<<"first value:"<<output[0][0][0][0];
    vector<float> output_1d;
    for (int b = 0; b < batch_size; b++)
        for (int i = 0; i < out_channels; i++)
        {
            for (int j = 0; j < output_height; j++)
            {
                for (int k = 0; k < output_width; k++)
                {
                    output_1d.push_back(output[b][i][j][k]);
                }
            }
        }
    write_to_binary("outputs/conv2.bin", output_1d);
    cout<<"finished";
   return 0;
}