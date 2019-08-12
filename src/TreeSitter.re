module Parser = Parser;

let test = () => print_endline("Hello, world!");

external testBinding: unit => unit = "rets_hello";

