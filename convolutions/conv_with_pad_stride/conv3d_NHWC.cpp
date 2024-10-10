#include<iostream>
#include<vector>
#include "cnpy.h"
#include "../../utils/utils.hpp"
using namespace std;


void conv3d(){
     // loading data
    cnpy::NpyArray inputData = cnpy::npy_load("../../inputs/py_input_nhwc.npy");
    cnpy::NpyArray kernelData = cnpy::npy_load("../../weights/conv1_wt_nhwc.npy");
    int padding = 1;
    int stride = 1;

    // Check the shape
    vector<size_t> ishape = inputData.shape;
    vector<size_t> kshape = kernelData.shape;
    
    //Storing data
    float* idata = inputData.data<float>();
    float* kdata = kernelData.data<float>();

    // Get the dimensions
    size_t batch_size = ishape[0];
    size_t input_height = ishape[1];
    size_t input_width = ishape[2];
    size_t input_channels = ishape[3];

    size_t k_height = kshape[0];
    size_t k_width = kshape[1];
    size_t in_channels = kshape[2];
    size_t out_channels = kshape[3];
    
    
    
    cout<<"hello-1";
    //check dimensions of the data
    cout<<batch_size<<" "<<input_channels<<" "<<input_height<<" "<<input_width<<endl;
    cout<<out_channels<<" "<<in_channels<<" "<<k_height<<" "<<k_width<<endl;

    cout<<"hello0";
    // Creating input using a 4D vector
    vector<vector<vector<vector<float>>>> input(
        batch_size, vector<vector<vector<float>>>(input_height,
            vector<vector<float>>(input_width, vector<float>(input_channels))));

    // Creating kernel using a 4D vector
    vector<vector<vector<vector<float>>>> kernel(
    k_height, vector<vector<vector<float>>>(k_width,
        vector<vector<float>>(in_channels, vector<float>(out_channels))));

    cout<<"hello1";
    //storing data from input file to input vector
    for (size_t i = 0; i < batch_size; ++i) {
        for (size_t j = 0; j < input_height; ++j) {
            for (size_t k = 0; k < input_width; ++k) {
                for (size_t l = 0; l <input_channels; ++l) {
                    size_t index = i * (input_height * input_width * input_channels) + j * (input_width * input_channels) + k * input_channels + l;
                    input[i][j][k][l] = idata[index];
                }
            }
        }
    }
    cout<<"hello2\n";
    // //storing data from input file to input vector
    for (size_t i = 0; i <k_height ; ++i) {
        for (size_t j = 0; j < k_width; ++j) {
            for (size_t k = 0; k < in_channels; ++k) {
                for (size_t l = 0; l <out_channels; ++l) {
                    size_t index = i * (k_width * in_channels * out_channels) + j * (in_channels * out_channels) + k * out_channels + l;
                    kernel[i][j][k][l] = kdata[index];
                }
            }
        }
    }
    cout<<"hello3\n";


    // // height and width of output vector
    int output_height = (input_height - k_height + 2 * padding) / stride + 1;
    int output_width = (input_width - k_width + 2 * padding) / stride + 1;
    cout<<"hello4\n";

    vector<vector<vector<vector<float>>>> padded_input(batch_size, vector<vector<vector<float>>>(
            input_height + 2 * padding, vector<vector<float>>(
            input_width + 2 * padding, vector<float>(in_channels, 0))));
    cout<<"hello5\n";
    for (int b = 0; b < batch_size; ++b) {
        for (int i = 0; i < input_height; ++i) {
            for (int j = 0; j < input_width; ++j) {
                for (int c = 0; c < in_channels; ++c) {
                    padded_input[b][i + padding][j + padding][c] = input[b][i][j][c];
                }
            }
        }
    }
    cout<<"hello6\n";

    // creating output vector
    vector<vector<vector<vector<float>>>> output(batch_size,
    vector<vector<vector<float>>>(output_height,
    vector<vector<float>>(output_width,
    vector<float> (out_channels))));
    
    // performing convolution
    for(int b = 0; b<batch_size; b++){
        for (int i = 0; i < output_height; ++i) {
            for (int j = 0; j < output_width; ++j) {
                for (int oc = 0; oc < out_channels; ++oc) {
                    float sum = 0.0;
                    for (int ki = 0; ki < k_height; ++ki) {
                        for (int kj = 0; kj < k_width; ++kj) {
                            for (int ic = 0; ic < in_channels; ++ic) {
                                int input_i = i * stride + ki;
                                int input_j = j * stride + kj;
                                sum += padded_input[b][input_i][input_j][ic] * kernel[ki][kj][ic][oc];
                            }
                        }
                    }
                    output[b][i][j][oc] = sum;
                    // break;
                }
            }
        }
    }


    cout<<endl<<"first value:"<<output[0][0][0][0];
    // first value:0.542983
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
    write_to_binary("../../outputs/conv3_NHWC.bin", output_1d);
    cout<<"finished";
  
}


int main(){
    // // // Perform the convolution
    conv3d();
    return 0;

}