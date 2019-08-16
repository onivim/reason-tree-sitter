open Treesitter;
open BenchFramework;

Printexc.record_backtrace(true);

let dir = Filename.dirname(Sys.argv[0]);

let jsonParser = Parser.json();
let simpleJson = "[1, \"2\"]";
let read_file = filename => {
  let lines = ref([]);
  let chan = open_in(filename);
  try (
    {
      while (true) {
        print_endline("Reading line...");
        lines := [input_line(chan), ...lines^];
        print_endline("Line read!");
      };
      lines^
    }
  ) {
  | End_of_file =>
    close_in(chan);
    print_endline ("EOF");
    List.rev(lines^)
  };
};

print_endline ("Starting to read large file...");
let largeJson = read_file(dir ++ "/" ++ "canada.json");
let largeJsonString = largeJson |> String.concat("\n");
let largeJsonArray = largeJson |> Array.of_list;
print_endline ("Finished reading");

let parse = (v: string) => () => {
  let _ = Parser.parseString(jsonParser, v);
};

let parseArray = (v: array(string)) => () => {
  let _ = Parser.parseArray(jsonParser, None, v);
};

let setup = () => ();
let options = Reperf.Options.create(~iterations=10, ());

bench(
  ~name="parseString: Small JSON",
  ~options,
  ~setup,
  ~f=parse(simpleJson),
  (),
);

bench(
  ~name="parseString: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=parse(largeJsonString),
  (),
);

bench(
  ~name="parseArray: Large JSON (canada.json)",
  ~options,
  ~setup,
  ~f=parseArray(largeJsonArray),
  (),
);
