open Treesitter;
open BenchFramework;

Printexc.record_backtrace(true);

let jsonParser = Parser.json();
let cParser = Parser.c();
let jsParser = Parser.javascript();
let tsParser = Parser.typescript();

let simpleJson = "[1, \"2\", { \"test\": [1] }]";
let simpleC = "int main() { return 1; }";
let simpleJs = "main = () => 1;";
let simpleTs = "let main = function(x: number): number { return x };";

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

doBench("String: Small JSON", parse(simpleJson, jsonParser));
doBench("String: Small C", parse(simpleC, cParser));
doBench("String: Small JS", parse(simpleJs, jsParser));
doBench("String: Small TS", parse(simpleTs, tsParser));

doBench("String: Large JSON", parse(TestData.largeJsonString, jsonParser));
doBench("String: Large C", parse(TestData.largeCString, cParser));
doBench("String: Large JS", parse(TestData.largeJSString, jsParser));
doBench("String: Large TS", parse(TestData.largeTSString, tsParser));

doBench(
  "Array: Large JSON",
  parseArray(TestData.largeJsonArray, jsonParser),
);

doBench("Array: Large C", parseArray(TestData.largeCArray, cParser));
doBench("Array: Large JS", parseArray(TestData.largeJSArray, jsParser));
doBench("Array: Large TS", parseArray(TestData.largeTSArray, tsParser));
