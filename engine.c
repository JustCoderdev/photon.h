#include "lib/include/photon.h"

#include <stdio.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

static char* shift(int* argc, char*** argv)
{
	char *ret;
	if(*argc == 0) return NULL;

	ret = **argv;

	(*argc)--;
	(*argv)++;

	return ret;
}

int main(int argc, char** argv)
{
	Bool requested_stop = false;
	Window_State window_state = {0};
	float aspect_ratio;

	Runner_State* runner_state = NULL;
	Runner_Actions runner_actions = {0};
	char* so_filename = NULL;

	if(display_init(&window_state, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		fprintf(stderr, "ERROR:%s:%d: Could not initialise display\n", __FILE__, __LINE__);
		goto fail_free_display;
	}

	aspect_ratio = (float)window_state.viewport.width / (float)window_state.viewport.height;
	printf("Viewport %dx%d (%.2f)\n", window_state.viewport.width, window_state.viewport.height, aspect_ratio);

	glViewport(0, 0, window_state.viewport.width, window_state.viewport.height);
	GL_LOG_ERRORS();

	printf("Started as: %s\n", shift(&argc, &argv));

	so_filename = shift(&argc, &argv);
	if(so_filename == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: No shared object filename provided!\n", __FILE__, __LINE__);
		goto fail_free_display;
	}

	if(runner_load(so_filename, &runner_actions)) goto fail_free_display;
	runner_state = runner_actions.runner_init(&window_state);
	if(runner_state == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
		goto fail_free_display;
	}

	printf("Initiated runner with state saved in 0x%p\n", (void*)runner_state);

	while(display_is_alive(&window_state) && !requested_stop)
	{
		if(window_state.runner_should_reload)
		{
			runner_actions.runner_deinit(runner_state);
			if(runner_unload(&runner_actions)) goto fail_free_display;
			printf("Uninitiated runner\n");

			/* -------------------- */

			if(runner_load(so_filename, &runner_actions)) goto fail_free_display;
			runner_state = runner_actions.runner_init(&window_state);
			if(runner_state == NULL)
			{
				fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
				goto fail_free_display;
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
	if(runner_unload(&runner_actions)) goto fail_free_display;


	display_free();
	return success;


fail_free_display:
	display_free();
	return failure;
}
