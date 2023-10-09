#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <cmap.h>


typedef struct node_s
{
	struct node_s	*high;
	struct node_s	*low;
	size_t		tsz;
	size_t		ksz;
	cmap_key_t	key;
	uint64_t	value[];
} node_t;


node_t	*
node_new(cmap_key_t key, void *value, size_t type_size)
{
    node_t *node;

    node = malloc(sizeof(*node) + type_size);
    if (!node)
        return (node);
    *node = (node_t){.high = NULL, .low = NULL, .key = key, .tsz = type_size};
	memcpy(node->value, value, type_size);
    return (node);
}

int
node_del_all(node_t *node, void (*del_key)(cmap_key_t), void (*del_val)(void *))
{
	if (!node)
		return (-1);
	if (node->high)
		node_del_all(node->high, del_key, del_val);
	if (node->low)
		node_del_all(node->low, del_key, del_val);
	if (del_key)
		del_key(node->key);
	if (del_val)
		del_val(node->value);
	free(node);
	return (0);
}

int
node_iter(node_t *node, void *any,  void(*f)(cmap_key_t key, void *val_addr, void *any))
{
	if (!node)
		return (-1);
	if (node->low)
		node_iter(node->low, any, f);
	if (node->high)
		node_iter(node->high, any, f);
	f(node->key, node->value, any);
	return (0);
}




int
node_insert(node_t *node, node_t *new, int (*cmp)(cmap_key_t, cmap_key_t))
{
	int	res;

	if (!node || !new || !cmp)
		return (-1);
	res = cmp(node->key, new->key);
	if (!res)
		return (-1);
	else if (res > 0)
	{
		if (node->high)
			return (node_insert(node->high, new, cmp));
		else
            node->high = new;
	}
	else
	{
		if (node->low)
			return (node_insert(node->low, new, cmp));	
		else
			node->low = new;
	}
	return (0);
}

int
node_find(node_t *node, cmap_key_t key, int (*cmp)(cmap_key_t, cmap_key_t), void *ret)
{
	int	res;

	if (!node || !cmp)
		return (-1);
	res = cmp(node->key, key);
	if (!res)
		memcpy(ret ,node->value, node->tsz);
	else if (res > 0)
		return (node_find(node->high, key, cmp, ret));
	else
		return (node_find(node->low, key, cmp, ret));
    return (0);
}

void	*
node_ptr(node_t *node, cmap_key_t key, int (*cmp)(cmap_key_t, cmap_key_t))
{
	int	res;

	if (!node || !cmp)
		return (NULL);
	res = cmp(node->key, key);
	if (!res)
		return ((void *)node->value);
	else if (res > 0)
		return (node_ptr(node->high, key, cmp));
	else
		return (node_ptr(node->low, key, cmp));
    return (NULL);
}

node_t	*
node_cut_most_low(node_t *node)
{
	node_t	*ret;

	if (!node->low)
		return (NULL);
	ret = node_cut_most_low(node->low);
	if (!ret)
	{
		ret = node->low;
		node->low = node->low->high;
		ret->low = NULL;
		ret->high = NULL;
	}
	return (ret);
}

node_t	*
node_cut_most_high(node_t *node)
{
	node_t *ret;

	if (!node->high)
		return (NULL);
	ret = node_cut_most_high(node->high);
	if (!ret)
	{
		ret = node->high;
		node->high = node->high->low;
		ret->low = NULL;
		ret->high = NULL;
	}
	return (ret);
}



void	*
node_cut_from_low(node_t *root)
{
	node_t	*tmp;

	tmp = NULL;
	if (root->low)
	{
		tmp = node_cut_most_high(root->low);
		if (tmp == NULL)
		{
			tmp = root->low;
			tmp->high = root->high;
			root->low = NULL;
			root->high = NULL;
		}
		else
		{
			tmp->low = root->low;
			tmp->high = root->high;
			root->low = NULL;
			root->high = NULL;
		}
	}
	return (tmp);
}

void	*
node_cut_from_high(node_t *root)
{
	node_t	*tmp;

	tmp = NULL;
	if (root->high)
	{
		tmp = node_cut_most_low(root->high);
		if (tmp == NULL)
		{
			tmp = root->high;
			tmp->low = root->low;
			root->low = NULL;
			root->high = NULL;
		}
		else
		{
			tmp->low = root->low;
			tmp->high = root->high;
			root->low = NULL;
			root->high = NULL;
		}
	}
	return (tmp);
}

void	*
node_cut(node_t **nodep, cmap_key_t key, int (*cmp)(cmap_key_t, cmap_key_t))
{
	int		res;
	node_t	*root;
	node_t	*tmp;

	if (!nodep|| !(*nodep) || !cmp)
		return (NULL);
	root = *nodep;
	res = cmp(root->key, key);
	if (!res)
	{
		tmp = node_cut_from_low(root);
		if (tmp == NULL)
			tmp = node_cut_from_high(root);
		*nodep = tmp;
		root->low = NULL;
		root->high = NULL;
		return (root);
	}
	else if (res > 0)
		return (node_cut(&root->high, key, cmp));
	else
		return (node_cut(&root->low, key, cmp));
}

/*******************************************************************/

static int
cmap_cmp(cmap_key_t v1, cmap_key_t v2)
{
	return ((int)(v1 - v2));
}

int
cmap_init(cmap_t *map, int (*cmp)(cmap_key_t, cmap_key_t), size_t type_size)
{
	if (!map)
		return (-1);
	if (cmp)
		map->cmp = cmp;
	else
		map->cmp = cmap_cmp;
	map->first = NULL;
	map->tsz = type_size;
	return (0);
}

int
cmap_destroy(cmap_t *map, void (*del_key)(cmap_key_t), void (*del_val)(void *))
{
	if (!map)
		return (-1);
	node_del_all(map->first, del_key, del_val);
	return (0);
}

int
cmap_insert(cmap_t *map, cmap_key_t key, void *value_addr)
{
	node_t  *new;

	if (!map || !value_addr)
		return (-1);
	new = node_new(key, value_addr, map->tsz);
	if (!new)
		return (-1);
	if (!map->first)
		map->first = new;
	else if (0 > node_insert(map->first, new, map->cmp))
	{
		free(new);
		return (-1);
	}
	return (0);
}

int
cmap_find(cmap_t* map, cmap_key_t key, void *ret)
{
	if (!map || !ret)
		return (-1);
       	return (node_find(map->first, key, map->cmp, ret));
}

void *
cmap_ptr(cmap_t* map, cmap_key_t key)
{
	if (!map)
		return (NULL);
       	return (node_ptr(map->first, key, map->cmp));
}

int
cmap_erase(cmap_t *map, cmap_key_t key, void (*del_key)(cmap_key_t), void (*del_val)(void *))
{
	node_t	*ret;

	if (!map)
		return (-1);
	ret = node_cut(&map->first, key, map->cmp);
	if (!ret)
		return (-1);
	if (del_key)
		del_key(ret->key);
	if (del_val)
		del_val(ret->value);
	free(ret);
	return (0);
}


int
cmap_iter(cmap_t *map, void *any,  void(*f)(cmap_key_t key, void *val_addr, void *any))
{
	if (!map || !f)
		return (-1);
	node_iter(map->first, any, f);
	return (0);
}

#ifdef __cplusplus
 }
#endif

