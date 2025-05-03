#ifndef PHOTON_H
#define PHOTON_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* OpenGL 1.1 spec <https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf> */
#include <GL/gl.h>
#include <GLFW/glfw3.h>


/* Types */
/* ------------------------------------------------------------ */

typedef unsigned char     n8;
typedef unsigned short    n16;
typedef unsigned int      n32;
typedef unsigned long int n64;

typedef signed char     i8;
typedef signed short    i16;
typedef signed int      i32;
typedef signed long int i64;

typedef enum {
	true  = (1 == 1),
	false = (1 != 1)
} bool;

typedef enum {
	success = false,
	failure = true
} error;

typedef int Errno;

typedef float Ratio;
typedef float Radian;


typedef struct {
	float x, y;
} Point;

typedef struct {
	int width, height;
} Size;

typedef struct {
	float r, g, b;
} ColorRGB;


/* Macros */
/* ------------------------------------------------------------ */

#define PI 3.1415926535f

/* -1..1 -> 0..1 */
#define RAD_TO_RAT(RAD) ((float)(((RAD) + 1) / 2))
/* 0..1 -> -1..1 */
#define RAT_TO_RAD(RAT) ((float)(((RAT) - 0.5f ) * 2))

#define CLAMP(MIN, VAL, MAX) ((VAL) > (MAX) ? (MAX) : ((VAL) < (MIN) ? (MIN) : (VAL)))
#define CLAMP_RAD(RAD) CLAMP(-1, (RAD), 1)

#define GL_LOG_ERRORS() glLogErrors(__FILE__, __LINE__)


/* Geometry */
/* ------------------------------------------------------------ */

extern void draw_circle_flat(Point pos, float size, Ratio aspect_ratio, n8 steps, ColorRGB color);


/* Display */
/* ------------------------------------------------------------ */

extern GLFWwindow *g_window;
extern Size g_viewport;
extern bool g_display_is_alive;

extern error display_init(Size* viewport);
extern void  display_refresh(void);
extern void  display_free(void);

extern bool  display_is_alive(void);


/* Input */
/* ------------------------------------------------------------ */

extern void inputs_poll(void);
extern void inputs_get_cursor(Point* cursor_pos);




/* Utils */
/* ------------------------------------------------------------ */

extern error glLogErrors(char* file, int line);


#endif /* PHOTON_H */
