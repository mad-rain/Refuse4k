/*
   Copyright (c) 2001, 2002 Mad Rain. FeNoMeN.
*/

#define AFF_LEN 3000
#define TIMER_CONST 0.4f
#define FADE_DIV 2

#define STACK_SIZE 0x1000
#define MPI 3.141592654f


#ifdef X11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "font.c"
#endif /* X11 */
#ifdef VGL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <machine/console.h>
#include <vgl.h>
#include "font.c"
#endif /* VGL */

#ifndef DPMI
#include <time.h>
#define memset4(d,b,c) memset(d,b,c*4)
#endif /* !DPMI */

#include <math.h>

#ifdef DPMI
unsigned char *font;
#ifdef RELEASE
int stack[STACK_SIZE];
#endif
#endif /* DPMI */


int cur_time;

#ifndef DPMI

#include <stdint.h>
#include <time.h>

int m_get_time() {
	struct timespec TimeVal;
	clock_gettime(CLOCK_BOOTTIME, &TimeVal);

	uint64_t Time = TimeVal.tv_sec;
	Time *= 1000000000;
	Time += TimeVal.tv_nsec;

	return Time / (1000000000 / 128);
}

#else
#define m_get_time() current_time
#endif


#include "4k.h"
#include "text.c"
#include "texture.c"
#include "morph.h"
//#include "iso.h"
#include "radial.c"
#include "surf.c"
#include "timer.c"
#include "parts.c"


typedef struct {
         void (*update)();
         unsigned char r, g, b;
} t_effect;

t_effect effects[] = {
           {update2, 0x00, 0xff, 0xff},
           {update5, 0x00, 0xff, 0x00},
           {update4, 0x00, 0x00, 0xff}, 
           {update3, 0xff, 0xff, 0x00},
           {update1, 0xff, 0x00, 0xff},
           {0}
};

#ifdef X11

Window root_window, cur_window;
Display *display;
Visual *visual;
XColor rgb[0x100];
Colormap cmap;
unsigned long cells[0x100];
int screen;
XEvent event;
XImage *image;
GC gc;

unsigned short pal16[0x100];
unsigned int pal32[0x100];
void *XVideoBuffer;

#endif /* X11 */

void set_palette()
{
#ifdef X11

        {
                int c = 0x100;
                int *src = pal;
                unsigned int *dest = pal32;
                do {
                        unsigned char r = (*src ++ * pal_bright) >> 6;
                        unsigned char g = (*src ++ * pal_bright) >> 6;
                        unsigned char b = (*src ++ * pal_bright) >> 6;
                        *dest ++ = b + (g << 8) + (r << 16);
                } while (--c);
        }

#endif /* X11 */

#ifdef VGL
        unsigned char red[0x100];
        unsigned char green[0x100];
        unsigned char blue[0x100];

        unsigned char *r = red;
        unsigned char *g = green;
        unsigned char *b = blue;
        
        int *s = pal;
        
        int c = 0x100;
        do {
                *r ++ = (*s ++ * pal_bright) >> 8;
                *g ++ = (*s ++ * pal_bright) >> 8;
                *b ++ = (*s ++ * pal_bright) >> 8;       
        } while (--c);

        VGLSetPalette(red, green, blue);
#endif /* VGL */
}

#if !defined(RELEASE) && defined(DPMI)

char dpmi_regs[50];

void init_font();
#pragma aux init_font = " mov bx,10h  "\
                        " mov edi,offset dpmi_regs "\
                        " push edi "\
                        " mov ecx,25 "\
                        " xor eax,eax "\
                        " rep stosw "\
                        " pop edi "\
                        " mov ax,300h "\
                        " mov byte ptr [edi+11h],6 "\
                        " mov word ptr [edi+1ch],1130h "\
                        " int 31h "\
                        " movzx eax,word ptr [edi+22h] "\
                        " shl eax,4 "\
                        " movzx ecx,word ptr [edi+8] "\
                        " add eax,ecx "\
                        " mov dword ptr [font],eax " modify[eax ebx ecx edi];

#endif


#ifdef DPMI
void main()
#else
int main()
#endif
{
        t_effect *effect = effects;

#if !defined(RELEASE) && defined(DPMI)
        VideoMem = (unsigned char *)0xA0000L;
        init_font();
#endif

#ifdef X11
        XSizeHints *size;
        XTextProperty name; 
        char *screen_name = "REFUSE 4k :)";

        if ((display = XOpenDisplay(NULL)) == NULL) {
                printf("can't open display\n");
                return 0;
        }
                

        screen = DefaultScreen(display);
        visual = DefaultVisual(display, screen);
        root_window = RootWindow(display, screen);
        gc = DefaultGC(display, screen);

        int fixed_bpp = 32;
    
        cur_window = XCreateSimpleWindow(display, root_window, 0, 0,
                                         320, 200, 
                                         CopyFromParent, CopyFromParent, CopyFromParent);

        XStringListToTextProperty((char **)&screen_name, 1, &name);

        size = XAllocSizeHints();                    
        size->flags = PMinSize | PMaxSize;
        size->min_width = size->max_width = 320;
        size->min_height = size->max_height = 200;

        XSetWMProperties(display, cur_window, &name, &name,
                         NULL, 0, size, NULL, NULL);

        XVideoBuffer = (void *) malloc(320 * 200 * fixed_bpp / 8);
        
        set_palette();

        image = XCreateImage(display, visual, DefaultDepth(display, screen), ZPixmap,
                             0,(char *)XVideoBuffer, 320,
                             200, fixed_bpp,
                             320 * fixed_bpp / 8);

        if (image == NULL) {
                printf("can't create image\n");
                return 0;
        }
                             
        XMapWindow(display, cur_window);
        // XSelectInput(display, cur_window, KeyPressMask);
        
#endif /* X11 */

#ifdef VGL
        VGLInit(SW_VGA_CG320);
        VGLKeyboardInit(VGL_XLATEKEYS);
#endif /* VGL */

#ifdef DPMI
        set_video_mode(0x13);
        setup_timer_proc();

        set_timer_speed(1193100 / 128);
//      set_timer_speed(1931);
#endif


#include "texture.ini"
#include "morph.ini"

        cur_time = 0;
         {
  int b_time = m_get_time();
        do {
                {
                        int r = effect->r;
                        int g = effect->g;
                        int b = effect->b;
#include "pal.inc"
                }

                do {
                     
                        unsigned int pal_bright2 = (AFF_LEN-cur_time) / FADE_DIV;
                        pal_bright  = cur_time / FADE_DIV;
                        if (pal_bright2 < pal_bright) pal_bright = pal_bright2;
                        if (pal_bright > 128) pal_bright = 0x80;
     
#ifdef DPMI
                        if (m_kbhit()) { m_getch(); goto stop; }
#endif /* DPMI */

#ifdef X11
/*
        if (XCheckWindowEvent(display, cur_window, KeyPressMask, &event) == 1)
                goto stop;
*/
#endif

#ifdef VGL
                        if (VGLKeyboardGetCh()) goto stop;
#endif /* VGL */
                        effect->update();
#ifndef DPMI    
                        set_palette();
#endif /* DPMI */

#ifdef X11
                        memset(video_buffer, 0, 320);
                        
                        {
                                unsigned char *src = video_buffer;
                                unsigned int *dest = XVideoBuffer;
                                int size = 320 * 200;
                                do *dest ++ = pal32[*src ++]; while (--size);
                        }
                
                        XPutImage(display, cur_window, gc,
                                  image, 0, 0, 0, 0, 320, 200);

#endif /* X11 */

#ifdef VGL
        memset(video_buffer, 0, 320);
        memcpy(VGLDisplay->Bitmap, video_buffer, 64000);
#endif /* VGL */

#ifdef DPMI
      update_video();
#endif /* DPMI */

          cur_time = m_get_time() - b_time;
      } while (cur_time < AFF_LEN);
      b_time += AFF_LEN;
      cur_time -= AFF_LEN;
      effect++;
   } while (effect->update != 0);
 }
stop:

#ifdef X11
        XCloseDisplay(display);
        free(XVideoBuffer);
        return 0;
#endif /* X11 */
#ifdef VGL
        VGLKeyboardEnd();
        VGLEnd();
        return 0;
#endif /* VGL */
#ifdef DPMI
        set_timer_speed(0);
        set_video_mode(0x03);
#endif /* DPMI */
}
