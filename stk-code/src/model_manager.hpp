#include <torch/script.h>
#include "karts/abstract_kart.hpp"
// #include "karts/controller/soccer_ai.hpp"


// Declare the model as an extern variable to be accessible across different files
extern torch::jit::script::Module model;
extern std::vector<float> mean;
extern std::vector<float> scale;

enum class TargetEncode : int {
    Ball = 0,
    Powerup = 1,
    OppChaser = 2,
    ClosestOpp = 3
};

// Function declarations
void load_model();
torch::Tensor prepare_input(AbstractKart *kart, float steer, float accel, TargetEncode target_encoded, Vec3 target_point, int target_node, Vec3 ball_aim);
std::vector<torch::Tensor> evaluate_actions(const std::vector<torch::Tensor> inputs);
void load_scaler_parameters();