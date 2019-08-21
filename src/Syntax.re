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

module Token {
  type t = 
  | NamedToken(Position.t, list(string), string)
  | UnnamedToken(Position.t, list(string));

  let ofNode = (node: Node.t) => {
    let isNamed = Node.isNamed(node);
    let position = Node.getStartPoint(node);
    let _endPosition = Node.getEndPoint(node);

    switch (isNamed) {
    | false => UnnamedToken(position, []);
    | true => {
      let nodeType = Node.getType(node);
      NamedToken(position, [], nodeType);
    }
    }
  };

  let show = (v: t) => {
    switch (v) {
    | NamedToken(p, _, s) => "NamedToken(" ++ Position.show(p) ++ ":"  ++ s ++ ")"
    | UnnamedToken(p, _) => "UnnamedToken(" ++ Position.show(p) ++ ")"
    }
  }
}

let getParentScopes = (node: Node.t) => {
  
  let rec f = (node: Node.t, scopes: list(string)) => {

    prerr_endline ("Getting parent...");
    let parent = Node.getParent(node);
    
    prerr_endline ("Checking null...");
    if (Node.isNull(parent)) {
      prerr_endline ("null");
      scopes
    } else {
    prerr_endline ("not null");
      switch(Node.isNamed(parent)) {
      | true => f(parent, [Node.getType(parent), ...scopes])
      | false => f(parent, scopes)
      }
    }
  };

  f(node, []);
};

let getTokens = (node: Node.t) => {
  let rec f = (node: Node.t, tokens: list(Token.t)) => {
    let childCount = Node.getChildCount(node);

    switch (childCount) {
    | 0 => [Token.ofNode(node), ...tokens]
    | _ =>  let children = Node.getChildren(node);
    List.fold_left((prev, curr) => {
      f(curr, prev); 
    }, tokens, children);
    }
  };

  f(node, []);
};
