#include<iostream>
#include<vector>
#include "cnpy.h"
using namespace std;


int main(){

    cnpy::NpyArray inputData = cnpy::npy_load("py_input.npy");
    cnpy::NpyArray kernelData = cnpy::npy_load("conv1_wt.npy");

    // Check the shape
    vector<size_t> ishape = inputData.shape;
    vector<size_t> kshape = kernelData.shape;
    
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

    cout<<batch_size<<" "<<input_channels<<" "<<input_height<<" "<<input_width<<endl;
    cout<<out_channels<<" "<<in_channels<<" "<<k_height<<" "<<k_width<<endl;

    // Create a kernel using a 4D vector
    vector<vector<vector<vector<float>>>> input(
        batch_size, vector<vector<vector<float>>>(input_channels,
            vector<vector<float>>(input_height, vector<float>(input_width))));

    // Create a kernel using a 4D vector
    vector<vector<vector<vector<float>>>> kernel(
    out_channels, vector<vector<vector<float>>>(in_channels,
        vector<vector<float>>(k_height, vector<float>(k_width))));



    // enter the  values of the 4D array
    // std::cout << "First few values in 4D array:" << std::endl;
    for (size_t i = 0; i < batch_size; ++i) {
        for (size_t j = 0; j < input_channels; ++j) {
            for (size_t k = 0; k < input_height; ++k) {
                for (size_t l = 0; l <input_width; ++l) {
                    size_t index = i * (input_channels * input_height * input_width) + j * (input_height * input_width) + k * input_width + l;
                    input[i][j][k][l] = idata[index];
                    // std::cout << "input[" << i << "][" << j << "][" << k << "][" << l << "] = " << idata[index] << std::endl;
                }
            }
        }
    }
     for (size_t i = 0; i < out_channels; ++i) {
        for (size_t j = 0; j < in_channels; ++j) {
            for (size_t k = 0; k < k_height; ++k) {
                for (size_t l = 0; l <k_width; ++l) {
                    size_t index = i * (in_channels * k_height * k_width) + j * (k_height * k_width) + k * k_width + l;
                    kernel[i][j][k][l] = kdata[index];
                    // std::cout << "input[" << i << "][" << j << "][" << k << "][" << l << "] = " << idata[index] << std::endl;
                }
            }
        }
    }






    for (size_t i = 0; i < 1; ++i) {
        for (size_t j = 0; j < 1; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                for (size_t l = 0; l <2; ++l) {
                    // size_t index = i * (input_channels * input_height * input_width) + j * (input_height * input_width) + k * input_width + l;
                    // input[i][j][k][l] = idata[index];
                    std::cout << "input[" << i << "][" << j << "][" << k << "][" << l << "] = " << input[i][j][k][l] << std::endl;
                }
            }
        }
    }
       for (size_t i = 0; i < 1; ++i) {
        for (size_t j = 0; j < 1; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                for (size_t l = 0; l <2; ++l) {
                    // size_t index = i * (input_channels * input_height * input_width) + j * (input_height * input_width) + k * input_width + l;
                    // input[i][j][k][l] = idata[index];
                    std::cout << "kernel[" << i << "][" << j << "][" << k << "][" << l << "] = " << kernel[i][j][k][l] << std::endl;
                }
            }
        }
    }


    int output_height = input_height - k_height + 1;
    int output_width = input_width - k_width + 1;
    cout<<endl<<"helloo12";
    cout<<"output_height: "<<output_height;
    cout<<"output_width: "<<output_width;

    vector<vector<vector<vector<float>>>> output(batch_size,
    vector<vector<vector<float>>>(out_channels,
    vector<vector<float>>(output_height,
    vector<float> (output_width))));
    
    // output.assign(batch_size,
    // vector<vector<vector<float>>>(out_channels,
    // vector<vector<float>>(output_height,
    // vector<float> (output_width,0))));

//     cout<<"after output.assign: "<<output_width;

    
    // cout<<endl<<"helloo13";
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
                    // cout << "output[" << b << "][" << o_c << "][" << i << "][" << j << "] = "
                    //      << output[b][o_c][i][j] << endl;

                }
            }
        }
    }

    for (size_t i = 0; i < 1; ++i) {
        for (size_t j = 0; j < 1; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                for (size_t l = 0; l <2; ++l) {
                    // size_t index = i * (input_channels * input_height * input_width) + j * (input_height * input_width) + k * input_width + l;
                    // input[i][j][k][l] = idata[index];
                    std::cout << "output[" << i << "][" << j << "][" << k << "][" << l << "] = " << output[i][j][k][l] << std::endl;
                }
            }
        }
    }

    // cout<<endl<<"helloo14";
    



//     cnpy::NpyArray imageData = cnpy::npy_load("py_input.npy");
//     cnpy::NpyArray kernelData = cnpy::npy_load("conv1_wt.npy");
//     cout<<"helloo2";

//     const vector<size_t>& imageShape = imageData.shape;
//     const vector<size_t>& kernelShape= kernelData.shape;
//     cout<<"helloo3";

//     // Extract dimensions
//     out_channels = kernelShape[0]; // Number of filters
//     in_channels = kernelShape[1]; // Height of each filter
//     k_height = kernelShape[2]; // Width of each filter
//     k_width= kernelShape[3]; // Depth of each filter (number of channels)

//     // Extract dimensions
//     batch_size = imageShape[0]; // Number of filters
//     img_channels = imageShape[1]; // Height of each filter
//     img_height= imageShape[2]; // Width of each filter
//     img_width = imageShape[3]; // Depth of each filter (number of channels)

//     cout<<"helloo4";

// //     cout<<"before D4";

//     cout<<"out_channels : "<<out_channels<<endl;
//     cout<<"in_channels : "<<in_channels<<endl;
//     cout<<"k_height : "<<k_height<<endl;
//     cout<<"k_width : "<<k_width<<endl;
    
    
//     cout<<"helloo5";

//     cout<<"batch_size : "<<batch_size<<endl;
//     cout<<"img_channels : "<<img_channels<<endl;
//     cout<<"img_height : "<<img_height<<endl;
//     cout<<"img_width : "<<img_width<<endl;
    
//     cout<<"helloo5";
// // //     cout<<"after D4";

//     // Create a kernel using a 4D vector
//     vector<vector<vector<vector<size_t>>>> input(
//         batch_size, vector<vector<vector<size_t>>>(img_channels,
//             vector<vector<size_t>>(img_height, vector<size_t>(img_width))));


// //     // Create a kernel using a 4D vector
//     vector<vector<vector<vector<size_t>>>> kernel(
//         out_channels, vector<vector<vector<size_t>>>(in_channels,
//             vector<vector<size_t>>(k_height, vector<size_t>(k_width))));
//     cout<<"helloo6";

// //     //         // Create a kernel using a 4D vector
// //     // vector<vector<vector<vector<float>>>> kernel(
// //     //     D1, vector<vector<vector<float>>>(D2,
// //     //         vector<vector<float>>(D3, vector<float>(D4))));

// //     cout<<"after kernal";

// // Access the data from the NPY array
//     float* idata = imageData.data<float>();

// //     // Access the data from the NPY array
//     float* kdata = kernelData.data<float>();
//     cout<<endl<<"helloo7";

//     // Fill the input with data from the NPY array
//     for (size_t i = 0; i < batch_size; ++i) {
//         for (size_t j = 0; j < img_channels; ++j) {
//             for (size_t k = 0; k < img_height; ++k) {
//                 for (size_t l = 0; l < img_width; ++l) {
//                     // Calculate the index in the 1D data array
//                     // size_t index = i+j+k+l;
//                     size_t index = i * (img_channels * img_height * img_width) + j * (img_height  * img_width) + k * img_width + l;
//                     input[i][j][k][l] = idata[index]; // Copy data to the kernel
//                     // size_t index = i * (img_height * img_width * img_channels) + j * (img_width * img_channels) + k * img_channels + l;
//                     // input[i][j][k][l] = idata[index]; // Copy data to the kernel

//                 //     size_t index = i + j + k+ l;
//                 //     kernel[i][j][k][l] = index; // Copy data to the kernel
//                 }
//             }
//         }
//     }

//     // Fill the kernel with data from the NPY array
//     for (size_t i = 0; i < out_channels; ++i) {
//         for (size_t j = 0; j < in_channels; ++j) {
//             for (size_t k = 0; k < k_height; ++k) {
//                 for (size_t l = 0; l < k_width; ++l) {
//                     // Calculate the index in the 1D data array
//                     size_t index = i * (in_channels * k_height * k_width) + j * (k_height * k_width) + k * k_width + l;
//                     kernel[i][j][k][l] = kdata[index]; // Copy data to the kernel

//                 //     size_t index = i + j + k+ l;
//                 //     kernel[i][j][k][l] = index; // Copy data to the kernel
//                 }
//             }
//         }
//     }
//     cout<<endl<<"helloo8";

//     // Print the iput values for verification
//     // for (size_t i = 0; i < batch_size; ++i) {
//     //     cout << "Filter " << i << ":" << endl;
//     //     for (size_t j = 0; j < img_channels; ++j) {
//     //         for (size_t k = 0; k < img_height; ++k) {
//     //             for (size_t l = 0; l < img_width; ++l) {
//     //                 cout << "input[" << i << "][" << j << "][" << k << "][" << l << "] = "
//     //                      << input[i][j][k][l] << endl;
//     //             }
//     //         }
//     //     }
//     // }

//     // // Print the kernel values for verification
//     for (size_t i = 0; i < out_channels; ++i) {
//         cout << "Filter " << i << ":" << endl;
//         for (size_t j = 0; j < in_channels; ++j) {
//             for (size_t k = 0; k < k_height; ++k) {
//                 for (size_t l = 0; l < k_width; ++l) {
//                     cout << "kernel[" << i << "][" << j << "][" << k << "][" << l << "] = "
//                          << kernel[i][j][k][l] << endl;
//                 }
//             }
//         }
//     }
//     cout<<endl<<"helloo9";
// //     cout<<"finished";
    
//     vector<vector<vector<vector<double>>>> output;
//     // vector<vector<vector<vector<double>>>> input;
//     cout<<endl<<"helloo10";

//     // input.assign(batch_size,
// //     // vector<vector<vector<double>>>(img_height,
// //     // vector<vector<double>>(img_width,
// //     // vector<double>(img_channels,1.0))) );

// //     cout<<endl<<"helloo11";

// //     for(int i =0; i<batch_size; i++){
// //         cout<<"batch "<<i;
// //         for(int j =0; j<img_height; j++){
// //             for(int k =0; k<img_width; k++){
// //                 for(int l =0; l<img_channels; l++){
// //                     cout<<input[i][j][k][l]<<" ";
// //                 }
// //                 cout<<"\n";
// //             }
            
// //         }
// //         cout<<"\n\n\n";
// //     }



// //     int output_height = img_height - k_height + 1;
// //     int output_width = img_width - k_width + 1;
// //     cout<<endl<<"helloo12";
// // //     cout<<"output_height: "<<output_height;
// // //     cout<<"output_width: "<<output_width;

// //     output.assign(batch_size,
// //     vector<vector<vector<double>>>(output_height,
// //     vector<vector<double>>(output_width,
// //     vector<double> (out_channels,0))));

// //     cout<<"after output.assign: "<<output_width;

    
//     cout<<endl<<"helloo13";
//     // for(int b = 0; b<batch_size; b++){
//     //     for(int i =0; i<output_height; i++){
//     //         for(int j =0; j<output_width; j++){
//     //             for(int o_c = 0; o_c<out_channels;o_c++){
//     //                 double sum =0.0;
//     //                 for(int k_i=0; k_i<k_height; k_i++){
//     //                     for(int k_j = 0; k_j<k_width; k_j++){
//     //                         for(int i_c = 0; i_c<in_channels; i_c++){
//     //                             int output_i = k_i +i;
//     //                             int output_j = k_j +j;
//     //                             sum+=input[b][output_i][output_j][i_c] * kernel[k_i][k_j][i_c][o_c];
//     //                         }
//     //                     }
//     //                 }
//     //                 output[b][i][j][o_c] = sum;
//     //                 cout << "output[" << b << "][" << i << "][" << j << "][" << o_c << "] = "
//     //                      << output[b][i][j][o_c] << endl;

//     //             }
//     //         }
//     //     }
//     // }
//     cout<<endl<<"helloo14";

//     // cout<<"output: "<<output[0][0][0][0];


}