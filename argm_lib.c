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

node_t	*node_cut_most_low(node_t *node)
{
	node_t	*ret;

	if (node->low == NULL)
		return (NULL);
	ret = node_cut_most_low(node->low);
	if (ret == NULL)
	{
		ret = node->low;
		node->low = node->low->high;
		ret->high = NULL;
	}
	return (ret);
}

node_t	*node_cut_most_high(node_t *node)
{
	node_t *ret;

	if (node->high == NULL)
		return (NULL);
	ret = node_cut_most_high(node->high);
	if (ret == NULL)
	{
		ret = node->high;
		node->high = node->high->low;
		ret->low = NULL;
	}
	return (ret);
}


/*
int	node_del_one(node_t *node, void (*del_key)(void *), void (*del_val)(void *))
{
	if (node == NULL)
		return (-1);
	if (node->high != NULL)
		node_del_one(node->high, del_key, del_val);
	if (node->low != NULL)
		node_del_one(node->low, del_key, del_val);
	if (del_key)
		del_key(node->key);
	if (del_val)
		del_val(node->value);
	free(node);
	return (0);
}

int	node_del_all(node_t *node, void (*del_key)(void *), void (*del_val)(void *))
{
	if (node == NULL)
		return (-1);
	if (node->high != NULL)
		node_del(node->high, del_key, del_val);
	if (node->low != NULL)
		node_del(node->low, del_key, del_val);
	if (del_key)
		del_key(node->key);
	if (del_val)
		del_val(node->value);
	free(node);
	return (0);
}

*/

int	node_insert(node_t *node, node_t *new, int (*compare)(void*, void *))
{
	int	res;

	if (node == NULL || new == NULL || compare == NULL)
		return (-1);
	res = compare(node->key, new->key);
	if (!res)
		return (-1);
	else if (res > 0)
	{
		if (node->high == NULL)
			node->high = new;
		else
			return (node_insert(node->high, new, compare));
	}
	else
	{
		if (node->low == NULL)
			node->low = new;
		else
			return (node_insert(node->low, new, compare));	
	}
	return (0);
}

node_t	*node_new(void *key, void *value)
{
	node_t	*node;

	if (key == NULL || value == NULL)
		return (NULL);
	node = malloc(sizeof(node_t));
	if (node == NULL)
		return (NULL);
	node->key = key;
	node->value = value;
	node->high = NULL;
	node->low = NULL;
	return (node);
}





int	argm_insert(argm_t	*map, void *key, void *val)
{
	int			res;
	node_t	*new;

	if (map == NULL || key == NULL || val == NULL)
		return (-1);
	res = 0;
	new = node_new(key, val);
	if (new == NULL)
		return (-1);
	if (map->first == NULL)
		map->first = new;
	else
		res = node_insert(map->first, new, map->compare);
	if (res < 0)
	{
		free(new);
		return (-1);
	}
	++map->nof_nodes;
	return (0);
}

argm_t	*argm_new(int (*compare)(void *, void *))
{
	argm_t	*argm;

	if (compare == NULL)
		return (NULL);
	argm = malloc(sizeof(argm_t));
	if (argm == NULL)
		return (NULL);
	argm->first = NULL;
	argm->nof_nodes = 0;
	argm->compare = compare;
	return (argm);
}

