/*
     ArrayParser.re

     Specialized parser for arrays of string
 */

type t;

module Baseline {
     type t = {
          lengths: array(int),
          tree: Tree.t,
     };

     let create = (~lengths, ~tree, ()) => {
          lengths,
          tree
     };
}

module Delta {
     type t = {
          tree: Tree.t,
     };

     let getOffsetToLine = (startLine, endLine, lines: array(int)) => {
          let i = ref(startLine); 
          let offset = ref(0);
          
          while (i^ < endLine) {
               offset := offset^ + Array.get(lines, i^);
               incr(i)
          };

          offset^
     };
     
     let getOffsetToLineStr = (startLine, endLine, lines: array(string)) => {
          let i = ref(startLine); 
          let offset = ref(0);
          
          while (i^ < endLine) {
               offset := offset^ + String.length(Array.get(lines, i^)) + 1;
               incr(i)
          };

          offset^
     };

     let create = (baseline: Baseline.t, startLine: int, oldEndLine: int, newLines: array(string)) => {
          let { lengths, tree }: Baseline.t = baseline;
          let startByte = getOffsetToLine(0, startLine, lengths);
          let oldEndByte = getOffsetToLine(0, oldEndLine, lengths);
          
          let len = Array.length(newLines);
          let newEndByte = startByte + getOffsetToLineStr(0, Array.length(newLines), newLines);
          let newEndLine = startLine + len;

          let newTree = Tree.edit(tree, startByte, oldEndByte, newEndByte, startLine, oldEndLine, newEndLine);
          { tree: newTree }
     };
}

let parse = (parser: Parser.t, delta: option(Delta.t), lines: array(string)) => {
    let len = Array.length(lines);
    let byteOffsets: array(int) = Array.make(len, 0);

    let f = (_byteOffset, line, _col) => {
       if (line < len) {
            let v = Array.get(lines, line);
            let strlen = String.length(v);
            Array.set(byteOffsets, line, strlen + 1);
            Some(v ++ "\n")
       }  else {
          None
       }
    };

    let oldTree = switch(delta) {
    | Some({tree}) => Some(tree)
    | None => None
    };

    let tree = Parser.parse(parser, oldTree, f);
    let baseline = Baseline.create(~tree, ~lengths=byteOffsets, ());
    (tree, baseline);
};
