#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

/* OpenGL 1.1 spec <https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf> */
#include <GL/gl.h>
#include <GL/glx.h>

/* XLib docs <https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Introduction_to_Xlib> */
#include <X11/Xlib.h>

unsigned long ec = 0;

#define PRINT_GL_ERRORS() do { \
			GLenum e; \
			do { \
				e = glGetError(); \
				switch(e) \
				{ \
					case GL_INVALID_ENUM: \
						printf("[%lu] GLERROR:%s:%d: INVALID_ENUM\n", ec++, __FILE__, __LINE__); \
						break; \
					case GL_INVALID_VALUE: \
						printf("[%lu] GLERROR:%s:%d: INVALID_VALUE\n", ec++, __FILE__, __LINE__); \
						break; \
					case GL_INVALID_OPERATION: \
						printf("[%lu] GLERROR:%s:%d: INVALID_OPERATION\n", ec++, __FILE__, __LINE__); \
						break; \
					case GL_STACK_OVERFLOW: \
						printf("[%lu] GLERROR:%s:%d: STACK_OVERFLOW\n", ec++, __FILE__, __LINE__); \
						break; \
					case GL_STACK_UNDERFLOW: \
						printf("[%lu] GLERROR:%s:%d: STACK_UNDERFLOW\n", ec++, __FILE__, __LINE__); \
						break; \
					case GL_OUT_OF_MEMORY: \
						printf("[%lu] GLERROR:%s:%d: OUT_OF_MEMORY\n", ec++, __FILE__, __LINE__); \
						exit(1); \
				} \
			} \
			while(e != GL_NO_ERROR); \
		} while(0)

/* Creating a window X11 <https://hereket.com/posts/linux_creating_x11_windows> */
int main(void) {
	int is_window_alive = 1;

	struct timespec now, old;
	time_t dt;

	struct {
		GLfloat x, y;
	} cursor_pos = {0, 0};

	struct {
		int width, height;
	} viewport_size = {1, 1};

	XVisualInfo* visual_info;
	GLint visual_attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	GLXContext gl_ctx;

	XSetWindowAttributes set_window_attr;
	Window root_w, window;

	Display* display = XOpenDisplay(NULL);
	if(display == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: Couldn't connect to X server\n", __FILE__, __LINE__);
		exit(1);
	}

	root_w = DefaultRootWindow(display);

	visual_info = glXChooseVisual(display, 0, visual_attr);
	if(visual_info == NULL)
	{
		fprintf(stderr, "ERROR:%s:%d: Appropriate visuals not found\n", __FILE__, __LINE__);
		exit(1);
	}

	set_window_attr.colormap = XCreateColormap(display, root_w, visual_info->visual, AllocNone);
	set_window_attr.event_mask = ExposureMask | KeyPressMask | PointerMotionMask;
	set_window_attr.background_pixel = 0xFF181818;

	window = XCreateWindow(display, root_w, 0, 0, 600, 400, 0,
			visual_info->depth, InputOutput, visual_info->visual,
			CWColormap | CWEventMask, &set_window_attr);

	XMapWindow(display, window);
	XStoreName(display, window, "awawa");

	gl_ctx = glXCreateContext(display, visual_info, NULL, GL_TRUE);
	glXMakeCurrent(display, window, gl_ctx);

	glEnable(GL_DEPTH_TEST);
	PRINT_GL_ERRORS();

	assert(clock_gettime(CLOCK_MONOTONIC, &old) == 0);
	while(is_window_alive) {
		XEvent event;
		XNextEvent(display, &event);

		assert(clock_gettime(CLOCK_MONOTONIC, &now) == 0);
		dt = now.tv_nsec - old.tv_nsec;
		old = now;

#define DTtoHZ(DT) (1/((double)(DT)/1e9))
		printf("DeltaTime %lu - HZ %f\n", dt, DTtoHZ(dt));

		switch(event.type) {
			case Expose:
			{
				XWindowAttributes window_attr;

				/* XExposeEvent* expose_event = (XExposeEvent *)&event; */
				/* if(expose_event->count > 0) break; */

				XGetWindowAttributes(display, window, &window_attr);
				glViewport(0, 0, window_attr.width, window_attr.height);
				PRINT_GL_ERRORS();

				viewport_size.width = window_attr.width;
				viewport_size.height = window_attr.height;
				break;
			}

			case MotionNotify:
			{
				XMotionEvent* motion_event = (XMotionEvent *)&event;

				cursor_pos.x = ( ( (GLfloat)motion_event->x / (GLfloat)viewport_size.width  ) - 0.5f ) *  2;
				cursor_pos.y = ( ( (GLfloat)motion_event->y / (GLfloat)viewport_size.height ) - 0.5f ) * -2;
				/* printf("MOUSE: RAW %dx%d - PROCESSED %.2fx%.2f\n", motion_event->x, motion_event->y, cursor_pos.x, cursor_pos.y); */
				break;
			}

			case KeyPress:
			{
				XKeyPressedEvent* key_event = (XKeyPressedEvent *)&event;
				if(key_event->keycode == XKeysymToKeycode(display, XK_Escape))
				{
					is_window_alive = 0;
				}
				break;
			}
		}


		glClearColor(0.094f, 0.094f, 0.094f, 1.0);
		PRINT_GL_ERRORS();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		PRINT_GL_ERRORS();

		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(  0.0f,  0.5f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( -0.5f, -0.5f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( +0.5f, -0.5f, 0.0f);
		glEnd();

		glPointSize(10.0f);
		PRINT_GL_ERRORS();

		glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -0.5f, 0.0f);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  0.0f, 0.0f);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 1.0f,  0.5f, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(cursor_pos.x, cursor_pos.y, 0.0f);
		glEnd();
		PRINT_GL_ERRORS();

		printf("MOUSE %.2fx%.2f\n", cursor_pos.x, cursor_pos.y);

		glXSwapBuffers(display, window);
		PRINT_GL_ERRORS();

		XFlush(display);
	}

	glXMakeCurrent(display, None, NULL);
	glXDestroyContext(display, gl_ctx);

	XDestroyWindow(display, window);
	XCloseDisplay(display);

	return 0;
}
