open TestFramework;

open Treesitter;

describe("Syntax", ({describe, _}) => {

  let jsonParser = Parser.json();
  let (tree, _) = ArrayParser.parse(jsonParser, None, [|"[1, 2]"|]);
  let simpleNode = Tree.getRootNode(tree);

  let (tree, _) = ArrayParser.parse(jsonParser, None, [|"[1,]"|]);
  let errorNode = Tree.getRootNode(tree);
  // "(value (array (number) (string (string_content))))",

  describe("getErrorRanges", ({test, _}) => {
    test("returns empty list when none", ({expect, _}) => {
      let errors = Syntax.getErrorRanges(simpleNode);
      expect.int(List.length(errors)).toBe(0);
    });
    test("returns error when present", ({expect, _}) => {
      let errors = Syntax.getErrorRanges(errorNode);
      expect.int(List.length(errors)).toBe(1);
    });
  });
});
