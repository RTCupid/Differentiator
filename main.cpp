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

    printf ("%% - plus\n& - minus");

    tree_t expr = {};
    ExpressionCtor (&expr);

    printf ("\nvalue DIV = %d\n\n", '/');
    printf ("\nvalue ADD = %d\n\n", '+');

    // expr.root = NewNode (OP, DIV, NewNode (OP, ADD, _NUM(30), _NUM(50)),
    //                                       NewNode (OP, SUB, _NUM(12), _X));

    MakeExpression (&expr, "Expression.txt");

    DifferentiatorGraphviz (&expr);
    ExpressionDtor (&expr);
    printf ("End Programm\n");
    return 0;
}
