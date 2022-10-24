#include <iostream>

#include "Encoder.h"
#include "HDC.h"

int main() {

  /* Init */
  HDC hdc;

  // Variables will need to be set to values besides default at some point
  int classes = 5;
  int dim = 10000;
  int features = 100;
  Encoder encoder(features, dim);
  torch::Tensor model = torch::zeros({classes, dim}, hdc.options);

  return 0;
}