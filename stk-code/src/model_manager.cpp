#include <iostream>
#include "model_manager.hpp"
#include "modes/soccer_world.hpp"
#include "karts/abstract_kart.hpp"


// Define the model as a global variable
torch::jit::script::Module model;


void load_model() {
        // Adjust the path as necessary
        model = torch::jit::load("/Users/marcel/Desktop/project/model/soccer_ai_model.pt");
}

float calculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// torch::Tensor prepare_input(float ball_X, float ball_Z, float ball_aim_X, float ball_aim_Z, float dist_to_ball, float kart_X, float kart_Z, float vel_X, float vel_Z,
//                             float speed, float steer, float accel, float brake, float skid) {
//     torch::Tensor input_tensor = torch::tensor({ball_X, ball_Z, ball_aim_X, ball_aim_Z, dist_to_ball, kart_X, kart_Z, vel_X, vel_Z, speed, steer, accel, brake, skid}, torch::dtype(torch::kFloat32));
//     input_tensor = input_tensor.unsqueeze(0); // Add a batch dimension
//     return input_tensor;
// }

torch::Tensor prepare_input(AbstractKart *kart, float steer, float accel, float brake, float skid) {
    SoccerWorld *world = dynamic_cast<SoccerWorld*>(World::getWorld());
    torch::Tensor input_tensor = torch::tensor({world->getBallPosition().getX(), world->getBallPosition().getZ(),
                    world->getBallAimPosition(world->getKartTeam(kart->getWorldKartId())).getX(), world->getBallAimPosition(world->getKartTeam(kart->getWorldKartId())).getZ(),
                    calculateDistance(kart->getXYZ().getX(), kart->getXYZ().getZ(), world->getBallPosition().getX(), world->getBallPosition().getZ()),
                    kart->getXYZ().getX(), kart->getXYZ().getZ(), kart->getVelocity().getX(), kart->getVelocity().getZ(), kart->getSpeed(), steer, accel, brake, skid}, torch::dtype(torch::kFloat32));
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

