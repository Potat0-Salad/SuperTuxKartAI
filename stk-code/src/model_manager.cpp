#include <iostream>
#include "model_manager.hpp"
#include "modes/soccer_world.hpp"
#include "karts/abstract_kart.hpp"
#include "nlohmann/json.hpp"
#include "items/attachment.hpp"
#include "items/powerup.hpp"
#include "nlohmann/json.hpp"
#include "karts/controller/arena_ai.hpp"

#include <algorithm>
#include <random> // for std::default_random_engine
#include <fstream>

#include <thread>
#include <atomic>


torch::jit::script::Module model;
std::vector<float> mean;
std::vector<float> scale;
float gamma = 0.99; // Define the discount factor
int batch_size = 64;  // Define the batch size for training
const int max_buffer_size = 10000;
using json = nlohmann::json;

std::deque<Experience> experiences;
std::optional<Experience> pending_experience;

void load_model() {
    try {
        model = torch::jit::load("/Users/marcel/Desktop/project/model/soccer_ai_model.pt");
    } catch (const c10::Error& e) {
        Log::error("Error loading the model: ", e.what());
    }
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
        steer,
        accel,
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

std::vector<torch::Tensor> evaluate_actions(const std::vector<torch::Tensor> inputs) {
    std::vector<torch::Tensor> outputs;

    // Ensure the model is in evaluation mode
    model.eval();

    for (const auto& input : inputs) {
        // Forward pass through the model
        torch::Tensor output = model.forward({input}).toTensor();
        outputs.push_back(output);
    }

    return outputs;
}

std::vector<Experience> sample_experiences(const std::deque<Experience>& experiences, int batch_size) {
    std::vector<Experience> mini_batch;
    std::vector<int> indices(experiences.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::default_random_engine generator(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), generator);

    for (int i = 0; i < batch_size; ++i) {
        mini_batch.push_back(experiences[indices[i]]);
    }

    return mini_batch;
}

void train_model(const std::vector<Experience>& mini_batch, float gamma) {
    Log::info("Training model...", "");
    // Convert experiences to JSON
    nlohmann::json j_experiences = nlohmann::json::array();
    for (const auto& exp : mini_batch) {
        nlohmann::json j_exp;
        j_exp["state"] = std::vector<float>(exp.state.data_ptr<float>(), exp.state.data_ptr<float>() + exp.state.numel());
        j_exp["action"] = exp.action;
        j_exp["reward"] = exp.reward;
        j_exp["next_state"] = std::vector<float>(exp.next_state.data_ptr<float>(), exp.next_state.data_ptr<float>() + exp.next_state.numel());
        j_exp["done"] = exp.done;
        j_experiences.push_back(j_exp);
    }

    // Write JSON to file
    std::ofstream file("/Users/marcel/Desktop/project/model/experiences.json");
    if (file.is_open()) {
        file << j_experiences.dump(4); // Pretty print with 4 spaces indentation
        file.close();
    } else {
        Log::error("Unable to open file for saving experiences", " in train model");
    }

    // Trigger the Python script to train the model
    int result = system("python /Users/marcel/Desktop/project/model/train_model.py /Users/marcel/Desktop/project/model/experiences.json");
    if (result == 0) {
        Log::info("Model trained successfully!", "");
    } else {
        Log::error("Model training failed with error code: ", std::to_string(result).c_str());
    }
}



void save_experiences() {

    Log::info("Saving experiences...", "");
    // Convert experiences to JSON
    nlohmann::json j_experiences = nlohmann::json::array();
    for (const auto& exp : experiences) {
        nlohmann::json j_exp;
        j_exp["state"] = std::vector<float>(exp.state.data_ptr<float>(), exp.state.data_ptr<float>() + exp.state.numel());
        j_exp["action"] = exp.action;
        j_exp["reward"] = exp.reward;
        j_exp["next_state"] = std::vector<float>(exp.next_state.data_ptr<float>(), exp.next_state.data_ptr<float>() + exp.next_state.numel());
        j_exp["done"] = exp.done;
        j_experiences.push_back(j_exp);
    }

    // Write JSON to file
    std::ofstream file("/Users/marcel/Desktop/project/model/experiences.json");
    if (file.is_open()) {
        file << j_experiences.dump(4); // Pretty print with 4 spaces indentation
        file.close();
    } else {
        Log::error("Unable to open file for saving experiences", " in save experiences");
    }
}

void load_experiences() {
    Log::info("Loading experiences...", "");
    std::ifstream file("/Users/marcel/Desktop/project/model/experiences.json");
    if (file.is_open()) {
        nlohmann::json j_experiences;
        file >> j_experiences;
        file.close();

        experiences.clear();
        for (const auto& j_exp : j_experiences) {
            Experience exp;
            exp.state = torch::tensor(j_exp["state"].get<std::vector<float>>(), torch::kFloat32).view({1, -1});
            exp.action = j_exp["action"];
            exp.reward = j_exp["reward"];
            exp.next_state = torch::tensor(j_exp["next_state"].get<std::vector<float>>(), torch::kFloat32).view({1, -1});
            exp.done = j_exp["done"];
            experiences.push_back(exp);
        }
    } else {
        Log::error("Unable to open file for loading experiences", " in load experiences");
    }
}