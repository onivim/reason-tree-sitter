/*
     Node.re

     Stubs for bindings to the `TSTree` object
 */

type t;

type point = {
  row: int,
  column: int,
};

external toString: t => string = "rets_node_string";

external getChildCount: t => int = "rets_node_child_count";
external getChild: (t, int) => t = "rets_node_child";
external getParent: t => t = "rets_node_parent";

external getNamedChildCount: t => int = "rets_node_named_child_count";
external getNamedChild: (t, int) => t = "rets_node_named_child";

external getNextSibling: t => int = "rets_node_next_sibling";
external getPrevSibling: t => int = "rets_node_prev_sibling";

external getDescendantForPointRange: (t, int, int, int, int) => t =
  "rets_node_descendant_for_point_range";

external getStartByte: t => int = "rets_node_start_byte";
external getEndByte: t => int = "rets_node_end_byte";

external getStartPoint: t => point = "rets_node_start_point";
external getEndPoint: t => point = "rets_node_end_point";

external hasChanges: t => bool = "rets_node_has_changes";
external hasError: t => bool = "rets_node_has_error";

external isMissing: t => bool = "rets_node_is_missing";
external isNull: t => bool = "rets_node_is_null";
external isNamed: t => bool = "rets_node_is_named";
external isError: t => bool = "rets_node_is_error";
external isExtra: t => bool = "rets_node_is_extra";

external getSymbol: t => int = "rets_node_symbol";
external getType: t => string = "rets_node_type";
