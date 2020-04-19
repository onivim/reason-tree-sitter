/*
     Javascript.re

     Javascript scope converter.
 */

let json = {|
{
    "name": "JavaScript",
    "scopeName": "source.js",
    "type": "tree-sitter",
    "parser": "tree-sitter-javascript",
    "fileTypes": [
        "js",
        "jsx"
    ],
    "injectionRegex": "^js$|^JS$|javascript|JavaScript",
    "firstLineRegex": [
        "^#!.*\\b(node)\\r?\\n",
        "vim\\b.*\\bset\\b.*\\b(filetype|ft|syntax)=(js|javascript)"
    ],
    "folds": [
        {
            "type": "comment"
        },
        {
            "type": [
                "jsx_element",
                "template_string"
            ],
            "start": {
                "index": 0
            },
            "end": {
                "index": -1
            }
        },
        {
            "type": "jsx_self_closing_element",
            "start": {
                "index": 1
            },
            "end": {
                "index": -2
            }
        },
        {
            "start": {
                "index": 0,
                "type": "{"
            },
            "end": {
                "index": -1,
                "type": "}"
            }
        },
        {
            "start": {
                "index": 0,
                "type": "["
            },
            "end": {
                "index": -1,
                "type": "]"
            }
        },
        {
            "start": {
                "index": 0,
                "type": "("
            },
            "end": {
                "index": -1,
                "type": ")"
            }
        },
        {
            "type": [
                "switch_case",
                "switch_default"
            ],
            "start": {
                "index": 0
            },
            "end": {
                "type": "break_statement",
                "index": -1
            }
        },
        {
            "type": [
                "switch_case",
                "switch_default"
            ],
            "start": {
                "index": 0
            }
        }
    ],
    "comments": {
        "start": "// "
    },
    "scopes": {
        "program": "source.js",
        "property_identifier": [
            {
                "match": "^[$A-Z_]+$",
                "scopes": "constant.other.property.js"
            },
            "variable.other.object.property"
        ],
        "member_expression > property_identifier": "variable.other.object.property.unquoted",
        "formal_parameters > identifier": "variable.parameter.function",
        "formal_parameters > rest_parameter > identifier": "variable.parameter.rest.function",
        "shorthand_property_identifier": [
            {
                "match": "^[$A-Z_]{2,}$",
                "scopes": "constant.other"
            }
        ],
        "class > identifier, new_expression > identifier": "meta.class",
        "jsx_opening_element > identifier, jsx_closing_element > identifier, jsx_self_closing_element > identifier": [
            {
                "match": "^[A-Z]",
                "scopes": "meta.class.component.jsx"
            }
        ],
        "call_expression > identifier": [
            {
                "match": "^require$",
                "scopes": "support.function"
            },
            "entity.name.function"
        ],
        "arrow_function > identifier:nth-child(0)": "variable.parameter.function",
        "function > identifier": "entity.name.function",
        "function_declaration > identifier": "entity.name.function",
        "generator_function > identifier": "entity.name.function",
        "call_expression > super": "support.function.super",
        "method_definition > property_identifier": "entity.name.function",
        "call_expression > member_expression > property_identifier": "entity.name.function",
        "identifier": [
            {
                "match": "^(global|module|exports|__filename|__dirname)$",
                "scopes": "support.variable"
            },
            {
                "match": "^(window|event|document|performance|screen|navigator|console)$",
                "scopes": "support.variable.dom"
            },
            {
                "exact": "require",
                "scopes": "support.function"
            },
            {
                "match": "^[$A-Z_]{2,}$",
                "scopes": "constant.other"
            },
            {
                "match": "^[A-Z]",
                "scopes": "meta.class"
            }
        ],
        "number": "constant.numeric",
        "string": "string.quoted",
        "regex": "string.regexp",
        "escape_sequence": "constant.character.escape",
        "template_string": "string.quoted.template",
        "undefined": "constant.language",
        "null": "constant.language.null",
        "true": "constant.language.boolean.true",
        "false": "constant.language.boolean.false",
        "comment": [
            {
                "match": "^//",
                "scopes": "comment.line"
            },
            "comment.block"
        ],
        "hash_bang_line": "comment.block",
        "jsx_expression > \"{\", jsx_expression > \"}\", template_substitution > \"${\", template_substitution > \"}\"": "punctuation.section.embedded",
        "template_substitution": "embedded.source",
        "\"(\"": "punctuation.definition.parameters.begin.bracket.round",
        "\")\"": "punctuation.definition.parameters.end.bracket.round",
        "\"{\"": "punctuation.definition.function.body.begin.bracket.curly",
        "\"}\"": "punctuation.definition.function.body.end.bracket.curly",
        "\";\"": "punctuation.terminator.statement.semicolon",
        "\"[\"": "punctuation.definition.array.begin.bracket.square",
        "\"]\"": "punctuation.definition.array.end.bracket.square",
        "\"var\"": "storage.type",
        "\"let\"": "storage.type",
        "\"class\"": "storage.type",
        "\"extends\"": "storage.modifier",
        "\"const\"": "storage.modifier",
        "\"static\"": "storage.modifier",
        "\"function\"": "storage.type.function",
        "\"=>\"": "storage.type.function.arrow",
        "\"=\"": "keyword.operator.js",
        "\"+=\"": "keyword.operator.js",
        "\"-=\"": "keyword.operator.js",
        "\"*=\"": "keyword.operator.js",
        "\"/=\"": "keyword.operator.js",
        "\"%=\"": "keyword.operator.js",
        "\"<<=\"": "keyword.operator.js",
        "\">>=\"": "keyword.operator.js",
        "\">>>=\"": "keyword.operator.js",
        "\"&=\"": "keyword.operator.js",
        "\"^=\"": "keyword.operator.js",
        "\"|=\"": "keyword.operator.js",
        "\"!\"": "keyword.operator.js",
        "\"+\"": "keyword.operator.js",
        "\"-\"": "keyword.operator.js",
        "\"*\"": "keyword.operator.js",
        "\"/\"": "keyword.operator.js",
        "\"%\"": "keyword.operator.js",
        "\"==\"": "keyword.operator.js",
        "\"===\"": "keyword.operator.js",
        "\"!=\"": "keyword.operator.js",
        "\"!==\"": "keyword.operator.js",
        "\">=\"": "keyword.operator.js",
        "\"<=\"": "keyword.operator.js",
        "\">\"": "keyword.operator.js",
        "\"<\"": "keyword.operator.js",
        "\":\"": "keyword.operator.js",
        "\"?\"": "keyword.operator.js",
        "\"&&\"": "keyword.operator.js",
        "\"||\"": "keyword.operator.js",
        "\"&\"": "keyword.operator.js",
        "\"~\"": "keyword.operator.js",
        "\"^\"": "keyword.operator.js",
        "\">>\"": "keyword.operator.js",
        "\">>>\"": "keyword.operator.js",
        "\"<<\"": "keyword.operator.js",
        "\"|\"": "keyword.operator.js",
        "\"++\"": "keyword.operator.js",
        "\"--\"": "keyword.operator.js",
        "\"...\"": "keyword.operator.spread.js",
        "\"in\"": "keyword.operator.in",
        "\"instanceof\"": "keyword.operator.instanceof",
        "\"of\"": "keyword.operator.of",
        "\"new\"": "keyword.operator.new",
        "\"typeof\"": "keyword.operator.typeof",
        "\"get\"": "keyword.operator.setter",
        "\"set\"": "keyword.operator.setter",
        "\".\"": "meta.delimiter.period",
        "\",\"": "meta.delimiter.comma",
        "\"as\"": "keyword.control",
        "\"if\"": "keyword.control",
        "\"do\"": "keyword.control",
        "\"else\"": "keyword.control",
        "\"while\"": "keyword.control",
        "\"for\"": "keyword.control",
        "\"return\"": "keyword.control",
        "\"break\"": "keyword.control",
        "\"continue\"": "keyword.control",
        "\"throw\"": "keyword.control",
        "\"try\"": "keyword.control",
        "\"catch\"": "keyword.control",
        "\"finally\"": "keyword.control",
        "\"switch\"": "keyword.control",
        "\"case\"": "keyword.control",
        "\"default\"": "keyword.control",
        "\"export\"": "keyword.control",
        "\"import\"": "keyword.control",
        "\"from\"": "keyword.control",
        "\"yield\"": "keyword.control",
        "\"async\"": "keyword.control",
        "\"await\"": "keyword.control",
        "\"debugger\"": "keyword.control",
        "\"delete\"": "keyword.control",
        "jsx_attribute > property_identifier": "entity.other.attribute-name",
        "jsx_opening_element > identifier": "entity.name.tag",
        "jsx_closing_element > identifier": "entity.name.tag",
        "jsx_self_closing_element > identifier": "entity.name.tag"
    }
}
|};
