#include <stdio.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"
#include "colors.h"

#define _NUM(val) NewNode(NUM, (val), NULL, NULL)
#define _X        NewNode(VAR, 'X', NULL, NULL)

int main ()
{
    printf (GRN "Start programm\n" RESET);

    printf ("%% - plus\n& - minus\n");
    printf ("EQU = '='  = <%d>\n"
            "ADD = '+'  = <%d>\n"
            "SUB = '-'  = <%d>\n"
            "MUL = '*'  = <%d>\n"
            "DIV = '/'  = <%d>\n"
            "    = '('  = <%d>\n"
            "    = ')'  = <%d>\n"
            "    = 'x'  = <%d>\n", '=', '%', '&', '*', '/', '(', ')', 'x');

    tree_t expr = {};
    ExpressionCtor (&expr);

    char array[52] = "(((";
    char buffer[52] = {};
    int offset = 0;
    sscanf (array, "%[^(]%n", buffer, &offset);
    printf ("buffer = <%s>\n", buffer);
    printf ("offset = %d\n", offset);

    printf ("expression : <%s>\n", expr.data);

    expr.diff = Differentiator (&expr, expr.root);

    //printf ("Answer = %lf\n", Evaluate (expr.root));

    DifferentiatorGraphviz (&expr, DIFF);

    //DiffDump (&expr);

    WriterTexExpression (&expr);

    ExpressionDtor (&expr);
    printf ("End Programm\n");
    return 0;
}
