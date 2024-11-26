#ifndef DIFFDUMP_H
    #define DIFFDUMP_H

//--ret-value---function-------------argument------------------------------------------------------

    void    DifferentiatorGraphviz  (tree_t* expr);

    void    MakeDotFileGraphviz     (tree_t* expr);

    void    PrintGraphviz           (tree_t expr, node_t* node, FILE* dot_file);

    void    PrintNodeDot            (FILE* dot_file, node_t* node);

//-------------------------------------------------------------------------------------------------

#endif
