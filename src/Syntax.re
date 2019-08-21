/*
     Syntax.re

     Stubs for bindings to the `TSTree` object
 */

let getErrorRanges = (node: Node.t) => {

    let rec f = (node: Node.t, errors: []) => {
      let hasError = Node.hasError(node);

      switch (hasError) {
      | false => errors
        | true => switch (Node.isError(node)) {
        | false => {
            let children = Node.getChildren(node);
            List.fold_left((prev, curr) => {
              f(curr, prev);
            }, errors, children);
        }
      }
      }
    };

    f(node, []);
};
