// GeomertyLove.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include"imgui\imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>

#include "Point.h"
#include "ShaderManager.h"
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

std::vector<Point> points;
GLFWwindow* window;
GLuint vertex_buffer, vao;
GLint mvp_location, position_location, color_location, program;
int width, height;

void Initialize();

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void Render()
{
	glm::mat4x4 model, proj, mvp;

	model = glm::mat4(1.0);
	proj = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 100.0f);
	mvp = proj * model;

	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void callbackMousePos(GLFWwindow *window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		y = height - y;
		std::cout << "Point " << x << " " << y << std::endl;
		Point point(x, y);
		points.push_back(point);
	}
}

int main(int, char**)
{
	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	Initialize();
	glEnable(GL_CULL_FACE);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{
			static float f = 0.0f;
			ImGui::Text("Hello, world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			//if (ImGui::Button("Test Window")) show_test_window ^= 1;
			if (ImGui::Button("Another Window")) show_another_window ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window, this time using an explicit Begin/End pair
		if (show_another_window)
		{
			ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello");
			ImGui::End();
		}

		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

    return 0;
}

void Initialize()
{
	// Setup window
	width = 1280;
	height = 720;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, "Geometry Love", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	//glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, callbackMousePos);
	
	program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");

	GLfloat vertices[] = {
		width/2, height, 0.0f,
		0.0f, 0.0, 0.0f,
		width, 0, 0.0f,
	};

	mvp_location = glGetUniformLocation(program, "MVP");
	position_location = glGetAttribLocation(program, "position");
	//color_location = glGetAttribLocation(program, "color_in");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid *)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
}