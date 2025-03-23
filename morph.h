
#define MORPH_SIZE 32

const float morph_dist = 180.0;

typedef struct { 
                int o, z;
} t_morph_order;

#include "mapper.c"

#define v_count (MORPH_SIZE*(MORPH_SIZE+1))
#define p_count (MORPH_SIZE*(MORPH_SIZE-1))

float   morph_sintab[0x100];
int     morph_frame;

float   morph_alpha;
float   morph_beta;
float   morph_gamma;

float   morph_v[v_count][4];
float   morph_v2[v_count][4];
int     morph_v3[v_count][4];
int     morph_p[p_count + MORPH_SIZE][4];
int     morph_nv[v_count][2];
float   morph_np[v_count][4];
t_morph_order     morph_z[v_count];
t_morph_order     morph_order_tmp;


void 
morph_qsort(register t_morph_order *a,register t_morph_order *b)
{
        int m;
        t_morph_order *oa, *ob;
        oa = a;
        ob = b;
        m = a->z;
        a--, b++;
        for(;;) {
                do a++; while (a->z > m);
                do b--; while (b->z < m);
                if (a<b) {
                        morph_order_tmp = *a;
                        *a = *b;
                        *b = morph_order_tmp;
                } else break;
        }

        if (a - 1 > oa) morph_qsort(oa ,a - 1);
        if (b + 1 < ob) morph_qsort(b + 1, ob);
}
