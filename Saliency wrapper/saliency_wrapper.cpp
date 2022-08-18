#include "saliency_wrapper.h"
#include "saliency.h"


saliency saliency_util = saliency();


bool InitNet(const char* model_path)
{
	return saliency_util.init(model_path);
}

bool GetFixation(unsigned char* image, unsigned char* bias_map, float salient_weight, int* uv_coordinates)
{
	return saliency_util.get_fixation(image, bias_map, salient_weight, uv_coordinates);
}

bool GetFixationSalmap(unsigned char* image, unsigned char* bias_map,
	float salient_weight, unsigned char* salmap, int* uv_coordinates)
{
	return saliency_util.get_fixation_salmap(image, bias_map, salient_weight, salmap, uv_coordinates);
}
