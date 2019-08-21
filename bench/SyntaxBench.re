open Treesitter;
open BenchFramework;

Printexc.record_backtrace(true);

let jsonParser = Parser.json();

let largeTree = Parser.parseString(jsonParser, TestData.largeJsonString);
let largeNode = Tree.getRootNode(largeTree);

let getTokens = (node: Node.t, ()) => {
  let _ = Syntax.getTokens(node);
  ();
};

let getErrors = (node: Node.t, ()) => {
  let _ = Syntax.getErrorRanges(node);
  ();
};

let setup = () => ();
let options = Reperf.Options.create(~iterations=10, ());


bench(
  ~name="getTokens: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=getTokens(largeNode),
  (),
);

bench(
  ~name="getErrors: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=getErrors(largeNode),
  (),
);
