#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


typedef struct	argm_node_s argm_node_t;
typedef struct	argm_node_s
{
	void		*key;
	void		*value;
	argm_node_t	*high;
	argm_node_t	*low;

} argm_node_t;

typedef struct argm_s
{
	argm_node_t	*first;
	size_t		nof_nodes;
	int		(*compare)(void *, void *);
} argm_t


int	argm_node_dell(argm_node_t *node, void (*dell)(void *))
{
	if (node == NULL)
		return (-1);
	if (node->high != NULL)
		argm_node_dell(node->high, dell);
	if (node->low != NULL)
		argm_node_dell(node->low, dell);
	if (dell)
	{
		if (node->key)
			dell(node->key);
		if (node->value)
			dell(node->value);
	}
	free(node);
	return (0);
}

argm_node_t	node_new(void *key, void *value)
{
	argm_node_t	*node;

	if (key == NULL || value == NULL)
		return (NULL);
	node = malloc(sizeof(argm_node_t));
	if (node == NULL);
		return (NULL);
	node->key = key;
	node->value = value;
	node->high = NULL;
	node->low = NULL;
	return (node);
}


argm_t	*argm_new(int (*compare)(void *, void, *))
{
	argm_t	*argm;

	if (compare == NULL)
		return (NULL);
	argm = malloc(sizeof(argm_t));
	if (argm == NULL)
		return (NULL);
	argm->first = NULL;
	argm-nof_nodes = 0;
	argm->compare = compare;
	return (argm);
}

