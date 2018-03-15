#include <iostream>
#include <math.h>
#include "include\Shader.h" 
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "include\stb_image.h"
#include <vector>

using namespace std;
using namespace glm;

const GLint WIDTH = 800, HEIGHT = 600;
const GLfloat piDef = 3.14159265358979323846264338327950288f;
const GLfloat convertToRadians = piDef / 180.0f;
const GLuint vboSize = 4;
const GLuint vaoSize = 4;
const GLuint eboSize = 1;

// For cones 
const GLuint triangles = 4;	// More triangles means finer cone
GLfloat coneVertices[triangles + 2]; // Size is all bottom vertices of the triangles + cone top vertex
GLuint coneIndices[triangles * 3]; // no. of triangles * 3 points per triangle

//Accurate representation of window relative to the screen resolution
int screenWidth, screenHeight;
GLfloat sphereRadius = 0.7f;

GLuint vbos[vboSize], vaos[vaoSize], ebos[eboSize];
GLuint sphereShader;
GLuint boxShader;
GLuint coneShader;
GLfloat timeValue;
unsigned int texture1;

// Mesh sphereMesh = Mesh(octahedronVertices, octahedronIndices);

enum scenes {
	scene_a,
	scene_b,
	scene_c,
	scene_d
} currentScene;

// Creation of all vertices and indices for all shapes
//
GLfloat octahedronVertices[]
{
	// Positions			
	0.0f, 0.0f, 1.0f,		
	0.0f, 0.0f, -1.0f,		
	-1.0f, -1.0f, 0.0f,		
	1.0f, -1.0f, 0.0f,		
	1.0f, 1.0f, 0.0f,		
	-1.0f, 1.0f, 0.0f,		
};

unsigned int octahedronIndices[]
{
	0, 3, 4,
	0, 4, 5,
	0, 5, 2,
	0, 2, 3,
	1, 4, 3,
	1, 5, 4,
	1, 2, 5,
	1, 3, 2,
};

GLfloat boxVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

GLboolean exitWindow = false;

// Structs used for cone
struct Vertex {
	GLfloat position[3];
	GLfloat color[3];
};
vector<Vertex> v;

// Structs used for cylinder 
struct each_pole {
	GLfloat x, z, y_start, y_end;
}; 
vector<each_pole> each_pole_vector;


void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods); 
void setupGeometry();
void setupTexture();
void setupViewProjectionMatrix(GLuint shader);
void passModelToShader(GLuint shader, mat4 modelMatrix);
void renderShape(GLuint shader, GLuint vao, GLboolean elementsFlag, GLuint size, mat4 modelMatrix, GLboolean tessFlag, GLboolean wireframeFlag, GLboolean textureFlag);
void cone1(GLuint vao, GLuint vbo);
void renderScene_a();
void renderScene_b();
void renderScene_c();
void renderScene_d();

int main()
{
	currentScene = scene_a;
	
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	//Create glfwWindow object
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mani's CWK 1 :)", nullptr, nullptr);
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create glfw window" << endl;
	}

	glfwMakeContextCurrent(window);

	//Just to say, using modern version of glew, and then check if it initialises
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to intialise GLEW" << endl;
	}

	//Define viewport diemensions
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);

	// Call the keycallback method everytime we interact with the keyboard
	glfwSetKeyCallback(window, keyCallback);
	// Makes sure keyboard event is getting polled
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	//create the shader program
	sphereShader = Shader::createShader("./shaders/sphereCore.vs", "./shaders/sphereCore.frag", "./shaders/sphereCore.tesc", "./shaders/sphereCore.tese");
	boxShader = Shader::createShader("./shaders/boxCore.vs", "./shaders/boxCore.frag");
	coneShader = Shader::createShader("./shaders/coneCore.vs", "./shaders/coneCore.frag");

	// Set up Vertices and indices
	setupGeometry();
	setupTexture();

	while (!glfwWindowShouldClose(window) && !exitWindow)
	{
		// Update timeValue for this frame
		timeValue = (GLfloat) glfwGetTime();

		//Render, and clear color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (currentScene)
		{
		case scene_a:
			renderScene_a();
			break;
		case scene_b:
			renderScene_b();
			break;
		case scene_c:
			renderScene_c();
			break;
		case scene_d:
			renderScene_d();
			break;
		default:
			std::cout << "No scenes" << endl;
			break;
		}

		//Swap screen buffers
		glfwSwapBuffers(window);
		//check if any events have been activated, and call corresponding response function
		glfwPollEvents();
	}

	glDeleteVertexArrays(vaoSize, vaos);
	glDeleteBuffers(vboSize, vbos);
	glDeleteBuffers(eboSize, ebos);


	//terminate glfw and terminate all resources allocated by it
	glfwTerminate();
	return EXIT_SUCCESS;
}


void renderScene_a()
{
	mat4 sphereModelMatrix;
	sphereModelMatrix = rotate(sphereModelMatrix, timeValue * radians(30.0f), vec3(0.0f, 1.0f, 0.0f));

	// Let's get some sizing going
	GLfloat radius = 0.3f * sin(timeValue) + 1.0f;
	// Updates the size of the sphere
	unsigned int sphereRadiusLoc = glGetUniformLocation(sphereShader, "sphereRadius");
	glUniform1f(sphereRadiusLoc, radius);

	// Checks if the sphere will have lighting
	unsigned int lightingFlagLoc = glGetUniformLocation(sphereShader, "lightingFlag");
	glUniform1f(lightingFlagLoc, 0);

	GLfloat tessLevel = 12.0f;
	unsigned int tessLevelLoc = glGetUniformLocation(sphereShader, "tessLevel");
	glUniform1f(tessLevelLoc, tessLevel);

	renderShape(sphereShader, vaos[0], true, sizeof(octahedronIndices), sphereModelMatrix, true, true, false);
}
void renderScene_b()
{
	mat4 sphereModelMatrix;
	sphereModelMatrix = rotate(sphereModelMatrix, timeValue * radians(45.0f), vec3(0.0f, 1.0f, 0.0f));

	GLfloat radius = 1.0f;
	// Updates the size of the sphere
	unsigned int sphereRadiusLoc = glGetUniformLocation(sphereShader, "sphereRadius");
	glUniform1f(sphereRadiusLoc, radius);

	// Checks if the sphere will have lighting
	unsigned int lightingFlagLoc = glGetUniformLocation(sphereShader, "lightingFlag");
	glUniform1f(lightingFlagLoc, 1);

	GLfloat tessLevel = 12.0f;
	unsigned int tessLevelLoc = glGetUniformLocation(sphereShader, "tessLevel");
	glUniform1f(tessLevelLoc, tessLevel);

	renderShape(sphereShader, vaos[0], true, sizeof(octahedronIndices), sphereModelMatrix, true, false, false);
}
void renderScene_c()
{
	const vec3 rotation = vec3(0.5f, 1.0f, 0.0f);
	const float angleRad = radians(90.f);

	// Initial setup
	mat4 coneModelMatrix[3];
	coneModelMatrix[0] = rotate(coneModelMatrix[0], angleRad, rotation);
	coneModelMatrix[0] = scale(coneModelMatrix[0], vec3(0.5f, 0.5f, 0.5f));
	coneModelMatrix[0] = translate(coneModelMatrix[0], vec3(0.0f, 0.0f, -3.0f));
	
	GLfloat radius = 1.0f;
	// Updates the size of the sphere
	unsigned int sphereRadiusLoc = glGetUniformLocation(sphereShader, "sphereRadius");
	glUniform1f(sphereRadiusLoc, radius);

	// Checks if the sphere will have lighting
	unsigned int lightingFlagLoc = glGetUniformLocation(sphereShader, "lightingFlag");
	glUniform1f(lightingFlagLoc, 0);

	GLfloat tessLevel = 12.0f;
	unsigned int tessLevelLoc = glGetUniformLocation(sphereShader, "tessLevel");
	glUniform1f(tessLevelLoc, tessLevel);


	// Cone 
	glUseProgram(coneShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	passModelToShader(coneShader, coneModelMatrix[0]);
	setupViewProjectionMatrix(coneShader);
	glBindVertexArray(vaos[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, v.size());
	glBindVertexArray(0);

	// hull
	mat4 sphereModelMatrix;
	sphereModelMatrix = rotate(sphereModelMatrix, angleRad, rotation);
	sphereModelMatrix = scale(sphereModelMatrix, vec3(0.5f, 0.5f, 2.0f));
	renderShape(sphereShader, vaos[0], true, sizeof(octahedronIndices), sphereModelMatrix, true, true, false);

	// back bit
	mat4 sphereModelMatrix2;
	sphereModelMatrix2 = rotate(sphereModelMatrix2, angleRad, rotation);
	sphereModelMatrix2 = scale(sphereModelMatrix2, vec3(0.5f, 0.6f, 0.3f));
	sphereModelMatrix2 = translate(sphereModelMatrix2, vec3(0.0f, 0.0f, 3.2f));
	renderShape(sphereShader, vaos[0], true, sizeof(octahedronIndices), sphereModelMatrix2, true, true, false);


	coneModelMatrix[1] = rotate(coneModelMatrix[1], angleRad, rotation);
	coneModelMatrix[1] = scale(coneModelMatrix[1], vec3(0.2f, 0.2f, 0.2f));
	coneModelMatrix[1] = translate(coneModelMatrix[1], vec3(0.0f, 0.9f, 4.f));

	coneModelMatrix[2] = rotate(coneModelMatrix[2], angleRad, rotation);
	coneModelMatrix[2] = scale(coneModelMatrix[2], vec3(0.2f, 0.2f, 0.2f));
	coneModelMatrix[2] = translate(coneModelMatrix[2], vec3(0.0f, -0.9f, 4.f));

	glUseProgram(coneShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	passModelToShader(coneShader, coneModelMatrix[1]);
	setupViewProjectionMatrix(coneShader);
	glBindVertexArray(vaos[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, v.size());
	glBindVertexArray(0);

	passModelToShader(coneShader, coneModelMatrix[2]);
	setupViewProjectionMatrix(coneShader);
	glBindVertexArray(vaos[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, v.size());
	glBindVertexArray(0);

}
void renderScene_d() 
{
	//mat4 sphereModelMatrix;
	//sphereModelMatrix = rotate(sphereModelMatrix, timeValue * radians(45.0f), vec3(0.0f, 1.0f, 0.0f));

	//// Updates the size of the sphere
	//unsigned int sphereRadiusLoc = glGetUniformLocation(sphereShader, "sphereRadius");
	//glUniform1f(sphereRadiusLoc, sphereRadius);

	//unsigned int texFlagLoc = glGetUniformLocation(sphereShader, "texFlag");
	//glUniform1i(texFlagLoc, 1);

	//GLfloat tessLevel = 12.0f;
	//unsigned int tessLevelLoc = glGetUniformLocation(sphereShader, "tessLevel");
	//glUniform1f(tessLevelLoc, tessLevel);
	//renderShape(sphereShader, vaos[0], true, sizeof(octahedronIndices), sphereModelMatrix, true, false, true);

	mat4 boxModelMatrix;
	boxModelMatrix = translate(boxModelMatrix, vec3(0.0f, 0.0f, 0.0f));
	boxModelMatrix = rotate(boxModelMatrix, timeValue * radians(60.0f), vec3(0.5f, 1.0f, 0.0f));
	renderShape(boxShader, vaos[1], false, sizeof(boxVertices), boxModelMatrix, false, false, true);
}

void renderShape(GLuint shader, GLuint vao, GLboolean elementsFlag, GLuint size, mat4 modelMatrix, GLboolean tessFlag, GLboolean wireframeFlag, GLboolean textureFlag)
{
	glUseProgram(shader);
	
	passModelToShader(shader, modelMatrix);
	setupViewProjectionMatrix(shader);

	/* Check whether the shape needs to be in wireframe or not
		and set polygonMode accordingly
	*/
	if (wireframeFlag) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (textureFlag) {
		glBindTexture(GL_TEXTURE_2D, texture1);
	}

	glBindVertexArray(vao);

	// Determines if there are tesselation shaders involved and if using ebos or just vaos.
	if (elementsFlag) {
		if (tessFlag) {
			glPatchParameteri(GL_PATCH_VERTICES, 3); // tell OpenGL that every patch has 3 verts (HAVE HARDCODED FOR TRIANGLES)
			glDrawElements(GL_PATCHES, size, GL_UNSIGNED_INT, 0); 
		}
		else {
			glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0); 
		}
	}
	else {
		if (tessFlag) {
			glPatchParameteri(GL_PATCH_VERTICES, 3);
			glDrawArrays(GL_TRIANGLES, 0, size);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, size);
		}
	}

	glBindVertexArray(0);
}

void setupGeometry()
{

	// Generate the buffer and array objbects 
	glGenBuffers(vboSize, vbos);
	glGenVertexArrays(vaoSize, vaos);
	glGenBuffers(eboSize, ebos);

	// ------------ For the Octahedron
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(octahedronVertices), octahedronVertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(octahedronIndices), octahedronIndices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ------------- Do the same for the box
	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	//For the points 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// For the texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Unbind the vbo and vao, but DO NOT unbind the ebo before the vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cone1(vaos[2], vbos[2]);

}

void setupTexture()
{
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set the texture wrapping/filtering options --> to the one bound
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S = x for textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T = y for textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the image on the texture 
	int texWidth, texHeight, nrChannels;
	unsigned char *tex1Data = stbi_load("./images/box1.jpg", &texWidth, &texHeight, &nrChannels, 0);
	// If the image is loaded properly then create image on the temporarily bound texture object
	if (tex1Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1Data);
		glGenerateMipmap(GL_TEXTURE_2D); // Generates a mipmap for the texture for all levels 
	}
	else
	{
		std::cout << "Failed to load texture image! " << endl;
	}
	// Free the image memory after generating the texture and corresponding the mipmaps
	stbi_image_free(tex1Data);

}

void passModelToShader(GLuint shader, mat4 modelMatrix)
{
	// modelMatrix = rotate(modelMatrix, timeValue * radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));

}

void setupViewProjectionMatrix(GLuint shader)
{
	mat4 viewMatrix;
	mat4 projectionMatrix;

	viewMatrix = translate(viewMatrix, vec3(0.0f, 0.0f, -4.0f));
	projectionMatrix = perspective(radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	// Set the 3d matrix transforms
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projectionMatrix));
}

void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN) {
		std::cout << "Key unknown" << endl;
	}

	// Set exit window bool to true, thus closes window
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
		std::cout << "Exiting Program" << endl;
		exitWindow = true;
	}

	switch (key)
	{
	case GLFW_KEY_A:
		std::cout << "Key A Pressed. " << endl;
		currentScene = scene_a;
		std::cout << currentScene << endl;
		break;
	case GLFW_KEY_B:
		std::cout << "Key B Pressed. " << endl;
		currentScene = scene_b;
		std::cout << currentScene << endl;
		break;
	case GLFW_KEY_C:
		std::cout << "Key C Pressed. " << endl;
		currentScene = scene_c;
		std::cout << currentScene << endl;
		break;
	case GLFW_KEY_D:
		std::cout << "Key D Pressed. " << endl;
		currentScene = scene_d;
		std::cout << currentScene << endl;
		break;
	default:
		break;
	}

}

void cone1(GLuint vao, GLuint vbo) {
	float cf = 0.0;
	Vertex t;
	t.color[0] = cf;
	cf = 1. - cf;
	t.color[1] = cf;
	cf = 1. - cf;
	t.color[2] = cf;
	cf = 1. - cf;
	t.position[0] = 0.0f;
	t.position[1] = 0.0f;
	t.position[2] = 0.0f;

	v.push_back(t); // Apex
	int lod = 64;
	float step = 2. * 3.141596 / float(lod);
	float Radius = 0.6f;
	for (float a = 0; a <= (2. * 3.141596 + step); a += step) {
		float c = Radius * cos(a);
		float s = Radius * sin(a);
		t.position[0] = c;
		t.position[1] = s;
		t.position[2] = 2.0; // set to 0.0 for a circle, >= 1.0 for a cone.
		t.color[0] = cf;
		cf = 1. - cf;
		t.color[1] = cf;
		cf = 1. - cf;
		t.color[2] = cf;
		cf = 1. - cf;
		v.push_back(t);
	}
	printf("Size %d\n", v.size());
	//
	glBindVertexArray(vao);
	/* Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	/* Copy the vertex data from cone to our buffer */
	/* v,size() * sizeof(GLfloat) is the size of the cone array, since it contains 12 Vertex values */
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(struct Vertex), v.data(), GL_STATIC_DRAW);
	/* Specify that our coordinate data is going into attribute index 0, and contains three doubles per vertex */
	/* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0 */
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));
	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);
	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
	/* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
	occupied by the first attribute (position) */
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));   // bug );
																																	 /* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);  /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
	glBindVertexArray(0);
}
