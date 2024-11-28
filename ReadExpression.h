#ifndef READEXPRESSION_H
    #define READEXPRESSION_H

    enum errExpr_t;

    struct node_t;

    errExpr_t   MakeExpression  (tree_t* expr, const char* namefile);

    node_t*     RunExpression   (tree_t* expr);

    void        InputExpression (tree_t* expr);

    void        PrintExprDump (tree_t* expr, int ID);

    size_t      NodeType        (tree_t* expr, int value);

#endif
