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
 * Function:  tools hash map.
 * Created on: 2019-12-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <generated/autoconf.h>
#include <Terror.h>
#include <Ttypes.h>
#include <list.h>
#include <hash_map.h>
#ifdef CONFIG_HASHMAP_MD5
#include <md5.h>
#endif
/* hmap -> hhead - head_node -> hnode */
struct hnode {
	struct list_head node;
	char hkey[16];
	void *value;
};

struct hhead {
	struct list_head head_node;		/* head node */
	Tatomic_u32 node_size;			/* current list node size */
};

struct hmap {
	struct hhead *phheads;			/* all first head node array */
	Tatomic_u32 node_allsize;		/* current list node size */
	size_t size;				/* first size */
};

#define KEY_MAX 16

static int get_key(uint8_t *in, uint32_t l, uint8_t *out);

static inline int cmp_key(void *key1, void *key2, size_t l)
{
	return memcmp(key1, key2, l);
}

static inline int get_head_num(struct hmap *phmap, void *hkey, size_t hkey_l)
{
	return *(uint32_t *)hkey % phmap->size;
}

static inline struct hhead *get_head(struct hmap *phmap, int num)
{
	return &phmap->phheads[num];
}

static inline struct list_head *get_head_node(struct hhead *phhead)
{
	return &(phhead->head_node);
}

static inline struct hnode *get_node(struct hmap *phmap, void *hkey, size_t hkey_l)
{
	int num;
	struct hhead *phheads;
	struct hnode *tmp;

	num = get_head_num(phmap, hkey, hkey_l);
	phheads = get_head(phmap, num);

	list_for_each_entry(tmp, &(phheads->head_node), node)
		if (!cmp_key(tmp->hkey, hkey, hkey_l))
			return tmp;

	return NULL;
}

static inline uint32_t def_hash(uint8_t *s, uint32_t l, uint8_t *out)
{
	uint32_t h = 0, i = 0;

	while (i++ < l)
		h = 17 * h + *s++;

	*(uint32_t *)out = h;

	return h;
}

static inline int get_key(uint8_t *in, uint32_t l, uint8_t *out)
{
	int outl = 0;
#ifdef CONFIG_HASHMAP_MD5
	md5(in, l, out);
	outl = 16;
#else
	def_hash(in, l, out);
	outl = 4;
#endif
	return outl;
}

static inline void set_key(uint8_t *dest, uint8_t *src, uint32_t l)
{
	memcpy(dest, src, l);
}

static inline void set_value(struct hnode *phnode, void *value)
{
	phnode->value = value;
}

static inline void *get_value(struct hnode *phnode)
{
	return phnode->value;
}

int Thmap_create(void **pphmap, size_t size)
{
	int i;
	struct hhead *phheads = NULL;
	struct hmap *phmap = NULL;
	struct list_head *head_node;

	if (size <= 0)
		return T_EINVAL;

	phmap = calloc(1, sizeof(*phmap));
	if (!phmap)
		return T_ENOMEM;

	phheads = calloc(size, sizeof(*phheads));
	if (!phheads) {
		free(phmap);
		return T_ENOMEM;
	}

	phmap->phheads = phheads;
	phmap->size = size;
	for (i = 0; i < size; i++) {
		head_node = get_head_node(get_head(phmap, i));
		INIT_LIST_HEAD(head_node);
	}

	*pphmap = phmap;

	return T_SUCCESS;
}

void Thmap_destroy(void **pphmap)
{
	struct hmap *phmap = *pphmap;

	free(phmap->phheads);
	phmap->phheads = NULL;
	free(*pphmap);
	*pphmap = NULL;
}

int Thmap_insert(void *_phmap, void *key, size_t key_l, void *value, void **old_value)
{
	struct hmap *phmap = _phmap;
	struct hhead *phhead = NULL;
	struct hnode *phnode = NULL;
	struct hnode *tmp = NULL;
	struct list_head *head_node;
	uint8_t hkey[KEY_MAX];
	uint32_t hkey_l = 0;
	int num;

	if (!phmap || !key || !value || !key_l)
		return T_EINVAL;

	/* first get hkey */
	hkey_l = get_key(key, key_l, hkey);
	/* get node from hkey */
	tmp = get_node(phmap, hkey, hkey_l);
	/* if node exists, replace the old value */
	if (tmp && (tmp->value != value)) {
		/* if old value pointer exists, return old value */
		if (old_value)
			*old_value = tmp->value;

		/* replace the old value */
		tmp->value = value;
			return T_SUCCESS;
	}
	/* else  malloc new node, add list*/
	phnode = calloc(1, sizeof(*phnode));
	if (!phnode)
		return T_ENOMEM;

	set_value(phnode, value);
	set_key(phnode->hkey, hkey, hkey_l);

	/* get head num from hkey */
	num = get_head_num(phmap, hkey, hkey_l);
	/* get hhead from num */
	phhead = get_head(phmap, num);
	/* get head node from hhead */
	head_node = get_head_node(phhead);

	list_add_tail(&phnode->node, head_node);
	Tatomic_addf(&phmap->node_allsize, 1);
	Tatomic_addf(&phhead->node_size, 1);

	return T_SUCCESS;
}

int Thmap_delete(void *_phmap, void *key, size_t key_l, void **old_value)
{
	struct hmap *phmap = _phmap;
	struct list_head *first = NULL;
	struct hhead *phhead;
	struct hnode *phnode = NULL;
	char hkey[KEY_MAX];
	uint32_t hkey_l = 0;
	int num;

	if (!phmap || !key)
		return T_EINVAL;

	/* first get hkey */
	hkey_l = get_key(key, key_l, hkey);
	/* get head num from hkey */
	num = get_head_num(phmap, hkey, hkey_l);
	/* get head node from hhead */
	phhead = get_head(phmap, num);

	/* if hhead is empty, return  error value */
	if (Tatomic_loadn(&phhead->node_size) == 0)
		return T_ENOENT;

	/* get node from hkey */
	phnode = get_node(phmap, hkey, hkey_l);
	/* if node exists, delete node and free memory */
	if (phnode && !cmp_key(phnode->hkey, hkey, hkey_l)) {
		list_del(&phnode->node);
		/* if old_value pointer exists, return old value */
		if (old_value)
			*old_value = phnode->value;

		free(phnode);
		Tatomic_subf(&(phmap->node_allsize), 1);
		Tatomic_subf(&(phhead->node_size), 1);
		return T_SUCCESS;
	}

	return T_ENOENT;
}

int Thmap_search(void *_phmap, void *key, size_t key_l, void **value)
{
	struct hmap *phmap = _phmap;
	struct list_head *first = NULL;
	struct hnode *phnode = NULL;
	char hkey[KEY_MAX];
	uint32_t hkey_l = 0;

	if (!phmap || !key)
		return T_EINVAL;

	hkey_l = get_key(key, key_l, hkey);
	phnode = get_node(phmap, hkey, hkey_l);
	if (phnode && !cmp_key(phnode->hkey, hkey, hkey_l)) {
		*value = phnode->value;
		return T_SUCCESS;
	}

	return T_ENOENT;
}

int Thmap_get_node_size(void *_phmap)
{
	struct hmap *phmap = _phmap;
	return Tatomic_loadn(&phmap->node_allsize);
}

void *Thmap_get_list(void *_phmap, void *key, size_t key_l)
{
	struct hmap *phmap = _phmap;
	struct hhead *phhead;
	uint8_t hkey[KEY_MAX];
	uint32_t hkey_l = 0;
	int num;

	hkey_l = get_key(key, key_l, hkey);
	num = get_head_num(phmap, hkey, hkey_l);
	phhead = get_head(phmap, num);

	return get_head_node(phhead);
}

void *Thmap_get_value(void **node)
{
	struct hnode *phnode;
	struct list_head *head = *node;

	*node = head->next;
	phnode = container_of(head->next, struct hnode, node);

	return phnode->value;
}
