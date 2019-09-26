/*
 * This file is tools member.
 *
 * Copyright (c) 2015-2019, tianlongqin, <qtl_linux@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function:  tools debug.
 * Created on: 2019-09-25
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <match_algo.h>

//match strstr algo
static int match_str_init(void *private);
static unsigned char *match_str(void *private, int *pos);

// match sunday algo
static int match_sun_init(void *private);
static unsigned char *match_sun(void *private, int *pos);

// match horspool algo
static int match_hp_init(void *private);
static unsigned char *match_hp(void *private, int *pos);

// match KMP algo
static int match_kmp_init(void *private);
static unsigned char *match_kmp(void *private, int *pos);

// match Boyer-Moore algo
//static int match_bm_init(void *private);
//static unsigned char *match_bm(void *private, int *pos);


struct match_algo_data {
	const unsigned char *s, *t;
	size_t s_l, t_l;
	unsigned char aux[256];
	int *paux2;
};

struct match_algo_paras {
	int algo;
	struct match_algo_data data;
};

struct match_algo_ops {
	int (*match_algo_init)(void *);
	unsigned char *(*match_algo)(void *, int *);
};

struct match_algo {
	struct match_algo_paras para;
	struct match_algo_ops *ops;
};

#define ALGO_OPS(name) \
static struct match_algo_ops name##_ops = { \
	.match_algo_init = match_##name##_init, \
	.match_algo = match_##name, 	\
};	\

ALGO_OPS(sun);
ALGO_OPS(hp);
ALGO_OPS(str);
ALGO_OPS(kmp);
//ALGO_OPS(bm);

static int match_str_init(void *private)
{
	return 0;
}

static unsigned char *match_str(void *private, int *ppos)
{
	struct match_algo_paras *para = private;
	struct match_algo_data *data = &para->data;
	const unsigned char *s = data->s;
	const unsigned char *t = data->t;

	unsigned char *r = strstr(s, t);
	if (!r)
		*ppos = 0;
	else
		*ppos = r - s;

	return r;
}

static int match_sun_init(void *private)
{
	int i;
	struct match_algo_paras *para = private;
	struct match_algo_data *data = &para->data;

	memset(data->aux, data->t_l + 1, 256);

	for(i = 0; i < data->t_l; i++)
		data->aux[data->t[i]] = data->t_l - i;

	return 0;
}

static unsigned char *match_sun(void *private, int *ppos)
{
	struct match_algo_paras *para = private;
	struct match_algo_data *data = &para->data;
	const unsigned char *s = data->s;
	const unsigned char *t = data->t;
	const size_t s_l = data->s_l;
	const size_t t_l = data->t_l;
	int i, pos = 0;

	while(pos <= s_l - t_l) {
		for (i = 0; s[pos + i] == t[i] && i < t_l; i++);

		if (__builtin_expect(i == t_l, 0)) {
			*ppos =  pos;
			return (char *)s + pos;
		}

		pos += data->aux[s[pos + t_l]];
	}
	return NULL;
}

static int match_hp_init(void *private)
{
	int i;
	struct match_algo_paras *para = private;
	struct match_algo_data *data = &para->data;

	memset(data->aux, data->t_l, 256);

	for(i = 0; i < data->t_l - 1; i++)
		data->aux[data->t[i]] = data->t_l - i - 1;

	return 0;
}

static unsigned char *match_hp(void *private, int *ppos)
{
	struct match_algo_paras *para = private;
	struct match_algo_data *data = &para->data;
	const unsigned char *s = data->s;
	const unsigned char *t = data->t;
	const size_t s_l = data->s_l;
	const size_t t_l = data->t_l;
	int i, pos = 0;

	while(pos <= s_l - t_l) {
		for (i = t_l - 1; s[pos + i] == t[i]; i--);

		if (__builtin_expect(i == -1, 0)) {
			*ppos =  pos;
			return (unsigned char *)s + pos;
		}

		pos += data->aux[s[pos + t_l - 1]];
	}
	return NULL;
}

static int match_kmp_init(void *private)
{
#if 0
	int i = -1, j = 0;
	struct match_algo_paras *para = private;
	struct match_algo_data *data = &para->data;
	const unsigned char *t = data->t;
	const size_t t_l = data->t_l;
	int *next = data->paux2;
	if (next == NULL)
		return -1;

	next[0] = -1;
	while(j < t_l - 1) {
		if (i == -1 || t[i] == t[j]) {
			if (t[++i] == t[++j])
				next[j] = next[i];
			else
				next[j] = i;

		} else {
			i = next[i];
		}
	}
	return 0;
#endif
	return 0;
}

static unsigned char *match_kmp(void *private, int *ppos)
{
#if 0
	int i = 0, j = 0;
	const struct match_algo_paras *para = private;
	const struct match_algo_data *data = &para->data;
	const unsigned char *s = data->s, *t = data->t;
	const size_t s_l = data->s_l, t_l = data->t_l;
	const int *next = data->paux2;

	while (i < s_l && j < t_l) {
		if (j == -1 || s[i] == t[j]) {
			i++;
			j++;
		} else {
			j = next[j];
		}
	}

	if (j == t_l) {
		return (unsigned char *)&s[j - j];
	}

	return NULL;
#endif
	return NULL;
}

static inline void match_set_data(struct match_algo_paras *para, char *s, size_t s_l, char *t, size_t t_l)
{
	struct match_algo_data *d = &para->data;
	d->s = s;
	d->t = t;
	d->s_l = s_l;
	d->t_l = t_l;
}

static inline void match_set_algo(struct match_algo_paras *para, int algo)
{
	para->algo = algo;
}

int Tmatch_algo_init(void *h, int algo, char *s, size_t s_l, char *t, size_t t_l)
{
	if (!(h && s && t && s_l && t_l))
		return -1;

	struct match_algo *m = h;
	struct match_algo_ops **ops = &m->ops;
	struct match_algo_paras *para = &m->para;

	switch (algo) {
		case MATCH_ALGO_STR:
			*ops = &str_ops;
			break;
		case MATCH_ALGO_SUN:
			*ops = &sun_ops;
			break;
		case MATCH_ALGO_BM:
			//*ops = &bm_ops;
			break;
		case MATCH_ALGO_KMP:
			*ops = &kmp_ops;
			break;
		case MATCH_ALGO_HP:
			*ops = &hp_ops;
			break;
		default:
			return -2;
	}

	match_set_data(para, s, s_l, t, t_l);
	match_set_algo(para, algo);
	return (*ops)->match_algo_init((void *)para);
}

unsigned char *Tmatch_algo(void *h, int *ppos)
{
	struct match_algo *m = h;
	struct match_algo_ops *ops = m->ops;
	struct match_algo_paras *para = &m->para;
	return ops->match_algo(para, ppos);
}

inline void *Tmatch_new()
{
	return calloc(1, sizeof(struct match_algo));
}

inline void Tmatch_free(void *h)
{
	free(h);
}

inline void Tmatch_set_aux(void *h, int *aux)
{
	struct match_algo *m = h;
	m->para.data.paux2 = aux;
}
