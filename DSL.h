#ifndef DSL_H
    #define DSL_H

    #define _VAR        NewNode(VAR,     x, NULL, NULL)
    #define _NUM(value) NewNode(NUM, value, NULL, NULL)

    #define _ADD(L,R)   NewNode(OP, ADD, L, R)
    #define _SUB(L,R)   NewNode(OP, SUB, L, R)
    #define _MUL(L,R)   NewNode(OP, MUL, L, R)
    #define _DIV(L,R)   NewNode(OP, DIV, L, R)
    #define _SIN(L,R)   NewNode(OP, SIN, L, R)
    #define _COS(L,R)   NewNode(OP, COS, L, R)
    #define  _LN(L,R)   NewNode(OP, LN,  L, R)
    #define _DEG(L,R)   NewNode(OP, DEG, L, R)
#endif
