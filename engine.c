#include <photon.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

int main(void)
{
	Bool requested_stop = false;
	Window_State window_state = {0};
	float aspect_ratio;

	Runner_State* runner_state = NULL;
	Runner_Actions runner_actions = {0};

	if(display_init(&window_state, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		fprintf(stderr, "ERROR:%s:%d: Could not initialise display\n", __FILE__, __LINE__);
		display_free();
		return failure;
	}

	aspect_ratio = (float)window_state.viewport.width / (float)window_state.viewport.height;
	printf("Viewport %dx%d (%.2f)\n", window_state.viewport.width, window_state.viewport.height, aspect_ratio);

	glViewport(0, 0, window_state.viewport.width, window_state.viewport.height);
	GL_LOG_ERRORS();


	if(runner_load(&runner_actions))
	{
		display_free();
		return failure;
	}

	runner_state = runner_actions.runner_init(&window_state);
	if(runner_state == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
		display_free();
		return failure;
	}

	printf("Initiated runner with state saved in 0x%p\n", (void*)runner_state);


	while(display_is_alive(&window_state) && !requested_stop)
	{
#if 0
		glClearColor(0.094f, 0.094f, 0.094f, 1.0);
		GL_LOG_ERRORS();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL_LOG_ERRORS();

		/* -------------------- */
#endif
		if(window_state.runner_should_reload)
		{
			runner_actions.runner_deinit(runner_state);
			if(runner_unload(&runner_actions))
			{
				display_free();
				return failure;
			}

			/* -------------------- */

			if(runner_load(&runner_actions))
			{
				display_free();
				return failure;
			}

			runner_state = runner_actions.runner_init(&window_state);
			if(runner_state == NULL)
			{
				fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
				display_free();
				return failure;
			}

			printf("Initiated runner with state saved in 0x%p\n", (void*)runner_state);
			window_state.runner_should_reload = false;
		}

		requested_stop = runner_actions.runner_loop(runner_state);

		/* -------------------- */

		display_refresh(&window_state);
		inputs_poll(&window_state);
	}

	runner_actions.runner_deinit(runner_state);
	if(runner_unload(&runner_actions))
	{
		display_free();
		return failure;
	}

	display_free();
	return success;
}
