open TestFramework;

open Treesitter;
open Treesitter.TreeSitterScopes;

describe("Tree-sitter -> Scopes", ({describe, _}) => {
  describe("c", ({test, _}) =>
    test("basic parse case", ({expect, _}) => {
      let cParser = Parser.getParserForLanguage(Languages.C);
      let tree = Parser.parseString(cParser, "int main() { return 1; }");
      let cScopeConverter =
        TextMateConverter.getScopeConverterForLanguage(Languages.C);

      let node = Tree.getRootNode(tree);
      let ret = Node.toString(node);
      expect.string(ret).toEqual(
        "(translation_unit (function_definition type: (primitive_type) declarator: (function_declarator declarator: (identifier) parameters: (parameter_list)) body: (compound_statement (return_statement (number_literal)))))",
      );

      let intChildNode = Node.getChild(Node.getChild(node, 0), 0);
      let intType = Node.getType(intChildNode);
      expect.string(intType).toEqual("primitive_type");

      let tmScope =
        TextMateConverter.getTextMateScope(~token=intType, cScopeConverter);
      expect.string(tmScope).toEqual("support.storage.type");
    })
  )
});
