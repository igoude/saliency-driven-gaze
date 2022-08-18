#include "saliency.h"

saliency::saliency() {}

bool saliency::init(const char* model_path) {
  // Load saliency module
  _module = torch::jit::load(model_path, torch::kCUDA);
}

bool saliency::get_fixation(unsigned char* image, unsigned char* bias_map, float salient_weight, int* uv_coordinates) {
  // Compute saliency map float32 to get true fixation value !
  // Create cv::Mat from raw input image
  cv::Mat cv_image(_image_size, _image_size, CV_8UC4);
  memcpy((void *)cv_image.data,	image, sizeof(torch::kU8) * 4 * _image_size * _image_size);

  // Convert Color Space
  cv::cvtColor(cv_image, cv_image, cv::COLOR_RGBA2RGB);
  cv_image.convertTo(cv_image, CV_32FC3, 1.0f / 255.0f);

  auto input_tensor = torch::from_blob(cv_image.data,	{ 1, _image_size, _image_size, 3 });

  input_tensor = input_tensor.permute({ 0, 3, 1, 2 });
  input_tensor = input_tensor.contiguous();

  // Normalize channels
  input_tensor[0][0] = input_tensor[0][0].sub_(0.485).div(0.229);
  input_tensor[0][1] = input_tensor[0][1].sub_(0.456).div(0.224);
  input_tensor[0][2] = input_tensor[0][2].sub_(0.406).div(0.225);
  input_tensor = input_tensor.to(torch::kCUDA);

  // Pass Data Through Neural Net to Get Output
  at::Tensor result = _module.forward({input_tensor}).toTensor();
  at::Tensor salmap_tensor = result.squeeze(0).detach();

  // Saliency tensor normalization
  salmap_tensor -= torch::min(salmap_tensor);
  salmap_tensor /= (1e-7 + torch::max(salmap_tensor));

  // Convert to cv::Mat
  salmap_tensor = salmap_tensor.permute({1, 2, 0}).to(torch::kCPU);
  cv::Mat cv_salmap(_image_size, _image_size, CV_32FC1);
  std::memcpy((void *)cv_salmap.data, salmap_tensor.data_ptr(), sizeof(float) * salmap_tensor.numel());

  // Apply salient weight
  cv_salmap = (cv_salmap * salient_weight) + (1.0f - salient_weight);

  // Get bias map
  cv::Mat cv_bias(_image_size, _image_size, CV_8UC4);
  std::memcpy((void *)cv_bias.data, bias_map, sizeof(torch::kU8) * 4 * _image_size * _image_size);
  cv::extractChannel(cv_bias, cv_bias, 0);
  cv_bias.convertTo(cv_bias, CV_32FC1, 1.0f / 255.0f);

  // Saliency map to probability distribution
  cv::Scalar sum_salmap = cv::sum(cv_salmap);
  cv::Scalar sum_bias = cv::sum(cv_bias);

  // Normalize distributions
  cv_salmap = cv_salmap / sum_salmap;
  cv_bias = cv_bias / sum_bias;

  // Create the fixation probability map
  cv::Mat fixation_proba = cv_bias.mul(cv_salmap);

  // Initialize the random generator for sampling
  std::random_device rd;
  std::mt19937 gen(rd());

  // Cast the Mat into a float vector
  std::vector<float> fix_proba;
  fix_proba.assign((float*)fixation_proba.data, (float*)fixation_proba.data + fixation_proba.total() * fixation_proba.channels());

  // Creates the distribution
  std::discrete_distribution<> fixation_distribution(std::begin(fix_proba), std::end(fix_proba));

  // Sample N pixels in the 256*256 vector
  int nb_samples = 20;
  std::vector<int> samples(nb_samples);
  std::vector<float> probas(nb_samples);
  for(int i=0; i<nb_samples; i++)
  {
    int sample = fixation_distribution(gen);
    samples[i] = sample;
    probas[i] = fix_proba[sample];
  }

  // Keep the max proba
  int maxProbaIndex = std::max_element(probas.begin(),probas.end()) - probas.begin();
  int fix_sample = samples[maxProbaIndex];

  // Height
  uv_coordinates[0] = fix_sample % _image_size;
  // Width
  uv_coordinates[1] = (int)fix_sample / _image_size;

  return true;
}

// DEBUG
bool saliency::get_fixation_salmap(unsigned char* image, unsigned char* bias_map, float salient_weight, unsigned char* salmap, int* uv_coordinates) {
  // Compute saliency map float32 to get true fixation value !
  // Create cv::Mat from raw input image
  cv::Mat cv_image(_image_size, _image_size, CV_8UC4);
  std::memcpy((void *)cv_image.data, image, sizeof(torch::kU8) * 4 * _image_size * _image_size);

  // Convert Color Space
  cv::cvtColor(cv_image, cv_image, cv::COLOR_RGBA2RGB);
  cv_image.convertTo(cv_image, CV_32FC3, 1.0f / 255.0f);

  auto input_tensor = torch::from_blob(cv_image.data,	{ 1, _image_size, _image_size, 3 });
  input_tensor = input_tensor.permute({ 0, 3, 1, 2 });
  input_tensor = input_tensor.contiguous();

  // Normalize channels
  input_tensor[0][0] = input_tensor[0][0].sub_(0.485).div(0.229);
  input_tensor[0][1] = input_tensor[0][1].sub_(0.456).div(0.224);
  input_tensor[0][2] = input_tensor[0][2].sub_(0.406).div(0.225);
  input_tensor = input_tensor.to(torch::kCUDA);

  // Pass Data Through Neural Net to Get Output
  at::Tensor result = _module.forward({input_tensor}).toTensor();
  at::Tensor salmap_tensor = result.squeeze(0).detach();

  // Saliency tensor normalization
  salmap_tensor -= torch::min(salmap_tensor);
  salmap_tensor /= (1e-7 + torch::max(salmap_tensor));

  // Convert the resulting saliency map to cv::Mat
  salmap_tensor = salmap_tensor.permute({1, 2, 0}).to(torch::kCPU);
  cv::Mat cv_salmap(_image_size, _image_size, CV_32FC1);
  std::memcpy((void *)cv_salmap.data, salmap_tensor.data_ptr(), sizeof(float) * salmap_tensor.numel());

  // Apply salient weight
  cv_salmap = (cv_salmap * salient_weight) + (1.0f - salient_weight);

  /* COPY SALIENCY MAP */
  // Copy the saliency map into 8bits texture after float parsing (OUTPUT salmap)
  cv::Mat cv_salChan(_image_size, _image_size, CV_8UC1);
  cv_salmap.convertTo(cv_salChan, CV_8UC1, 255);
  cv::cvtColor(cv_salChan, cv_salChan, cv::COLOR_GRAY2RGBA);
  std::memcpy(salmap, (void *)cv_salChan.data, sizeof(torch::kU8) * 4 * _image_size * _image_size);
  /* COPY SALIENCY MAP */

  // Get bias map
  cv::Mat cv_bias(_image_size, _image_size, CV_8UC4);
  std::memcpy((void *)cv_bias.data, bias_map, sizeof(torch::kU8) * 4 * _image_size * _image_size);
  cv::extractChannel(cv_bias, cv_bias, 0);
  cv_bias.convertTo(cv_bias, CV_32FC1, 1.0f / 255.0f);

  // Saliency map to probability distribution
  cv::Scalar sum_salmap = cv::sum(cv_salmap);
  cv::Scalar sum_bias = cv::sum(cv_bias);

  // Normalize distributions
  cv_salmap = cv_salmap / sum_salmap;
  cv_bias = cv_bias / sum_bias;

  // Create the fixation probability
  cv::Mat fixation_proba = cv_bias.mul(cv_salmap);

  // Initialize the random generator for sampling
  std::random_device rd;
  std::mt19937 gen(rd());

  // Cast the Mat into a float vector
  std::vector<float> fix_proba;
  fix_proba.assign((float*)fixation_proba.data, (float*)fixation_proba.data + fixation_proba.total() * fixation_proba.channels());

  // Creates the distribution
  std::discrete_distribution<> fixation_distribution(std::begin(fix_proba), std::end(fix_proba));

  // Sample N pixels in the 256*256 vector
  int nb_samples = 20;
  std::vector<int> samples(nb_samples);
  std::vector<float> probas(nb_samples);
  for(int i=0; i<nb_samples; i++)
  {
    int sample = fixation_distribution(gen);
    samples[i] = sample;
    probas[i] = fix_proba[sample];
  }

  // Keep the max proba
  int maxProbaIndex = std::max_element(probas.begin(), probas.end()) - probas.begin();
  int fix_sample = samples[maxProbaIndex];

  // Height
  uv_coordinates[0] = fix_sample % _image_size;
  // Width
  uv_coordinates[1] = (int)(fix_sample / _image_size);

  return true;
}
