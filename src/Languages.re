/*
     Languages.re
 */

type t =
  | Json
  | C
  | Cpp
  | Python
  | Javascript
  | Typescript
  | Tsx;

let getLanguageFromScope = scope =>
  switch (scope) {
  | "source.json" => Some(Json)
  | "source.c" => Some(C)
  | "source.cpp" => Some(Cpp)
  | "source.python" => Some(Python)
  | "source.js" => Some(Javascript)
  | "source.ts" => Some(Typescript)
  | "source.tsx" => Some(Tsx)
  | _ => None
  };
