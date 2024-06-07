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
torch::Tensor prepare_input(AbstractKart *kart, TargetEncode target_encoded, Vec3 target_point, Vec3 ball_aim);
torch::Tensor evaluate_action(torch::Tensor input);  // Assuming you are using this function
void load_scaler_parameters();