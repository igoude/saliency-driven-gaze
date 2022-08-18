cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(saliency_wrapper)

find_package(Torch REQUIRED)

set(Torch_DIR C:/Libtorch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set( OpenCV_FOUND 1 )
set( OpenCV_DIR C:/OpenCV/build)
set( OpenCV_INC_DIR C:/OpenCV/build/include)
set (OpenCV_LIB_DIR  C:/OpenCV/build/x64/vc15/lib)
include_directories( ${OpenCV_INC_DIR} )


add_library(saliency_wrapper SHARED saliency_wrapper.cpp saliency_wrapper.h saliency.cpp saliency.h)

target_link_libraries(saliency_wrapper ${TORCH_LIBRARIES} ${OpenCV_LIBS}
${OpenCV_LIB_DIR}/opencv_world453.lib)

set_property(TARGET saliency_wrapper PROPERTY CXX_STANDARD 14)

if (MSVC)
	file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
	add_custom_command(TARGET saliency_wrapper
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${TORCH_DLLS}
		$<TARGET_FILE_DIR:x64)
endif (MSVC)
