#include "include\Shader.h"

int Shader::createShader(const GLchar *vertexPath, const GLchar *fragmentPath) {
	// Retrieve the vertex/fragment source code from filepath
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	// Ensures ifstream ojects can throw exceptions
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);
	try {
		// Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "Error Reading Shader Files" << endl;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	// Compile the shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Error Compiling Vertex Shader\n" << infoLog << endl;
	}

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "Error Compiling Fragment Shader\n" << infoLog << endl;
	}

	// Shader Program
	int Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);

	// Print any linking errors
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		cout << "Error linking Shader Program\n" << infoLog << endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return Program;
}


// Overridden method used ot create shader, with geo tesc tese shaders
int Shader::createShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *tesConPath, const GLchar *tesEvalPath) {
	// Retrieve the vertex/fragment/ source code from filepath
	string vertexCode;
	string fragmentCode;
	string tesConCode;
	string tesEvalCode;

	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream tcShaderFile;
	ifstream teShaderFile;

	// Ensures ifstream ojects can throw exceptions
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);
	tcShaderFile.exceptions(ifstream::badbit);
	teShaderFile.exceptions(ifstream::badbit);

	try {
		// Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		tcShaderFile.open(tesConPath);
		teShaderFile.open(tesEvalPath);

		stringstream vShaderStream, fShaderStream, tcShaderStream, teShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		tcShaderStream << tcShaderFile.rdbuf();
		teShaderStream << teShaderFile.rdbuf();

		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		tcShaderFile.close();
		teShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		tesConCode = tcShaderStream.str();
		tesEvalCode = teShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "Error Reading Shader Files" << endl;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *tcShaderCode = tesConCode.c_str();
	const GLchar *teShaderCode = tesEvalCode.c_str();


	// Compile the shaders below
	GLuint vertex, fragment, tesCon, tesEval;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Error Compiling Vertex Shader\n" << infoLog << endl;
	}

	// Tesselation Control Shader
	tesCon = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tesCon, 1, &tcShaderCode, NULL);
	glCompileShader(tesCon);

	glGetShaderiv(tesCon, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(tesCon, 512, NULL, infoLog);
		cout << "Error Compiling Tesselation Control Shader\n" << infoLog << endl;
	}

	// Tesselation Evaluation Shader
	tesEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tesEval, 1, &teShaderCode, NULL);
	glCompileShader(tesEval);

	glGetShaderiv(tesEval, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(tesEval, 512, NULL, infoLog);
		cout << "Error Compiling Tesselation Evaluation Shader\n" << infoLog << endl;
	}

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "Error Compiling Fragment Shader\n" << infoLog << endl;
	}

	// Shader Program
	int Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, tesCon);
	glAttachShader(Program, tesEval);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);

	// Print any linking errors
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		cout << "Error linking Shader Program\n" << infoLog << endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(tesCon);
	glDeleteShader(tesEval);
	glDeleteShader(fragment);

	return Program;
}



// Overridden method used ot create shader, with geo tesc tese shaders
int Shader::createShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath, const GLchar *tesConPath, const GLchar *tesEvalPath) {
	// Retrieve the vertex/fragment/geometry source code from filepath
	string vertexCode;
	string fragmentCode;
	string geometryCode;
	string tesConCode;
	string tesEvalCode;

	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream gShaderFile;
	ifstream tcShaderFile;
	ifstream teShaderFile;

	// Ensures ifstream ojects can throw exceptions
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);
	gShaderFile.exceptions(ifstream::badbit);
	tcShaderFile.exceptions(ifstream::badbit);
	teShaderFile.exceptions(ifstream::badbit);

	try {
		// Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geometryPath);
		tcShaderFile.open(tesConPath);
		teShaderFile.open(tesEvalPath);

		stringstream vShaderStream, fShaderStream, gShaderStream, tcShaderStream, teShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		tcShaderStream << tcShaderFile.rdbuf();
		teShaderStream << teShaderFile.rdbuf();

		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();
		tcShaderFile.close();
		teShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
		tesConCode = tcShaderStream.str();
		tesEvalCode = teShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "Error Reading Shader Files" << endl;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	const GLchar *tcShaderCode = tesConCode.c_str();
	const GLchar *teShaderCode = tesEvalCode.c_str();


	// Compile the shaders below
	GLuint vertex, fragment, geometry, tesCon, tesEval;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Error Compiling Vertex Shader\n" << infoLog << endl;
	}

	// Tesselation Control Shader
	tesCon = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tesCon, 1, &tcShaderCode, NULL);
	glCompileShader(tesCon);

	glGetShaderiv(tesCon, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(tesCon, 512, NULL, infoLog);
		cout << "Error Compiling Tesselation Control Shader\n" << infoLog << endl;
	}

	// Tesselation Evaluation Shader
	tesEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tesEval, 1, &teShaderCode, NULL);
	glCompileShader(tesEval);

	glGetShaderiv(tesEval, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(tesEval, 512, NULL, infoLog);
		cout << "Error Compiling Tesselation Evaluation Shader\n" << infoLog << endl;
	}

	// Geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);

	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometry, 512, NULL, infoLog);
		cout << "Error Compiling Geometry Shader\n" << infoLog << endl;
	}

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "Error Compiling Fragment Shader\n" << infoLog << endl;
	}

	// Shader Program
	int Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, tesCon);
	glAttachShader(Program, tesEval);
	glAttachShader(Program, geometry);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);

	// Print any linking errors
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		cout << "Error linking Shader Program\n" << infoLog << endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(tesCon);
	glDeleteShader(tesEval);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return Program;
}

