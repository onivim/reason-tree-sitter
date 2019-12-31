open Treesitter;
open BenchFramework;

Printexc.record_backtrace(true);

let jsonParser = Parser.json();
let cParser = Parser.c();
let jsParser = Parser.javascript();

let simpleJson = "[1, \"2\", { \"test\": [1] }]";
let simpleC = "int main() { return 1; }";
let simpleJs = "main = () => 1;";

let parse = (v: string, parser: Parser.t, ()) => {
  let _ = Parser.parseString(parser, v);
  ();
};

let parseArray = (v: array(string), parser: Parser.t, ()) => {
  let _ = ArrayParser.parse(parser, None, v);
  ();
};

let setup = () => ();
let options = Reperf.Options.create(~iterations=10, ());

bench(
  ~name="parseString: Small JSON",
  ~options,
  ~setup,
  ~f=parse(simpleJson, jsonParser),
  (),
);

bench(
  ~name="parseString: Small C",
  ~options,
  ~setup,
  ~f=parse(simpleC, cParser),
  (),
);

bench(
  ~name="parseString: Small JS",
  ~options,
  ~setup,
  ~f=parse(simpleJs, jsParser),
  (),
);

bench(
  ~name="parseString: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=parse(TestData.largeJsonString, jsonParser),
  (),
);

bench(
  ~name="parseString: Large C (sqlite3.c)",
  ~options,
  ~setup,
  ~f=parse(TestData.largeCString, cParser),
  (),
);

bench(
  ~name="parseString: Large JS (react-dom.development.js)",
  ~options,
  ~setup,
  ~f=parse(TestData.largeJSString, jsParser),
  (),
);

bench(
  ~name="parseArray: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=parseArray(TestData.largeJsonArray, jsonParser),
  (),
);

bench(
  ~name="parseArray: Large C (sqlite3.c)",
  ~options,
  ~setup,
  ~f=parseArray(TestData.largeCArray, cParser),
  (),
);

bench(
  ~name="parseArray: Large JS (react-dom.development.js)",
  ~options,
  ~setup,
  ~f=parseArray(TestData.largeJSArray, jsParser),
  (),
);
