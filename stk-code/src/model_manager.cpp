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

torch::Tensor prepare_input(AbstractKart *kart, float steer, float accel, float brake, float skid) {
    SoccerWorld *world = dynamic_cast<SoccerWorld*>(World::getWorld());
    torch::Tensor input_tensor = torch::tensor({
                                    (float)kart->getWorldKartId(),
                                    world->getBallPosition().getX(), 
                                    world->getBallPosition().getZ(),
                                    world->getBallAimPosition(world->getKartTeam(kart->getWorldKartId())).getX(), 
                                    world->getBallAimPosition(world->getKartTeam(kart->getWorldKartId())).getZ(), 
                                    kart->getPreviousXYZ().getX(), 
                                    kart->getPreviousXYZ().getZ(),
                                    calculateDistance(kart->getXYZ().getX(), kart->getXYZ().getZ(), world->getBallPosition().getX(), world->getBallPosition().getZ()),
                                    kart->getXYZ().getX(), 
                                    kart->getXYZ().getZ(), 
                                    kart->getVelocity().getX(), 
                                    kart->getVelocity().getZ(), 
                                    kart->getSpeed(), 
                                    steer,
                                    accel, 
                                    brake, 
                                    skid,
                                    // world->getTimeTicks()
                                }, torch::dtype(torch::kFloat32));

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

