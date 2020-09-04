#ifndef __HASH_MAP_HH_19910612__
#define __HASH_MAP_HH_19910612__
#include <list.h>
#include <Tatomic.h>
	
extern int Thmap_create(void **pphmap, size_t size);
extern int Thmap_insert(void *phmap, void *key, size_t key_l, void *value, void **old_value);
extern int Thmap_search(void *phmap, void *key, size_t key_l, void **value);
extern int Thmap_delete(void *phmap, void *key, size_t key_l, void **old_value);
extern void Thmap_destroy(void **pphmap);
extern int Thmap_get_node_size(void *phmap);


/* Support Thmap_list_for_each , Not recommended to use alone*/
extern void *Thmap_get_list(void *phmap, void *hkey, size_t hkey_l);
extern void *Thmap_get_value(void **node);
#define Thmap_list_for_each(phmap, value, key, key_l, tmp1, tmp2)					\
	for (tmp1 = Thmap_get_list(phmap, key, key_l) , tmp2 = tmp1 , value = Thmap_get_value(&tmp2); 	\
	     tmp1 != tmp2;										\
	     pvalue = Thmap_get_value(&tmp2))

#endif
