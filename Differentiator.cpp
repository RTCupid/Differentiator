#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"
#include "DSL.h"
#include "colors.h"

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

    expr->tex_file = fopen ("Expression.tex", "wt");
    fprintf (expr->tex_file, "\\documentclass[a4paper, 12 pt]{article}\n\n"
                             "\\usepackage[english, russian]{babel}\n"
                             "\\usepackage[T2A]{fontenc}\n"
                             "\\usepackage[utf8]{inputenc}\n"
                             "\\usepackage{geometry}\n\n");

    fprintf (expr->tex_file, "\\geometry{top=4ex}\n"
                             "\\geometry{bottom=4ex}\n"
                             "\\geometry{left=5em}\n"
                             "\\geometry{right=5em}\n\n");

    fprintf (expr->tex_file, "\\author{Муратов Артём}\n");
    fprintf (expr->tex_file, "\\date{3Б класс}\n");
    fprintf (expr->tex_file, "\\title{Домашняя работа}\n");

    fprintf (expr->tex_file, "\\maketitle\n\n");

    fprintf (expr->tex_file, "№9(1) Вычислить:");
    WriteTexExpression (expr, expr->root, EXPR);
    fprintf (expr->tex_file, "\\]\n");
    fprintf (expr->tex_file, "\n\n");

    fprintf (expr->tex_file, "Решение:\n\n");

    return EXPR_OK;
}

node_t* Differentiator (tree_t* expr, node_t* node)
{
    printf ("Differentiator, node->type = %lu\n", node->type);

    if (node->type == NUM || !IsNotConstExpression (expr, node))
    {
        node_t* node_diff = _NUM(0);

        fprintf (expr->tex_file, "Пока примем без доказательства, что:\n");
        WriteExprAndDifferential (expr, node, node_diff);

        return node_diff;
    }
    if (node->type == VAR)
    {
        node_t* node_diff = _NUM(1);

        fprintf (expr->tex_file, "Очевидно,\n");
        WriteExprAndDifferential (expr, node, node_diff);

        return node_diff;
    }
    if (node->type == OP)
    {
        if (node->value == ADD)
        {
            node_t* node_diff = _ADD(
                                    Differentiator (expr, node->left),
                                    Differentiator (expr, node->right));

            fprintf (expr->tex_file, "На уроке рисования вам докажут, что:\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == SUB)
        {
            node_t* node_diff = _SUB(
                                    Differentiator (expr, node->left),
                                    Differentiator (expr, node->right));

            fprintf (expr->tex_file, "По основной теореме дифференцирования:\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == MUL)
        {
            node_t* node_diff = _ADD(
                                    _MUL(
                                        Differentiator (expr, node->left),
                                        Copy (node->right)),
                                    _MUL(
                                        Copy (node->left),
                                        Differentiator (expr, node->right)));

            fprintf (expr->tex_file, "Заметим, что:\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == DIV)
        {
            node_t* node_diff = _DIV(
                                    _SUB(
                                        _MUL(
                                            Differentiator (expr, node->left),
                                            Copy (node->right)),
                                        _MUL(
                                            Copy (node->left),
                                            Differentiator (expr, node->right))),
                                    _DEG(
                                        Copy (node->right),
                                        _NUM(2)));

            fprintf (expr->tex_file, "Продифференцируем простую функцию и получим:\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == SIN)
        {
            printf (RED "function is sin\n" RESET);
            node_t* node_diff = _MUL(
                                    _COS(
                                        Copy (node->left)),
                                    Differentiator (expr, node->left));

            fprintf (expr->tex_file, "Ещё более очевидно\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == COS)
        {
            node_t* node_diff = _MUL(
                                    _SIN(
                                        Copy (node->left)),
                                    Differentiator (expr, node->left));

            fprintf (expr->tex_file, "Перейдя в комплексное поле и обратно, получим\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == LN)
        {
            node_t* node_diff = _MUL(
                                    _DIV(
                                        _NUM(1),
                                        Copy (node->left)),
                                    Differentiator (expr, node->left));

            fprintf (expr->tex_file, "По табличке из задачника Кудрявцева находим\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
        if (node->value == DEG)
        {
            node_t* node_degree = _MUL(
                                    _LN(
                                        Copy (node->left)),
                                    Copy(node->right));

            node_t* node_diff = _MUL(
                                    Copy (node),
                                    Differentiator (expr, node_degree));

            ClearTree (node_degree);

            fprintf (expr->tex_file, "После простых преобразований:\n");
            WriteExprAndDifferential (expr, node, node_diff);

            return node_diff;
        }
    }
    fprintf (expr->dbg_log_file, "ERROR: unknown type\n");
    printf ("ERROR: unknown type\n");
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

node_t* SimplifyExpr (tree_t* expr, node_t* node) //you (йоу) simplify
{
    int n_change_elems = 1;
    while (n_change_elems != 0)
    {
        n_change_elems = 0;
        node = SimplifyConstExpr (expr, node, &n_change_elems);
        printf ("n_change_elems const expr = %d\n", n_change_elems);
        node = RemoveNeutralExpr (expr, node, &n_change_elems);
    }
    return node;
}

node_t* RemoveNeutralExpr (tree_t* expr, node_t* crnt_node, int* n_change_elems)
{
    if (crnt_node == NULL)
    {
        return NULL;
    }
    if (crnt_node->type == OP)
    {
        if (crnt_node->value == ADD || crnt_node->value == SUB)
        {
            if (crnt_node->left->value == 0)
            {
                *n_change_elems += 1;
                node_t* ret_node = crnt_node->right;

                free (crnt_node->left);
                crnt_node->left = NULL;
                free (crnt_node);
                crnt_node = NULL;

                return ret_node;
            }
            if (crnt_node->right->value == 0)
            {
                *n_change_elems += 1;
                node_t* ret_node = crnt_node->left;

                free (crnt_node->right);
                crnt_node->right = NULL;
                free (crnt_node);
                crnt_node = NULL;

                return ret_node;
            }
        }

        if (crnt_node->value == MUL)
        {
            if (crnt_node->left->value == 1)
            {
                *n_change_elems += 1;
                node_t* ret_node = crnt_node->right;

                free (crnt_node->left);
                crnt_node->left = NULL;
                free (crnt_node);
                crnt_node = NULL;

                return ret_node;
            }
            if (crnt_node->right->value == 1)
            {
                *n_change_elems += 1;
                node_t* ret_node = crnt_node->left;

                free (crnt_node->right);
                crnt_node->right = NULL;
                free (crnt_node);
                crnt_node = NULL;

                return ret_node;
            }
            if (crnt_node->left->value == 0 || crnt_node->right->value == 0)
            {
                ClearTree (crnt_node);
                *n_change_elems += 1;
                return NewNode (NUM, 0, NULL, NULL);
            }
        }

        crnt_node->left  = RemoveNeutralExpr (expr, crnt_node->left, n_change_elems);
        crnt_node->right = RemoveNeutralExpr (expr, crnt_node->right, n_change_elems);
    }
    return crnt_node;
}

node_t* SimplifyConstExpr (tree_t* expr, node_t* node, int* n_change_elems)
{
    if (node->left == NULL || node->right == NULL)
    {
        return node;
    }

    if (!IsNotConstExpression (expr, node))
    {
        node_t* new_node = NewNode (NUM, Evaluate (node), NULL, NULL);
        ClearTree (node);
        *n_change_elems += 1;
        return new_node;
    }

    //left
    node->left  = SimplifyConstExpr (expr, node->left, n_change_elems );
    //right
    node->right = SimplifyConstExpr (expr, node->right, n_change_elems);
    return node;
}

bool IsNotConstExpression (tree_t* expr, node_t* crnt_node)
{
    if (!crnt_node->left && !crnt_node->right)
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

void WriteExprAndDifferential (tree_t* expr, node_t* node_expr, node_t* node_diff)
{
    /*---Expression---*/
    WriteTexExpression (expr, node_expr, EXPR);

    fprintf (expr->tex_file, "\t= ");

    /*---Differential---*/
    WriteTexExpression (expr, node_diff, DIFF);
}

void WriteTexExpression (tree_t* expr, node_t* node, my_mode_t mode)
{
    if (mode == EXPR)
    {
        //Expression
        fprintf (expr->tex_file, "\t\\[\\left(");

        RecursiveWriteExpression (expr, node);

        fprintf (expr->tex_file, "\\right)'\n");
    }
    else if (mode == DIFF)
    {
        //Differential
        RecursiveWriteExpression (expr, node);

        fprintf (expr->tex_file, "\\]\n");
    }
}

void RecursiveWriteExpression (tree_t* expr, node_t* node)
{
    if (node == NULL)
    {
        fprintf (expr->dbg_log_file, "Recursive write expression, node = NULL\n");
        return;
    }
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
        else if (node->value == SIN)
        {
            fprintf (expr->tex_file, "sin");
            fprintf (expr->dbg_log_file, "sin");

            /*...left...*/
            fprintf (expr->tex_file, "{");
            RecursiveWriteExpression (expr, node->left);
            fprintf (expr->tex_file, "}");
        }
        else if (node->value == COS)
        {
            fprintf (expr->tex_file, "cos");
            fprintf (expr->dbg_log_file, "cos");

            /*...left...*/
            fprintf (expr->tex_file, "{");
            RecursiveWriteExpression (expr, node->left);
            fprintf (expr->tex_file, "}");
        }
        else if (node->value == DEG)
        {
            /*...left...*/
            fprintf (expr->tex_file, "(");
            RecursiveWriteExpression (expr, node->left);

            fprintf (expr->tex_file, "%c", (int)node->value);
            fprintf (expr->dbg_log_file, "%c", (int)node->value);

            /*...right...*/
            fprintf (expr->tex_file, "{");
            RecursiveWriteExpression (expr, node->right);
            fprintf (expr->tex_file, "}");
            fprintf (expr->tex_file, ")");
        }
        else if (node->value == LN)
        {
            fprintf (expr->tex_file, "ln");
            fprintf (expr->dbg_log_file, "ln");

            /*...left...*/
            fprintf (expr->tex_file, "{");
            RecursiveWriteExpression (expr, node->left);
            fprintf (expr->tex_file, "}");
        }
        else if (node->value == MUL)
        {
            /*...left...*/
            fprintf (expr->tex_file, "(");
            RecursiveWriteExpression (expr, node->left);

            fprintf (expr->tex_file, "{\\cdot}");
            fprintf (expr->dbg_log_file, "%c", (int)node->value);

            /*...right...*/
            RecursiveWriteExpression (expr, node->right);
            fprintf (expr->tex_file, ")");
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
        fprintf (expr->tex_file, "\\left(%g\\right)", node->value);
        fprintf (expr->dbg_log_file, "(%g)", node->value);
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
        if (node->value == DEG)
        {
            return pow (Evaluate (node->left), Evaluate (node->right));
        }
        if (node->value == SIN)
        {
            return sin (Evaluate (node->left));
        }
        if (node->value == COS)
        {
            return cos (Evaluate (node->left));
        }
        if (node->value == LN)
        {
            return log (Evaluate (node->left));
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

    fprintf (expr->tex_file, "\n\\end{document}\n");
    fclose (expr->tex_file);

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

