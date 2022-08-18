#define DataStorage_API __declspec(dllexport)

#ifdef __cplusplus

extern "C"
{
#endif

	DataStorage_API bool InitNet(const char* model_path);

#ifdef __cplusplus
}

extern "C"
{
#endif

	DataStorage_API bool GetFixation(unsigned char* image, unsigned char* bias_map, float salient_weight, int* uv_coordinates);

#ifdef __cplusplus
}

extern "C"
{
#endif

	DataStorage_API bool GetFixationSalmap(unsigned char* image, unsigned char* bias_map,
		float salient_weight, unsigned char* salmap, int* uv_coordinates);

#ifdef __cplusplus
}

#endif
