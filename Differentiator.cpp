#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"

static double GLOBALX = 0;

errExpr_t ExpressionCtor (tree_t* expr)
{
    expr->dbg_log_file = fopen ("dbg_log_file.txt", "wt");
    VerifyOpenFile (expr->dbg_log_file, "AkinatorCtor");

    expr->log_file = fopen ("log_file.htm", "wt");
    VerifyOpenFile (expr->log_file, "AkinatorCtor");

    expr->crnt_node = NULL;

    InputExpression (expr);

    MakeExpression (expr, "Expression.txt");

    printf ("Finish expression construction \n");
    printf ("expr->root = %p\n", expr->root);

    return EXPR_OK;
}

node_t* Differentiator (tree_t* expr, node_t* node)
{
    if (node->type == NUM)
    {
        return NewNode (NUM, 0, NULL, NULL);
    }
    if (node->type == VAR)
    {
        return NewNode (NUM, 1, NULL, NULL);
    }
    if (node->type == OP)
    {
        if (node->value == ADD)
        {
            return NewNode (OP, ADD, Differentiator (expr, node->left), Differentiator (expr, node->right));
        }
        if (node->value == SUB)
        {
            return NewNode (OP, SUB, Differentiator (expr, node->left), Differentiator (expr, node->right));
        }
        if (node->value == MUL)
        {
            return NewNode (OP, ADD,
                            NewNode (OP, MUL, Differentiator (expr, node->left), Copy (node->right)),
                            NewNode (OP, MUL, Copy (node->left), Differentiator (expr, node->right)));
        }
        if (node->value == DIV)
        {
            return NewNode (OP, DIV,
                        NewNode (OP, SUB,
                            NewNode (OP, MUL, Differentiator (expr, node->left), Copy (node->right)),
                            NewNode (OP, MUL, Copy (node->left), Differentiator (expr, node->right))),
                        NewNode (OP, MUL, Copy (node->right), Copy (node->right)));
        }
    }
    fprintf (expr->dbg_log_file, "ERROR: unknown type\n");
    return NULL;
}

node_t* Copy (node_t* old_node)
{
    if (old_node == NULL)
    {
        return NULL;
    }
    return NewNode (old_node->type, old_node->value, Copy (old_node->left), Copy (old_node->right));
}

node_t* SimplifyExpr (tree_t* expr, node_t* node)
{
    if (node->type == NUM)
    {
        printf ("node_value = %f\n", node->value);
    }
    else
    {
        printf ("node_value = %c\n", (int)node->value);
    }

    if (node->left == NULL)
    {
        return node;
    }

    if (!IsNotConstExpression (expr, node))
    {
        node_t* new_node = NewNode (NUM, Evaluate (node), NULL, NULL);
        ClearTree (node);
        return new_node;
    }

    //left
    node->left  = SimplifyExpr (expr, node->left );
    //right
    node->right =  SimplifyExpr (expr, node->right);
    return node;
}

bool IsNotConstExpression (tree_t* expr, node_t* crnt_node)
{
    if (!crnt_node->left)
    {
        if (crnt_node->value == 'x')
            return true;
        else
            return false;
    }

    if (IsNotConstExpression (expr, crnt_node->left))
    {
        return true;
    }

    if (IsNotConstExpression (expr, crnt_node->right))
    {
        return true;
    }
    return false;
}

void WriterTexExpression (tree_t* expr)
{
    expr->tex_file = fopen ("Expression.tex", "wt");
    fprintf (expr->tex_file, "\\documentclass[a4paper, 12 pt]{article}\n\n"
                             "\\usepackage[english, russian]{babel}\n"
                             "\\usepackage[T2A]{fontenc}\n"
                             "\\usepackage[utf8]{inputenc}\n\n");
    fprintf (expr->tex_file, "\\begin{document}\n");
    //Expression
    fprintf (expr->tex_file, "\t$\\left(");

    RecursiveWriteExpression (expr, expr->root);

    fprintf (expr->tex_file, "\\right)'$\n");

    //Differential
    fprintf (expr->tex_file, "\t$= ");

    RecursiveWriteExpression (expr, expr->diff);

    fprintf (expr->tex_file, "$\n");

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

            /*...left...*/
            RecursiveWriteExpression (expr, node->left);
            fprintf (expr->tex_file, "}{");
            fprintf (expr->dbg_log_file, "}{");

            /*...right...*/
            RecursiveWriteExpression (expr, node->right);
            fprintf (expr->tex_file, "}");
            fprintf (expr->dbg_log_file, "}");
        }
        else if (node->value == MUL)
        {
            /*...left...*/
            RecursiveWriteExpression (expr, node->left);

            fprintf (expr->tex_file, "%c", (int)node->value);
            fprintf (expr->dbg_log_file, "%c", (int)node->value);

            /*...right...*/
            RecursiveWriteExpression (expr, node->right);
        }
        else
        {
            /*...left...*/
            fprintf (expr->tex_file, "(");
            RecursiveWriteExpression (expr, node->left);

            fprintf (expr->tex_file, "%c", (int)node->value);
            fprintf (expr->dbg_log_file, "%c", (int)node->value);

            /*...right...*/
            RecursiveWriteExpression (expr, node->right);
            fprintf (expr->tex_file, ")");
        }
    }
    else if (node->type == NUM)
    {
        fprintf (expr->tex_file, "%f", node->value);
        fprintf (expr->dbg_log_file, "%f", node->value);
        return;
    }
    else if (node->type == VAR)
    {
        fprintf (expr->tex_file, "%c", (int)node->value);
        fprintf (expr->dbg_log_file, "%c", (int)node->value);
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
        if (node->value == ADD)
        {
            return Evaluate (node->left) + Evaluate (node->right);
        }
        if (node->value == SUB)
        {
            return Evaluate (node->left) - Evaluate (node->right);
        }
        if (node->value == MUL)
        {
            return Evaluate (node->left) * Evaluate (node->right);
        }
        if (node->value == DIV)
        {
            return Evaluate (node->left) / Evaluate (node->right);
        }
    }
    return ERROR_EVALUATE;
}

node_t* NewNode (size_t type, double value, node_t* left, node_t* right)
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
    fclose (expr->log_file);

    ClearTree (expr->root);

    ClearTree (expr->diff);

    free (expr->data);
    expr->data = 0;

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

