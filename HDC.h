#include <cmath>

#include <ATen/ATen.h>

/* This is a place holder. This struct definition should be removed after the
 * rest of the encoder code exits */
struct Encoder {};

struct HDC {
  int classes;
  int dim;
  Encoder encoder;

  void init(int classes, int features, int dim = 4000);

  /* Should not be void, update later */
  void call(at::Tensor x, bool encoded = false);

  void predict(at::Tensor x, bool encoded = false);

  void probabilities(at::Tensor x, bool encoded = false);

  void scores(at::Tensor x, bool encoded = false);

  void encode(at::Tensor x);

  /* This function also has 2 additional paramaters with the Union type imported
   * from typing. I believe this just does a bitwise or, but this will need to
   * be fixed in the future */
  void fit(at::Tensor x, at::Tensor y, bool encoded = false, float lr = 0.035,
           int epochs = 120, bool one_pass_fit = true);

  /* This uses *args, Find out what should replace this for the c version */
  void to();

  /* Not yet able to determine parameter types */
  void _one_pass_fit();
  void _iterative_fit();
};