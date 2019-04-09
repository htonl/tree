#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

/* function to provide the tree to compare data in nodes */
typedef int (*compare_cb) (node_t *n1, node_t *n2);
typedef void (*traverse_cb) (node_t *node);
typedef void (*free_cb) (void* data);

void tree_init(tree_t *tree)
{
    printf("in tree_init\n");
    tree->head = NULL;
}

node_t *tree_node_create_with_data(void *data)
{
    node_t *node;
    node = malloc(sizeof(node_t));

    if (!node)
    {
        printf("malloc failed for creating node\n");
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* 
   This should only be called once left and right
   are no longer needed by node
 */
void tree_free_node(tree_t *tree, node_t **node)
{
    if ((*node)->data)
    {
        tree->free((*node)->data); 
    }
    /* remove references to other nodes before freeing*/
    (*node)->left = NULL;
    (*node)->right = NULL;
    free((*node));
}

void set_compare_cb(tree_t *tree, compare_cb cb)
{
    tree->compare = cb;
}

void set_traverse_cb(tree_t *tree, traverse_cb cb)
{
    tree->traverse = cb;
}

void set_free_cb(tree_t *tree, free_cb cb)
{
    tree->free = cb;
}

/* Insert new_node into tree */
static int _tree_insert_from_node(tree_t *tree, node_t **tree_node, node_t *new_node)
{
    printf("in _tree_insert\n");
    int cmp;
    if (*tree_node == NULL)
    {
        *tree_node = new_node;
	return 0;
    }
    cmp = tree->compare(*tree_node, new_node);
    if (cmp < 0)
    {
        _tree_insert_from_node(tree, &((*tree_node)->right), new_node);
    }
    else if (cmp >0)
    {
        _tree_insert_from_node(tree, &((*tree_node)->left), new_node);
    }
    return 0;
}

/* Add node to tree*/
int tree_insert_node(tree_t *tree, node_t *new_node)
{
    printf("in tree_insert\n");
    if (!tree)
    {
        printf("Tree pointer NULL\n");
        return -1;
    }
    if (!new_node)
    {
        printf("No node given to add\n");
        return -1;
    }
    _tree_insert_from_node(tree, &tree->head, new_node);
    return 0;
}

static void _tree_traverse_and_execute_from_node(node_t *node,
                                                 traverse_cb do_stuff)
{
    printf("in _tree_traverse\n");
    if (node == NULL)
    {
        return;
    }
    else
    {
        do_stuff(node);
        _tree_traverse_and_execute_from_node(node->left, do_stuff);
        _tree_traverse_and_execute_from_node(node->right, do_stuff);
    }
}
        

int tree_traverse_and_execute(tree_t *tree)
{
    printf("in tree traverse\n");
    if (!tree)
    {
        printf("Tree is NULL\n");
        return -1;
    }
    if (tree->head == NULL)
    {
        printf("Tree head is NULL\n");
        return -1;
    }
    _tree_traverse_and_execute_from_node(tree->head, tree->traverse);
    return 0;
}


