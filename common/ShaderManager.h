#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>

GLuint LoadShaders(const char *vertexFilePath, const char *fragmentFilePath);