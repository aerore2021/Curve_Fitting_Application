#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "implot.h"
#include "implot_internal.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include "setup.h"
#define MAX_INPUT_NUMBER 10000

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class Shader {
public:
	unsigned int ID;//×ÅÉ«Æ÷³ÌÐòID
	unsigned int VAO, VBO, EBO;
	unsigned int texture;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void initArgument(const char* imagePath);
	void loadTexture(unsigned char* data, int width, int height);
	~Shader();
};


void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s \n", error, description);
}

int SetUpWindow(GLFWwindow* window)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//enable vsync

	//setup dear imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	
	//setup dear imgui style
	ImGui::StyleColorsDark();

	//setup platform/render backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();
}

void Start_Frame(void)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Rendering(GLFWwindow* window, ImVec4 clear_color)
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void CleanUp(GLFWwindow* window)
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}