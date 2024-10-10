#!/bin/bash
cd convolutions/conv_with_pad
# Compile the C++ code
g++ conv3d_NCHW.cpp ../../utils/utils.cpp -o conv3d_NCHW -lcnpy

# Execute the compiled program
./conv3d_NCHW

cd ..
cd ..
cd python_model
# Run the Python script for convolution
python model.py

# Compare the output binary files
python validate.py ../outputs/conv3_NCHW.bin ../outputs/py_conv3d_nchw_output.bin
cd ..
