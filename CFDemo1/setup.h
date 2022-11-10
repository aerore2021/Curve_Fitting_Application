#pragma once
#ifndef setup_h

void glfw_error_callback(int error, const char* description);
int SetUpWindow(GLFWwindow* window);
void Start_Frame(void);
void Rendering(GLFWwindow* window, ImVec4 clear_color);
void CleanUp(GLFWwindow* window);
#endif // !setup_h
