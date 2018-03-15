#define GLEW_STATIC
#include <GL\glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	static int createShader(const GLchar *vertexPath, const GLchar *fragmentPath);
	static int createShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *tesConPath, const GLchar *tesEvalPath);
	static int createShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath, const GLchar *tesConPath, const GLchar *tesEvalPath);
private:
	// Now one cannot create an instance of this class
	Shader() {}
};
