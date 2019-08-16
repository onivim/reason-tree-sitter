open TestFramework;

open Treesitter;

describe("ArrayParser", ({describe, _}) => {
  describe("parse", ({test, _}) => {
    test("parses a single line array", ({expect, _}) => {
      let jsonParser = Parser.json();
      let (tree, _) = ArrayParser.parse(jsonParser, None, [|"[1, \"2\"]"|]);
      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual(
        "(value (array (number) (string (string_content))))",
      );
    });
    test("parses a multi-line array", ({expect, _}) => {

      let multiLineArray = [|
      "[",
      "1,",
      "\"2\"",
      "]",
      "", 
      |];

      let jsonParser = Parser.json();
      let (tree, _) = ArrayParser.parse(jsonParser, None, multiLineArray);
      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual(
        "(value (array (number) (string (string_content))))",
      );
    });
  });
  describe("incremental parse", ({test, _}) => {
    test("incrementally update single line", ({expect, _}) => {
      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, [|"[1, \"2\"]"|]);

      let update = [|"[1]"|];
      let delta = ArrayParser.Delta.create(baseline, 0, 1, update);
      
      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), update);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual(
        "(value (array (number)))",
      );
    });
  });
});
