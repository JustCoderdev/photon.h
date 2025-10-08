#ifndef PHOTON_H
#define PHOTON_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* OpenGL 1.1 spec <https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf> */
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <photon-common.h>


/* Geometry */
/* ------------------------------------------------------------ */

extern void draw_circle_flat(Point pos, float size, Ratio aspect_ratio, n8 steps, ColorRGB color);
extern void draw_triangles(Point pos, float size, Ratio aspect_ratio, n8 steps, ColorRGB color);


/* Display */
/* ------------------------------------------------------------ */

extern GLFWwindow *g_window;
extern Size g_viewport;
extern Bool g_display_is_alive;

extern Error display_init(Size* viewport);
extern void  display_refresh(void);
extern void  display_free(void);

extern Bool  display_is_alive(void);


/* Input */
/* ------------------------------------------------------------ */

extern void inputs_poll(void);
extern void inputs_get_cursor(Point* cursor_pos);


/* Runner API */
/* ------------------------------------------------------------ */

#define RUNNER_DL_NAME "gas.so"

#ifndef PHOTON_RUNNER_H
typedef void Runner_State;
#endif /* PHOTON_RUNNER_H */

typedef Runner_State* (*Runner_Init_Func)(Size viewport, float aspect_ratio);
typedef Bool          (*Runner_Loop_Func)(Runner_State* state); /* float delta_time */
typedef void          (*Runner_Deinit_Func)(Runner_State* state);

typedef struct {
	void*              handle;
	Runner_Init_Func   runner_init;
	Runner_Loop_Func   runner_loop;
	Runner_Deinit_Func runner_deinit;
} Runner_Actions;

extern Error runner_load(Runner_Actions* actions);
extern Error runner_unload(Runner_Actions* actions);


/* Utils */
/* ------------------------------------------------------------ */

extern Error glLogErrors(char* file, int line);


/* Pedantic Workarounds */
/* ------------------------------------------------------------ */

extern Runner_Init_Func   pw_vp_to_fp_runner_init(void* func);
extern Runner_Loop_Func   pw_vp_to_fp_runner_loop(void* func);
extern Runner_Deinit_Func pw_vp_to_fp_runner_deinit(void* func);


#endif /* PHOTON_H */
