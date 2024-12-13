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

    printf ("expression : <%s>\n", expr.data);

    DiffDump (&expr, EXPR);
    DifferentiatorGraphviz (&expr, EXPR);

    expr.diff = Differentiator (&expr, expr.root);
    printf (GRN "differentiator completed\n" RESET);

    /*---Before-Simplify---*/
    DifferentiatorGraphviz (&expr, DIFF);

    expr.diff = SimplifyExpr (&expr, expr.diff);

    /*---After-Simplify---*/
    DifferentiatorGraphviz (&expr, DIFF);

    DiffDump (&expr, DIFF);

    fprintf (expr.tex_file, "Упростив выражение получаем ответ:\n");
    WriteExprAndDifferential (&expr, expr.root, expr.diff, ANSW);

    ExpressionDtor (&expr);
    printf ("End Programm\n");
    return 0;
}
