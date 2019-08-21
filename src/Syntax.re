/*
     Syntax.re

    Helpers to connect TSNode with a syntax highlighting solution
 */

open Types;

let getErrorRanges = (node: Node.t) => {

    print_endline("getErrorRanges: ");

    let rec f = (node: Node.t, errors: list(Range.t) ) => {
      let hasError = Node.hasError(node);

      switch (hasError) {
      | false => errors
        | true => 
          print_endline ("Checking isError...");
          let isError = Node.isError(node);
          print_endline ("isError: " ++ string_of_bool(isError));
          switch (isError) {
            | true => [Node.getRange(node), ...errors]
            | false => {
                print_endline ("getting children...");
                let children = Node.getChildren(node);
                print_endline ("got children!");
                let i = ref(0);
                List.fold_left((prev, curr) => {
                  print_endline ("checking idx: " ++ string_of_int(i^));
                  incr(i);
                  f(curr, prev);
                }, errors, children);
            }
      }
      }
    };

    f(node, []);
};
