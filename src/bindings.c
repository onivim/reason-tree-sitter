#include <string.h>
#include <tree_sitter/api.h>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

// Externals
TSLanguage *tree_sitter_json();

CAMLprim value rets_hello(value unit) {
  printf("Hello, world!\n");
  return Val_unit;
}

typedef struct _parser {
  TSParser *parser;
} parser;

void finalize_parser(value v) {
  parser *p;
  p = (parser *)Data_custom_val(v);
  ts_parser_delete(p->parser);
}

static struct custom_operations parser_custom_ops = {
  identifier: "parser handling",
  finalize: finalize_parser,
  compare: custom_compare_default,
  hash: custom_hash_default,
  serialize: custom_serialize_default,
  deserialize: custom_deserialize_default
};

CAMLprim value rets_parser_new_json(value unit) {
  CAMLparam0();
  CAMLlocal1(v);
  parser parserWrapper;
  TSParser *parser = ts_parser_new();
  parserWrapper.parser = parser;
  
  v = caml_alloc_custom (&parser_custom_ops, sizeof(parser), 0, 1);
  memcpy(Data_custom_val(v), &parserWrapper, sizeof(parser));
  ts_parser_set_language(parser, tree_sitter_json());
  CAMLreturn(v);
};
