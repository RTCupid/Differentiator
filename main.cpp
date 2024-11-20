#include <stdio.h>

#include "Differentiator.h"
#include "DiffDump.h"
#include "DiffVerificator.h"

#define _NUM(val) NewNode(NUM, (val), NULL, NULL)
#define _X        NewNode(VAR, 'X', NULL, NULL)

int main ()
{
    tree_t expr = {};
    ExpressionCtor (&expr);

    expr.root = NewNode (OP, DIV, NewNode (OP, ADD, _NUM(30), _NUM(50)),
                                          NewNode (OP, SUB, _NUM(12), _X));

    DifferentiatorGraphviz (&expr);
    ExpressionDtor (&expr);
    return 0;
}
