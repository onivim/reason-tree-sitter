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

static struct custom_operations TSNode_custom_ops = {
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


const char *rets_read(void *payload, uint32_t byte_offset, TSPoint position,
                 uint32_t *bytes_read) {
  //printf("[READ] byte_offset: %d line: %d col: %d\n", byte_offset, position.row, position.column);

  //printf("call test call...\n");
  value *closure = caml_named_value("rets__parse_read");
  value result = caml_callback3(*closure, Val_int(byte_offset), Val_int(position.row), Val_int(position.column));

  char *ret = NULL;
  *bytes_read = 0;
  if (Is_block(result)) {
    value strVal = Field(result, 0);

    char *str = String_val(strVal);
    *bytes_read = strlen(str);
    ret = str;
  }
  //printf("test call DONE\n");
  
  return ret;
}

CAMLprim value rets_parser_parse(value vParser, value vTree, value vRead) {
  CAMLparam3(vParser, vTree, vRead);
  CAMLlocal1(ret);

  parser_W *p = Data_custom_val(vParser);
  TSParser *tsparser = p->parser;
  
  TSTree *oldTree = NULL;
  // Some(tree)
  if (Is_block(vTree)) {
    tree_W *t = Data_custom_val(Field(vTree, 0));
    oldTree = t->tree;
  }

  TSInput input;
  input.payload = (void *)vRead;
  input.read = &rets_read;
  input.encoding = TSInputEncodingUTF8;

  TSTree *tree = ts_parser_parse(tsparser, oldTree, input);

  tree_W treeWrapper;
  treeWrapper.tree = tree;
  ret = caml_alloc_custom(&tree_custom_ops, sizeof(tree_W), 0, 1);
  memcpy(Data_custom_val(ret), &treeWrapper, sizeof(tree_W));
  
  CAMLreturn(ret);
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

CAMLprim value rets_tree_edit_native(
  value vTree, 
  value vStartByte, 
  value vOldEndByte,
  value vNewEndByte,
  value vStartLine,
  value vOldEndLine,
  value vNewEndLine) {
  CAMLparam5(vTree, vStartByte, vOldEndByte, vNewEndByte, vStartLine);
  CAMLxparam2(vOldEndLine, vNewEndLine);
  
  printf("rets_tree_edit");
  tree_W *t = Data_custom_val(vTree);
  TSTree *tree = t->tree;

  TSInputEdit edit;
  edit.start_byte = Long_val(vStartByte);
  edit.old_end_byte = Long_val(vOldEndByte);
  edit.new_end_byte = Long_val(vNewEndByte);

  edit.start_point.row = Long_val(vStartLine);
  edit.start_point.column = 0;
  edit.old_end_point.row = Long_val(vOldEndLine);
  edit.old_end_point.column = 0;
  edit.new_end_point.row = Long_val(vNewEndLine);
  edit.new_end_point.column = 0;

  ts_tree_edit(tree, &edit);

  CAMLreturn(vTree);
  };


CAMLprim value rets_tree_edit_bytecode(value *argv, int argn) {
  return rets_tree_edit_native(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
}

CAMLprim value rets_node_string(value vNode) {
  CAMLparam1(vNode);
  CAMLlocal1(v);

  TSNode *node = Data_custom_val(vNode);
  char *sz = ts_node_string(*node);

  v = caml_copy_string(sz);
  free(sz);

  CAMLreturn(v);
};

CAMLprim value rets_node_type(value vNode) {
  CAMLparam1(vNode);
  CAMLlocal1(v);

  TSNode *node = Data_custom_val(vNode);
  const char *sz = ts_node_type(*node);

  v = caml_copy_string(sz);
  CAMLreturn(v);
};

CAMLprim value rets_node_child_count(value vNode) {
  CAMLparam1(vNode);
  CAMLlocal1(v);

  TSNode *node = Data_custom_val(vNode);
  uint32_t c = ts_node_child_count(*node);

  CAMLreturn(Val_int(c));
};

CAMLprim value rets_node_child(value vNode, value vX) {
  CAMLparam2(vNode, vX);
  CAMLlocal1(v);

  TSNode *node = Data_custom_val(vNode);
  uint32_t idx = Int_val(vX);

  TSNode child = ts_node_child(*node, idx);
  v = caml_alloc_custom(&TSNode_custom_ops, sizeof(TSNode), 0, 1);
  memcpy(Data_custom_val(v), &child, sizeof(TSNode));
  CAMLreturn(v);
};

CAMLprim value rets_node_named_child_count(value vNode) {
  CAMLparam1(vNode);
  CAMLlocal1(v);

  TSNode *node = Data_custom_val(vNode);
  uint32_t c = ts_node_named_child_count(*node);

  CAMLreturn(Val_int(c));
};

CAMLprim value rets_node_named_child(value vNode, value vX) {
  CAMLparam2(vNode, vX);
  CAMLlocal1(v);

  TSNode *node = Data_custom_val(vNode);
  uint32_t idx = Int_val(vX);

  TSNode child = ts_node_named_child(*node, idx);
  v = caml_alloc_custom(&TSNode_custom_ops, sizeof(TSNode), 0, 1);
  memcpy(Data_custom_val(v), &child, sizeof(TSNode));
  CAMLreturn(v);
};
