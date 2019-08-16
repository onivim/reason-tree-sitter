/*
     Tree.re

     Stubs for bindings to the `TSTree` object
 */

type t;

external getRootNode: t => Node.t = "rets_tree_root_node";

external edit: (t, int, int, int, int, int, int) => t = "rets_tree_edit_bytecode" "rets_tree_edit_native";
