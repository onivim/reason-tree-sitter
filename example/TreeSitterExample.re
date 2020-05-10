open Treesitter;
open Treesitter.TreeSitterScopes;

type t = {
  file: ref(string),
  line: ref(int),
  scope: ref(string),
};

let helpText = {|treesitter example

Test and check treesitter scope matching.
Usage:

esy @example run --file test.c --line 8 --scope source.c

Flags:
|};

let argList = cliObj => {
  /* Space at the start of every string so the Arg.align works */
  Arg.align([
    (
      "--file",
      Arg.Set_string(cliObj.file),
      " Generate a config file in the default or given location.",
    ),
    (
      "--line",
      Arg.Set_int(cliObj.line),
      " Link the passed data file(s) to the previous run.",
    ),
    (
      "--scope",
      Arg.Set_string(cliObj.scope),
      " List all the completed runs.",
    ),
  ]);
};
let anonFunc = _ => raise(Arg.Bad("Invalid argument"));

let getArgs = () => {
  let cliObj = {file: ref(""), line: ref(0), scope: ref("")};
  Arg.parse(argList(cliObj), _ => (), helpText);

  cliObj;
};

let getLineFromFile = (file, lineNum) => {
  let ic = open_in(file);
  let rec readLine = i => {
    switch (input_line(ic)) {
    | line =>
      if (i == lineNum) {
        close_in(ic);
        Some(line);
      } else {
        readLine(succ(i));
      }
    | exception End_of_file =>
      close_in(ic);
      None;
    };
  };

  readLine(1);
};

let printNode = (converter, node) => {
  let tm = Node.toString(node);
  let nodeType = Node.getType(node);
  let scopes = Syntax.getParentScopes(node);
  let scope = TextMateConverter.getTextMateScope(~token=nodeType, ~path=scopes, converter);
  print_endline(nodeType ++ " : " ++ tm ++ " : " ++ scope);
};

let main = () => {
  let args = getArgs();
  let language =
    switch (Languages.getLanguageFromScope(args.scope^)) {
    | Some(l) => l
    | None => invalid_arg("Invalid scope...")
    };

  let parser = Parser.getParserForLanguage(language);
  let scopeConverter =
    TextMateConverter.getScopeConverterForLanguage(language);

  let line = getLineFromFile(args.file^, args.line^);
  let tree =
    switch (line) {
    | Some(l) => Parser.parseString(parser, l)
    | None => invalid_arg("Invalid line number for file...")
    };
  let node = Tree.getRootNode(tree);

  print_endline("Parsed tree: " ++ Node.toString(node));

  let rec getAllNodes = (index, n, tokens) => {
    let childCount = Node.getChildCount(n);

    switch (childCount) {
    | 0 => [n, ...tokens]
    | _ =>
      let children = Node.getChildren(n);
      let (_, tokens) =
        List.fold_left(
          (prev, curr) => {
            let (index, tokens) = prev;
            let idx =
              Node.isNamed(curr) ? Node.getBoundedNamedIndex(curr) : index;
            let newTokens = getAllNodes(idx, curr, tokens);
            (idx, newTokens);
          },
          (0, tokens),
          children,
        );
      tokens;
    };
  };

  print_endline("\nnodeType : TS Node : TM Scope\n");
  getAllNodes(0, node, [])
  |> List.rev
  |> List.map(printNode(scopeConverter));
};

main();
