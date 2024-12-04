#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "Differentiator.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"
#include "colors.h"

errExpr_t MakeExpression (tree_t* expr, const char* namefile)
{
    FILE* base_file = fopen (namefile, "rt");
    VerifyOpenFile (base_file, "MakeExpression");

    expr->root = RunExpression (expr);

    expr->crnt_node = expr->root;

    fclose (base_file);
    return EXPR_OK;
}

node_t* RunExpression (tree_t* expr)
{
    char bracket = '\0';
    static int ID = 0;

    int offset = 0;
    PrintExprDump (expr, ID);
    offset = 0;
    sscanf (expr->data + ID, "%c%n", &bracket, &offset);
    ID += offset;
    fprintf (expr->dbg_log_file, "first bracket = <%c>\n", bracket);
    fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

    if (bracket == '(' )
    {
        char buffer[MAX_LEN_BUF] = {};

        PrintExprDump (expr, ID);
        offset = 0;
        int nsymbols = sscanf (expr->data + ID, "%[^()]%n", buffer, &offset);
        fprintf (expr->dbg_log_file, "nsymbols = %d\n", nsymbols);
        fprintf (expr->dbg_log_file, "offset = %d\n", offset);
        ID += offset;
        fprintf (expr->dbg_log_file, "<%s>\n", buffer);
        fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

        /*---LEFT-ARGUMENT---*/
        PrintExprDump (expr, ID);
        offset = 0;
        sscanf (expr->data + ID, "%c%n", &bracket, &offset);
        ID += offset;
        fprintf (expr->dbg_log_file, "second bracket in func = <%c>\n", bracket);
        fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

        node_t* new_left = NULL;
        node_t* new_right = NULL;
        if (bracket == '(')
        {
            ID--;
            fprintf (expr->dbg_log_file, "ID -> %d\n", ID);


            new_left = RunExpression (expr);

            char operation = '\0';
            PrintExprDump (expr, ID);
            offset = 0;
            sscanf (expr->data + ID, "%c%n", &operation, &offset);
            ID += offset;
            fprintf (expr->dbg_log_file, "main value = <%c>\n", operation);
            fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

            if (operation == ')')
            {
                size_t type = NodeType (expr, buffer[0]);
                return NewNode (type, buffer[0], new_left, new_right);
            }

            size_t type = NodeType (expr, operation);

            /*---RIGHT-ARGUMENT---*/
            PrintExprDump (expr, ID);
            offset = 0;
            sscanf (expr->data + ID, "%c%n", &bracket, &offset);
            ID += offset;
            fprintf (expr->dbg_log_file, "third bracket in func = <%c>\n", bracket);
            fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

            if (bracket == '(')
            {
                //ungetc ('(', base_file);
                ID--;
                fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

                new_right = RunExpression (expr);
                offset = 0;
                sscanf (expr->data + ID, "%c%n", &bracket, &offset);
                ID += offset;
            }

            PrintExprDump (expr, ID);
            fprintf (expr->dbg_log_file, "end bracket = <%c>\n", bracket);
            fprintf (expr->dbg_log_file, "ID -> %d\n", ID);

            if (bracket == ')')
            {
                return NewNode (type, operation, new_left, new_right);
            }
        }

        if (bracket == ')')
        {
            int value = 0;
            if (isalpha(buffer[0]))
            {
                value = 'x';
            }
            else
            {
                value = atoi (buffer);
            }
            size_t type = NodeType (expr, value);

            fprintf (expr->dbg_log_file, "return\n\n");
            return NewNode (type, value, NULL, NULL);
        }
        else
        {
            fprintf (expr->dbg_log_file, "ERROR: recursive function return null\n");
            return NULL;
        }
    }
    else
    {
        fprintf (expr->dbg_log_file, "ERROR: recursive function return null\n");
        return NULL;
    }
}

void PrintExprDump (tree_t* expr, int ID)
{
    fprintf (expr->dbg_log_file, "\nReader:\n");
    fprintf (expr->dbg_log_file, "%s", expr->data);
    for (int i = 0; i < expr->size_data; i++)
    {
        if (i == ID)
        {
            fprintf (expr->dbg_log_file, "^");
        }
        else
        {
            fprintf (expr->dbg_log_file, "_");
        }
    }
    fprintf (expr->dbg_log_file, "\n");
    for (int i = 0; i < 10; i++)
    {
        fprintf (expr->dbg_log_file, "%d", i);

    }
    fprintf (expr->dbg_log_file, "\n\n");
    fprintf (expr->dbg_log_file, "scanf:\n");
}


void InputExpression (tree_t* expr)
{
    struct stat fileInf = {};

    int err = stat ("Expression.txt", &fileInf);
    if (err != 0)
    {
        fprintf(expr->dbg_log_file, "Stat err %d\n", err);
    }

    fprintf (expr->dbg_log_file, "\n%lu\n", (size_t)fileInf.st_size);
    fprintf (expr->dbg_log_file, "count of char = %lu\n", (size_t)fileInf.st_size / sizeof (char));

    char* expression = (char*)calloc ((size_t)fileInf.st_size + 1, sizeof(char));      // каллочу буффер, чтобы в него считать текст

    FILE* expr_file = fopen ("Expression.txt", "rt");

    if (expr_file == NULL)
    {
        fprintf (expr->dbg_log_file, "File opening error\n");
        fprintf (expr->dbg_log_file, "errno = <%d>\n", errno);
        perror("Expression.txt\n");
    }

    expr->size_data = (int) fread (expression, sizeof (char), (size_t)fileInf.st_size, expr_file); // с помощью fread читаю файл в буффер, сохраняю возвращаемое значение fread ()

    if (expr->size_data == 0)
    {
        fprintf (expr->dbg_log_file, "errno = <%d>\n", errno);
        perror ("Expression.txt");
    }

    fprintf (expr->dbg_log_file, "\n%s\n", expression);                    // вывожу вид выражения

    fclose (expr_file);                                                   // закрываю файл

    fprintf (expr->dbg_log_file, "sizeOfFile = <%d>\n\n", expr->size_data);

    expr->data = expression;
}

size_t NodeType (tree_t* expr, int value)
 {
    fprintf (expr->dbg_log_file, "In NodeType value = %d\n", value);
    if (value == EQU || value == ADD || value == SUB || value == MUL || value == DIV || value == DEG || value == SIN || value == COS)
        return OP;
    if (value == 'x')
        return VAR;
    else
        return NUM;
 }
