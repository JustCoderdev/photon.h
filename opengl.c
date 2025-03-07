#include <stdio.h>
#include <stdlib.h>

/* OpenGL 1.1 spec <https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf> */
#include <GL/gl.h>

/* XLib docs <https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Introduction_to_Xlib> */
#include <X11/Xlib.h>
#include <X11/Xutil.h>


/* Creating a window X11 <https://hereket.com/posts/linux_creating_x11_windows> */
static
int main2(void)
{
	int is_window_alive = 1;

	/* int screen_n; */
	XSetWindowAttributes window_attr = {0};
	Window root_w, window;

	Display* display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "ERROR:%s:%d: Couldn't connect to X server\n", __FILE__, __LINE__);
		exit(1);
	}

	root_w = XDefaultRootWindow(display);
	/* screen_n = DefaultScreen(display); */
	/* GC gl_ctx = XDefaultGC(display, screen_n); */


	window_attr.background_pixel = 0xFF181818;
	/* window = XCreateSimpleWindow(display, root_w, 0, 0, 600, 400, 0, 0, 0xFF181818); */
	window = XCreateWindow(display, root_w, 0, 0, 600, 400, 0,
			CopyFromParent, CopyFromParent, CopyFromParent, CWBackPixel, &window_attr);

	XMapWindow(display, window);

	while(is_window_alive) {
		XEvent event = {0};
		XNextEvent(display, &event);

		switch(event.type) {
			case KeyPress:
			case KeyRelease:
			{
				XKeyPressedEvent* key_event = (XKeyPressedEvent *)&event;

				printf("Keyreleased\n");

				if(key_event->keycode == XKeysymToKeycode(display, XK_Escape))
				{
					is_window_alive = 0;
				}
				break;
			}
		}
	}

	XDestroyWindow(display, window);
	XCloseDisplay(display);

	return 0;
}

/* // -- Written in C -- // */

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include <GL/gl.h>
#include <GL/glx.h>
/* #include <GL/glu.h> */

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

static void DrawAQuad(void) {
	glClearColor(0.094f, 0.094f, 0.094f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(  0.0f,  0.5f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( -0.5f, -0.5f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( +0.5f, -0.5f, 0.0f);
	glEnd();
}

int main(void) {
	int is_window_alive = 1;
	(void)main2;

	dpy = XOpenDisplay(NULL);

	if(dpy == NULL)
	{
		printf("Cannot connect to X server\n");
		exit(0);
	}

	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);

	if(vi == NULL)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}


	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;

	win = XCreateWindow(dpy, root, 0, 0, 600, 400, 0,
			vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, "awawa");

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	glEnable(GL_DEPTH_TEST);

	while(is_window_alive) {
		XNextEvent(dpy, &xev);

		switch(xev.type) {
			case Expose:
			{
				XGetWindowAttributes(dpy, win, &gwa);
				glViewport(0, 0, gwa.width, gwa.height);
				DrawAQuad();
				glXSwapBuffers(dpy, win);
				break;
			}

			case KeyPress:
			{
				XKeyPressedEvent* key_event = (XKeyPressedEvent *)&xev;
				if(key_event->keycode == XKeysymToKeycode(dpy, XK_Escape))
				{
					is_window_alive = 0;
				}
				break;
			}
		}
	}

	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 0;
}wd
