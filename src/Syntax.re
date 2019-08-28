/*
     Syntax.re

    Helpers to connect TSNode with a syntax highlighting solution
 */

open Types;

let getErrorRanges = (node: Node.t) => {
  let rec f = (node: Node.t, errors: list(Range.t)) => {
    let hasError = Node.hasError(node);

    switch (hasError) {
    | false => errors
    | true =>
      let isError = Node.isError(node);
      isError
        ? [Node.getRange(node), ...errors]
        : {
          let children = Node.getChildren(node);
          let i = ref(0);
          List.fold_left(
            (prev, curr) => {
              incr(i);
              f(curr, prev);
            },
            errors,
            children,
          );
        };
    };
  };

  f(node, []);
};
type scope = (int, string);

module Token = {
  type t = (Position.t, Position.t, list(scope), string);

  let ofNode = (~getTokenName, scopes, node: Node.t) => {
    let isNamed = Node.isNamed(node);
    let position = Node.getStartPoint(node);
    let endPosition = Node.getEndPoint(node);
    let range = Range.create(~startPosition=position, ~endPosition, ());
    let tokenName = getTokenName(range);

    switch (isNamed) {
    | false => (position, endPosition, scopes, tokenName)
    | true =>
      let nodeType = Node.getType(node);
      let scopes = [(0, nodeType), ...scopes];
      (position, endPosition, scopes, tokenName);
    };
  };

  let getPosition = (v: t) => {
    let (p, _, _, _) = v;
    p;
  };

  let getEndPosition = (v: t) => {
    let (_, e, _, _) = v;
    e;
  };

  let getName = (v: t) => {
    let (_, _, _, n) = v;
    n;
  };

  let _showScope = (scope: scope) => {
    let (idx, s) = scope;
    "(" ++ string_of_int(idx) ++ ":" ++ s ++ ")";
  };

  let show = (v: t) => {
    let (p, e, scopes, tok) = v;
    let stringScopes = List.map(_showScope, scopes);
    let scopes = String.concat(".", stringScopes);
    "Token("
    ++ Position.show(p)
    ++ " - "
    ++ Position.show(e)
    ++ ":"
    ++ scopes
    ++ "|"
    ++ tok
    ++ ")";
  };
};

let getParentScopes = (node: Node.t) => {
  let rec f = (node: Node.t, scopes: list(scope)) => {
    let parent = Node.getParent(node);

    if (Node.isNull(parent)) {
      scopes;
    } else {
      Node.isNamed(parent)
        ? f(
            parent,
            [
              (Node.getBoundedNamedIndex(parent), Node.getType(parent)),
              ...scopes,
            ],
          )
        : f(parent, scopes);
    };
  };

  f(node, []);
};

let createArrayTokenNameResolver = (v: array(string), range: Range.t) =>
  if (range.startPosition.line != range.endPosition.line) {
    "";
  } else if (range.startPosition.line >= Array.length(v)) {
    "";
  } else {
    let lineNumber = range.startPosition.line;
    let line = v[lineNumber];

    let len = String.length(line);

    if (len == 0 || range.startPosition.column == range.endPosition.column) {
      "";
    } else {
      "\""
      ++ String.sub(
           line,
           range.startPosition.column,
           range.endPosition.column - range.startPosition.column,
         )
      ++ "\"";
    };
  };

let getTokens = (~getTokenName, ~range: Range.t, node: Node.t) => {
  let nodeToUse =
    Node.getDescendantForPointRange(
      node,
      range.startPosition.line,
      range.startPosition.column,
      range.endPosition.line,
      range.endPosition.column,
    );

  let parentScopes = getParentScopes(node) |> List.rev;

  let rec f = (index, n: Node.t, tokens: list(Token.t), scopes: list(scope)) => {
    let startPosition = Node.getStartPoint(n);
    let endPosition = Node.getEndPoint(n);

    if (endPosition.line < range.startPosition.line
        || endPosition.line == range.startPosition.line
        && endPosition.column < range.startPosition.column
        || startPosition.line > range.endPosition.line
        || startPosition.line == range.endPosition.line
        && endPosition.column > range.endPosition.column) {
      tokens;
    } else {
      let childCount = Node.getChildCount(n);

      switch (childCount) {
      | 0 => [Token.ofNode(~getTokenName, scopes, n), ...tokens]
      | _ =>
        let children = Node.getChildren(n);
        let newScopes =
          switch (Node.isNamed(n)) {
          | false => scopes
          | true => [(index, Node.getType(n)), ...scopes]
          };
        let (_, tokens) =
          List.fold_left(
            (prev, curr) => {
              let (index, tokens) = prev;
              let idx =
                Node.isNamed(curr) ? Node.getBoundedNamedIndex(curr) : index;
              let newTokens = f(idx, curr, tokens, newScopes);
              (idx, newTokens);
            },
            (0, tokens),
            children,
          );
        tokens;
      };
    };
  };

  f(0, nodeToUse, [], parentScopes) |> List.rev;
};
