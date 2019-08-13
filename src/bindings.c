#include <string.h>
#include <tree_sitter/api.h>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

// External syntaxes
TSLanguage *tree_sitter_json();


typedef struct _parser {
  TSParser *parser;
} parser_W;

typedef struct _tree {
  TSTree *tree;
} tree_W;

void finalize_parser(value v) {
  parser_W *p;
  p = (parser_W *)Data_custom_val(v);
  ts_parser_delete(p->parser);
}

void finalize_tree(value v) {
  tree_W *p;
  p = (tree_W *)Data_custom_val(v);
  ts_tree_delete(p->tree);
}

static struct custom_operations parser_custom_ops = {
  identifier : "parser handling",
  finalize : finalize_parser,
  compare : custom_compare_default,
  hash : custom_hash_default,
  serialize : custom_serialize_default,
  deserialize : custom_deserialize_default
};

static struct custom_operations tree_custom_ops = {
  identifier : "tree handling",
  finalize : finalize_tree,
  compare : custom_compare_default,
  hash : custom_hash_default,
  serialize : custom_serialize_default,
  deserialize : custom_deserialize_default
};

static struct custom_operations TSNode_custom_ops  = {
  identifier : "TSNode handling",
  finalize : custom_finalize_default,
  compare : custom_compare_default,
  hash : custom_hash_default,
  serialize : custom_serialize_default,
  deserialize : custom_deserialize_default
};

CAMLprim value rets_parser_new_json(value unit) {
  CAMLparam0();
  CAMLlocal1(v);

  parser_W parserWrapper;
  TSParser *parser = ts_parser_new();
  parserWrapper.parser = parser;

  v = caml_alloc_custom(&parser_custom_ops, sizeof(parser_W), 0, 1);
  memcpy(Data_custom_val(v), &parserWrapper, sizeof(parser_W));
  ts_parser_set_language(parser, tree_sitter_json());
  CAMLreturn(v);
};

CAMLprim value rets_parser_parse_string(value vParser, value vSource) {
  CAMLparam2(vParser, vSource);
  CAMLlocal1(v);

  parser_W *p = Data_custom_val(vParser);
  TSParser *tsparser = p->parser;

  const char *source_code = String_val(vSource);
  TSTree *tree =
      ts_parser_parse_string(tsparser, NULL, source_code, strlen(source_code));

  tree_W treeWrapper;
  treeWrapper.tree = tree;

  v = caml_alloc_custom(&tree_custom_ops, sizeof(tree_W), 0, 1);
  memcpy(Data_custom_val(v), &treeWrapper, sizeof(tree_W));
  CAMLreturn(v);
};

CAMLprim value rets_tree_root_node(value vTree) {
  CAMLparam1(vTree);
  CAMLlocal1(v);

  tree_W *t = Data_custom_val(vTree);
  TSTree *tree = t->tree;

  TSNode node = ts_tree_root_node(tree);
  v = caml_alloc_custom(&TSNode_custom_ops, sizeof(TSNode), 0, 1);  
  memcpy(Data_custom_val(v), &node, sizeof(TSNode));
  CAMLreturn(v);
};

CAMLprim value rets_node_string(value vNode) {
  CAMLparam1(vNode);
  CAMLlocal1(v);

  TSNode* node = Data_custom_val(vNode);
  char *sz = ts_node_string(*node);
  
  v = caml_copy_string(sz);
  free(sz);

  CAMLreturn(v);
};
