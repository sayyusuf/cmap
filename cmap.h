#ifndef CMAP_H
#define CMAP_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


typedef struct node_s
{
	void			*key;
	void			*value;
	struct node_s	*parent;
	struct node_s	*high;
	struct node_s	*low;
} node_t;

typedef struct
{
	node_t	*first;
	int (*compare)(void *, void *);
} cmap_t;


int cmap_new(cmap_t *map, int (*compare)(void *, void *));
int	cmap_destroy(cmap_t *map, void (*del_key)(void *), void (*del_val)(void *));

int	cmap_insert(cmap_t *map, void *key, void *val);
int cmap_find(cmap_t* map, void *key, void **ret);
int	cmap_erase(cmap_t *map, void *key, void (*del_key)(void *), void (*del_val)(void *));

#endif