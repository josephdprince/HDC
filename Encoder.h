/*
Implementation of the encoder in c++ from onlinehd: https://github.com/BIASLab-UCI/onlinehd/blob/master/onlinehd/encoder.py
"Translated" by: Hong Dong
Last Updated: 10/15/2022
*/

#pragma once

class Encoder{
    private:
        int dim, num_featuers;
        //basis, base - pytorch related vars
    public:
        Encoder(int features, int dim = 4000);

        //pytorch related function
        Encoder to();
};