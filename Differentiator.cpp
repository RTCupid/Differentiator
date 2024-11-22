#include <stdio.h>
#include <stdlib.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"

static GLOBALX = 0;

errExpr_t ExpressionCtor (tree_t* expr)
{
    expr->dbg_log_file = fopen ("dbg_log_file.txt", "wt");
    VerifyOpenFile (expr->dbg_log_file, "AkinatorCtor");

    expr->crnt_node = NULL;

    return EXPR_OK;
}

double Evaluate (node_t* node)
{
    if (node->type == NUM)
        return node->value;
    if (node->type == VAR)
        return GLOBALX;
    if (node->type == OP)
    {
        switch (node->value)
        {
            case ADD: return Evaluate (node->left) + Evaluate (node->right);

            case SUB: return Evaluate (node->left) - Evaluate (node->right);

            case MUL: return Evaluate (node->left) * Evaluate (node->right);

            case DIV: return Evaluate (node->left) / Evaluate (node->right);

            case EQU:
        }
    }


}

node_t* NewNode (size_t type, int value, node_t* left, node_t* right)
{
    node_t* node = (node_t*) calloc (1, sizeof (*node));

    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

void ExpressionDtor (tree_t* expr)
{
    fclose (expr->dbg_log_file);

    ClearTree (expr->root);

    expr->crnt_node = NULL;
}

void ClearTree (node_t* node)
{
    if (!node)
    {
        return;
    }
    if (node->left)
    {
        ClearTree (node->left);
    }
    if (node->right)
    {
        ClearTree (node->right);
    }

    free (node);
    node = NULL;
}

