/*
     Python.re

     Python scope converter.
 */

let json = {|
{
	"name": "Python",
	"scopeName": "source.python",
	"type": "tree-sitter",
	"parser": "tree-sitter-python",
	"firstLineRegex": [
		"^#![ \\t]*/.*\\bpython[\\d\\.]*\\b",
		"vim\\b.*\\bset\\b.*\\b(filetype|ft|syntax)=python"
	],
	"fileTypes": [
		"py",
		"pyi",
		"pyw",
		"gyp",
		"gypi",
		"SConstruct",
		"Sconstruct",
		"sconstruct",
		"SConscript",
		"wsgi"
	],
	"folds": [
		{
			"type": [
				"if_statement"
			],
			"start": {
				"type": ":"
			},
			"end": {
				"type": [
					"elif_clause",
					"else_clause"
				]
			}
		},
		{
			"type": [
				"if_statement",
				"elif_clause",
				"else_clause",
				"for_statement",
				"try_statement",
				"with_statement",
				"while_statement",
				"class_definition",
				"function_definition",
				"async_function_definition"
			],
			"start": {
				"type": ":"
			}
		},
		{
			"start": {
				"type": "(",
				"index": 0
			},
			"end": {
				"type": ")",
				"index": -1
			}
		},
		{
			"start": {
				"type": "[",
				"index": 0
			},
			"end": {
				"type": "]",
				"index": -1
			}
		},
		{
			"start": {
				"type": "{",
				"index": 0
			},
			"end": {
				"type": "}",
				"index": -1
			}
		}
	],
	"comments": {
		"start": "# "
	},
	"scopes": {
		"module": "source.python",
		"comment": "comment.line",
		"string": "string.quoted",
		"escape_sequence": "constant.character.escape",
		"interpolation": "meta.embedded",
		"interpolation > \"{\"": "punctuation.section.embedded",
		"interpolation > \"}\"": "punctuation.section.embedded",
		"class_definition > identifier": "entity.name.type.class",
		"function_definition > identifier": "entity.name.function",
		"call > identifier:nth-child(0)": [
			{
				"match": "^(abs|all|any|ascii|bin|bool|breakpoint|bytearray|bytes|callable|chr|classmethod|compile|complex|delattr|dict|dir|divmod|enumerate|eval|exec|filter|float|format|frozenset|getattr|globals|hasattr|hash|help|hex|id|input|int|isinstance|issubclass|iter|len|list|locals|map|max|memoryview|min|next|object|oct|open|ord|pow|print|property|range|repr|reversed|round|set|setattr|slice|sorted|staticmethod|str|sum|super|tuple|type|vars|zip|__import__)$",
				"scopes": "support.function"
			},
			"entity.name.function"
		],
		"call > attribute > identifier:nth-child(2)": "entity.name.function",
		"identifier": {
			"match": "^(BaseException|Exception|TypeError|StopAsyncIteration|StopIteration|ImportError|ModuleNotFoundError|OSError|ConnectionError|BrokenPipeError|ConnectionAbortedError|ConnectionRefusedError|ConnectionResetError|BlockingIOError|ChildProcessError|FileExistsError|FileNotFoundError|IsADirectoryError|NotADirectoryError|InterruptedError|PermissionError|ProcessLookupError|TimeoutError|EOFError|RuntimeError|RecursionError|NotImplementedError|NameError|UnboundLocalError|AttributeError|SyntaxError|IndentationError|TabError|LookupError|IndexError|KeyError|ValueError|UnicodeError|UnicodeEncodeError|UnicodeDecodeError|UnicodeTranslateError|AssertionError|ArithmeticError|FloatingPointError|OverflowError|ZeroDivisionError|SystemError|ReferenceError|BufferError|MemoryError|Warning|UserWarning|DeprecationWarning|PendingDeprecationWarning|SyntaxWarning|RuntimeWarning|FutureWarning|ImportWarning|UnicodeWarning|BytesWarning|ResourceWarning|GeneratorExit|SystemExit|KeyboardInterrupt)$",
			"scopes": "support.type.exception"
		},
		"attribute > identifier:nth-child(2)": "variable.other.object.property",
		"decorator": "entity.name.function.decorator",
		"none": "constant.language",
		"true": "constant.language",
		"false": "constant.language",
		"integer": "constant.numeric",
		"float": "constant.numeric",
		"type > identifier": "support.storage.type",
		"class_definition > argument_list > attribute": "entity.other.inherited-class",
		"class_definition > argument_list > identifier": "entity.other.inherited-class",
		"class_definition > argument_list > keyword_argument > attribute": "entity.other.inherited-class",
		"class_definition > argument_list > keyword_argument > identifier:nth-child(2)": "entity.other.inherited-class",
		"\"class\"": "storage.type.class",
		"\"def\"": "storage.type.function",
		"\"lambda\"": "storage.type.function",
		"\"global\"": "storage.modifier.global",
		"\"nonlocal\"": "storage.modifier.nonlocal",
		"parameters > identifier": "variable.parameter.function",
		"parameters > list_splat > identifier": "variable.parameter.function",
		"parameters > dictionary_splat > identifier": "variable.parameter.function",
		"default_parameter > identifier:nth-child(0)": "variable.parameter.function",
		"keyword_argument > identifier:nth-child(0)": "variable.parameter.function",
		"\"if\"": "keyword.control",
		"\"else\"": "keyword.control",
		"\"elif\"": "keyword.control",
		"\"while\"": "keyword.control",
		"\"for\"": "keyword.control",
		"\"return\"": "keyword.control",
		"\"break\"": "keyword.control",
		"\"continue\"": "keyword.control",
		"\"pass\"": "keyword.control",
		"\"raise\"": "keyword.control",
		"\"yield\"": "keyword.control",
		"\"await\"": "keyword.control",
		"\"async\"": "keyword.control",
		"\"try\"": "keyword.control",
		"\"except\"": "keyword.control",
		"\"with\"": "keyword.control",
		"\"as\"": "keyword.control",
		"\"finally\"": "keyword.control",
		"\"import\"": "keyword.control",
		"\"from\"": "keyword.control",
		"\"print\"": "keyword.other",
		"\"assert\"": "keyword.other",
		"\"exec\"": "keyword.other",
		"\"del\"": "keyword.other",
		"\"+\"": "keyword.operator",
		"\"-\"": "keyword.operator",
		"\"*\"": "keyword.operator",
		"\"/\"": "keyword.operator",
		"\"%\"": "keyword.operator",
		"\"**\"": "keyword.operator",
		"\"//\"": "keyword.operator",
		"\"==\"": "keyword.operator",
		"\"!=\"": "keyword.operator",
		"\"<>\"": "keyword.operator",
		"\">\"": "keyword.operator",
		"\"<\"": "keyword.operator",
		"\">=\"": "keyword.operator",
		"\"<=\"": "keyword.operator",
		"\"=\"": "keyword.operator",
		"\"+=\"": "keyword.operator",
		"\"-=\"": "keyword.operator",
		"\"*=\"": "keyword.operator",
		"\"/=\"": "keyword.operator",
		"\"%=\"": "keyword.operator",
		"\"**=\"": "keyword.operator",
		"\"//=\"": "keyword.operator",
		"\"&\"": "keyword.operator",
		"\"|\"": "keyword.operator",
		"\"^\"": "keyword.operator",
		"\"~\"": "keyword.operator",
		"\"<<\"": "keyword.operator",
		"\">>\"": "keyword.operator",
		"binary_operator > \"@\"": "keyword.operator",
		"binary_operator > \"@=\"": "keyword.operator",
		"\"in\"": "keyword.operator.logical.python",
		"\"and\"": "keyword.operator.logical.python",
		"\"or\"": "keyword.operator.logical.python",
		"\"not\"": "keyword.operator.logical.python",
		"\"is\"": "keyword.operator.logical.python"
	}
}
|};
