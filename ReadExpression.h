#ifndef READEXPRESSION_H
    #define READEXPRESSION_H

    enum errExpr_t;

    struct node_t;

    errExpr_t MakeExpression (tree_t* expr, const char* namefile);

    node_t* RunExpression (tree_t* expr, FILE* base_file);

    size_t NodeType (tree_t* expr, int value);

#endif
