#ifndef __HASH_MAP_HH_19910612__
#define __HASH_MAP_HH_19910612__
#include <list.h>
#include <Tatomic.h>



extern int Thmap_create(void **pphmap, size_t size);
extern int Thmap_insert(void *phmap, void *key, size_t key_l, void *value, void **old_value);
extern void *Thmap_search(void *phmap, void *key, size_t key_l);
extern int Thmap_delete(void *phmap, void *key, size_t key_l, void **old_value);
extern void Thmap_destroy(void **pphmap);

extern int Thmap_get_node_size(void *phmap);
#endif
