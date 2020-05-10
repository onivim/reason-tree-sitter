/*
     C.re

     C scope converter.
 */

let json = {|
{
  "name": "C",
  "scopeName": "source.c",
  "type": "tree-sitter",
  "parser": "tree-sitter-c",
  "injectionRegex": "c|C",
  "fileTypes": [
    "h",
    "c",
    "h.in"
  ],
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
    "translation_unit": "source.c",
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
    "preproc_function_def > identifier:nth-child(1)": "entity.name.function.preprocessor",
    "preproc_arg": "meta.preprocessor.macro",
    "call_expression > identifier,\ncall_expression > field_expression > field_identifier,\nfunction_declarator > identifier": "entity.name.function",
    "statement_identifier": "constant.variable",
    "field_identifier": "variable.other.member",
    "type_identifier": "support.storage.type",
    "primitive_type": "support.storage.type",
    "\"signed\"": "support.storage.type",
    "\"unsigned\"": "support.storage.type",
    "\"short\"": "support.storage.type",
    "\"long\"": "support.storage.type",
    "char_literal": "string.quoted.single",
    "string_literal": "string.quoted.double",
    "system_lib_string": "string.quoted.other",
    "escape_sequence": "constant.character.escape",
    "number_literal": "constant.numeric.decimal",
    "null": "constant.language.null",
    "true": "constant.language.boolean",
    "false": "constant.language.boolean",
    "auto": "storage.modifier",
    "\"extern\"": "storage.modifier",
    "\"register\"": "storage.modifier",
    "\"static\"": "storage.modifier",
    "\"inline\"": "storage.modifier",
    "\"const\"": "storage.modifier",
    "\"volatile\"": "storage.modifier",
    "\"restrict\"": "storage.modifier",
    "\"_Atomic\"": "storage.modifier",
    "function_specifier": "storage.modifier",
    "\";\"": "punctuation.terminator.statement",
    "\"[\"": "punctuation.definition.begin.bracket.square",
    "\"]\"": "punctuation.definition.end.bracket.square",
    "\",\"": "punctuation.separator.delimiter",
    "char_literal > \"'\"": "punctuation.definition.string",
    "string_literal > \"\\\"\"": "punctuation.definition.string",
    "\"{\"": "punctuation.section.block.begin.bracket.curly",
    "\"}\"": "punctuation.section.block.end.bracket.curly",
    "\"(\"": "punctuation.section.parens.begin.bracket.round",
    "\")\"": "punctuation.section.parens.end.bracket.round",
    "\"sizeof\"": "keyword.operator.sizeof",
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
    "\"<\"": "keyword.operator",
    "\">\"": "keyword.operator",
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
    "\":\"": "keyword.operator"
  }
}
|};
