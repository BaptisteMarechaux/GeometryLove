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
#include "Math.h"
#include "Triangulation.h"


struct Line
{
	float x1, y1;
	float x2, y2;
};

std::vector<Point2D> points;
std::vector<Point2D> hull;
std::vector<Point2D> triangulation2D;

Triangulation T;

GLFWwindow* window;
GLuint vertexBufferPoints, vertexBufferHull, vertexBufferDelaunay, vaoPoints, vaoHull, vaoDelaunay;
GLuint mvp_location, position_location, color_location, program;
int width, height;
int index = 0, indexHull = 0;
int nbFrames = 0;
double lastTime = 0.0f;

float last_mousex, last_mousey, last_mousez;
float first_mousex, first_mousey;
float last_movex, last_movey;

int _selectMovePoint = 0;

static bool drawPoints = true;
static bool jarvisMarchEnabled = false;
static bool grahamScanEnabled = false;
static bool divideAndConquerEnabled = false;
static bool triangulationEnabled = true;
static bool movePointEnabled = false;
static bool convexHull = false;

void Initialize();
void majPoints();
void majTriangulation();

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void Render()
{
	glm::mat4x4 model, proj, mvp;

	model = glm::mat4(1.0);
	proj = glm::ortho(0.0f, float(width), float(height), 0.0f, 0.0f, 100.0f);
	mvp = proj * model;
	
	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
	glPointSize(10.0f);

	if (movePointEnabled)
	{
		drawPoints = false;
		hull = jarvisMarchEnabled ? jarvisMarch(points) : (grahamScanEnabled ? grahamScan(points) : jarvisMarch(points));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHull);
		glBufferData(GL_ARRAY_BUFFER, hull.size() * sizeof(Point), hull.data(), GL_STATIC_DRAW);
	}

	//to ensure maj of buffer hull 
	if ((jarvisMarchEnabled || grahamScanEnabled) && convexHull != true)
	{
		hull = jarvisMarchEnabled ? jarvisMarch(points) : (grahamScanEnabled ? grahamScan(points) : jarvisMarch(points));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHull);
		glBufferData(GL_ARRAY_BUFFER, hull.size() * sizeof(Point), hull.data(), GL_STATIC_DRAW);

		convexHull = true;
	}
	else if(!jarvisMarchEnabled && !grahamScanEnabled)
		convexHull = false;

	if (points.size() > 0)
	{
		glBindVertexArray(vaoPoints);
		glDrawArrays(GL_POINTS, 0, points.size());
		glBindVertexArray(0);
	}
	//triangulation display
	if (triangulation2D.size() > 0 && triangulationEnabled)
	{
		glBindVertexArray(vaoDelaunay);
		glDrawArrays(GL_LINES, 0, triangulation2D.size());
		glBindVertexArray(0);
	}

	if (hull.size() > 0 && jarvisMarchEnabled)
	{
		glBindVertexArray(vaoHull);
		glDrawArrays(GL_LINE_LOOP, 0, hull.size());
		glBindVertexArray(0);
	}
	else if (hull.size() > 0 && grahamScanEnabled)
	{
		glBindVertexArray(vaoHull);
		glDrawArrays(GL_LINE_LOOP, 0, hull.size());
		glBindVertexArray(0);
	}
}

void callbackMousePos(GLFWwindow *window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && drawPoints && ImGui::IsMouseHoveringAnyWindow() == 0)
	{
		std::cout << "Point " << x << " " << y << std::endl;
		Point2D point(x, y);
		points.push_back(point);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * points.size(), points.data(), GL_STATIC_DRAW);

		if (jarvisMarchEnabled || grahamScanEnabled)
		{
			hull = jarvisMarchEnabled ? jarvisMarch(points) : (grahamScanEnabled ? grahamScan(points) : jarvisMarch(points));
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHull);
			glBufferData(GL_ARRAY_BUFFER, hull.size() * sizeof(Point), hull.data(), GL_STATIC_DRAW);
		}

		T.Add(point);
		std::vector<Edge> edges_temp = T.GetAretes();
		triangulation2D.clear();
		for (int i = 0; i < edges_temp.size(); i++)
		{
			triangulation2D.push_back(Point2D(edges_temp[i].p1.x, edges_temp[i].p1.y));
			triangulation2D.push_back(Point2D(edges_temp[i].p2.x, edges_temp[i].p2.y));
		}

		//std::cout << std::endl;
		//std::cout << "Aretes " << T.GetAretes().size() << std::endl;
		//std::cout << "Sommets " << T.GetSommets().size() << std::endl;
		//std::cout << "Triangles " << T.GetTriangles().size() << std::endl;
		//std::cout << std::endl;

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferDelaunay);
		glBufferData(GL_ARRAY_BUFFER, triangulation2D.size() * sizeof(Point2D), triangulation2D.data(), GL_STATIC_DRAW);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && movePointEnabled && ImGui::IsMouseHoveringAnyWindow() == 0)
	{
		select_close(x, y, _selectMovePoint, points);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && ImGui::IsMouseHoveringAnyWindow() == 0 && movePointEnabled)
	{
		if (_selectMovePoint != -1)
		{
			points[_selectMovePoint].x = x;
			points[_selectMovePoint].y = y;

			majPoints();
			majTriangulation();
			_selectMovePoint = -1;
		}
	}
}

void callbackMouseMove(GLFWwindow *window, double x, double y)
{
	last_mousex = x;
	last_mousey = y;
	if (movePointEnabled && _selectMovePoint != -1)
	{
		points[_selectMovePoint].x = last_mousex;
		points[_selectMovePoint].y = last_mousey;

		majPoints();
		majTriangulation();
	}
}

int main(int, char**)
{
	bool show_test_window = true;
	bool reset = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	Initialize();
	glEnable(GL_CULL_FACE);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Convex Hull & Voronoi");
		ImGui::Text("This window is here to use the application!");

		ImGui::Checkbox("Triangulation", &triangulationEnabled);
		ImGui::Columns(2, "mixed");
		ImGui::Separator();

		ImGui::Text("Enveloppe convexe");
		//Modes de sélection d'enveloppe convexe
		//On ne peut choisir qu'un seul de ces modes à la fois
		if (!triangulationEnabled)
		{
			if (!jarvisMarchEnabled && !grahamScanEnabled && !divideAndConquerEnabled)
			{
				ImGui::Checkbox("Jarvis March", &jarvisMarchEnabled);
				ImGui::Checkbox("Graham Scan", &grahamScanEnabled);
				ImGui::Checkbox("Divide and Conquer", &divideAndConquerEnabled);
			}
			else
			{
				if (jarvisMarchEnabled)
					ImGui::Checkbox("Jarvis March", &jarvisMarchEnabled);
				else if (grahamScanEnabled)
					ImGui::Checkbox("Graham Scan", &grahamScanEnabled);
				else if (divideAndConquerEnabled)
					ImGui::Checkbox("Divide and Conquer", &divideAndConquerEnabled);
			}
		}
		ImGui::NextColumn();

		ImGui::Text("Graphic Manipulation");
		if(!drawPoints)
			ImGui::Checkbox("Move Points", &movePointEnabled);
		if(!movePointEnabled)
			ImGui::Checkbox("Draw Points", &drawPoints);

		ImGui::Columns(1);
		ImGui::Separator();

		if (ImGui::Button("Reset")) reset ^= 1;
		ImGui::ColorEdit3("Clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		ImGui::End();

		static bool p_open = true;
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		if (!ImGui::Begin("Example: Fixed Overlay", &p_open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return 0;
		}
		ImGui::Text("FPS Counter");
		ImGui::Separator();
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}

		if (reset)
		{
			T.Reset();

			points.clear();
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
			glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(Point2D), points.data(), GL_STATIC_DRAW);

			hull.clear();
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHull);
			glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(Point2D), hull.data(), GL_STATIC_DRAW);

			triangulation2D.clear();
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferDelaunay);
			glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Point2D), triangulation2D.data(), GL_STATIC_DRAW);
			reset = false;

			std::cout << std::endl << std::endl << std::endl << std::endl;
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
	width = 1600;
	height = 900;

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
	glfwSetCursorPosCallback(window, callbackMouseMove);

	program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");

	GLfloat vertices[] = {
		width/2, height, 0.0f,
		0.0f, 0.0, 0.0f,
		width, 0, 0.0f,
	};
	_selectMovePoint = -1;

	mvp_location = glGetUniformLocation(program, "MVP");
	position_location = glGetAttribLocation(program, "position");
	//color_location = glGetAttribLocation(program, "color_in");

	glGenVertexArrays(1, &vaoPoints);
	glBindVertexArray(vaoPoints);
	
	glGenBuffers(1, &vertexBufferPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER,  100 * sizeof(Point2D), points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);

	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoHull);
	glBindVertexArray(vaoHull);

	glGenBuffers(1, &vertexBufferHull);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHull);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(Point2D), hull.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);

	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoDelaunay);
	glBindVertexArray(vaoDelaunay);
	
	glGenBuffers(1, &vertexBufferDelaunay);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferDelaunay);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Point2D), triangulation2D.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);
	
	glBindVertexArray(0);
}


void majPoints()
{
	glBindVertexArray(vaoPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * points.size(), points.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void majTriangulation()
{
	T.Reset();
	for(int i = 0; i < points.size(); i++)
		T.Add(points[i]);

	std::vector<Edge> edges_temp = T.GetAretes();
	triangulation2D.clear();
	for (int i = 0; i < edges_temp.size(); i++)
	{
		triangulation2D.push_back(Point2D(edges_temp[i].p1.x, edges_temp[i].p1.y));
		triangulation2D.push_back(Point2D(edges_temp[i].p2.x, edges_temp[i].p2.y));
	}

	glBindVertexArray(vaoDelaunay);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferDelaunay);
	glBufferData(GL_ARRAY_BUFFER, triangulation2D.size() * sizeof(Point2D), triangulation2D.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

