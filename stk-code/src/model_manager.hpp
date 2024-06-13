#include <torch/script.h>
#include "karts/abstract_kart.hpp"
// #include "karts/controller/soccer_ai.hpp"



// Declare the model as an extern variable to be accessible across different files
extern torch::jit::script::Module model;
extern std::vector<float> mean;
extern std::vector<float> scale;
extern const int max_buffer_size;


extern float gamma;
extern int batch_size;

enum class TargetEncode : int {
    Ball = 0,
    Powerup = 1,
    OppChaser = 2,
    ClosestOpp = 3
};

struct Experience {
    torch::Tensor state;
    int action;
    float reward;
    torch::Tensor next_state;
    bool done;
};

extern std::deque<Experience> experiences;
extern std::optional<Experience> pending_experience;

// Function declarations
void load_model();
float calculateDistance(float x1, float y1, float x2, float y2);
torch::Tensor prepare_input(AbstractKart *kart, float steer, float accel, TargetEncode target_encoded, Vec3 target_point, int target_node, Vec3 ball_aim);
std::vector<torch::Tensor> evaluate_actions(const std::vector<torch::Tensor> inputs);
void load_scaler_parameters();
std::vector<Experience> sample_experiences(const std::deque<Experience>& experiences, int batch_size);
void train_model(const std::vector<Experience>& mini_batch, float gamma);
void save_experiences();
void load_experiences();