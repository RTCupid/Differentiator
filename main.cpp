#include <stdio.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"
#include "ReadExpression.h"

#define _NUM(val) NewNode(NUM, (val), NULL, NULL)
#define _X        NewNode(VAR, 'X', NULL, NULL)

int main ()
{
    printf ("Start programm\n");

    printf ("%% - plus\n& - minus\n");

    tree_t expr = {};
    ExpressionCtor (&expr);

    printf ("Answer = %lf\n", Evaluate (expr.root));

    DifferentiatorGraphviz (&expr);
    ExpressionDtor (&expr);
    printf ("End Programm\n");
    return 0;
}
