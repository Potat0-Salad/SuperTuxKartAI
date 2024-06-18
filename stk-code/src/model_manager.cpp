#include <iostream>
#include "model_manager.hpp"
#include "modes/soccer_world.hpp"
#include "karts/abstract_kart.hpp"
#include "nlohmann/json.hpp"
#include "karts/controller/arena_ai.hpp"
#include "items/attachment.hpp"
#include "items/powerup.hpp"


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

torch::Tensor prepare_input(AbstractKart *kart, float steer, float accel, TargetEncode target_encoded, Vec3 target_point, int target_node, Vec3 ball_aim) {
    bool powerup;

    // Check if the kart pointer is null
    if (!kart) {
        Log::error("prepare_input", "Null pointer for kart");
        return torch::Tensor();
    }
    
    // Get the world pointer and check if it is null
    SoccerWorld *world = dynamic_cast<SoccerWorld*>(World::getWorld());
    if (!world) {
        Log::error("prepare_input", "Null pointer for world");
        return torch::Tensor();
    }

    if(kart->getPowerup()->getType() == PowerupManager::POWERUP_NOTHING &&
       kart->getAttachment()->getType() != Attachment::ATTACH_SWATTER){
        powerup = false;
    }
    else{
        powerup = true;
    }

    std::vector<float> input_values = {
        (float)kart->getWorldKartId(),
        world->getBallPosition().getX(), 
        world->getBallPosition().getZ(),
        ball_aim.getX(),
        ball_aim.getY(),
        (float)world->getBallNode(),
        kart->getPreviousXYZ().getX(), 
        kart->getPreviousXYZ().getZ(),
        world->getBallHeading(),
        (float)world->ballApproachingGoal(KART_TEAM_BLUE),
        calculateDistance(kart->getXYZ().getX(), kart->getXYZ().getZ(), world->getBallPosition().getX(), world->getBallPosition().getZ()),
        kart->getXYZ().getX(), 
        kart->getXYZ().getZ(),
        kart->getVelocity().getX(),
        kart->getVelocity().getZ(),
        kart->getSpeed(),
        // steer,
        // accel,
        (float)world->getSectorForKart(kart),
        // (float)target_encoded,
        // target_point.getX(),
        // target_point.getZ(),
        (float)world->getSectorForKart(world->getKart(0)),
        (float)world->getSectorForKart(world->getKart(1)),
        world->getKart(0)->getXYZ().getX(),
        world->getKart(0)->getXYZ().getZ(),
        world->getKart(1)->getXYZ().getX(),
        world->getKart(1)->getXYZ().getZ(),
        (float)powerup
    };

    // Normalize the input values
    for (size_t i = 0; i < input_values.size(); ++i) {
        if (i >= mean.size() || i >= scale.size()) {
            Log::error("prepare_input", "Index out of bounds during normalization");
            return torch::Tensor();
        }
        input_values[i] = (input_values[i] - mean[i]) / scale[i];
    }

    torch::Tensor input_tensor = torch::from_blob(input_values.data(), {1, (int)input_values.size()}, torch::kFloat32);
    // Log::info("Input tensor created successfully", " ");

    // std::stringstream ss;
    // ss << input_tensor;
    // Log::info("Input tensor:", ss.str().c_str());

    return input_tensor.clone(); // Ensure tensor is not referencing the original data buffer
}



torch::Tensor evaluate_action(torch::Tensor input) {
    model.eval(); // Ensure the model is in evaluation mode
    // Forward pass through the model
    torch::Tensor output = model.forward({input}).toTensor();
    std::stringstream ss;
    ss << output;
    Log::info("Output tensor:", ss.str().c_str());
    return output;
}


