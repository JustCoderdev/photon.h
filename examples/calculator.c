#include "../lib/include/photon-runner.h"
#include "GL/gl.h"

#define CAT_TEXTURE_FILENAME "cat.bmp"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <string.h>
/* void *memset(void s[.n], int c, size_t n); */

void component_button(Runner_State* state, Corner corner_tl, Size size, ColorRGB color);

Runner_State* runner_init(Window_State* window_state)
{
	Runner_State* state = malloc(sizeof(*state));
	(void)memset(state, 0, sizeof(*state));

	state->window_state = window_state;
	state->cat_image = image_bmp_load(CAT_TEXTURE_FILENAME);
	assert(state->cat_image.texel != NULL);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &state->cat_texture_id);
	glBindTexture(GL_TEXTURE_2D, state->cat_texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			state->cat_image.size.width,
			state->cat_image.size.height,
			0, GL_BGR, GL_UNSIGNED_BYTE,
			state->cat_image.texel);

	return state;
}

Bool runner_loop(Runner_State* state)
{
	/* Size viewport = state->window_state->viewport; */
	float aspect_ratio = (float)state->window_state->viewport.width / (float)state->window_state->viewport.height;

	Point cursor_pos;
	inputs_get_cursor(state->window_state, &state->cursor_pos);
	cursor_pos = state->cursor_pos;
	(void)cursor_pos, (void)aspect_ratio;

	/* -------------------- */

	glClearColor(0.094f, 0.094f, 0.094f, 1.0);
	GL_LOG_ERRORS();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_LOG_ERRORS();

	/* -------------------- */

	/* CAT */
	/* ---------------------------------------- */
	{
		Point tl_pnt, br_pnt;
		/* ColorRGB color = { 0.0f, 1.0f, 0.0f }; */

		Size size = { 200, 300 };

		Corner corner_tl = { 10, 10 };
		Corner offset = corner_tl;

		offset.x += (unsigned int)size.width;
		offset.y += (unsigned int)size.height;

		tl_pnt = corner_to_point(state->window_state, corner_tl);
		br_pnt = corner_to_point(state->window_state, offset);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(tl_pnt.x, br_pnt.y, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(tl_pnt.x, tl_pnt.y, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(br_pnt.x, tl_pnt.y, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(br_pnt.x, br_pnt.y, 0.0f);
		glEnd();
		GL_LOG_ERRORS();
	}

#if 0
	/* "BUTTONS" */
	/* ---------------------------------------- */
	{
		ColorRGB red = { 1.0f, 0.0f, 0.0f };
		ColorRGB grn = { 0.0f, 1.0f, 0.0f };
		ColorRGB ble = { 0.0f, 0.0f, 1.0f };
		ColorRGB gry = { 0.2f, 0.2f, 0.2f };
		Size size = { 200, 100 }, scrn = { 0, 0 };
		Corner Z = { 10, 10 }, A = { 30, 30 }, B = { 70, 70 }, C = { 110, 110 };

		scrn.width = state->window_state->viewport.width - 20;
		scrn.height = state->window_state->viewport.height - 20;

		component_button(state, Z, scrn, gry);

		component_button(state, A, size, red);
		component_button(state, B, size, grn);
		component_button(state, C, size, ble);
	}
#endif

#if 0
	/* CURSOR LINES */
	/* ---------------------------------------- */

	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0, cursor_pos.y, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0, cursor_pos.y, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0, cursor_pos.y, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0, cursor_pos.y, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(cursor_pos.x, -1.0, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(cursor_pos.x, 0.0, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(cursor_pos.x, 0.0, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(cursor_pos.x, 1.0, 0.0f);
	glEnd();
	GL_LOG_ERRORS();


	glPointSize(10.0f);
	GL_LOG_ERRORS();

	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0, -1.0, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0, 1.0, 0.0f);
	glEnd();
	GL_LOG_ERRORS();
#endif

	/* -------------------- */

	return false;
}

void runner_deinit(Runner_State* state)
{
	glDeleteTextures(1, &state->cat_texture_id);
	image_unload(&state->cat_image);
}


/* Demos */
/* ------------------------------------------------------------ */

void component_button(
		Runner_State* state,
		Corner corner_tl,
		Size size,
		ColorRGB color)
{
	Point tl_pnt, br_pnt;

	Corner offset = corner_tl;
	offset.x += (unsigned int)size.width;
	offset.y += (unsigned int)size.height;

	tl_pnt = corner_to_point(state->window_state, corner_tl);
	br_pnt = corner_to_point(state->window_state, offset);

	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(color.r, color.g, color.b);
		glVertex3f(tl_pnt.x, tl_pnt.y, 0.0f); glVertex3f(tl_pnt.x, br_pnt.y, 0.0f);
		glVertex3f(br_pnt.x, tl_pnt.y, 0.0f); glVertex3f(br_pnt.x, br_pnt.y, 0.0f);
	glEnd();
	GL_LOG_ERRORS();
}

#if 0
void demo_point_over_cursor(Point cursor_pos)
{
	glPointSize(10.0f);
	GL_LOG_ERRORS();

	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(cursor_pos.x, cursor_pos.y, 0.0f);
	glEnd();
	GL_LOG_ERRORS();
}

void demo_rainbow_triangle(void)
{
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  0.5f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(+0.5f, -0.5f, 0.0f);
	glEnd();
	GL_LOG_ERRORS();
}

void demo_inverse_circle_fill(n8 steps, float size, float aspect_ratio)
{
	n8 i;
	float bx = -0.5f;
	float by = -0.5f;

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.194f, 0.094f, 0.094f);
		glVertex2f(0.5f,  0.5f * aspect_ratio);

		for(i = 0; i <= steps; ++i)
		{
			double deg = ((float)i / (float)steps) * PI / 2;
			glVertex2f(
				 bx + (float)cos(deg) * size,
				(by + (float)sin(deg) * size) * aspect_ratio
			);
		}

	glEnd();
	GL_LOG_ERRORS();

	glPointSize(10.0f);
	GL_LOG_ERRORS();

	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.5f,  0.5f * aspect_ratio);
		glColor3f(0.0f, 1.0f, 0.0f);
		for(i = 0; i <= steps; ++i)
		{
			double deg = ((float)i / (float)steps) * PI / 2;
			glVertex2f(
				 bx + (float)cos(deg) * size,
				(by + (float)sin(deg) * size) * aspect_ratio
			);
		}
	glEnd();
	GL_LOG_ERRORS();
}

/* "Utils" */
/* ------------------------------------------------------------ */

void draw_gay_circle(float x, float y, n8 steps, float aspect_ratio, float size)
{
	n8 i;

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
	GL_LOG_ERRORS();
}

#endif

