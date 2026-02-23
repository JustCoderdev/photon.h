#ifndef PHOTON_COMMON_H
#define PHOTON_COMMON_H

/* OpenGL 1.1 spec <https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf> */
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#define DEBUG_OPENGL 1
#define DEBUG_OPENGL_COMMAND (1 && DEBUG_OPENGL)

#define DEBUG_IO 1
#define DEBUG_IO_CURSOR   (0 && DEBUG_IO)
#define DEBUG_IO_KEYBOARD (1 && DEBUG_IO)


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
} Bool;

typedef enum {
	success = false,
	failure = true
} Error;

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
	float width, height;
} SizeF;

typedef struct {
	float r, g, b;
} ColorRGB;


typedef struct {
	GLFWwindow *window;
	Bool display_is_alive;
	Bool runner_should_reload;
	Size viewport;
} Window_State;

typedef struct {
	unsigned int key_a : 1;
	unsigned int key_b : 1;
	unsigned int key_c : 1;
	unsigned int key_d : 1;
	unsigned int key_e : 1;
	unsigned int key_f : 1;
	unsigned int key_g : 1;
	unsigned int key_h : 1;
	unsigned int key_i : 1;
	unsigned int key_j : 1;
	unsigned int key_k : 1;
	unsigned int key_l : 1;
	unsigned int key_m : 1;
	unsigned int key_n : 1;
	unsigned int key_o : 1;
	unsigned int key_p : 1;
	unsigned int key_q : 1;
	unsigned int key_r : 1;
	unsigned int key_s : 1;
	unsigned int key_t : 1;
	unsigned int key_u : 1;
	unsigned int key_v : 1;
	unsigned int key_w : 1;
	unsigned int key_x : 1;
	unsigned int key_y : 1;
	unsigned int key_z : 1;

	unsigned int smb_sqt : 1; /* ' */
	unsigned int smb_cma : 1; /* , */
	unsigned int smb_min : 1; /* - */
	unsigned int smb_dot : 1; /* . */
	unsigned int smb_fsl : 1; /* / */

	unsigned int num_0 : 1;
	unsigned int num_1 : 1;
	unsigned int num_2 : 1;
	unsigned int num_3 : 1;
	unsigned int num_4 : 1;
	unsigned int num_5 : 1;
	unsigned int num_6 : 1;
	unsigned int num_7 : 1;
	unsigned int num_8 : 1;
	unsigned int num_9 : 1;

	unsigned int smb_scl : 1; /* ; */
	unsigned int smb_equ : 1; /* = */
	unsigned int smb_lbk : 1; /* [ */
	unsigned int smb_bsl : 1; /* \ */
	unsigned int smb_rbk : 1; /* ] */
	unsigned int smb_btk : 1; /* ` */
} PhysicalPressedKeyField;

/* Macros */
/* ------------------------------------------------------------ */

#define PI 3.1415926535f

/* -1..1 -> 0..1 */
#define RAD_TO_RAT(RAD) ((float)(((RAD) + 1) / 2))
/* 0..1 -> -1..1 */
#define RAT_TO_RAD(RAT) ((float)(((RAT) - 0.5f ) * 2))

#define CLAMP(MIN, VAL, MAX) ((VAL) > (MAX) ? (MAX) : ((VAL) < (MIN) ? (MIN) : (VAL)))
#define CLAMP_RAD(RAD) CLAMP(-1, (RAD), 1)

#if DEBUG_OPENGL_COMMAND
#define GL_LOG_ERRORS() glLogErrors(__FILE__, __LINE__)
#else
#define GL_LOG_ERRORS()
#endif


#endif /* PHOTON_COMMON_H */

