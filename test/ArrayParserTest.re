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

    let tokenRangeMatches = (~token, ~range: Types.Range.t, ()) => {
      let startPosition = Syntax.Token.getPosition(token);
      let endPosition = Syntax.Token.getEndPosition(token);

      range.startPosition.line == startPosition.line
      && range.startPosition.column == startPosition.column
      && range.endPosition.line == endPosition.line
      && range.endPosition.column == endPosition.column
    };
    test("token positions are preserved when deleting a line", ({expect, _}) => {
      let start = [|"[", "", "]"|];

      let endv = [|"[", "]"|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [||];
      let delta = ArrayParser.Delta.create(baseline, 1, 2, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let range = Types.Range.create(
        ~startPosition=Types.Position.create(~line=0, ~column=0, ()),
        ~endPosition=Types.Position.create(~line=3, ~column=0, ()),
        (),
      );
      prerr_endline ("-----START-------");
      let getTokenName = Syntax.createArrayTokenNameResolver(endv);
      let tokens = Syntax.getTokens(~getTokenName, ~range, node);

      // Validate tokens aren't shifted when deleting a row
      let leftBracket = List.nth(tokens, 0);
      expect.bool(tokenRangeMatches(
        ~token=leftBracket,
        ~range=Types.Range.createi(~startLine=0,~startColumn=0, ~endLine=0, ~endColumn=1, ()),
        ()
      )).toBe(true);

      expect.string(Syntax.Token.getName(leftBracket)).toEqual("\"[\"");
      
      let rightBracket = List.nth(tokens, 1);
      expect.bool(tokenRangeMatches(
        ~token=rightBracket,
        ~range=Types.Range.createi(~startLine=1,~startColumn=0, ~endLine=1, ~endColumn=1, ()),
        ()
      )).toBe(true);
      expect.string(Syntax.Token.getName(rightBracket)).toEqual("\"]\"");
      List.iter((t) => prerr_endline (Syntax.Token.show(t)), tokens);
    });
    test("token positions are preserved when adding a line", ({expect, _}) => {
      let start = [|"[", "]"|];

      let endv = [|"[", "", "]"|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [|""|];
      let delta = ArrayParser.Delta.create(baseline, 1, 1, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let getTokenName = (_) => "";
      let range = Types.Range.create(
        ~startPosition=Types.Position.create(~line=0, ~column=0, ()),
        ~endPosition=Types.Position.create(~line=3, ~column=0, ()),
        (),
      );
      prerr_endline ("-----START-------");
      let tokens = Syntax.getTokens(~getTokenName, ~range, node);

      // Validate tokens aren't shifted when deleting a row
      let leftBracket = List.nth(tokens, 0);
      expect.bool(tokenRangeMatches(
        ~token=leftBracket,
        ~range=Types.Range.createi(~startLine=0,~startColumn=0, ~endLine=0, ~endColumn=1, ()),
        ()
      )).toBe(true);
      
      let rightBracket = List.nth(tokens, 1);
      expect.bool(tokenRangeMatches(
        ~token=rightBracket,
        ~range=Types.Range.createi(~startLine=2,~startColumn=0, ~endLine=2, ~endColumn=1, ()),
        ()
      )).toBe(true);
      List.iter((t) => prerr_endline (Syntax.Token.show(t)), tokens);
    });
    test("token positions are preserved when modifying a line", ({expect, _}) => {
       let start = [|"[", "", "]"|];

      let endv = [|"[", "a", "]"|];

      let jsonParser = Parser.json();
      let (_, baseline) = ArrayParser.parse(jsonParser, None, start);

      let update = [|"a"|];
      let delta = ArrayParser.Delta.create(baseline, 1, 2, update);

      let (tree, _) = ArrayParser.parse(jsonParser, Some(delta), endv);

      let node = Tree.getRootNode(tree);
      let range = Types.Range.create(
        ~startPosition=Types.Position.create(~line=0, ~column=0, ()),
        ~endPosition=Types.Position.create(~line=3, ~column=0, ()),
        (),
      );
      let getTokenName = Syntax.createArrayTokenNameResolver(endv);
      prerr_endline ("-----START-------");
      let tokens = Syntax.getTokens(~getTokenName, ~range, node);

      // Validate tokens aren't shifted when deleting a row
      let leftBracket = List.nth(tokens, 0);
      expect.bool(tokenRangeMatches(
        ~token=leftBracket,
        ~range=Types.Range.createi(~startLine=0,~startColumn=0, ~endLine=0, ~endColumn=1, ()),
        ()
      )).toBe(true);

      expect.string(Syntax.Token.getName(leftBracket)).toEqual("\"[\"");
      
      let rightBracket = List.nth(tokens, 2);
      expect.bool(tokenRangeMatches(
        ~token=rightBracket,
        ~range=Types.Range.createi(~startLine=2,~startColumn=0, ~endLine=2, ~endColumn=1, ()),
        ()
      )).toBe(true);
      expect.string(Syntax.Token.getName(rightBracket)).toEqual("\"]\"");
      List.iter((t) => prerr_endline (Syntax.Token.show(t)), tokens);
    });
  });
});
