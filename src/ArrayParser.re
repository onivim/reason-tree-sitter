/*
     ArrayParser.re

     Specialized parser for arrays of string
 */

type t;

module Baseline = {
  type t = {
    lines: array(string),
    tree: Tree.t,
  };

  let create = (~lines, ~tree, ()) => {lines, tree};

  let lineToByte = (v: t, line) => {
    let node = Tree.getRootNode(v.tree);
    let startLine = max(line - 1, 0);
    let nearest = Node.getDescendantForPointRange(
      node,
      startLine,
      0,
      startLine,
      1,
    );

    let startByte = Node.getStartByte(nearest);
    let startPoint = Node.getStartPoint(nearest);

    let byteAtBeginning = startByte - startPoint.column;

    let i = ref(startPoint.line);
    let byte = ref(byteAtBeginning);
    let len = Array.length(v.lines);
    while(i^ < line && i^ < len) {
      let idx = i^;
      let s = v.lines[idx];
      byte := byte^ + String.length(s) + 1;
      incr(i);
    };
    byte^;
  };
};

module Delta = {
  type t = {
    tree: Tree.t,
    startLine: int,
    oldEndLine: int,
    newLines: array(string),
  };

  let getOffsetToLineStr = (startLine, endLine, lines: array(string)) => {
    let i = ref(startLine);
    let offset = ref(0);

    while (i^ < endLine) {
      offset := offset^ + String.length(lines[i^]) + 1;
      incr(i);
    };

    offset^;
  };
  
  let create =
      (
        baseline: Baseline.t,
        startLine: int,
        oldEndLine: int,
        newLines: array(string),
      ) => {
    let {tree, _}: Baseline.t = baseline;
    let startByte = Baseline.lineToByte(baseline, startLine);
    let oldEndByte = Baseline.lineToByte(baseline, oldEndLine);

    let len = Array.length(newLines);
    let newEndByte = startByte + getOffsetToLineStr(0, len, newLines);
    let newEndLine = startLine + len;

    let newTree =
      Tree.edit(
        tree,
        startByte,
        oldEndByte,
        newEndByte,
        startLine,
        oldEndLine,
        newEndLine,
      );
    {
      tree: newTree,
      startLine,
      oldEndLine,
      newLines,
    };
  };
};

let parse = (parser: Parser.t, delta: option(Delta.t), lines: array(string)) => {
  //let len = Array.length(lines);

  // The interop between C <-> OCaml is expensive for large files.
  // We should look to see if we can instead access the array directly
  // from the C side.
/*  let f = (_byteOffset, line, col) =>
    if (line < len) {
      let v = lines[line] ++ "\n";
      let strlen = String.length(v);

      if (col < strlen) {
        let ret = String.sub(v, col, strlen - col);
        Some(ret);
      } else {
        None;
      };
    } else {
      None;
    };*/

  let oldTree =
    switch (delta) {
    | Some({tree, _}) => Some(tree)
    | None => None
    };

  let tree = Parser.parseArray(parser, oldTree, lines);
  let baseline = Baseline.create(~tree, ~lines, ());
  (tree, baseline);
};
