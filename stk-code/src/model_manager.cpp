#include "model_manager.hpp"
#include <iostream>

// Define the model as a global variable
torch::jit::script::Module model;

void load_model() {
        // Adjust the path as necessary
        model = torch::jit::load("/Users/marcel/Desktop/project/model/soccer_ai_model.pt");
        std::cout << "Model loaded successfully\n";

}

torch::Tensor prepare_input(float ball_X, float ball_Z, float ball_aim_X, float ball_aim_Z, float ball_appr_goal,float dist_to_ball, float kart_X, float kart_Z, float vel_X, float vel_Z,
                            float speed, float steer, float accel, float brake, float skid) {
    torch::Tensor input_tensor = torch::tensor({ball_X, ball_Z, ball_aim_X, ball_aim_Z, ball_appr_goal, dist_to_ball, kart_X, kart_Z, vel_X, vel_Z, speed, steer, accel, brake, skid}, torch::dtype(torch::kFloat32));
    input_tensor = input_tensor.unsqueeze(0); // Add a batch dimension
    return input_tensor;
}


std::vector<torch::Tensor> evaluate_actions(std::vector<torch::Tensor> inputs)
{
    std::vector<torch::Tensor> outputs;
    model.eval(); // Ensure the model is in evaluation mode
    for (const auto& input_tensor : inputs) {
        // Forward pass through the model
        torch::Tensor output = model.forward({input_tensor}).toTensor();
        outputs.push_back(output);
    }
    return outputs;
}

