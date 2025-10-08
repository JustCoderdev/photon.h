#ifndef PHOTON_H
#define PHOTON_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#include <photon-common.h>


/* Geometry */
/* ------------------------------------------------------------ */

extern void draw_circle_flat(Point pos, float size, Ratio aspect_ratio, n8 steps, ColorRGB color);
extern void draw_triangles(Point pos, float size, Ratio aspect_ratio, n8 steps, ColorRGB color);


/* Display */
/* ------------------------------------------------------------ */

extern Error display_init(Window_State* state, n16 viewport_width, n16 viewport_height);
extern Bool  display_is_alive(Window_State* state);
extern void  display_refresh(Window_State* state);
extern void  display_free(void);


/* Input */
/* ------------------------------------------------------------ */

extern void inputs_poll(Window_State* state);
extern void inputs_get_cursor(Window_State* state, Point* cursor_pos);


/* Runner API */
/* ------------------------------------------------------------ */

#define RUNNER_DL_NAME "gas.so"

#ifndef PHOTON_RUNNER_H
typedef void Runner_State;
#endif /* PHOTON_RUNNER_H */

typedef Runner_State* (*Runner_Init_Func)(Window_State* window_state);
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
