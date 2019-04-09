#ifndef TREE_H
#define TREE_H

/* generic node structure */
typedef struct node
{
    void *data;
    struct node *left;
    struct node *right;
} node_t;

/*
   Callbacks needed by tree struct
 */
typedef int (*compare_cb) (node_t *n1, node_t *n2);
typedef void (*traverse_cb) (node_t *node);
typedef void (*free_cb) (void* data);

/* tree definition */
typedef struct tree
{
    node_t *head;
    compare_cb compare;
    traverse_cb traverse;
    free_cb free;
} tree_t;


/* 
   Initializ the tree with a NULL head and callbacks
 */
void tree_init(tree_t *tree);

/*
   Setters for compare and traverse callbacks
   
   Compare will be called on 2 node objects.

   Traverse will be called on a single node, and traversal will be completed
   from that node. Call on tree->head to traverse entire tree.

   Free will be called with the (void*) argument from the data of the node
 */
void set_compare_cb(tree_t *tree, compare_cb cb);
void set_traverse_cb(tree_t *tree, traverse_cb cb);
void set_free_cb(tree_t *tree, free_cb cb);

/*
   Allocate and initialize a new node wtih some data
 */
node_t *tree_node_create_with_data(void *data);

/*
   Free an allocated node
 */
void tree_free_node(tree_t *tree, node_t **node);

/*
   Insert a node into tree
 */
int tree_insert_node(tree_t *tree, node_t *new_node);

/* 
   Traverse tree, visiting every node and executing the traverse_cb on data at
   each node
 */
int tree_traverse_and_execute(tree_t *tree);

#endif
