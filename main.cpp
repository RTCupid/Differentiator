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
            "ADD = '%%'  = <%d>\n"
            "SUB = '&'  = <%d>\n"
            "MUL = '*'  = <%d>\n"
            "DIV = '/'  = <%d>\n"
            "    = '('  = <%d>\n"
            "    = ')'  = <%d>\n"
            "    = 'x'  = <%d>\n", '=', '%', '&', '*', '/', '(', ')', 'x');

    tree_t expr = {};
    ExpressionCtor (&expr);

    printf ("expression : <%s>\n", expr.data);
    //printf ("Answer = %lf\n", Evaluate (expr.root));

    DifferentiatorGraphviz (&expr);

    //DiffDump (&expr);

    //WriterTexExpression (&expr);

    ExpressionDtor (&expr);
    printf ("End Programm\n");
    return 0;
}
