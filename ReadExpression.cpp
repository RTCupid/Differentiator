#include <stdio.h>
#include <stdlib.h>

#include "Differentiator.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"

errExpr_t MakeExpression (tree_t* expr, const char* namefile)
{
    FILE* base_file = fopen (namefile, "rt");
    VerifyOpenFile (base_file, "MakeExpression");

    expr->root = RunExpression (expr, base_file);

    expr->crnt_node = expr->root;

    fclose (base_file);
    return EXPR_OK;
}

node_t* RunExpression (tree_t* expr, FILE* base_file)
{
    char symbol = '\0';
    fscanf (base_file, " %c", &symbol);
    fprintf (expr->dbg_log_file, "start symbol = <%c>\n", symbol);

    if (symbol == '(')
    {
        int value = 0;
        if (!fscanf (base_file, "%d", &value))
        {
            fprintf (expr->dbg_log_file, "value is not a num\n");

            fscanf (base_file, " %c", (char*)&value);
            fprintf (expr->dbg_log_file, "value = <%c>\n\n", (char)value);
        }
        fprintf (expr->dbg_log_file, "value = <%c>\n\n", value);

        size_t type = NodeType (expr, value);
        fprintf (expr->dbg_log_file, "type = %lu\n", type);

        node_t* node = NewNode (type, value , NULL, NULL);

        fscanf (base_file, " %c", &symbol);
        fprintf (expr->dbg_log_file, "end symbol = <%c>\n", symbol);

        if (symbol == ')')
        {
            fprintf (expr->dbg_log_file, "return\n");
            return node;
        }
        else if (symbol == '(')
        {
            ungetc (symbol, base_file);
            /*......LEFT......*/
            node->left  = RunExpression (expr, base_file);
            /*......RIGHT.....*/
            node->right = RunExpression (expr, base_file);
        }
        fscanf (base_file, " %c", &symbol);
        fprintf (expr->dbg_log_file, "end symbol = <%c>\n", symbol);

        if (symbol == ')')
        {
            return node;
        }
        else
        {
            fprintf (expr->dbg_log_file, "ERROR: unknown symbol \" not a ( or )\"\n");
        }
    }
    fprintf (expr->dbg_log_file, "ERROR: uncorrect file of base\n");
    return NULL;
}

size_t NodeType (tree_t* expr, int value)
 {
    fprintf (expr->dbg_log_file, "In NodeType value = %d\n", value);
    if (value == EQU || value == ADD || value == SUB || value == MUL || value == DIV)
        return OP;
    if (value == 'x')
        return VAR;
    else
        return NUM;
 }
