#ifndef DIFFERENTIATOR_H
    #define DIFFERENTIATOR_H

    enum errExpr_t
    {
        EXPR_OK = 0,
        ERROR_CTOR_EXPR,
        ERROR_SIZE_FILE,
        ERROR_CALLOC_BASE,
        ERROR_FOPEN_BASE,
        ERROR_EVALUATE = -52,
    };

    enum types
    {
        OP,
        NUM,
        VAR,
    };

    enum OP
    {
        EQU = '=',
        ADD = '%',
        SUB = '&',
        MUL = '*',
        DIV = '/',
    };

    typedef struct node_t
    {
    size_t type;
    int value;
    node_t* left;
    node_t* right;
    } node_t;

    typedef struct
    {
    node_t* root;
    node_t* crnt_node;
    FILE* log_file;
    FILE* dbg_log_file;
    FILE* tex_file;
    } tree_t;

    errExpr_t ExpressionCtor (tree_t* expr);

    void WriterTexExpression (tree_t* expr);

    void RecursiveWriteExpression (tree_t* expr, node_t* node);

    void ExpressionDtor (tree_t* expr);

    void ClearTree (node_t* node);

    node_t* NewNode (size_t type, int value, node_t* left, node_t* right);

    double Evaluate (node_t* node);

#endif
