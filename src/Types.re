/*
     Types.re

     Helpers to support syntax highlight solutions
 */

module Position = {
  type t = {
    line: int,
    column: int,
  };

  let create = (~line, ~column, ()) => {line, column};

  let show = (v: t) => {
    string_of_int(v.line) ++ "," ++ string_of_int(v.column)
  }
};

module Range = {
  type t = {
    startPosition: Position.t,
    endPosition: Position.t,
  };

  let create = (~startPosition, ~endPosition, ()) => {
    startPosition,
    endPosition,
  };
};
