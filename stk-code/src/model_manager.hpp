#include <torch/script.h>
#include "karts/abstract_kart.hpp"


// Declare the model as an extern variable to be accessible across different files
extern torch::jit::script::Module model;

// Function declarations
void load_model();
torch::Tensor prepare_input(AbstractKart* kart, float steer, float accel, float brake, float skid);
std::vector<torch::Tensor> evaluate_actions(std::vector<torch::Tensor>inputs);  // Assuming you are using this function