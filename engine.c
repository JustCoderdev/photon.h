#include "lib/include/photon.h"

#include <stdio.h>

#include <assert.h>
#include <time.h>
/*
char *ctime(const time_t *restrict timep);
time_t time(time_t *_Nullable tloc);
*/

static Runner_State* runner_load_and_init(char* filename, Window_State* window_state_ptr, Runner_Actions* runner_actions_ptr)
{
	Runner_State* runner_state_ptr = NULL;

	if(runner_load(filename, runner_actions_ptr)) return NULL;

	assert(runner_actions_ptr->runner_init != NULL);
	runner_state_ptr = runner_actions_ptr->runner_init(window_state_ptr);
	if(runner_state_ptr == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
		return NULL;
	}

	printf("RUNNER:0x%p: Loaded and initialised runner\n", (void*)runner_actions_ptr->handle);
	return runner_state_ptr;
}

static Error runner_deinit_and_unload(Runner_State* runner_state_ptr, Runner_Actions* runner_actions_ptr)
{
	void* dlhandle_ptr = (void*)runner_actions_ptr->handle;

	if(runner_state_ptr == NULL) return success;

	if(runner_actions_ptr->runner_deinit != NULL)
	{
		runner_actions_ptr->runner_deinit(runner_state_ptr);
	}

	if(runner_unload(runner_actions_ptr)) return failure;

	printf("RUNNER:0x%p: Uninitialised and unloaded runner\n", dlhandle_ptr);
	return success;
}


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
	n16 runner_initiated_counter = 0;
	Bool requested_stop = false;
	Window_State window_state = {0};
	float aspect_ratio;

	Runner_State* runner_state_ptr = NULL;
	Runner_Actions runner_actions = {0};
	char* so_filename = NULL;

	if(display_init(&window_state, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		fprintf(stderr, "ERROR:%s:%d: Could not initialise display\n", __FILE__, __LINE__);
		goto failure_cleanup_display;
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
		goto failure_cleanup_display;
	}

	if(runner_load(so_filename, &runner_actions)) goto failure_cleanup_display;
	runner_state_ptr = runner_actions.runner_init(&window_state);
	if(runner_state_ptr == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: (RUNNER) Could not init runner\n", __FILE__, __LINE__);
		goto failure_cleanup_display;
	}

	printf("Initiated runner with state saved in 0x%p\n", (void*)runner_state_ptr);

	while(display_is_alive(&window_state) && !requested_stop)
	{
		if(window_state.runner_should_reload)
		{
			if(runner_deinit_and_unload(runner_state_ptr, &runner_actions))
				goto failure_cleanup_display;

			runner_state_ptr = runner_load_and_init(so_filename, &window_state, &runner_actions);
			if(runner_state_ptr == NULL) goto failure_cleanup_runner;

			window_state.runner_should_reload = false;
		}

		/* -------------------- */

		requested_stop = runner_actions.runner_loop(runner_state_ptr);

		/* -------------------- */

		display_refresh(&window_state);
		inputs_poll(&window_state);
	}

	if(runner_deinit_and_unload(runner_state_ptr, &runner_actions))
	{
		goto failure_cleanup_display;
	}

	display_free();
	return success;

	/* -------------------- */

failure_cleanup_runner:
	(void)runner_deinit_and_unload(runner_state_ptr, &runner_actions);

failure_cleanup_display:
	display_free();

	return failure;
}
