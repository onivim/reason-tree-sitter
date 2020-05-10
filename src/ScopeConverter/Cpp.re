/*
     Cpp.re

     C++ scope converter.
 */

let json = {|
{
  "name": "C++",
  "scopeName": "source.cpp",
  "type": "tree-sitter",
  "parser": "tree-sitter-cpp",
  "injectionRegex": "(c|C)(\\+\\+|pp|PP)",
  "fileTypes": [
    "cc",
    "cpp",
    "cp",
    "cxx",
    "c++",
    "cu",
    "cuh",
    "h",
    "hh",
    "hpp",
    "hxx",
    "h++",
    "inl",
    "ino",
    "ipp",
    "tcc",
    "tpp"
  ],
  "contentRegex": "\n\\s*(namespace|class|template)\\s+",
  "folds": [
    {
      "type": [
        "comment",
        "preproc_arg"
      ]
    },
    {
      "type": [
        "preproc_if",
        "preproc_ifdef",
        "preproc_elif"
      ],
      "end": {
        "type": [
          "preproc_else",
          "preproc_elif"
        ]
      }
    },
    {
      "type": [
        "preproc_if",
        "preproc_ifdef"
      ],
      "end": {
        "index": -1
      }
    },
    {
      "type": [
        "preproc_else",
        "preproc_elif"
      ],
      "start": {
        "index": 0
      }
    },
    {
      "type": [
        "enumerator_list",
        "compound_statement",
        "declaration_list",
        "field_declaration_list",
        "parameter_list",
        "argument_list",
        "initializer_list",
        "parenthesized_expression",
        "template_parameter_list",
        "template_argument_list"
      ],
      "start": {
        "index": 0
      },
      "end": {
        "index": -1
      }
    },
    {
      "type": "case_statement",
      "start": {
        "index": 0
      },
      "end": {
        "type": "break_statement",
        "index": -1
      }
    },
    {
      "type": "case_statement",
      "start": {
        "index": 0
      }
    }
  ],
  "comments": {
    "start": "// "
  },
  "scopes": {
    "translation_unit": "source.cpp",
    "comment": "comment.block",
    "identifier": [
      {
        "match": "^[A-Z\\d_]+$",
        "scopes": "constant.other"
      }
    ],
    "\"#if\"": "keyword.control.directive",
    "\"#ifdef\"": "keyword.control.directive",
    "\"#ifndef\"": "keyword.control.directive",
    "\"#elif\"": "keyword.control.directive",
    "\"#else\"": "keyword.control.directive",
    "\"#endif\"": "keyword.control.directive",
    "\"#define\"": "keyword.control.directive",
    "\"#include\"": "keyword.control.directive",
    "preproc_directive": "keyword.control.directive",
    "\"if\"": "keyword.control",
    "\"else\"": "keyword.control",
    "\"do\"": "keyword.control",
    "\"for\"": "keyword.control",
    "\"while\"": "keyword.control",
    "\"break\"": "keyword.control",
    "\"continue\"": "keyword.control",
    "\"return\"": "keyword.control",
    "\"switch\"": "keyword.control",
    "\"case\"": "keyword.control",
    "\"default\"": "keyword.control",
    "\"goto\"": "keyword.control",
    "\"struct\"": "keyword.control",
    "\"enum\"": "keyword.control",
    "\"union\"": "keyword.control",
    "\"typedef\"": "keyword.control",
    "\"class\"": "keyword.control",
    "\"using\"": "keyword.control",
    "\"namespace\"": "keyword.control",
    "\"template\"": "keyword.control",
    "\"typename\"": "keyword.control",
    "\"try\"": "keyword.control",
    "\"catch\"": "keyword.control",
    "\"throw\"": "keyword.control",
    "\"__attribute__\"": "keyword.attribute",
    "preproc_function_def > identifier:nth-child(1)": "entity.name.function.preprocessor",
    "preproc_arg": "meta.preprocessor.macro",
    "template_function > identifier": [
      {
        "match": "^(static|const|dynamic|reinterpret)_cast$",
        "scopes": "keyword.operator"
      }
    ],
    "call_expression > identifier,\ncall_expression > field_expression > field_identifier,\ncall_expression > scoped_identifier > identifier,\ntemplate_function > identifier,\ntemplate_function > scoped_identifier > identifier,\ntemplate_method > field_identifier,\nfunction_declarator > identifier,\nfunction_declarator > field_identifier,\nfunction_declarator > scoped_identifier > identifier,\ndestructor_name > identifier": "entity.name.function",
    "statement_identifier": "constant.variable",
    "field_identifier": "variable.other.member",
    "type_identifier": "support.storage.type",
    "primitive_type": "support.storage.type",
    "\"unsigned\"": "support.storage.type",
    "\"signed\"": "support.storage.type",
    "\"short\"": "support.storage.type",
    "\"long\"": "support.storage.type",
    "auto": "support.storage.type",
    "char_literal": "string.quoted.single",
    "string_literal": "string.quoted.double",
    "system_lib_string": "string.quoted.other",
    "raw_string_literal": "string.quoted.other",
    "escape_sequence": "constant.character.escape",
    "preproc_include > string_literal > escape_sequence": "string.quoted.double",
    "number_literal": "constant.numeric.decimal",
    "null": "constant.language.null",
    "nullptr": "constant.language.null",
    "true": "constant.language.boolean",
    "false": "constant.language.boolean",
    "\"extern\"": "storage.modifier",
    "\"static\"": "storage.modifier",
    "\"register\"": "storage.modifier",
    "\"friend\"": "storage.modifier",
    "\"inline\"": "storage.modifier",
    "\"explicit\"": "storage.modifier",
    "\"const\"": "storage.modifier",
    "\"constexpr\"": "storage.modifier",
    "\"volatile\"": "storage.modifier",
    "\"restrict\"": "storage.modifier",
    "function_specifier": "storage.modifier",
    "\"public\"": "storage.modifier",
    "\"private\"": "storage.modifier",
    "\"protected\"": "storage.modifier",
    "\"final\"": "storage.modifier",
    "\"override\"": "storage.modifier",
    "\"virtual\"": "storage.modifier",
    "\"noexcept\"": "storage.modifier",
    "\"mutable\"": "storage.modifier",
    "\";\"": "punctuation.terminator.statement",
    "\"[\"": "punctuation.definition.begin.bracket.square",
    "\"]\"": "punctuation.definition.end.bracket.square",
    "access_specifier > \":\"": "punctuation.definition.visibility.colon",
    "base_class_clause > \":\"": "punctuation.definition.inheritance.colon",
    "base_class_clause > \",\"": "punctuation.definition.separator.class.comma",
    "field_declaration > \",\"": "punctuation.separator.delimiter",
    "parameter_list > \",\"": "punctuation.separator.delimiter",
    "field_initializer_list > \":\"": "punctuation.definition.initialization.colon",
    "field_initializer_list > \",\"": "punctuation.separator.delimiter",
    "\"::\"": "punctuation.separator.method.double-colon",
    "template_parameter_list > \"<\"": "punctuation.definition.template.bracket.angle",
    "template_parameter_list > \">\"": "punctuation.definition.template.bracket.angle",
    "template_argument_list > \">\"": "punctuation.definition.template.bracket.angle",
    "template_argument_list > \"<\"": "punctuation.definition.template.bracket.angle",
    "char_literal > \"'\"": "punctuation.definition.string",
    "string_literal > \"\\\"\"": "punctuation.definition.string",
    "\"{\"": "punctuation.section.block.begin.bracket.curly",
    "\"}\"": "punctuation.section.block.end.bracket.curly",
    "\"(\"": "punctuation.section.parens.begin.bracket.round",
    "\")\"": "punctuation.section.parens.end.bracket.round",
    "\"sizeof\"": "keyword.operator.sizeof",
    "\"new\"": "keyword.operator",
    "\"delete\"": "keyword.operator",
    "\".\"": "keyword.operator.member",
    "\"->\"": "keyword.operator.member",
    "\"*\"": "keyword.operator",
    "\"-\"": "keyword.operator",
    "\"+\"": "keyword.operator",
    "\"/\"": "keyword.operator",
    "\"%\"": "keyword.operator",
    "\"++\"": "keyword.operator",
    "\"--\"": "keyword.operator",
    "\"==\"": "keyword.operator",
    "\"!\"": "keyword.operator",
    "\"!=\"": "keyword.operator",
    "relational_expression > \"<\"": "keyword.operator",
    "relational_expression > \">\"": "keyword.operator",
    "\">=\"": "keyword.operator",
    "\"<=\"": "keyword.operator",
    "\"&&\"": "keyword.operator",
    "\"||\"": "keyword.operator",
    "\"&\"": "keyword.operator",
    "\"|\"": "keyword.operator",
    "\"^\"": "keyword.operator",
    "\"~\"": "keyword.operator",
    "\"<<\"": "keyword.operator",
    "\">>\"": "keyword.operator",
    "\"=\"": "keyword.operator",
    "\"+=\"": "keyword.operator",
    "\"-=\"": "keyword.operator",
    "\"*=\"": "keyword.operator",
    "\"/=\"": "keyword.operator",
    "\"%=\"": "keyword.operator",
    "\"<<=\"": "keyword.operator",
    "\">>=\"": "keyword.operator",
    "\"&=\"": "keyword.operator",
    "\"^=\"": "keyword.operator",
    "\"|=\"": "keyword.operator",
    "\"?\"": "keyword.operator",
    "conditional_expression > \":\"": "keyword.operator"
  }
}
|};
