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
    string_of_int(v.line) ++ "," ++ string_of_int(v.column);
  };
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

  let createi = (
    ~startLine,
    ~startColumn,
    ~endLine,
    ~endColumn,
    ()
  ) => {
    create(
    ~startPosition=Position.create(~line=startLine, ~column=startColumn, ()),
    ~endPosition=Position.create(~line=endLine, ~column=endColumn, ()),
    ()
    );
  };

  let isInRange = (range: t, position: Position.t) => {
    (
      position.line == range.startPosition.line
      && position.column >= range.startPosition.column
      || position.line > range.startPosition.line
    )
    && (
      position.line == range.endPosition.line
      && position.column <= range.endPosition.column
      || position.line < range.endPosition.line
    );
  };

  let show = (v: t) => {
    "Range - start: "
    ++ Position.show(v.startPosition)
    ++ " end: "
    ++ Position.show(v.endPosition);
  };
};
