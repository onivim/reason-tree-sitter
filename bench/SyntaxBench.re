open Treesitter;
open BenchFramework;

open TreeSitter.Types;

Printexc.record_backtrace(true);

let jsonParser = Parser.json();

let largeTree = Parser.parseString(jsonParser, TestData.largeJsonString);
let largeNode = Tree.getRootNode(largeTree);
let largeResolver =
  Syntax.createArrayTokenNameResolver(TestData.largeJsonArray);

let fullRange =
  Range.create(
    ~startPosition=Position.create(~line=0, ~column=0, ()),
    ~endPosition=Position.create(~line=7, ~column=0, ()),
    (),
  );

let subRange =
  Range.create(
    ~startPosition=Position.create(~line=3, ~column=0, ()),
    ~endPosition=Position.create(~line=3, ~column=50, ()),
    (),
  );

let getTokens = (range, resolver, node: Node.t, ()) => {
  let _ = Syntax.getTokens(~range, ~getTokenName=resolver, node);
  ();
};

let getErrors = (node: Node.t, ()) => {
  let _ = Syntax.getErrorRanges(node);
  ();
};

let setup = () => ();
let options = Reperf.Options.create(~iterations=10, ());

bench(
  ~name="getTokens: Large JSON (canada.json) - all tokens",
  ~options,
  ~setup,
  ~f=getTokens(fullRange, largeResolver, largeNode),
  (),
);

bench(
  ~name="getTokens: Large JSON (canada.json) - sub range",
  ~options,
  ~setup,
  ~f=getTokens(subRange, largeResolver, largeNode),
  (),
);

bench(
  ~name="getErrors: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=getErrors(largeNode),
  (),
);
