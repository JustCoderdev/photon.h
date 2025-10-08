#ifndef PHOTON_COMMON_H
#define PHOTON_COMMON_H


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


#endif /* PHOTON_COMMON_H */

