#include <photon.h>

#include<dlfcn.h>
/* 
void *dlopen(const char *filename, int flags);
int dlclose(void *handle);
void *dlsym(void *restrict handle, const char *restrict symbol);
char *dlerror(void);
*/

#include <string.h>
/*
void *memset(void s[.n], int c, size_t n);
*/


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

Error display_init(Window_State* state, n16 viewport_width, n16 viewport_height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	state->window = glfwCreateWindow(viewport_width, viewport_height, "Emulator", NULL, NULL);
	if(state->window == NULL) return failure;

	glfwMakeContextCurrent(state->window);
	printf("OpenGL Version %s\n", glGetString(GL_VERSION));

	glfwGetFramebufferSize(state->window, &state->viewport.width, &state->viewport.height);
	state->display_is_alive = true;

	return success;
}

void display_refresh(Window_State* state)
{
	assert(state->window != NULL);
	glfwSwapBuffers(state->window);
}

void display_free(void)
{
	glfwTerminate();
}

Bool display_is_alive(Window_State* state)
{
	state->display_is_alive = !glfwWindowShouldClose(state->window);
	return state->display_is_alive;
}

/* Input */
/* ------------------------------------------------------------ */

extern void inputs_poll(Window_State* state)
{
	glfwPollEvents();

	/* Check if should close window */
	if(glfwGetKey(state->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(state->window, true);
		state->display_is_alive = false;
	}

	if(glfwGetKey(state->window, GLFW_KEY_R) == GLFW_PRESS)
	{
		state->runner_should_reload = true;
	}
}

void inputs_get_cursor(Window_State* window_state, Point* cursor_pos)
{
	double cx, cy; /* cursor xy */
	float  rx, ry; /* rad xy */

	int vw = window_state->viewport.width,
		vh = window_state->viewport.height;

	glfwGetCursorPos(window_state->window, &cx, &cy);

	rx = CLAMP_RAD( RAT_TO_RAD(cx / vw));
	ry = CLAMP_RAD(-RAT_TO_RAD(cy / vh));

	if(rx == cursor_pos->x && ry == cursor_pos->y) return;

	printf("IO:CURSOR: %0.3fx%0.3f (raw %.0fx%.0f)\n", rx, ry, cx, cy);
	cursor_pos->x = rx;
	cursor_pos->y = ry;
}


/* Runner */
/* ------------------------------------------------------------ */

extern Error runner_load(Runner_Actions* actions)
{
	assert(memset(actions, 0, sizeof(Runner_Actions)) == actions);

	actions->handle = dlopen(RUNNER_DL_NAME, RTLD_NOW);
	if(actions->handle == NULL)
	{
		printf("ERROR:%s:%d: (RUNNER) Could not load the dynamic library '" RUNNER_DL_NAME "': %s\n",
				__FILE__, __LINE__, dlerror());
		return failure;
	}

	actions->runner_init = pw_vp_to_fp_runner_init(dlsym(actions->handle, "runner_init"));
	if(actions->runner_init == NULL)
	{
		printf("ERROR:%s:%d: (RUNNER) Could not load the symbol 'runner_init': %s \n",
				__FILE__, __LINE__, dlerror());
		return failure;
	}

	actions->runner_loop = pw_vp_to_fp_runner_loop(dlsym(actions->handle, "runner_loop"));
	if(actions->runner_loop == NULL)
	{
		printf("ERROR:%s:%d: (RUNNER) Could not load the symbol 'runner_loop': %s \n",
				__FILE__, __LINE__, dlerror());
		return failure;
	}

	actions->runner_deinit = pw_vp_to_fp_runner_deinit(dlsym(actions->handle, "runner_deinit"));
	if(actions->runner_deinit == NULL)
	{
		printf("ERROR:%s:%d: (RUNNER) Could not load the symbol 'runner_deinit': %s \n",
				__FILE__, __LINE__, dlerror());
		return failure;
	}

	return success;
}

extern Error runner_unload(Runner_Actions* actions)
{
	if(dlclose(actions->handle) == 0)
	{
		assert(memset(actions, 0, sizeof(Runner_Actions)) == actions);
		return success;
	}
	else
	{
		printf("ERROR:%s:%d: (RUNNER) Could not unload the dynamic library '"  RUNNER_DL_NAME "': %s \n",
				__FILE__, __LINE__, dlerror());
		return failure;
	}
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
