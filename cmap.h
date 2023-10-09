#ifdef __cplusplus
extern "C" {
#endif

#ifndef CMAP_H
#define CMAP_H

#include <stdint.h>

typedef uint64_t	cmap_key_t;

typedef struct node_s	node_t;

typedef struct cmap_s	cmap_t;

typedef struct cmap_s
{
	node_t	*first;
	size_t	ksz;
	size_t	tsz;
	int	(*cmp)(cmap_key_t, cmap_key_t);
} cmap_t;


#define VAL_TO_KEY(val) ((cmap_key_t)(val))
#define VAL_TO_PTR(val) ((void *)(val))

int
cmap_init(cmap_t *map, int (*cmp)(cmap_key_t, cmap_key_t), size_t type_size);

int
cmap_destroy(cmap_t *map, void (*del_key)(cmap_key_t), void (*del_val)(void *));


int
cmap_insert(cmap_t *map, cmap_key_t key, void *value_addr);

int
cmap_find(cmap_t* map, cmap_key_t key, void *ret);

void	*
cmap_ptr(cmap_t* map, cmap_key_t key);

int
cmap_erase(cmap_t *map, cmap_key_t key, void (*del_key)(cmap_key_t), void (*del_val)(void *));

int
cmap_iter(cmap_t *map, void *any,  void(*f)(cmap_key_t key, void *val_addr, void *any));

#endif

#ifdef __cplusplus
 }
#endif

