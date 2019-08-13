/*
     Node.re

     Stubs for bindings to the `TSTree` object
 */

type t;

external toString: t => string = "rets_node_string";

external getChildCount: t => int = "rets_node_child_count";
external getChild: (t, int) => t = "rets_node_child";

external getNamedChildCount: t => int = "rets_node_named_child_count";
external getNamedChild: (t, int) => t = "rets_node_named_child";

external getType: t => string = "rets_node_type";
