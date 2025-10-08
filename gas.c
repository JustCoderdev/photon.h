#include <photon-runner.h>


#include <string.h>
/* void *memset(void s[.n], int c, size_t n); */

static void draw_gay_circle(float x, float y, n8 steps, float aspect_ratio, float size)
{
	n8 i;

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);

		for(i = 0; i < steps; ++i)
		{
			double deg = ((float)i / (float)steps) * 2 * PI;
			glColor3f(RAD_TO_RAT(sin(deg)), 0.0f, RAD_TO_RAT(cos(deg)));
			glVertex3f(x + (float)cos(deg) * size, y + (float)sin(deg) * size * aspect_ratio, 0.0f);
		}
	glEnd();
	GL_LOG_ERRORS();
}

Runner_State* runner_init(Size viewport, float aspect_ratio)
{
	Runner_State* state = malloc(sizeof(*state));
	(void)memset(state, 0, sizeof(*state));

	state->viewport = viewport;
	state->aspect_ratio = aspect_ratio;

	/* ---- REMOVE ME ----- */
	/* float x, float y, int steps, float aspect_ratio, float size */
	(void)draw_gay_circle(0, 0, 0, 0, 0);
	/* ---- REMOVE ME ----- */

	return state;
}

Bool runner_loop(Runner_State* state)
{
	Size viewport = state->viewport;
	float aspect_ratio = state->aspect_ratio;
	Point cursor_pos = state->cursor_pos;

	/* -------------------- */

	glClearColor(0.094f, 0.094f, 0.094f, 1.0);
	GL_LOG_ERRORS();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_LOG_ERRORS();

	/* -------------------- */

#define P0  -0.5f, -0.5f * aspect_ratio
#define P1F -0.5f,  0.5f * aspect_ratio
#define P1M  0.5f, -0.5f * aspect_ratio
#define P2   0.5f,  0.5f * aspect_ratio

#if 0
#define P0_C  0.37f, 0.37f, 0.37f
#define P1F_C 0.75f, 0.47f, 0.63f
#define P1M_C 0.42f, 0.50f, 0.82f
#define P2_C  1.00f, 0.96f, 0.59f

	/* Square */
	glBegin(GL_TRIANGLES);
		glColor3f(P0_C);  glVertex2f(P0);
		glColor3f(P1F_C); glVertex2f(P1F);
		glColor3f(P2_C);  glVertex2f(P2);

		glColor3f(P0_C);  glVertex2f(P0);
		glColor3f(P1M_C); glVertex2f(P1M);
		glColor3f(P2_C);  glVertex2f(P2);
	glEnd();
	GL_LOG_ERRORS();
#endif

#if 1
	/* Inverse circle fill */
	{
		n8 i;
		n8 steps = 10;
		float size = 1.0f;

		float bx = -0.5f;
		float by = -0.5f;

		glBegin(GL_TRIANGLE_FAN);
			glColor3f(0.194f, 0.094f, 0.094f);
			glVertex2f(P2);

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
			glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(P2);
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
#endif


#if 0
	/* Rainbow Triangle */
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  0.5f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(+0.5f, -0.5f, 0.0f);
	glEnd();
	GL_LOG_ERRORS();
#endif

#if 1
	/* Circle over Cursor */
	glPointSize(10.0f);
	GL_LOG_ERRORS();

	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(cursor_pos.x, cursor_pos.y, 0.0f);
	glEnd();
	GL_LOG_ERRORS();
#endif

#if 0
	/* Gay ball following circle */
	{
		float size = 0.2f;
		float x = CLAMP(-1 + size, cursor_pos.x, 1 - size),
			  y = CLAMP(-1 + size * aspect_ratio, cursor_pos.y, 1 - size * aspect_ratio);

		draw_gay_circle(x, y, 16, aspect_ratio, size);
	}
#endif

	/* -------------------- */

	display_refresh();

	inputs_poll();
	inputs_get_cursor(&state->cursor_pos);
	return false;
}

void runner_deinit(Runner_State* state)
{
	(void)state;
}
