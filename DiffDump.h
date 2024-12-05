#ifndef DIFFDUMP_H
    #define DIFFDUMP_H

//--ret-value---function-------------argument------------------------------------------------------

    void    DifferentiatorGraphviz  (tree_t* expr, my_mode_t mode);

    void    MakeDotFileGraphviz     (tree_t* expr, my_mode_t mode);

    void    PrintGraphviz           (tree_t expr, node_t* node, FILE* dot_file);

    void    PrintNodeDot            (FILE* dot_file, node_t* node);

    void    DiffDump                (tree_t* tree, my_mode_t mode);

    void    MakeDotFileDump         (tree_t* tree, my_mode_t mode);

    void    PrintDump               (tree_t tree, node_t* node, FILE* dot_file);

    void    PrintNodeDumpDot        (FILE* dot_file, node_t* node);

//-------------------------------------------------------------------------------------------------

#endif
