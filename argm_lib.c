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
		ret->low = NULL;
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
		ret->high = NULL;
	}
	return (ret);
}




int	node_del_all(node_t *node, void (*del_key)(void *), void (*del_val)(void *))
{
	if (node == NULL)
		return (-1);
	if (node->high != NULL)
		node_del_all(node->high, del_key, del_val);
	if (node->low != NULL)
		node_del_all(node->low, del_key, del_val);
	if (del_key != NULL)
		del_key(node->key);
	if (del_val != NULL)
		del_val(node->value);
	free(node);
	return (0);
}


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

void	*node_cut_from_low(node_t *root)
{
	node_t	*tmp;

	tmp = NULL;
	if (root->low != NULL)
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
	if (root->high != NULL)
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

	if (nodep == NULL || *nodep == NULL || key == NULL || compare == NULL)
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

void	*node_find(node_t *node, void *key, int (*compare)(void*, void *))
{
	int	res;

	if (node == NULL || key == NULL || compare == NULL)
		return (NULL);
	res = compare(node->key, key);
	if (!res)
		return (node->value);
	else if (res > 0)
			return (node_find(node->high, key, compare));
	else
			return (node_find(node->low, key, compare));	
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





int	argm_del(argm_t *map, void *key, void (*del_key)(void *), void (*del_val)(void *))
{
	node_t	*ret;

	if (map == NULL)
		return (-1);
	ret = node_cut(&map->first, key, map->compare);
	if (ret == NULL)
		return (-1);
	if (del_key != NULL)
		del_key(ret->key);
	if (del_val != NULL)
		del_val(ret->value);
	free(ret);
	--map->nof_nodes;
	return (0);
}

void	*argm_find(argm_t *map, void *key)
{

	if (map == NULL || key == NULL)
		return (NULL);
	return (node_find(map->first, key, map->compare));
}

int	argm_insert(argm_t	*map, void *key, void *val)
{
	node_t	*new;

	if (map == NULL || key == NULL || val == NULL)
		return (-1);
	new = node_new(key, val);
	if (new == NULL)
		return (-1);
	if (map->first == NULL)
		map->first = new;
	else if (node_insert(map->first, new, map->compare) < 0)
	{
		free(new);
		return (-1);
	}
	++map->nof_nodes;
	return (0);
}



int	argm_destroy(argm_t *map, void (*del_key)(void *), void (*del_val)(void *))
{
	if (map == NULL)
		return (-1);
	if (map->first != NULL && node_del_all(map->first, del_key, del_val) < 0)
		return (-1);
	free(map);
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
