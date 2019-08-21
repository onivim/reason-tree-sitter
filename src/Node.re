/*
     Node.re

     Stubs for bindings to the `TSTree` object
 */

open Types;

type t;

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

external getStartPoint: t => Position.t = "rets_node_start_point";
external getEndPoint: t => Position.t = "rets_node_end_point";

let getChildren = (node: t) => {
  let i = ref(0);
  print_endline ("getting count...");
  let count = getChildCount(node);
  print_endline ("got count: " ++ string_of_int(count));

  let children = ref([]);

  while (i^ < count) {
    print_endline ("getChild...");
    let child = getChild(node, i^);
    print_endline ("after getChild");

    print_endline ("CHILD: " ++ toString(child));
    
    children := [child, ...children^];
    incr(i);
  }

  List.rev(children^);
};

let getRange = (node: t) => {
  let startPosition = getStartPoint(node);
  let endPosition = getEndPoint(node);

  Range.create(~startPosition, ~endPosition, ());
};

external hasChanges: t => bool = "rets_node_has_changes";
external hasError: t => bool = "rets_node_has_error";

external isMissing: t => bool = "rets_node_is_missing";
external isNull: t => bool = "rets_node_is_null";
external isNamed: t => bool = "rets_node_is_named";
external isError: t => bool = "rets_node_is_error";
external isExtra: t => bool = "rets_node_is_extra";

external getSymbol: t => int = "rets_node_symbol";
external getType: t => string = "rets_node_type";
