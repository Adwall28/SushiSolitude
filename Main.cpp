#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<include/irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace std;
using namespace irrklang;

#include"ShaderClass.h"
#include"EBO.h"
#include"VAO.h"
#include"VBO.h"
#include"Texture.h"
#include"Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;



int main() 
{

	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
		return 0;
	//Gets file which has the music
	ISoundSource* snd = engine->addSoundSourceFromFile("Media/relaxing-mountains-rivers-streams-running-water-18178.mp3");
	//Sets Music Volume
	snd->setDefaultVolume(0.6f);
	//Plays background music
	engine->play2D(snd, true);

	// Initialise GLFW
	glfwInit();

	// Telling GLFW which version of OpenGL is in use. I am using version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Specifying to GLFW I am using the Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Vertices Co-ordinates (in this case, a triangle)
	GLfloat Trianglevertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //lower right corner
		0.0f, 0.5f * float(sqrt(3)) / 2 / 3, 0.0f //upper corner
	};


	GLfloat verticesy[] =
	{ //			COORDINATES				/			COLOURS			
		-0.5f, -0.5f * float(sqrt(3)) / 3,      0.0f,  0.8f, 0.3f, 0.02f, //lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3,       0.0f,  0.8f, 0.3f, 0.02f, //lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3,    0.0f,  1.0f, 0.6f, 0.32f,//upper corner
		-0.25f , 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, //inner left
		0.25f , 0.5f * float(sqrt(3)) / 6,      0.0f,  0.9f, 0.45f, 0.17f, //inner right
		0.0f, -0.5f * float(sqrt(3)) / 3,       0.0f,  0.8f, 0.3f, 0.02f //inner down
	};

	GLfloat verticesx[] =
	{ //			COORDINATES		/		COLOURS			/    TEXTURES
		-0.5f, -0.5f, 0.0f,				0.8f, 0.3f, 0.02f,      0.0f, 0.0f,//lower left corner
		-0.5f, 0.5f, 0.0f,				0.8f, 0.3f, 0.02f,      0.0f, 1.0f,//upper left corner
		0.5f, 0.5f, 0.0f,				1.0f, 0.6f, 0.32f,	    1.0f, 1.0f,//upper right corner
		0.5f , -0.5f, 0.0f,				0.9f, 0.45f, 0.17f,     1.0f, 0.0f,//lower left corner
	};

	GLfloat vertices[] =
	{
		-1.0, -1.0, 1.0,      1.0, 0.0, 0.0,      1.0f, 1.0f,
		1.0, -1.0, 1.0,		  0.0, 1.0, 0.0,	1.0f, 1.0f,
		1.0, 1.0, 1.0,		 0.0, 0.0, 1.0,		1.0f, 1.0f,
		-1.0, 1.0, 1.0,		 1.0, 1.0, 1.0,		1.0f, 1.0f,
		// back
		-1.0, -1.0, -1.0,   1.0, 0.0, 0.0,		1.0f, 1.0f,
		1.0, -1.0, -1.0,     0.0, 1.0, 0.0,		1.0f, 1.0f,
		1.0, 1.0, -1.0,      0.0, 0.0, 1.0,		1.0f, 1.0f,
		-1.0, 1.0, -1.0,	 1.0, 1.0, 1.0,		1.0f, 1.0f,
	};

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0,

		1,5,6,
		6,2,1,

		7,6,5,
		5,4,7,

		4,0,3,
		3,7,4,

		4,5,1,
		1,0,4,

		3,2,6,
		6,7,3
	};


	//Creates a window of 1500 width, 800 height, with the name "Sushi Solitude"
	GLFWwindow* window = glfwCreateWindow(width, height, "Sushi Solitude", NULL, NULL);

	// Error Checking if window doesn't load
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;

	}

	// Creates window that was previously stated
	glfwMakeContextCurrent(window);

	// Load Glad
	gladLoadGL();

	//Specifying the viewport of OpenGL
	//The 0,0 represent the co-ordinates of the bottom left point
	//The 1500,800 represent the co-ordinates of the top right point
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();




	//Texture

	Texture Salmon("Textures/Salmon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Salmon.texUnit(shaderProgram, "tex0", 0);

	Texture Sushi("Textures/Sushi.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Sushi.texUnit(shaderProgram, "tex0", 0);

	Texture Rice("Textures/SalmonWithRice.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Rice.texUnit(shaderProgram, "tex0", 0);


	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//Main while loop, so that program doesn't end until window has been closed
	while (!glfwWindowShouldClose(window))
	{
		//Make Grey colour
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		//Alternative colour choice
		//glClearColor(0.9f, 1.0f, 0.95f, 1.0f);
		//Clean back buffer and assigns new colour to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		camera.Matrix(shaderProgram, "camMatrix");

		//Binds Texture
		Sushi.Bind();

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			Sushi.Unbind();
			Salmon.Bind();
		}

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			Sushi.Unbind();
			Rice.Bind();
		}


		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		//Swaps buffers
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	//Stops Music
	engine->drop();

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	Sushi.Delete();
	shaderProgram.Delete();

	//Closes window
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}