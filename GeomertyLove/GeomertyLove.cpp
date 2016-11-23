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

std::vector<Point2D> points, hull;
std::vector<Point2D> triangulation2D;
std::vector<Point2D> voronoi;

//Debug
std::vector<Point2D> extPoints;
std::vector<GLfloat> colors;
std::vector<Point2D> normals;

Triangulation T;

GLFWwindow* window;

GLuint vertexBufferPoints, vertexBufferHull, vertexBufferDelaunay, vertexBufferVoronoi;
GLuint vaoPoints, vaoHull, vaoDelaunay, vaoVoronoi;

//Buffer Debug
GLuint vaoExt, vertexBufferExt, vaoNormals, vertexBufferNormal;

GLuint mvp_location, position_location, color_location, program, colorbuffer;
int width, height;
int index = 0, indexHull = 0;
int nbFrames = 0;
double lastTime = 0.0f;

//Colors
float fragmentColor[4];

ImVec4 triangulation_color = ImColor(226, 117, 34);
ImVec4 voronoi_color = ImColor(34, 53, 226);
ImVec4 hull_color = ImColor(226, 117, 34);

float last_mousex, last_mousey, last_mousez;
float first_mousex, first_mousey;
float last_movex, last_movey;

int _selectMovePoint = 0;

static bool drawPoints = true;
static bool deletePoints = false;
static bool jarvisMarchEnabled = false;
static bool grahamScanEnabled = false;
static bool divideAndConquerEnabled = false;
static bool triangulationEnabled = true;
static bool movePointEnabled = false;
static bool convexHull = false;
static bool voronoiEnabled = false;
static bool enhanceTriangulation = false;
static bool enhanceVoronoi = false;

void Initialize();
void majBuffer(int vertexBuffer, std::vector<Point2D> &vecteur);
void setColorToFragment(ImVec4 &color);

//reload entirely the triangulation
void reloadTriangulation();
//get aretes and maj list of edges to draw
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


	setColorToFragment(ImVec4(1, 0, 0, 1));
	//points display
	if (points.size() > 0)
	{
		glBindVertexArray(vaoPoints);
		glDrawArrays(GL_POINTS, 0, points.size());
		glBindVertexArray(0);
	}

	//triangulation display
	setColorToFragment(triangulation_color);
	if (triangulation2D.size() > 0 && triangulationEnabled)
	{
		if (enhanceTriangulation)
			glLineWidth(4);
		glBindVertexArray(vaoDelaunay);
		glDrawArrays(GL_LINES, 0, triangulation2D.size());
		glBindVertexArray(0);

		glLineWidth(1);
		/*Normals debug*/
		std::vector<glm::vec2> centers;
		std::vector<glm::vec2> normalEdges;

		T.GetNormalsTriangle(centers, normalEdges);
		normals.clear();
		for (int i = 0; i < normalEdges.size(); i++)
		{
			normals.push_back(Point2D(centers[i].x, centers[i].y));
			glm::vec2 point = centers[i] + glm::normalize(normalEdges[i]) * 30.0f;
			normals.push_back(Point2D(point.x, point.y));
		}
		majBuffer(vertexBufferNormal, normals);

		glBindVertexArray(vaoNormals);
		glDrawArrays(GL_LINES, 0, normals.size());
		glBindVertexArray(0);

		if (extPoints.size() > 0)
		{
			setColorToFragment(ImVec4(1,1,0,1));
			glBindVertexArray(vaoExt);
			glDrawArrays(GL_LINES, 0, extPoints.size());
			glBindVertexArray(0);
		}

		if (voronoi.size() > 1 && voronoiEnabled)
		{
			if (enhanceVoronoi)
				glLineWidth(4);
			setColorToFragment(voronoi_color);
			glBindVertexArray(vaoVoronoi);
			glDrawArrays(GL_LINES, 0, voronoi.size());
			glDrawArrays(GL_POINTS, 0, voronoi.size());
			glBindVertexArray(0);
		}
	}

	glLineWidth(1);
	setColorToFragment(hull_color);
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
		Point2D point(x, y);
		points.push_back(point);

		majBuffer(vertexBufferPoints, points);

		if (jarvisMarchEnabled || grahamScanEnabled)
		{
			hull = jarvisMarchEnabled ? jarvisMarch(points) : (grahamScanEnabled ? grahamScan(points) : jarvisMarch(points));
			majBuffer(vertexBufferHull, hull);
		}

		T.Add(point);
		majTriangulation();

		T.GetAllExtEdgesPoints(extPoints);
		majBuffer(vertexBufferExt, extPoints);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && deletePoints && ImGui::IsMouseHoveringAnyWindow() == 0)
	{
		std::cout << "Triangles " << T.GetTriangles().size() << std::endl;
		std::cout << "Aretes " << T.GetAretes().size() << std::endl;
		select_close(x, y, _selectMovePoint, points);
		if (_selectMovePoint != -1)
		{
			T.Delete(points[_selectMovePoint]);
			points.erase(points.begin() + _selectMovePoint);
		}
		//reloadTriangulation();
		majTriangulation();
		std::cout << "Triangles " << T.GetTriangles().size() << std::endl;
		std::cout << "Aretes " << T.GetAretes().size() << std::endl;

		_selectMovePoint = -1;
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
			majBuffer(vertexBufferPoints, points);

			reloadTriangulation();

			T.GetAllExtEdgesPoints(extPoints);
			majBuffer(vertexBufferExt, extPoints);

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
		majBuffer(vertexBufferPoints, points);

		reloadTriangulation();

		T.GetAllExtEdgesPoints(extPoints);
		majBuffer(vertexBufferExt, extPoints);
	}
}

int main(int, char**)
{
	bool show_test_window = true;
	bool reset = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(53, 53, 57);

	Initialize();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Convex Hull & Voronoi");
		ImGui::Text("This window is here to use the application!");

		ImGui::Columns(2, "mixed");
		ImGui::Separator();
		ImGui::Checkbox("Triangulation", &triangulationEnabled);
		ImGui::Checkbox("Enhance Triangulation", &enhanceTriangulation);

		ImGui::NextColumn();
		ImGui::Checkbox("Voronoi", &voronoiEnabled);
		ImGui::Checkbox("Enhance Voronoi", &enhanceVoronoi);

		ImGui::Columns(1);
		ImGui::Separator();

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
			}
			else
			{
				if (jarvisMarchEnabled)
					ImGui::Checkbox("Jarvis March", &jarvisMarchEnabled);
				else if (grahamScanEnabled)
					ImGui::Checkbox("Graham Scan", &grahamScanEnabled);
			}
		}
		ImGui::NextColumn();

		ImGui::Text("Graphic Manipulation");
		if(!drawPoints && !deletePoints)
			ImGui::Checkbox("Move Points", &movePointEnabled);
		if(!movePointEnabled && !deletePoints)
			ImGui::Checkbox("Draw Points", &drawPoints);
		if (!movePointEnabled && !drawPoints)
			ImGui::Checkbox("Delete Points", &deletePoints);

		ImGui::Columns(1);
		ImGui::Separator();

		if (ImGui::Button("Reset")) reset ^= 1;
		if (ImGui::Button("Test Window")) show_test_window ^= 1;

		ImGui::ColorEdit3("Triangulation color", (float*)&triangulation_color);
		ImGui::ColorEdit3("Voronoi color", (float*)&voronoi_color);
		ImGui::ColorEdit3("Convex hull color", (float*)&hull_color);

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
			majBuffer(vertexBufferPoints, points);

			hull.clear();
			majBuffer(vertexBufferHull, hull);

			triangulation2D.clear();
			majBuffer(vertexBufferDelaunay, triangulation2D);

			voronoi.clear();
			majBuffer(vertexBufferVoronoi, voronoi);

			reset = false;
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

	glfwSetMouseButtonCallback(window, callbackMousePos);
	glfwSetCursorPosCallback(window, callbackMouseMove);

	program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");
	_selectMovePoint = -1;

	mvp_location = glGetUniformLocation(program, "MVP");
	position_location = glGetAttribLocation(program, "position");
	color_location = glGetUniformLocation(program, "fragmentColor");

	float fragmentColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glProgramUniform4fv(program, color_location, 1, fragmentColor);

	glGenVertexArrays(1, &vaoPoints);
	glBindVertexArray(vaoPoints);
	
	glGenBuffers(1, &vertexBufferPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(Point2D), points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoHull);
	glBindVertexArray(vaoHull);

	glGenBuffers(1, &vertexBufferHull);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHull);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point2D), hull.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);

	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoDelaunay);
	glBindVertexArray(vaoDelaunay);
	
	glGenBuffers(1, &vertexBufferDelaunay);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferDelaunay);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point2D), triangulation2D.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoExt);
	glBindVertexArray(vaoExt);

	glGenBuffers(1, &vertexBufferExt);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferExt);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point2D), extPoints.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid *)0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoVoronoi);
	glBindVertexArray(vaoVoronoi);

	glGenBuffers(1, &vertexBufferVoronoi);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferVoronoi);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point2D), voronoi.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoNormals);
	glBindVertexArray(vaoNormals);

	glGenBuffers(1, &vertexBufferNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point2D), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
	glBindVertexArray(0);
}

void reloadTriangulation()
{
	T.Reset();
	for(int i = 0; i < points.size(); i++)
		T.Add(points[i]);

	majTriangulation();
}

void majTriangulation()
{
	const std::list<Edge>& edges_temp = T.GetAretes();
	triangulation2D.clear();
	std::list<Edge>::const_iterator it = edges_temp.begin();
	for (; it != edges_temp.end(); ++it)
	{
		triangulation2D.push_back(Point2D(it->p1.x, it->p1.y));
		triangulation2D.push_back(Point2D(it->p2.x, it->p2.y));
	}
	majBuffer(vertexBufferDelaunay, triangulation2D);
	T.GetVoronoi(voronoi);
	majBuffer(vertexBufferVoronoi, voronoi);
}

void majBuffer(int vertexBuffer, std::vector<Point2D> &vecteur)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * vecteur.size(), vecteur.data(), GL_STATIC_DRAW);
}

void setColorToFragment(ImVec4 &color)
{
	fragmentColor[0] = color.x;
	fragmentColor[1] = color.y;
	fragmentColor[2] = color.z;
	fragmentColor[3] = color.w;
	glProgramUniform4fv(program, color_location, 1, fragmentColor);
}