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
let doBench = (name, f) => bench(~name, ~options, ~setup, ~f, ());

doBench("parseString: Small JSON", parse(simpleJson, jsonParser));
doBench("parseString: Small C", parse(simpleC, cParser));
doBench("parseString: Small JS", parse(simpleJs, jsParser));

doBench(
  "parseString: Large JSON (canada.json)",
  parse(TestData.largeJsonString, jsonParser),
);

doBench(
  "parseString: Large C (sqlite3.c)",
  parse(TestData.largeCString, cParser),
);

doBench(
  "parseString: Large JS (react-dom.development.js)",
  parse(TestData.largeJSString, jsParser),
);

doBench(
  "parseArray: Large JSON (canada.json)",
  parseArray(TestData.largeJsonArray, jsonParser),
);

doBench(
  "parseArray: Large C (sqlite3.c)",
  parseArray(TestData.largeCArray, cParser),
);

doBench(
  "parseArray: Large JS (react-dom.development.js)",
  parseArray(TestData.largeJSArray, jsParser),
);
