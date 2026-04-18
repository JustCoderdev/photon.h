#include "../lib/include/photon-runner.h"

#include <stdlib.h>

#include <string.h>
/* void *memset(void s[.n], int c, size_t n); */

Runner_State* runner_init(Window_State* window_state)
{
	Runner_State* state = malloc(sizeof(*state));
	(void)memset(state, 0, sizeof(*state));

	state->window_state = window_state;

	return state;
}

const Point square_pos = { -0.5f, -0.5f };
const SizeF square_size = { 1.0f,  1.0f };

Bool runner_loop(Runner_State* state)
{
	/* Size viewport = state->window_state->viewport; */
	/* float aspect_ratio = (float)state->window_state->viewport.width / (float)state->window_state->viewport.height; */

	Point cursor_pos;
	inputs_get_cursor(state->window_state, &state->cursor_pos);
	cursor_pos = state->cursor_pos;
	(void)cursor_pos;

	/* -------------------- */

	glClearColor(0.094f, 0.094f, 0.094f, 1.0);
	GL_LOG_ERRORS();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_LOG_ERRORS();

	/* -------------------- */


	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.0, 0.0, 0.0); glVertex3f(square_pos.x,                      square_pos.y,                     0.0f);
		glColor3f(0.0, 1.0, 0.0); glVertex3f(square_pos.x + square_size.height, square_pos.y,                     0.0f);
		glColor3f(0.0, 0.0, 1.0); glVertex3f(square_pos.x,                      square_pos.y + square_size.width, 0.0f);
		glColor3f(0.5, 0.5, 0.5); glVertex3f(square_pos.x + square_size.height, square_pos.y + square_size.width, 0.0f);
	glEnd();
	GL_LOG_ERRORS();

	glPointSize(10.0f);
	GL_LOG_ERRORS();

	glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0); glVertex3f(square_pos.x,                      square_pos.y,                     0.0f);
		glColor3f(0.0, 1.0, 0.0); glVertex3f(square_pos.x + square_size.height, square_pos.y,                     0.0f);
		glColor3f(0.0, 0.0, 1.0); glVertex3f(square_pos.x,                      square_pos.y + square_size.width, 0.0f);
		glColor3f(0.5, 0.5, 0.5); glVertex3f(square_pos.x + square_size.height, square_pos.y + square_size.width, 0.0f);
	glEnd();
	GL_LOG_ERRORS();


	/* -------------------- */

	return false;
}

void runner_deinit(Runner_State* state)
{
	(void)state;
}
