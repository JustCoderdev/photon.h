#include "../include/photon.h"

#include <dlfcn.h>
/*
void *dlopen(const char *filename, int flags);
int dlclose(void *handle);
void *dlsym(void *restrict handle, const char *restrict symbol);
char *dlerror(void);
*/

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

static PhysicalPressedKeyField pressed_keys = {0};
static void glfw_character_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/* TODO: Reset after x frames */
	memset(&pressed_keys, 0, sizeof(pressed_keys));

	(void)window,(void)scancode;

#if DEBUG_IO_KEYBOARD
	printf("IO:KEYBOARD: ");
	switch(action)
	{
		case GLFW_PRESS   : printf("v "); break;
		case GLFW_REPEAT  : printf("v*"); break;
		case GLFW_RELEASE : printf("^ "); break;
		default: assert(0 && "UNREACHABLE");
	}
	printf(" %c (raw %d), mods %d\n", key, key, mods);
#endif

	switch(key)
	{
		case GLFW_KEY_A: pressed_keys.key_a = 1; break;
		case GLFW_KEY_B: pressed_keys.key_b = 1; break;
		case GLFW_KEY_C: pressed_keys.key_c = 1; break;
		case GLFW_KEY_D: pressed_keys.key_d = 1; break;
		case GLFW_KEY_E: pressed_keys.key_e = 1; break;
		case GLFW_KEY_F: pressed_keys.key_f = 1; break;
		case GLFW_KEY_G: pressed_keys.key_g = 1; break;
		case GLFW_KEY_H: pressed_keys.key_h = 1; break;
		case GLFW_KEY_I: pressed_keys.key_i = 1; break;
		case GLFW_KEY_J: pressed_keys.key_j = 1; break;
		case GLFW_KEY_K: pressed_keys.key_k = 1; break;
		case GLFW_KEY_L: pressed_keys.key_l = 1; break;
		case GLFW_KEY_M: pressed_keys.key_m = 1; break;
		case GLFW_KEY_N: pressed_keys.key_n = 1; break;
		case GLFW_KEY_O: pressed_keys.key_o = 1; break;
		case GLFW_KEY_P: pressed_keys.key_p = 1; break;
		case GLFW_KEY_Q: pressed_keys.key_q = 1; break;
		case GLFW_KEY_R: pressed_keys.key_r = 1; break;
		case GLFW_KEY_S: pressed_keys.key_s = 1; break;
		case GLFW_KEY_T: pressed_keys.key_t = 1; break;
		case GLFW_KEY_U: pressed_keys.key_u = 1; break;
		case GLFW_KEY_V: pressed_keys.key_v = 1; break;
		case GLFW_KEY_W: pressed_keys.key_w = 1; break;
		case GLFW_KEY_X: pressed_keys.key_x = 1; break;
		case GLFW_KEY_Y: pressed_keys.key_y = 1; break;
		case GLFW_KEY_Z: pressed_keys.key_z = 1; break;

		case GLFW_KEY_APOSTROPHE : pressed_keys.smb_sqt = 1; /* ' */ break;
		case GLFW_KEY_COMMA      : pressed_keys.smb_cma = 1; /* , */ break;
		case GLFW_KEY_MINUS      : pressed_keys.smb_min = 1; /* - */ break;
		case GLFW_KEY_PERIOD     : pressed_keys.smb_dot = 1; /* . */ break;
		case GLFW_KEY_SLASH      : pressed_keys.smb_fsl = 1; /* / */ break;

		case GLFW_KEY_0 : pressed_keys.num_0 = 1; break;
		case GLFW_KEY_1 : pressed_keys.num_1 = 1; break;
		case GLFW_KEY_2 : pressed_keys.num_2 = 1; break;
		case GLFW_KEY_3 : pressed_keys.num_3 = 1; break;
		case GLFW_KEY_4 : pressed_keys.num_4 = 1; break;
		case GLFW_KEY_5 : pressed_keys.num_5 = 1; break;
		case GLFW_KEY_6 : pressed_keys.num_6 = 1; break;
		case GLFW_KEY_7 : pressed_keys.num_7 = 1; break;
		case GLFW_KEY_8 : pressed_keys.num_8 = 1; break;
		case GLFW_KEY_9 : pressed_keys.num_9 = 1; break;

		case GLFW_KEY_SEMICOLON     : pressed_keys.smb_scl = 1; /* ; */ break;
		case GLFW_KEY_EQUAL         : pressed_keys.smb_equ = 1; /* = */ break;
		case GLFW_KEY_LEFT_BRACKET  : pressed_keys.smb_lbk = 1; /* [ */ break;
		case GLFW_KEY_BACKSLASH     : pressed_keys.smb_bsl = 1; /* \ */ break;
		case GLFW_KEY_RIGHT_BRACKET : pressed_keys.smb_rbk = 1; /* ] */ break;
		case GLFW_KEY_GRAVE_ACCENT  : pressed_keys.smb_btk = 1; /* ` */ break;
	}
}

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
#if DEBUG_OPENGL
	printf("OpenGL Version %s\n", glGetString(GL_VERSION));
#endif

	glfwGetFramebufferSize(state->window, &state->viewport.width, &state->viewport.height);
	state->display_is_alive = true;

	glfwSetKeyCallback(state->window, glfw_character_callback);

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
	if(
		glfwGetKey(state->window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwGetKey(state->window, GLFW_KEY_Q) == GLFW_PRESS
	) {
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

#if DEBUG_IO_CURSOR_ENABLE
	printf("IO:CURSOR: %0.3fx%0.3f (raw %.0fx%.0f)\n", rx, ry, cx, cy);
#endif

	cursor_pos->x = rx;
	cursor_pos->y = ry;

#if DEBUG_IO_CURSOR
	printf("IO:CURSOR: %0.3fx%0.3f (raw %.0fx%.0f)\n", rx, ry, cx, cy);
#endif
}

void inputs_get_keys(Window_State* state, PhysicalPressedKeyField* keys)
{
	(void)state;
	*keys = pressed_keys;
}



/* Runner */
/* ------------------------------------------------------------ */

extern Error runner_load(char* filename, Runner_Actions* actions)
{
	assert(memset(actions, 0, sizeof(Runner_Actions)) == actions);

	actions->handle = dlopen(filename, RTLD_NOW);
	if(actions->handle == NULL)
	{
		printf("ERROR:%s:%d: (RUNNER) Could not load the dynamic library '%s': %s\n",
				__FILE__, __LINE__, filename, dlerror());
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

Error runner_unload(Runner_Actions* actions)
{
	if(actions == NULL) return success;
	if(actions->handle == NULL) return success;

	if(dlclose(actions->handle) == 0)
	{
		assert(memset(actions, 0, sizeof(Runner_Actions)) == actions);
		return success;
	}
	else
	{
		printf("ERROR:%s:%d: (RUNNER) Could not unload the dynamic library: %s \n",
				__FILE__, __LINE__, dlerror());
		return failure;
	}
}


/* Utils */
/* ------------------------------------------------------------ */

Error glLogErrors(char* file, int line)
{
	static unsigned long ec = 0;

	char* msg;
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
			case GL_OUT_OF_MEMORY:     msg = "OUT_OF_MEMORY";     (void)msg, exit(failure);
			default: assert(0 && "UNREACHABLE");
		}
		printf("[%lu] GLERROR:%s:%d: %s\n", ec++, file, line, msg);
	}
	while(e != GL_NO_ERROR);
	return failure;
}

