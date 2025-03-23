
#define DELTA 0

#define BLUR_ADD 0.98f

float tun_sin[256];

float tunnel_a;
float tunnel_b;

float plain_a;
float plain_b;

void 
surf_plain(float y, float x)
{
     float t;
     t = (float)(65536.0 * 80.0)/(plain_a * x + plain_b * y + 30.0f);
     surf_dest[0] = t * x + 0x10000 * (16 * 3 - 1) + 0x8000;
     surf_dest[1] = t * y + 0x10000 * (8 * 4 - 1) + 0x8000;
}

void 
update2()
{
        {
                float tmp = cur_time * (float)(0.05 * TIMER_CONST);
                plain_a = sin(tmp);
                plain_b = cos(tmp);
        }

        compute_surf(surf_plain, &texture3[0][0]);
        radial_init((float)((float)sin((float)(cur_time * (float)
                           (0.025 * TIMER_CONST))) * (float)0.01 + BLUR_ADD));
        radial_do();
        print_text(&video_buffer[320 * 4 + 10], "REFUSE 4k");
}

float plain_i;

void 
update3()
{
        memset4(video_buffer, 0, 64000 / 4);
        morph_alpha = cur_time * (float)(0.0092 * TIMER_CONST);
        morph_beta  = -cur_time * (float)(0.0134 * TIMER_CONST);
        morph_gamma = cur_time * (float)(0.0145 * TIMER_CONST);
#include "morph.drw"

        radial_init((float)sin(cur_time*(float)(0.1 * TIMER_CONST)) *
                          0.01f + (float)(0.988-DELTA));
        radial_do();
}

float sphere_r;
int sphere_a;
int sphere_b;

void 
surf_sphere(float y,float x)
{
     float t;
     t = (float)(65536.0 * 300.0) / (((float)fabs((float)(16*16) - x*x - y*y) 
                                          / (float)18)*sphere_r + (float)20);
     surf_dest[0] = t * x + sphere_a;
     surf_dest[1] = t * y + sphere_b;
}

void 
update4()
{
        float tmp = cur_time * (float)(0.019 * TIMER_CONST);
        sphere_r = sin(tmp);
        sphere_a = cur_time * (int)(102400 * TIMER_CONST);
        sphere_b = cur_time * (int)(132900 * TIMER_CONST);

        compute_surf(surf_sphere, &texture2[0][0]);

        radial_init((float)(0.99 - DELTA));
        radial_do();
}

float flag_g;
float flag_s;
float flag_c;
float flag_i;

void 
surf_flag(float y, float x)
{
        float t;
        t = (float)(65536.0 * 800.0) / 30.0f;
        t /= (float)sqrt(x * x + y * y) * flag_i + 0.2f;
        surf_dest[0] = t * x;
        surf_dest[1] = t * y;
}

void 
update5()
{
        float time = cur_time * (float)(0.01 * TIMER_CONST);

        flag_g = cur_time * (float)(0.4 * TIMER_CONST);
        flag_c = (float)cos(time)*(float)0.1;
        flag_s = (float)sin(time)*(float)0.1;
        flag_i = (float)sin(time)*(float)0.1+(float)0.14;

        compute_surf(surf_flag, &texture[0][0]);
        radial_init((float)sin(cur_time * (float)(0.025 * TIMER_CONST))
                   * 0.01f + BLUR_ADD);
        radial_do();
}


void
surf_tunnel(float y, float x)
{
        float z, t, d, a, p;

        a = x * x + y * y;
        d = a * 0.6f;
        t = (float)sqrt(d) / a;
        z = t * 30.0f;
        p = ((float)atan2(x * t + 0.02f, y * t) + MPI) / (MPI * 2) * 256.0f;
        surf_dest[0] = (z * (float)(30.0 * 65536.0)) / tun_sin[(int)p] + tunnel_a;
        surf_dest[1] = p * 65536.0f + tunnel_b;
}

void 
update1()
{
        {
                int i = 256;
                float tmp = cur_time * (float)(TIMER_CONST);
                float *dest = tun_sin;
                do {
                        *dest ++ =
                          (float)sin( tmp*(float)0.10 + i/(float)256.0*MPI*4) +
                          (float)sin(-tmp*(float)0.10 + i/(float)256.0*MPI*6) + 2.9f;
                } while (--i);
        }

        tunnel_a = cur_time * (float)(128000 * TIMER_CONST);
        tunnel_b = cur_time * (float)(128000 * TIMER_CONST);

        compute_surf(surf_tunnel, &texture[0][0]);
        radial_init(0.98f);
        radial_do();
        print_text(video_buffer + 320*4 + 10, "kodex:");
        print_text(video_buffer + 320*(200-4-16*2) + (320-8*8*2-10), "Mad Rain");
}
