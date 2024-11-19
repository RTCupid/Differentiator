#include <stdio.h>
#include <stdlib.h>

#include "Differentiator.h"

node_t* NewNode (size_t type, int value, node_t* left, node_t* right)
{
    node_t* node = (node_t*) calloc (1, sizeof (*node));

    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}
