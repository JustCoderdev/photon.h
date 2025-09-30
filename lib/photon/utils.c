#include <photon.h>

/* Geometry */
/* ------------------------------------------------------------ */

void draw_circle_flat(Point pos, float size, Ratio aspect_ratio, n8 steps, ColorRGB color)
{
	n8 i;

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(color.r, color.g, color.b);
		glVertex3f(pos.x, pos.y, 0.0f);

		for(i = 0; i <= steps; ++i)
		{
			double deg = ((float)i / (float)steps) * 2 * PI;
			glVertex3f(pos.x + (float)cos(deg) * size, pos.y + (float)sin(deg) * size * aspect_ratio, 0.0f);
		}
	glEnd();
	GL_LOG_ERRORS();
}


/* Display */
/* ------------------------------------------------------------ */

GLFWwindow *g_window = NULL;
Size g_viewport = {0};
Bool g_display_is_alive = false;


Error display_init(Size* viewport)
{
	extern GLFWwindow* g_window;
	extern Size g_viewport;
	extern Bool g_display_is_alive;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	g_window = glfwCreateWindow(viewport->width, viewport->height, "Emulator", NULL, NULL);
	if(g_window == NULL) return failure;

	glfwMakeContextCurrent(g_window);
	printf("OpenGL Version %s\n", glGetString(GL_VERSION));

	glfwGetFramebufferSize(g_window, &g_viewport.width, &g_viewport.height);

	viewport->width = g_viewport.width;
	viewport->height = g_viewport.height;

	g_display_is_alive = true;

	return success;
}

void display_refresh(void)
{
	extern GLFWwindow* g_window;
	glfwSwapBuffers(g_window);
}

void display_free(void)
{
	glfwTerminate();
}

Bool display_is_alive(void)
{
	extern GLFWwindow* g_window;
	extern Bool g_display_is_alive;

	g_display_is_alive = !glfwWindowShouldClose(g_window);
	return g_display_is_alive;
}

/* Input */
/* ------------------------------------------------------------ */

extern void inputs_poll(void)
{
	extern GLFWwindow* g_window;

	glfwPollEvents();

	/* Check if should close window */
	if(glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(g_window, true);
	}
}

void inputs_get_cursor(Point* cursor_pos)
{
	extern GLFWwindow* g_window;
	extern Size g_viewport;

	double cx, cy; /* cursor xy */
	float  rx, ry; /* rad xy */

	int vw = g_viewport.width,
		vh = g_viewport.height;

	glfwGetCursorPos(g_window, &cx, &cy);

	rx = CLAMP_RAD( RAT_TO_RAD(cx / vw));
	ry = CLAMP_RAD(-RAT_TO_RAD(cy / vh));

	if(rx == cursor_pos->x && ry == cursor_pos->y) return;

	printf("IO:CURSOR: %0.3fx%0.3f (raw %.0fx%.0f)\n", rx, ry, cx, cy);
	cursor_pos->x = rx;
	cursor_pos->y = ry;
}


/* Utils */
/* ------------------------------------------------------------ */

Error glLogErrors(char* file, int line)
{
	static unsigned long ec = 0;

	char* msg = "NO_ERROR";
	GLenum e;

	do {
		switch(e = glGetError())
		{
			case GL_NO_ERROR:          return success;
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
	return failure;
}
