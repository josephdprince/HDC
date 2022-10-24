#pragma once

#include <cmath>

#include <ATen/ATen.h>
#include <torch/torch.h>
#include "Encoder.h"

struct HDC {
  /* Using default options
   * dtype -> kFloat32
   * layout -> kStrided
   * device -> kCPU
   * requires_grad -> false
   */
  c10::TensorOptions options = torch::TensorOptions();

  /* Update return type for functions */
  torch::Tensor predict(torch::Tensor x, bool encoded = false);

  torch::Tensor probabilities(torch::Tensor x, bool encoded = false);

  torch::Tensor scores(torch::Tensor& x, torch::Tensor& model, bool encoded = false);

  torch::Tensor cos_cdist(torch::Tensor& x1, torch::Tensor& x2, float eps = 1e-8);

  torch::Tensor encode(torch::Tensor x);

  /* This function also has 2 additional paramaters with the Union type imported
   * from typing. I believe this just does a bitwise or, but this will need to
   * be fixed in the future */
  void fit(torch::Tensor x, torch::Tensor y, bool encoded = false, float lr = 0.035,
           int epochs = 120, bool one_pass_fit = true);

  /* This uses *args, Find out what should replace this for the c version */
  void to();

  /* Not yet able to determine parameter types */
  void _one_pass_fit();
  void _iterative_fit();
};