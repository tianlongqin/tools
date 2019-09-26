#ifndef __MATCH_ALGO_19910612__
#define __MATCH_ALGO_19910612__

#define MATCH_ALGO_STR		0x01
#define MATCH_ALGO_SUN		0x02
#define MATCH_ALGO_BM		0x03
#define MATCH_ALGO_KMP		0X04
#define MATCH_ALGO_HP		0X05

#ifdef __cplusplus
extern "C" {
#endif

extern void *Tmatch_new();
extern void Tmatch_free(void *h);
extern void Tmatch_set_aux(void *h, int *aux);
extern int Tmatch_algo_init(void *h, int algo, char *s, size_t s_l, char *t, size_t t_l);
extern unsigned char *Tmatch_algo(void *h, int *ppos);

#ifdef __cplusplus
}
#endif
#endif
