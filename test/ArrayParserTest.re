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
      let multiLineArray = [|"[", "1,", "\"2\"", "]", ""|];

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
      let (_, baseline) =
        ArrayParser.parse(jsonParser, None, [|"[1, \"2\"]"|]);

      let update = [|"[1]"|];
      let delta = ArrayParser.Delta.create(baseline, 0, 1, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), update);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual("(value (array (number)))");
    });

    test("change single line", ({expect, _}) => {
      let start = [|"[", "1,", "\"2\",", "3", "]", ""|];

      let endv = [|"[", "1,", "2,", "3", "]", ""|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [|"2,"|];
      let delta = ArrayParser.Delta.create(baseline, 2, 3, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual(
        "(value (array (number) (number) (number)))",
      );
    });

    test("remove multiple lines", ({expect, _}) => {
      let start = [|"[", "1,", "\"2\",", "3", "]", ""|];

      let endv = [|"[", "]", ""|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [||];
      let delta = ArrayParser.Delta.create(baseline, 1, 4, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual("(value (array))");
    });

    test("add multiple lines", ({expect, _}) => {
      let start = [|"[", "]", ""|];

      let endv = [|"[", "1,", "\"2\",", "3", "]", ""|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [|"1,", "\"2\",", "3"|];
      let delta = ArrayParser.Delta.create(baseline, 1, 1, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual(
        "(value (array (number) (string (string_content)) (number)))",
      );
    });

    test("update multiple lines", ({expect, _}) => {
      let start = [|"[", "1,", "\"2\",", "3", "]", ""|];

      let endv = [|"[", "\"1\",", "2,", "\"3\"", "]"|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [|"\"1\",", "2,", "\"3\""|];
      let delta = ArrayParser.Delta.create(baseline, 1, 4, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      prerr_endline("RET: " ++ ret);
      expect.string(ret).toEqual(
        "(value (array (string (string_content)) (number) (string (string_content))))",
      );
    });
  });
});
