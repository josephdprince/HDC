/*
Implementation of the encoder in c++ from onlinehd: https://github.com/BIASLab-UCI/onlinehd/blob/master/onlinehd/encoder.py
"Translated" by: Hong Dong
Last Updated: 10/15/2022
*/

#include "Encoder.h";

Encoder::Encoder(int features, int dim = 4000){
    this->dim = dim;
    this->num_featuers = features;
}