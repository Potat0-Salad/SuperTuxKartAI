#include <torch/script.h>

// Declare the model as an extern variable to be accessible across different files
extern torch::jit::script::Module model;

// Function declarations
void load_model();
torch::Tensor prepare_input(float ball_X, float ball_Z, float ball_aim_X, float ball_aim_Z, float dist_to_ball, float kart_X, float kart_Z, float vel_X , float vel_Z,
    float speed, float steer, float accel, float brake, float skid);
std::vector<torch::Tensor> evaluate_actions(std::vector<torch::Tensor>inputs);  // Assuming you are using this function