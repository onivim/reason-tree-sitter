/*
     Node.re

     Stubs for bindings to the `TSTree` object
 */

type t;

external toString: t => string = "rets_node_string";

external getChildCount: t => int = "rets_node_child_count";
external getChild: (t, int) => t = "rets_node_child";
external getParent: t => t = "rets_node_parent";

external getNamedChildCount: t => int = "rets_node_named_child_count";
external getNamedChild: (t, int) => t = "rets_node_named_child";

external hasChanges: t => bool = "rets_node_has_changes";
external hasError: t => bool = "rets_node_has_error";

external isMissing: t => bool = "rets_node_is_missing";
external isNull: t => bool = "rets_node_is_null";
external isNamed: t => bool = "rets_node_is_named";
external isExtra: t => bool = "rets_node_is_extra";

external getSymbol: t => int = "rets_node_symbol";
external getType: t => string = "rets_node_type";
