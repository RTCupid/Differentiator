#include <stdio.h>
#include <stdlib.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"

static double GLOBALX = 0;

errExpr_t ExpressionCtor (tree_t* expr)
{
    expr->dbg_log_file = fopen ("dbg_log_file.txt", "wt");
    VerifyOpenFile (expr->dbg_log_file, "AkinatorCtor");

    expr->crnt_node = NULL;

    MakeExpression (expr, "Expression.txt");

    return EXPR_OK;
}

void WriterTexExpression (tree_t* expr)
{
    expr->tex_file = fopen ("Expression.tex", "wt");
    fprintf (expr->tex_file, "\\documentclass[a4paper, 12 pt]{article}\n\n"
                             "\\usepackage[english, russian]{babel}\n"
                             "\\usepackage[T2A]{fontenc}\n"
                             "\\usepackage[utf8]{inputenc}\n\n");
    fprintf (expr->tex_file, "\\begin{document}\n\t");
    fprintf (expr->tex_file, "\\[");

    RecursiveWriteExpression (expr, expr->root);

    fprintf (expr->tex_file, "\\]");
    fprintf (expr->tex_file, "\n\\end{document}\n");
    fclose (expr->tex_file);
}

void RecursiveWriteExpression (tree_t* expr, node_t* node)
{
    if (node->type == OP)
    {
        if (node->value == DIV)
        {
            fprintf (expr->tex_file, "\\frac{");
            fprintf (expr->dbg_log_file, "\\frac{");

            RecursiveWriteExpression (expr, node->left);
            fprintf (expr->tex_file, "}{");
            fprintf (expr->dbg_log_file, "}{");

            RecursiveWriteExpression (expr, node->right);
            fprintf (expr->tex_file, "}");
            fprintf (expr->dbg_log_file, "}");
        }
        else
        {
            /*...left...*/
            RecursiveWriteExpression (expr, node->left);

            fprintf (expr->tex_file, "\\%c", node->value);
            fprintf (expr->dbg_log_file, "%c", node->value);

            /*...right...*/
            RecursiveWriteExpression (expr, node->right);
        }
    }
    else if (node->type == NUM)
    {
        fprintf (expr->tex_file, "%d", node->value);
        fprintf (expr->dbg_log_file, "%d", node->value);
        return;
    }
    else if (node->type == VAR)
    {
        fprintf (expr->tex_file, "%c", node->value);
        fprintf (expr->dbg_log_file, "%c", node->value);
        return;
    }
    else
    {
        fprintf (expr->dbg_log_file, "\nERROR: unknown type of argument\n\n");
    }
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
            case ADD:
            {
                return Evaluate (node->left) + Evaluate (node->right);
            }
            case SUB:
            {
                return Evaluate (node->left) - Evaluate (node->right);
            }
            case MUL:
            {
                return Evaluate (node->left) * Evaluate (node->right);
            }
            case DIV:
            {
                return Evaluate (node->left) / Evaluate (node->right);
            }
            default:
            {
                printf ("ERROR: unknown operation %d = <%c>\n", node->value, node->value);
            }
        }
    }
    return ERROR_EVALUATE;
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

