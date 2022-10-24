#include "HDC.h"

#include <iostream>

torch::Tensor HDC::predict(torch::Tensor x, bool encoded) {

}

torch::Tensor HDC::scores(torch::Tensor& x, torch::Tensor& model, bool encoded) {
  torch::Tensor temp = x;
  if (!encoded) {
    temp = encode(temp);  
  }
  return cos_cdist(temp, model);
}

torch::Tensor HDC::cos_cdist(torch::Tensor& x1, torch::Tensor& x2, float eps) {
  torch::Tensor temp = torch::tensor(eps, options);
  std::tuple<torch::Tensor, torch::Tensor> norm1 = x1.norm(1).unsqueeze_(1).max(eps);
  std::tuple<torch::Tensor, torch::Tensor> norm2 = x2.norm(1).unsqueeze_(0).max(eps);

  torch::Tensor mult = torch::mm(x1, x2.transpose(0,1));
  // TODO: Find out how to use .div_ since norm is returning a tuple instead of a tensor
  // mult.div_(norm1).div_(norm2);
 
  return mult;
}

torch::Tensor HDC::encode(torch::Tensor x) {
  // TODO: encode using encoder class call function
}