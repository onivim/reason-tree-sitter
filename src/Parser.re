/*
     Parser.re

     Stubs for bindings to the `TSParser` object
 */

type t;

// Parsers for particular syntaxes
external json: unit => t = "rets_parser_new_json";

// General parser methods
external parseString: (t, string) => Tree.t = "rets_parser_parse_string";

type readFunction = (int, int, int) => option(string);

external _parse: (t, option(Tree.t), readFunction) => Tree.t = "rets_parser_parse";

type incrementalArray = (Tree.t, array(int))

let _parse_read_fn: ref(readFunction) = ref((_, _, _) => None);

let _parse_read= (byteOffset: int, line: int, col: int) => {
     (_parse_read_fn^)(byteOffset, line, col);
};

let parse = (parser: t, tree: option(Tree.t), readFunction) => {
     _parse_read_fn := readFunction;
     _parse(parser, tree, _parse_read);
}

Callback.register("rets__parse_read", _parse_read);

let parseArray = (parser: t, oldTree: option(incrementalArray), lines: array(string)) => {
    let len = Array.length(lines);
    let byteOffsets: array(int) = Array.make(len, 0);
    let f = (_byteOffset, line, _col) => {
       if (line < len) {
            let v = Array.get(lines, line);
            let strlen = String.length(v);
            Array.set(byteOffsets, line, strlen);
            Some(v ++ "\n")
       }  else {
          None
       }
    };

    let oldTree = switch(oldTree) {
    | Some((tree, _)) => Some(tree)
    | None => None
    };

    let tree = parse(parser, oldTree, f);
    (tree, (tree, byteOffsets));
};
