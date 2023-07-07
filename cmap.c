#include "cmap.h"

node_t *node_new(node_t *parent, void *key, void *value)
{
    node_t *node;

    node = malloc(sizeof(*node));
    if (!node)
        return (node);
    *node = (node_t){.high = NULL, .low = NULL, .key = key, .value = value, .parent = parent};
    return (node);
}

int	node_del_all(node_t *node, void (*del_key)(void *), void (*del_val)(void *))
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

int	node_insert(node_t *node, node_t *new, int (*compare)(void*, void *))
{
	int	res;

	if (!node || !new || !compare)
		return (-1);
	res = compare(node->key, new->key);
	if (!res)
		return (-1);
	else if (res > 0)
	{
		if (node->high)
			return (node_insert(node->high, new, compare));
		else
            node->high = new;
	}
	else
	{
		if (node->low)
			return (node_insert(node->low, new, compare));	
		else
			node->low = new;
	}
	return (0);
}

int node_find(node_t *node, void *key, int (*compare)(void*, void *), void **ret)
{
	int	res;

	if (!node || !key || !compare)
		return (-1);
	res = compare(node->key, key);
	if (!res)
        *ret = node->value;
	else if (res > 0)
		return (node_find(node->high, key, compare, ret));
	else
		return (node_find(node->low, key, compare, ret));
    return (0);
}

node_t	*node_cut_most_low(node_t *node)
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

node_t	*node_cut_most_high(node_t *node)
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



void	*node_cut_from_low(node_t *root)
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

void	*node_cut_from_high(node_t *root)
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

void	*node_cut(node_t **nodep, void *key, int (*compare)(void*, void *))
{
	int		res;
	node_t	*root;
	node_t	*tmp;

	if (!nodep|| !(*nodep) || !key || !compare)
		return (NULL);
	root = *nodep;
	res = compare(root->key, key);
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
		return (node_cut(&root->high, key, compare));
	else
		return (node_cut(&root->low, key, compare));
}






int cmap_new(cmap_t *map, int (*compare)(void *, void *))
{
    if (!map || !compare)
        return (-1);
    map->compare = compare;
    map->first = NULL;
    return (0);
}


int	cmap_destroy(cmap_t *map, void (*del_key)(void *), void (*del_val)(void *))
{
    if (!map)
        return (-1);
    node_del_all(map->first, del_key, del_val);
    return (0);
}

int	cmap_insert(cmap_t	*map, void *key, void *val)
{
    node_t  *new;

    if (!map || !key || !val)
        return (-1);
    new = node_new(NULL, key, val);
    if (!new)
        return (-1);
    if (!map->first)
        map->first = new;
    else if (node_insert(map->first, new, map->compare) < 0)
    {
        free(new);
        return (-1);
    }
    return (0);
}

int cmap_find(cmap_t* map, void *key, void **ret)
{
    if (!map || !key || !ret)
        return (-1);
    return (node_find(map->first, key, map->compare, ret));
}


int	cmap_erase(cmap_t *map, void *key, void (*del_key)(void *), void (*del_val)(void *))
{
    node_t	*ret;
    
    if (!map || !key)
        return (-1);
    ret = node_cut(&map->first, key, map->compare);
	if (!ret)
		return (-1);
	if (del_key)
		del_key(ret->key);
	if (del_val)
		del_val(ret->value);
	free(ret);
	return (0);
}