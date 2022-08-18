#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <memory>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "torch/torch.h"
#include <torch/script.h>


class saliency
{
public:
	/** Torch Model Loader */
	torch::jit::script::Module _module;

	/** Size of images */
	int _image_size = 256;


public:

	saliency();

	/**
	 * Initiate model
   * @param model_path - Path to the pytorch model (.pt)
	 * @return Whether the operation was sucessfull
	 */
	bool init(const char* model_path);

	/**
	 * Get a fixation point depending on saliency and bias maps
   * @param image - Rendering from the character's field of view (CV_8UC4)
   * @param bias_map - Blended bias maps (CV_8UC1)
	 * @param salient_weight - Weight to blend the saliency map with other bias
	 * @param uv_coordinates - Output UV coordinate corresponding to the stochastically choosen salient point
	 * @return Whether the operation was sucessfull
	 */
 	bool get_fixation(unsigned char* image, unsigned char* bias_map, float salient_weight, int* uv_coordinates);

	/**
	 * DEBUG: Get a fixation point depending on saliency and bias maps + return the computed saliency map
   * @param image - Rendering from the character's field of view (CV_8UC4)
   * @param bias_map - Blended bias maps (CV_8UC1)
	 * @param salient_weight - Weight to blend the saliency map with other bias
	 * @param salmap - Output the computed saliency map (CV_8UC1)
	 * @param uv_coordinates - Output UV coordinate corresponding to the stochastically choosen salient point
	 * @return Whether the operation was sucessfull
	 */
	bool get_fixation_salmap(unsigned char* image, unsigned char* bias_map, float salient_weight, unsigned char* salmap, int* uv_coordinates);

};
