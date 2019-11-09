#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void tree_init(tree_t *tree)
{
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

void tree_set_compare_cb(tree_t *tree, compare_cb cb)
{
    tree->compare = cb;
}

void tree_set_traverse_cb(tree_t *tree, traverse_cb cb)
{
    tree->traverse = cb;
}

void tree_set_free_cb(tree_t *tree, free_cb cb)
{
    tree->free = cb;
}


static void _tree_traverse_and_execute_from_node(tree_t *tree,
                                                 node_t *node)
{
    if (node == NULL)
    {
        return;
    }
    else
    {
        tree->traverse(node);
        _tree_traverse_and_execute_from_node(tree, node->left);
        _tree_traverse_and_execute_from_node(tree, node->right);
    }
}
        

int tree_traverse_and_execute(tree_t *tree)
{
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
    _tree_traverse_and_execute_from_node(tree, tree->head);
    return 0;
}

/* Insert new_node into tree */
static int _tree_insert_from_node(tree_t *tree, node_t **tree_node, node_t *new_node)
{
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
    else if (cmp >= 0)
    {
        _tree_insert_from_node(tree, &((*tree_node)->left), new_node);
    }
    return 0;
}

/* Add node to tree*/
int tree_insert_node(tree_t *tree, node_t *new_node)
{
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


static void traverse_and_add_nodes_to_subtree(tree_t *tree, node_t *node)
{
    if (!node->right && !node->left)
    {
        return;
    }
    if (node->right)
    {
        if (tree->head == NULL)
        {
            tree->head = node->right;
        }
        else
        {
            tree_insert_node(tree, node->right);
            traverse_and_add_nodes_to_subtree(tree, node->right);
        }
    }
    if (node->left)
    {
        if (tree->head == NULL)
        {
            tree->head = node->left;
        }
        else
        {
            tree_insert_node(tree, node->left);
            traverse_and_add_nodes_to_subtree(tree, node->left);
        }
    }
}

/*create a subtree using the specified child of node */
static tree_t *tree_create_subtree(tree_t *orig_tree, node_t *node)
{
    tree_t *tree = malloc(sizeof(tree_t));
    if (!tree)
    {
        printf("could not malloc new tree\n");
        return NULL;
    }
    tree_init(tree);
    tree->compare = orig_tree->compare;
    tree->traverse = orig_tree->traverse;
    tree->free = orig_tree->free;
    tree->head = NULL;

    traverse_and_add_nodes_to_subtree(tree, node);
    return tree;
}

static node_t *tree_search_for_parent_from_node(tree_t *tree,
                                                node_t **tree_node,
                                                node_t *node)
{
    int cmp;
    if ((*tree_node)->right == node ||
        (*tree_node)->left == node)
    {   
	    return *tree_node;
    }
    cmp = tree->compare(*tree_node, node);
    if (cmp < 0)
    {
        return tree_search_for_parent_from_node(tree, &((*tree_node)->right), node);
    }
    else if (cmp > 0)
    {
        return tree_search_for_parent_from_node(tree, &((*tree_node)->left), node);
    }
    // head node has no parent
    return NULL;
}

static node_t *tree_get_node_parent(tree_t *tree, node_t *node)
{
    node_t *parent;
    if (!tree)
    {
        printf("get parent Tree pointer NULL\n");
        return NULL;
    }
    if (!node)
    {
        printf("get parent: No node given\n");
        return NULL;
    }
    parent = tree_search_for_parent_from_node(tree, &tree->head, node);
    return parent;
}

/* Remove node from tree */
int tree_remove_node(tree_t *tree, node_t *node)
{
    tree_t *tmp_tree;
    node_t *parent_node;
    if(!tree)
    {
        printf("Tree pointer NULL\n");
        return -1;
    }
    if (!node)
    {
        printf("No node given to remove\n");
        return -1;
    }
    parent_node = tree_get_node_parent(tree, node);
    tmp_tree = tree_create_subtree(tree, node);
    if (parent_node == NULL)
    {
        tree->head = tmp_tree->head;
    }
    else if (parent_node->right == node)
    {
        parent_node->right = tmp_tree->head;
    }
    else if (parent_node->left == node)
    {
        parent_node->left = tmp_tree->head;
    }
    else
    {
        printf("Shouldn't get here :)\n");
    }
    tree_free_node(tree, &node);
    free(tmp_tree);
    return 0;
}
