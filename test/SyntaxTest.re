open TestFramework;

open Treesitter;

describe("Syntax", ({describe, _}) => {
  let jsonParser = Parser.json();
  let simpleArray = [|"[1, \"200\"]"|];
  let (tree, _) = ArrayParser.parse(jsonParser, None, simpleArray);
  let simpleNode = Tree.getRootNode(tree);
  let simpleNameResolver = Syntax.createArrayTokenNameResolver(simpleArray);

  let errorArray = [|"[1,  ]"|];
  let (tree, _) = ArrayParser.parse(jsonParser, None, errorArray);
  let errorNode = Tree.getRootNode(tree);
  let errorNameResolver = Syntax.createArrayTokenNameResolver(errorArray);
  // "(value (array (number) (string (string_content))))",

  let range =
    Types.Range.create(
      ~startPosition=Types.Position.create(~line=0, ~column=0, ()),
      ~endPosition=Types.Position.create(~line=1, ~column=0, ()),
      (),
    );

  describe("getErrorRanges", ({test, _}) => {
    test("returns empty list when none", ({expect, _}) => {
      let errors = Syntax.getErrorRanges(simpleNode);
      expect.int(List.length(errors)).toBe(0);
    });
    test("returns error when present", ({expect, _}) => {
      let errors = Syntax.getErrorRanges(errorNode);
      expect.int(List.length(errors)).toBe(1);

      let errorRange = List.hd(errors);

      let startLine = errorRange.startPosition.line;
      let endLine = errorRange.endPosition.line;

      let startColumn = errorRange.startPosition.column;
      let endColumn = errorRange.endPosition.column;

      expect.int(startLine).toBe(0);
      expect.int(endLine).toBe(0);

      expect.int(startColumn).toBe(2);
      expect.int(endColumn).toBe(3);
    });
  });
  describe("getTokens", ({test, _}) => {
    test("returns list of tokens in success case", ({expect, _}) => {
      let tokens =
        Syntax.getTokens(
          ~getTokenName=simpleNameResolver,
          ~range,
          simpleNode,
        );

      List.iter(v => prerr_endline(Syntax.Token.show(v)), tokens);
      expect.int(List.length(tokens)).toBe(7);
    });
    test("returns list of tokens in error case", ({expect, _}) => {
      prerr_endline("--ERROR--");
      let tokens =
        Syntax.getTokens(~getTokenName=errorNameResolver, ~range, errorNode);

      List.iter(v => prerr_endline(Syntax.Token.show(v)), tokens);
      expect.int(List.length(tokens)).toBe(4);
    });
  });
  describe("getParentScopes", ({test, _}) => {
    test("returns empty list for root", ({expect, _}) => {
      let scopes = Syntax.getParentScopes(simpleNode);

      expect.int(List.length(scopes)).toBe(0);
    });
    test("returns single item for first child", ({expect, _}) => {
      let firstChild = Node.getChild(simpleNode, 0);
      let scopes = Syntax.getParentScopes(firstChild);

      expect.bool(scopes == ["value"]).toBe(true);
    });
    test("returns multiple item for second child", ({expect, _}) => {
      let firstChild = Node.getChild(simpleNode, 0);
      let secondChild = Node.getChild(firstChild, 0);
      let scopes = Syntax.getParentScopes(secondChild);

      List.iter(s => prerr_endline("SCOPE: " ++ s), scopes);

      expect.bool(scopes == ["value", "array"]).toBe(true);
    });
  });
});
