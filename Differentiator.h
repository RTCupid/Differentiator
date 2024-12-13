#ifndef DIFFERENTIATOR_H
    #define DIFFERENTIATOR_H

    const size_t MAX_LEN_BUF = 64;

    enum errExpr_t
    {
        EXPR_OK = 0,
        ERROR_CTOR_EXPR,
        ERROR_SIZE_FILE,
        ERROR_CALLOC_BASE,
        ERROR_FOPEN_BASE,
        ERROR_EVALUATE = -52,
    };

    enum my_mode2_t
    {
        EVAL,
        ANSW,
    };

    enum my_mode_t
    {
        EXPR,
        DIFF,
    };

    enum types_t
    {
        OP,
        NUM,
        VAR,
    };

    enum OP_t
    {
        EQU = '=',
        ADD = '+',
        SUB = '-',
        MUL = '*',
        DIV = '/',
        DEG = '^',
        SIN = 's',
        COS = 'c',
         LN = 'l',
    };

    typedef struct node_t
    {
    size_t type;
    double value;
    node_t* left;
    node_t* right;
    } node_t;

    typedef struct
    {
    node_t* root;
    node_t* crnt_node;
    char* data;
    int size_data;
    FILE* log_file;
    FILE* dbg_log_file;
    FILE* tex_file;
    node_t* diff;
    } tree_t;

//--ret-value----function--------------------argument-------------------------------------------------------------

    errExpr_t   ExpressionCtor              (tree_t* expr);

    node_t*     Differentiator              (tree_t* expr, node_t* node);

    node_t*     SimplifyExpr                (tree_t* expr, node_t* node);

    node_t*     SimplifyConstExpr           (tree_t* expr, node_t* node, int* n_change_elems);

    node_t*     RemoveNeutralExpr           (tree_t* expr, node_t* node, int* n_change_elems);

    bool        IsNotConstExpression        (tree_t* expr, node_t* node);

    node_t*     Copy                        (node_t* old_node);

    void        WriteExprAndDifferential    (tree_t* expr, node_t* node_expr, node_t* node_diff, my_mode2_t mode);

    void        WriteTexExpression         (tree_t* expr, node_t* node, my_mode_t mode);

    void        RecursiveWriteExpression    (tree_t* expr, node_t* node);

    void        ExpressionDtor              (tree_t* expr);

    void        ClearTree                   (node_t* node);

    node_t*     NewNode                     (size_t type, double value, node_t* left, node_t* right);

    double      Evaluate                    (node_t* node);

    double      Exponent                    (double num, int degree);

//------------------------------------------------------------------------------------------------------------

#endif
