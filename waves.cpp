#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

#include <common/shader.hpp>
#include "ArcBall.hpp"
#include "HeightMesh.hpp"
#include "GerstnerWaveMesh.hpp"

float win_width = 1024.0f;
float win_height = 768.0f;

ArcBall ab(win_width,win_height);
vec3 ab_vec;
vec3 view_vector(-4,-3,-3);
vec3 view_center(0,0,0);
vec3 view_up;
mat4 Projection, View, Model, MVP;
float aspect_ratio;
float view_vector_length;

//-- These functions from the tutorial will not be used. 
GLuint CreateCube();
GLuint CreateTriangle();
GLuint CreateSquare();

#define WAVE_GRID_SIZE 200
#define WAVE_SIZE 20.0f
GerstnerWaveMesh* waves;

double timeFromStart=0.0f;

void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback (GLFWwindow*, int, int, int, int);
void cursor_pos_callback (GLFWwindow*, double, double);
void button_callback (GLFWwindow*, int, int, int);
void scroll_callback (GLFWwindow*, double, double);

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( (int)win_width, (int)win_height, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetKeyCallback(window,key_callback);
	glfwSetCursorPosCallback(window,cursor_pos_callback);
	glfwSetMouseButtonCallback(window,button_callback);
	glfwSetScrollCallback(window,scroll_callback);

	aspect_ratio = win_width / win_height;
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//GLuint cubeVAO = CreateCube();
	//GLuint triangleVAO = CreateTriangle();
	GLuint squareVAO = CreateSquare();

	waves = new GerstnerWaveMesh(WAVE_GRID_SIZE,WAVE_GRID_SIZE,WAVE_SIZE,WAVE_SIZE);
	//void Randomize(unsigned int n, float max_A, float max_L, float max_s);
	waves->Randomize(2,0.5f,3.0f,5.0f,1.0f);
//	waves->Randomize(5,0.2f,7.0f,10.0f,0.1f);
	waves->Randomize(5,0.1f,1.0f,2.0f,2.0f);
//	waves->AddWave(glm::vec2(1,1),0.3f,4.0f,0.2f,0.5f);
//	waves->AddWave(glm::vec2(0,4),0.1f,2.0f,0.5f,0.1f);
//	waves->AddWave(glm::vec2(2,-1),0.2f,10.0f,0.1f,0.3f);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	// Use our shader
	glUseProgram(programID);
	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

// Check if the ESC key was pressed or the window was closed
//	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
//		   glfwWindowShouldClose(window) == 0 );
	while( !glfwWindowShouldClose(window) ) {
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(45.0f, aspect_ratio, 0.1f, 100.0f);
		// Camera matrix
		view_up = glm::normalize(glm::cross(glm::cross(view_vector,vec3(0,1,0)),view_vector));
		view_vector_length = glm::length(view_vector);
		View = glm::lookAt(view_center-view_vector,view_center,view_up);
//			glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
//			glm::vec3(0,0,0), // and looks at the origin
//			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//		);
		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::mat4(1.0f);  // Changes for each model !
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		// For each model you render, since the MVP will be different (at least the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw nothing, see you in tutorial 2 !
		// 1rst attribute buffer : vertices
		//glBindVertexArray(cubeVAO);
		// Draw the Cube
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
 
		// Draw triangle
		//glBindVertexArray(triangleVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw square
		//glBindVertexArray(squareVAO);
		//glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (void*)0);
		double t = glfwGetTime();
		waves->Simulate(t-timeFromStart);
		timeFromStart = t;
		waves->Display();

//		glBindVertexArray(0);
//		glDisableVertexAttribArray(0);
//		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} 
	
//	glDeleteBuffers(1,&vertexbuffer);
//	glDeleteBuffers(1,&colorbuffer);
//	glDeleteBuffers(2,trianglebuffer);
//	glDeleteBuffers(2,squareVbo);
//	glDeleteVertexArrays(1,&VertexArrayID);
//	glDeleteVertexArrays(1,&triangleArrayID);
//	glDeleteVertexArrays(1,&squareVao);

	delete waves;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

//---------------------------------
// Supporting call-back functions
//---------------------------------
void framebuffer_size_callback (GLFWwindow* window, int width, int height) {
	win_width = (double)width;
	win_height = (double)height;
	ab.SetScreenSize(win_width,win_height);
	aspect_ratio = win_width/win_height;
	glViewport(0,0,width,height);
}


void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {
//	printf("KEY - %d %d %d %d\n",key,scancode,action,mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursor_pos_callback (GLFWwindow* window, double x, double y) {
	vec3 curr_ab_vec;
	bool bMB1 = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_1)==GLFW_PRESS);
	bool bMB2 = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2)==GLFW_PRESS);
	bool bMB3 = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_3)==GLFW_PRESS);
	bool bCtrl = (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS) || (glfwGetKey(window,GLFW_KEY_RIGHT_CONTROL)==GLFW_PRESS);
	if (bMB1 && bCtrl) {
		curr_ab_vec = ab.GetVector(x,y);
		vec3 axis = glm::cross(ab_vec,curr_ab_vec);
		double s = sqrt((1.0f+glm::dot(ab_vec,curr_ab_vec))*2.0f);
		quat qrot(s*0.5f,axis.x/s,axis.y/s,axis.z/s);
		view_vector = view_vector * qrot;
		view_up = view_up * qrot;
		ab_vec = curr_ab_vec;
	} else if (bMB2 && bCtrl) {
		curr_ab_vec = ab.GetVector(x,y);
		vec3 delta = (curr_ab_vec-ab_vec)*view_vector_length;	//-- in camera space 
		delta = glm::inverse(glm::mat3(View))*delta;	//-- in world space
		view_center = view_center - delta;
		ab_vec = curr_ab_vec;
	} else if (bMB3 && bCtrl) {
		curr_ab_vec = ab.GetVector(x,y);
		float factor = curr_ab_vec.y-ab_vec.y;
		view_vector = view_vector*(1.0f+factor);
		view_vector_length = glm::length(view_vector);
		ab_vec = curr_ab_vec;
	} else {
		ab_vec = ab.GetVector(x,y);
	}
}

void button_callback (GLFWwindow* window, int button, int action, int mods) {
	double x,y;
	glfwGetCursorPos(window,&x,&y);
	printf("BUTTON - %6.1f %6.1f %d %d %d\n",x,y,button,action,mods);
	if (button==GLFW_MOUSE_BUTTON_1 && action==GLFW_PRESS) {
		vec3 pointer;
		pointer = ab.GetVector(x,y);
		printf("%6.4f %6.4f %6.4f\n",pointer.x,pointer.y,pointer.z);
	} 
}

void scroll_callback (GLFWwindow* window, double x, double y) {
	if (y>0.0f) {
		view_vector = view_vector*0.9f;
		view_vector_length = glm::length(view_vector);
	} else if (y<0.0f) {
		view_vector = view_vector*1.1f;
		view_vector_length = glm::length(view_vector);
	}
}


//---------------------------------
// Object and drawing functions
//---------------------------------
GLuint CreateCube () {
	static const unsigned int index_data[] = {
		0, 5, 1,
		0, 4, 5,
		1, 5, 7, 
		1, 7, 3, 
		2, 3, 7, 
		2, 7, 6, 
		0, 2, 6,
		0, 6, 4, 
		4, 6, 7, 
		4, 7, 5, 
		0, 1, 3, 
		0, 3, 2
	};
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
	    0.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f,
	    0.0f, 1.0f, 0.0f,
	    0.0f, 1.0f, 1.0f,
	    1.0f, 0.0f, 0.0f,
	    1.0f, 0.0f, 1.0f,
	    1.0f, 1.0f, 0.0f,
	    1.0f, 1.0f, 1.0f
	};

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint indexbuffer;
	glGenBuffers(1,&indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index_data), index_data, GL_STATIC_DRAW);

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	GLuint colorbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &colorbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
	   1,                  // attribute 1.
	   3,                  // size	
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	return VertexArrayID;
}

GLuint CreateTriangle() {
	static const GLfloat triangle_vertex[] = {
	    -1.5f, 1.5f,-1.0f, // triangle 1 : begin
	    -1.0f, 1.5f, 1.0f,
	    -1.0f, 2.5f, 1.0f  // triangle 1 : end
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat triangle_color[] = {
	    1.0f,  0.0f,  0.0f,
	    0.0f,  1.0f,  0.0f,
	    0.0f,  0.0f,  1.0f
	};
	GLuint triangleArrayID;
	glGenVertexArrays(1, &triangleArrayID);
	glBindVertexArray(triangleArrayID);

	GLuint trianglebuffer[2];
	glGenBuffers(2, trianglebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trianglebuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex), triangle_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, trianglebuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_color), triangle_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	return triangleArrayID;
}

GLuint CreateSquare() {
	static const unsigned int square_index[] = {
	    0, 1,
	    1, 2,
	    2, 3,
	    3, 0
	};
	static const GLfloat square_vertex[] = {
	    1.5f, 0.0f, 1.0f,
	    2.0f, 0.0f,-0.5f,
	    2.0f, 2.0f,-0.5f,
	    1.5f, 2.0f, 1.0f
	};
	static const GLfloat square_color[] = {
	    1.0f,  0.0f,  0.0f,
	    0.0f,  1.0f,  0.0f,
	    0.0f,  0.0f,  1.0f,
	    1.0f,  1.0f,  1.0f
	};
	GLuint squareVao;
	glGenVertexArrays(1, &squareVao);
	glBindVertexArray(squareVao);

	GLuint squareVbo[3];
	glGenBuffers(3, squareVbo);
	glBindBuffer(GL_ARRAY_BUFFER, squareVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertex), square_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, squareVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_color), square_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareVbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_index), square_index, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	return squareVao;
}


