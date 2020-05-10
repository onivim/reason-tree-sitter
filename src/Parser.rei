/*
     Parser.rei
 */

type t;

/* [getParser(scope)] returns the parser for that language. */
let getParserForLanguage: Languages.t => t;

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
