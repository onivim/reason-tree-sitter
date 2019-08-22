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

module Token = {
  type t =
    | NamedToken(Position.t, list(string), string, string)
    | UnnamedToken(Position.t, list(string), string);

  let ofNode = (~getTokenName, scopes, node: Node.t) => {
    let isNamed = Node.isNamed(node);
    let position = Node.getStartPoint(node);
    let endPosition = Node.getEndPoint(node);
    let range = Range.create(~startPosition=position, ~endPosition, ());
    let tokenName = getTokenName(range);

    switch (isNamed) {
    | false => UnnamedToken(position, scopes, tokenName)
    | true =>
      let nodeType = Node.getType(node);
      NamedToken(position, scopes, nodeType, tokenName);
    };
  };

  let show = (v: t) => {
    switch (v) {
    | NamedToken(p, scopes, s, tok) =>
      let scopes = String.concat(".", scopes) ++ ".";

      "NamedToken("
      ++ Position.show(p)
      ++ ":"
      ++ scopes
      ++ s
      ++ "|"
      ++ tok
      ++ ")";
    | UnnamedToken(p, scopes, tok) =>
      let scopes = String.concat(".", scopes) ++ ".";
      "UnnamedToken(" ++ Position.show(p) ++ ":" ++ scopes ++ tok ++ ")";
    };
  };
};

let getParentScopes = (node: Node.t) => {
  let rec f = (node: Node.t, scopes: list(string)) => {
    let parent = Node.getParent(node);

    if (Node.isNull(parent)) {
      scopes;
    } else {
      Node.isNamed(parent)
        ? f(parent, [Node.getType(parent), ...scopes]) : f(parent, scopes);
    };
  };

  f(node, []);
};

let createArrayTokenNameResolver = (v: array(string), range: Range.t) =>
  if (range.startPosition.line != range.endPosition.line) {
    "";
  } else {
    let lineNumber = range.startPosition.line;
    let line = v[lineNumber];

    "\""
    ++ String.sub(
         line,
         range.startPosition.column,
         range.endPosition.column - range.startPosition.column,
       )
    ++ "\"";
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

  let rec f = (n: Node.t, tokens: list(Token.t), scopes: list(string)) => {
    let startPosition = Node.getStartPoint(n);
    let endPosition = Node.getEndPoint(n);

    if (!Range.isInRange(range, startPosition)
        && !Range.isInRange(range, endPosition)) {
      tokens;
    } else {
      let childCount = Node.getChildCount(n);

      switch (childCount) {
      | 0 => [Token.ofNode(~getTokenName, List.rev(scopes), n), ...tokens]
      | _ =>
        let children = Node.getChildren(n);
        let newScopes =
          switch (Node.isNamed(n)) {
          | false => scopes
          | true => [Node.getType(n), ...scopes]
          };
        List.fold_left(
          (prev, curr) => f(curr, prev, newScopes),
          tokens,
          children,
        );
      };
    };
  };

  f(nodeToUse, [], parentScopes);
};
