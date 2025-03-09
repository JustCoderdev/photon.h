#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* OpenGL 1.1 spec <https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf> */
#include <GL/gl.h>
#include <GLFW/glfw3.h>

typedef unsigned char     n8;
typedef unsigned short    n16;
typedef unsigned int      n32;
typedef unsigned long int n64;

typedef signed char     i8;
typedef signed short    i16;
typedef signed int      i32;
typedef signed long int i64;

typedef enum {
	true  = (1 == 1),
	false = (1 != 1)
} bool;

typedef enum {
	success = false,
	failure = true
} error;

typedef int Errno;

typedef struct {
	float x, y;
} Point;

typedef struct {
	int width, height;
} Size;

#define PI 3.1415926535f

/* -1..1 -> 0..1 */
#define RAD_TO_RAT(RAD) (float)(((RAD) + 1) / 2)
/* 0..1 -> -1..1 */
#define RAT_TO_RAD(RAT) (float)(((RAT) - 0.5f ) * 2)

#define PRINT_GL_ERRORS() glPrintErrors(__FILE__, __LINE__)
static void glPrintErrors(char* file, int line)
{
	static unsigned long ec = 0;

	char* msg = "NO_ERROR";
	GLenum e;

	do {
		switch(e = glGetError())
		{
			case GL_NO_ERROR: return;
			case GL_INVALID_ENUM:      msg = "INVALID_ENUM";      break;
			case GL_INVALID_VALUE:     msg = "INVALID_VALUE";     break;
			case GL_INVALID_OPERATION: msg = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:    msg = "STACK_OVERFLOW";    break;
			case GL_STACK_UNDERFLOW:   msg = "STACK_UNDERFLOW";   break;
			case GL_OUT_OF_MEMORY:     msg = "OUT_OF_MEMORY";     exit(failure);
			default: assert(0 && "UNREACHABLE");
		}
		printf("[%lu] GLERROR:%s:%d: %s\n", ec++, file, line, msg);
	}
	while(e != GL_NO_ERROR);
}


static void handle_inputs(GLFWwindow *window, Size* viewport, Point* cursor_pos)
{
	double cx, cy;
	int vw = viewport->width,
		vh = viewport->height;

	/* Update cursor */
	glfwGetCursorPos(window, &cx, &cy);
	cursor_pos->x = RAT_TO_RAD(cx / vw);
	cursor_pos->y = -RAT_TO_RAD(cy / vh);

	/* Update keyboard */
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

static void draw_gay_circle(float x, float y, int steps, float aspect_ratio, float size)
{
	int i;

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);

		for(i = 0; i <= steps; ++i)
		{
			double deg = ((float)i / (float)steps) * 2 * PI;
			glColor3f(RAD_TO_RAT(sin(deg)), 0.0f, RAD_TO_RAT(cos(deg)));
			glVertex3f(x + (float)cos(deg) * size, y + (float)sin(deg) * size * aspect_ratio, 0.0f);
		}
	glEnd();
	PRINT_GL_ERRORS();
}

static void draw_flat_circle(float x, float y, int steps, float aspect_ratio, float size, float r, float g, float b)
{
	int i;

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(r, g, b);
		glVertex3f(x, y, 0.0f);

		for(i = 0; i <= steps; ++i)
		{
			double deg = ((float)i / (float)steps) * 2 * PI;
			/* glColor3f(RAD_TO_RAT(sin(deg)), 0.0f, RAD_TO_RAT(cos(deg))); */
			glVertex3f(x + (float)cos(deg) * size, y + (float)sin(deg) * size * aspect_ratio, 0.0f);
		}
	glEnd();
	PRINT_GL_ERRORS();
}

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

/* Creating a window X11 <https://hereket.com/posts/linux_creating_x11_windows> */
int main(void) {
	float aspect_ratio = 0;
	Size viewport = {0};
	Point cursor_pos = {0};
	int i;

	GLFWwindow* window;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Emulator", NULL, NULL);
	if(window == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: Failed to create window\n", __FILE__, __LINE__);
		glfwTerminate();
		exit(failure);
	}

	glfwMakeContextCurrent(window);
	printf("OpenGL Version %s\n", glGetString(GL_VERSION));

	glfwGetFramebufferSize(window, &viewport.width, &viewport.height);
	aspect_ratio = (float)viewport.width / (float)viewport.height;
	printf("Viewport %dx%d (%.2f)\n", viewport.width, viewport.height, aspect_ratio);

	glViewport(0, 0, viewport.width, viewport.height);
	PRINT_GL_ERRORS();

	while(!glfwWindowShouldClose(window))
	{
		handle_inputs(window, &viewport, &cursor_pos);
		/* -------------------- */

		glClearColor(0.094f, 0.094f, 0.094f, 1.0);
		PRINT_GL_ERRORS();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		PRINT_GL_ERRORS();

		/* glBegin(GL_TRIANGLES); */
		/* 	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(  0.0f,  0.5f, 0.0f); */
		/* 	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( -0.5f, -0.5f, 0.0f); */
		/* 	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( +0.5f, -0.5f, 0.0f); */
		/* glEnd(); */
		/* PRINT_GL_ERRORS(); */

		glPointSize(10.0f);
		PRINT_GL_ERRORS();

		glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(cursor_pos.x, cursor_pos.y, 0.0f);
		glEnd();
		PRINT_GL_ERRORS();

		{
			/* float x = cursor_pos.x, */
			/* 	  y = cursor_pos.y; */
			float size = 0.2f;
			float x = 0.0f,
				  y = 0.0f;
			float xoff = 0.15f,
				  yoff = 0.5f;
			float r = 1.00f,
				  g = 0.0f,
				  b = 0.0f;

			draw_gay_circle(x - xoff, y + yoff, 16, aspect_ratio, size);
			draw_gay_circle(x + xoff, y + yoff, 16, aspect_ratio, size);

			/* glBegin(GL_TRIANGLES); */
			/* 	glColor3f(r, g, b); glVertex3f(x - xoff - size, y + yoff - 0.1f, 0.0f); */
			/* 	glColor3f(r, g, b); glVertex3f(x + xoff + size, y + yoff - 0.1f, 0.0f); */
			/* 	glColor3f(r, g, b); glVertex3f(0.0f, -0.3f, 0.0f); */
			/* glEnd(); */

			/* draw_flat_circle(x - xoff, y + yoff, 16, aspect_ratio, size, r, g, b); */
			/* draw_flat_circle(x + xoff, y + yoff, 16, aspect_ratio, size, r, g, b); */

			(void)i;
		}

		/* -------------------- */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return success;
}
