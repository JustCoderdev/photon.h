#ifndef PHOTON_RUNNER_H
#define PHOTON_RUNNER_H

#include <photon-common.h>

typedef struct {
	Size viewport;
	float aspect_ratio;
	Point cursor_pos;
} Runner_State;

#include <photon.h>


extern Runner_State* runner_init(Size viewport, float aspect_ratio);
extern Bool          runner_loop(Runner_State* state); /* float delta_time */
extern void          runner_deinit(Runner_State* state);

#endif /* PHOTON_RUNNER_H */
