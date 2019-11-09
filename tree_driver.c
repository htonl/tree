#include "tree.h"
#include <stdio.h>
#include <string.h>

static int test_compare(node_t *n1, node_t *n2)
{
    int v1 = *(int*)(n1->data);
    int v2 = *(int*)(n2->data);

    return v1 - v2;
}

static void test_traverse(node_t *node)
{
    printf("node->data = %d\n", *(int*)(node->data));
}

static void test_free(void* data)
{
    printf("freed: %d\n", *(int*)data);
}

/*
   Short test program to test the tree object
 */
int main(void)
{
    printf("in main\n");

    int a, b, c, d, e, f;
    a = 2;
    b = 1;
    c = 4;
    d = 5;
    e = 6;
    f = 3;

    tree_t test_tree;
    node_t *n1, *n2, *n3, *n4, *n5, *n6;

    printf("about to tree_init\n");
    tree_init(&test_tree);

    tree_set_compare_cb(&test_tree, test_compare);
    tree_set_traverse_cb(&test_tree, test_traverse);
    tree_set_free_cb(&test_tree, test_free);

    n1 = tree_node_create_with_data((void*)&a);
    n2 = tree_node_create_with_data((void*)&b);
    n3 = tree_node_create_with_data((void*)&c);
    n4 = tree_node_create_with_data((void*)&d);
    n5 = tree_node_create_with_data((void*)&e);
    n6 = tree_node_create_with_data((void*)&f);

    tree_traverse_and_execute(&test_tree);
    tree_insert_node(&test_tree, n1);
    tree_insert_node(&test_tree, n2);
    tree_insert_node(&test_tree, n3);
    tree_insert_node(&test_tree, n4);
    tree_insert_node(&test_tree, n5);
    tree_insert_node(&test_tree, n6);

    tree_traverse_and_execute(&test_tree);
    tree_remove_node(&test_tree, n3);
    tree_traverse_and_execute(&test_tree);
    tree_remove_node(&test_tree, n2);
    tree_traverse_and_execute(&test_tree);
    tree_remove_node(&test_tree, n1);
    tree_traverse_and_execute(&test_tree);
    tree_remove_node(&test_tree, n4);
    tree_traverse_and_execute(&test_tree);
    tree_remove_node(&test_tree, n5);
    tree_traverse_and_execute(&test_tree);
    tree_remove_node(&test_tree, n6);
    tree_traverse_and_execute(&test_tree);

    return 0;
}

