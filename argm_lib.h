#ifndef CMAP_H
#define CMAP_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


typedef struct	node_s node_t;

typedef struct	node_s
{
	void		*key;
	void		*value;
	node_t	*high;
	node_t	*low;

} node_t;

typedef struct argm_s
{
	node_t	*first;
	size_t		nof_nodes;
	int		(*compare)(void *, void *);
} argm_t;

int	argm_del(argm_t *map, void *key, void (*del_key)(void *), void (*del_val)(void *));
void	*argm_find(argm_t *map, void *key);

int	argm_insert(argm_t	*map, void *key, void *val);
int	argm_destroy(argm_t *map, void (*del_key)(void *), void (*del_val)(void *));
argm_t	*argm_new(int (*compare)(void *, void *));

#endif