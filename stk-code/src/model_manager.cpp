#include <iostream>
#include "model_manager.hpp"
#include "modes/soccer_world.hpp"
#include "karts/abstract_kart.hpp"
#include "nlohmann/json.hpp"

torch::jit::script::Module model;
std::vector<float> mean;
std::vector<float> scale;

void load_model() {
        // Adjust the path as necessary
        model = torch::jit::load("/Users/marcel/Desktop/project/model/soccer_ai_model.pt");
}

float calculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void load_scaler_parameters() {
    std::ifstream scaler_file("/Users/marcel/Desktop/project/model/scaler_parameters.json");
    nlohmann::json scaler_json;
    scaler_file >> scaler_json;
    mean = scaler_json["mean"].get<std::vector<float>>();
    scale = scaler_json["scale"].get<std::vector<float>>();
    
    // Print the loaded parameters for verification
    std::stringstream ss_mean, ss_scale;
    ss_mean << "Mean: ";
    ss_scale << "Scale: ";
    for (const auto& val : mean) {
        ss_mean << val << " ";
    }
    for (const auto& val : scale) {
        ss_scale << val << " ";
    }
    Log::info("", ss_mean.str().c_str());
    Log::info("", ss_scale.str().c_str());
}

torch::Tensor prepare_input(AbstractKart *kart, float steer, float accel, float brake, float skid) {
    SoccerWorld *world = dynamic_cast<SoccerWorld*>(World::getWorld());

    std::vector<float> input_values = {
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
        (float)world->getTimeTicks()
    };

    // Normalize the input values
    for (size_t i = 0; i < input_values.size(); ++i) {
        input_values[i] = (input_values[i] - mean[i]) / scale[i];
    }

    torch::Tensor input_tensor = torch::from_blob(input_values.data(), {1, (int)input_values.size()}, torch::kFloat32);

    std::stringstream ss;
    ss << input_tensor;
    Log::info("Input tensor:", ss.str().c_str());

    return input_tensor.clone(); // Ensure tensor is not referencing the original data buffer
}


std::vector<torch::Tensor> evaluate_actions(std::vector<torch::Tensor> inputs)
{
    std::vector<torch::Tensor> outputs;
    model.eval(); // Ensure the model is in evaluation mode
    for (const auto& input_tensor : inputs) {
        // Forward pass through the model
        torch::Tensor output = model.forward({input_tensor}).toTensor();
        outputs.push_back(output);

        std::stringstream ss;
        ss << output;
        Log::info("Output tensor:", ss.str().c_str());
    }

    return outputs;
}

