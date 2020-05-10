open Treesitter;
open BenchFramework;

Printexc.record_backtrace(true);

let jsonParser = Parser.getParserForLanguage(Languages.Json);
let cParser = Parser.getParserForLanguage(Languages.C);
let cppParser = Parser.getParserForLanguage(Languages.Cpp);
let jsParser = Parser.getParserForLanguage(Languages.Javascript);
let tsParser = Parser.getParserForLanguage(Languages.Typescript);
let pyParser = Parser.getParserForLanguage(Languages.Python);

let simpleJson = "[1, \"2\", { \"test\": [1] }]";
let simpleC = "int main() { return 1; }";
let simpleJs = "main = () => 1;";
let simpleTs = "let main = function(x: number): number { return x };";
let simplePy = "def main(): return 1";

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
doBench("String: Small C++", parse(simpleC, cppParser));
doBench("String: Small JS", parse(simpleJs, jsParser));
doBench("String: Small TS", parse(simpleTs, tsParser));
doBench("String: Small Python", parse(simplePy, pyParser));

doBench("String: Large JSON", parse(TestData.largeJsonString, jsonParser));
doBench("String: Large C", parse(TestData.largeCString, cParser));
doBench("String: Large C++", parse(TestData.largeCPPString, cppParser));
doBench("String: Large JS", parse(TestData.largeJSString, jsParser));
doBench("String: Large TS", parse(TestData.largeTSString, tsParser));
doBench("String: Large Python", parse(TestData.largePyString, pyParser));

doBench(
  "Array: Large JSON",
  parseArray(TestData.largeJsonArray, jsonParser),
);

doBench("Array: Large C", parseArray(TestData.largeCArray, cParser));
doBench("Array: Large C++", parseArray(TestData.largeCPPArray, cppParser));
doBench("Array: Large JS", parseArray(TestData.largeJSArray, jsParser));
doBench("Array: Large TS", parseArray(TestData.largeTSArray, tsParser));
doBench("Array: Large Python", parseArray(TestData.largePyArray, pyParser));
