#include <photon.h>


#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

int main(void)
{
	Bool requested_stop = false;
	Runner_State* runner_state = NULL;

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

	runner_state = runner_init(viewport, aspect_ratio);
	if(!runner_state)
	{
		fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
		display_free();
		return failure;
	}
	printf("Initiated runner with state saved in 0x%p\n", (void*)runner_state);


	while(display_is_alive() && !requested_stop)
	{
#if 0
		glClearColor(0.094f, 0.094f, 0.094f, 1.0);
		GL_LOG_ERRORS();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL_LOG_ERRORS();

		/* -------------------- */
#endif

		requested_stop = runner_loop(runner_state);

#if 0
		/* -------------------- */

		display_refresh();

		inputs_poll();
		inputs_get_cursor(&cursor_pos);
#endif
	}

	display_free();
	return success;
}
