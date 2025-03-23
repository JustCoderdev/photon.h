#include <photon.h>


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
	GL_LOG_ERRORS();
}


#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

int main(void)
{
	Point cursor_pos = {0};

	float aspect_ratio;
	Size viewport = { SCREEN_WIDTH, SCREEN_HEIGHT };
	if(display_init(&viewport))
	{
		fprintf(stderr, "ERROR:%s:%d: Could not initialise display\n", __FILE__, __LINE__);
		display_free();
		return failure;
	}

	aspect_ratio = (float)viewport.width / (float)viewport.height;
	printf("Viewport %dx%d (%.2f)\n", viewport.width, viewport.height, aspect_ratio);

	glViewport(0, 0, viewport.width, viewport.height);
	GL_LOG_ERRORS();

	while(display_is_alive())
	{
		glClearColor(0.094f, 0.094f, 0.094f, 1.0);
		GL_LOG_ERRORS();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL_LOG_ERRORS();

		/* -------------------- */

		/* glBegin(GL_TRIANGLES); */
		/* 	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(  0.0f,  0.5f, 0.0f); */
		/* 	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( -0.5f, -0.5f, 0.0f); */
		/* 	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( +0.5f, -0.5f, 0.0f); */
		/* glEnd(); */
		/* GL_LOG_ERRORS(); */

		glPointSize(10.0f);
		GL_LOG_ERRORS();

		glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(cursor_pos.x, cursor_pos.y, 0.0f);
		glEnd();
		GL_LOG_ERRORS();

		{
			float size = 0.2f;
			float x = cursor_pos.x,
				  y = cursor_pos.y;

			draw_gay_circle(x, y, 16, aspect_ratio, size);
		}

		/* -------------------- */
		display_refresh();

		inputs_poll();
		inputs_get_cursor(&cursor_pos);
	}

	display_free();
	return success;
}




#if 0 /* <3 */
float size = 0.2f;
float x = 0.0f,
	  y = 0.0f;
float xoff = 0.15f,
	  yoff = 0.5f;
float r = 1.00f,
	  g = 0.0f,
	  b = 0.0f;
/* glBegin(GL_TRIANGLES); */
/* 	glColor3f(r, g, b); glVertex3f(x - xoff - size, y + yoff - 0.1f, 0.0f); */
/* 	glColor3f(r, g, b); glVertex3f(x + xoff + size, y + yoff - 0.1f, 0.0f); */
/* 	glColor3f(r, g, b); glVertex3f(0.0f, -0.3f, 0.0f); */
/* glEnd(); */

/* draw_flat_circle(x - xoff, y + yoff, 16, aspect_ratio, size, r, g, b); */
/* draw_flat_circle(x + xoff, y + yoff, 16, aspect_ratio, size, r, g, b); */
#endif
