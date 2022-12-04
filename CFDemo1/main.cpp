#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "implot.h"
#include "implot_internal.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include "setup.h"
#include "DataInput.h"
#include "plotanddraw.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


int main(int,char**) {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Curve Fitting", NULL, NULL);
	SetUpWindow(window);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		Start_Frame();

		//菜单界面
		drawMenu();
		//数据输入
		input();
		//数据呈现
		showdata();
		//绘图
		Plotting();
		//拟合选项 
		FittingOptions();

		Rendering(window, clear_color);
		
	}
	//clean up
	CleanUp(window);
	return 0;
}