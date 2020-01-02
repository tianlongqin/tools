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
#include <list.h>
#include <hash_map.h>
#ifdef CONFIG_MD5
#include <md5.h>
#endif

struct hmap {
	struct list_head *first;
	size_t size;
	Tatomic_u32 node_size;
};

struct hnode {
	struct list_head node;
	char hkey[16];
	void *value;
};

#ifdef CONFIG_MD5
#define HMAP_GET_FIRST(phmap, key, key_l, hkey)	({\
	md5(key, key_l, hkey);					\
	(&(phmap->first[(*(uint32_t *)hkey) % phmap->size]));	\
})
#else
#error "please select digest algorithm"
#endif

#define KEY_CMP(key1, key2, len)	memcmp(key1, key2, len)

int Thmap_create(void **pphmap, size_t size)
{
	int i;
	struct list_head *first = NULL;
	struct hmap *phmap = NULL;

	if (size <= 0)
		return -1;

	phmap = calloc(1, sizeof(*phmap));
	first = calloc(size, sizeof(*first));
	if (!first || !phmap)
		return -1;

	for (i = 0; i < size; i++)
		INIT_LIST_HEAD(&first[i]);

	phmap->size = size;
	phmap->first = first;
	Tatomic_storen(&phmap->node_size, 0);
	*pphmap = phmap;

	return 0;
}

void Thmap_destroy(void **pphmap)
{
	free(*pphmap);
	*pphmap = NULL;
}

int Thmap_insert(void *_phmap, void *key, size_t key_l, void *value, void **old_value)
{
	struct hmap *phmap = _phmap;
	struct list_head *first = NULL;
	struct hnode *phnode = NULL;
	struct hnode *tmp = NULL;
	uint8_t hkey[16];

	if (!phmap || !key || !value || !key_l)
		return -1;

	first = HMAP_GET_FIRST(phmap, key, key_l, hkey);
	list_for_each_entry(tmp, first, node) {
		if (!KEY_CMP(tmp->hkey, hkey, 16)) {
			if (tmp->value != value)
				*old_value = tmp->value;
			tmp->value = value;
			return 0;
		}
	}

	phnode = malloc(sizeof(*phnode));
	if (!phnode)
		return -1;

	phnode->value = value;
	memcpy(phnode->hkey, hkey, 16);
	list_add_tail(&phnode->node, first);
	Tatomic_addf(&phmap->node_size, 1);

	return 0;
}

int Thmap_delete(void *_phmap, void *key, size_t key_l)
{
	struct hmap *phmap = _phmap;
	struct list_head *first = NULL;
	struct hnode *phnode = NULL;
	char hkey[16];

	if (!phmap || !key)
		return -1;

	first = HMAP_GET_FIRST(phmap, key, key_l, hkey);
	list_for_each_entry(phnode, first, node) {
		if (!KEY_CMP(phnode->hkey, hkey, 16)) {
			list_del(&phnode->node);
			free(phnode);
			Tatomic_subf(&(phmap->node_size), 1);
			break;
		}
	}

	return 0;
}

void *Thmap_delete_search(void *_phmap, void *key, size_t key_l)
{
	struct hmap *phmap = _phmap;
	struct list_head *first = NULL;
	struct hnode *phnode = NULL;
	char hkey[16];
	void *value;

	if (!phmap || !key)
		return NULL;

	first = HMAP_GET_FIRST(phmap, key, key_l, hkey);
	list_for_each_entry(phnode, first, node) {
		if (!KEY_CMP(phnode->hkey, hkey, 16)) {
			list_del(&phnode->node);
			value = phnode->value;
			free(phnode);
			Tatomic_subf(&(phmap->node_size), 1);
			return value;
		}
	}

	return NULL;
}

void *Thmap_search(void *_phmap, void *key, size_t key_l)
{
	struct hmap *phmap = _phmap;
	struct list_head *first = NULL;
	struct hnode *phnode = NULL;
	char hkey[16];

	if (!phmap || !key)
		return NULL;

	first = HMAP_GET_FIRST(phmap, key, key_l, hkey);
	list_for_each_entry(phnode, first, node) {
		if (!KEY_CMP(phnode->hkey, hkey, 16)) {
			return phnode->value;
		}
	}

	return NULL;
}

int Thmap_get_node_size(void *_phmap)
{
	struct hmap *phmap = _phmap;
	return Tatomic_loadn(&phmap->node_size);
}
