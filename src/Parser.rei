/*
     Parser.rei
 */

type t;

/* [json()] returns a new JSON parser */
let json: unit => t;

/* [c()] returns a new C parser */
let c: unit => t;

/* [cpp()] returns a new C++ parser */
let cpp: unit => t;

/* [python()] returns a new Python parser */
let python: unit => t;

/* [javascript()] returns a new Javascript parser */
let javascript: unit => t;

/* [typescript()] returns a new TS parser */
let typescript: unit => t;

/* [tsx()] returns a new TSX parser */
let tsx: unit => t;

/*
   [parseString(parser, contents)] parses a string with [parser],
   returning a parsed syntax tree.
 */
let parseString: (t, string) => Tree.t;

type readFunction = (int, int, int) => option(string);

/*
   [parse(parser, previousTree, readFunction)] parses arbitrary
   documents. The [readFunction] takes a [(byteOffset, line, column)]
   and returns an [option(string)] describing the results.

   [previousTree] is an [option(Tree.t)]. The first time parsing a document,
   None should be used. A tree can be provided to speed up incremental parsing.

   Returns a [Tree.t] with the output of the parsing.
 */
let parse: (t, option(Tree.t), readFunction) => Tree.t;
