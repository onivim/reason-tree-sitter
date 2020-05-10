let dir = Filename.dirname(Sys.argv[0]);

let read_file = filename => {
  let lines = ref([]);
  let chan = open_in(filename);
  try(
    {
      while (true) {
        // print_endline("Reading line...");
        lines := [input_line(chan), ...lines^];
                                                // print_endline("Line read!");
      };
      lines^;
    }
  ) {
  | End_of_file =>
    close_in(chan);
    print_endline("Finished reading " ++ filename);
    List.rev(lines^);
  };
};

print_endline("Loading test data prior to benchmark...");
let largeJson = read_file(dir ++ "/" ++ "canada.json");
let largeJsonString = largeJson |> String.concat("\n");
let largeJsonArray = largeJson |> Array.of_list;

let largeC = read_file(dir ++ "/" ++ "sqlite3.c");
let largeCString = largeC |> String.concat("\n");
let largeCArray = largeC |> Array.of_list;

let largeCPP = read_file(dir ++ "/" ++ "imgui.cpp");
let largeCPPString = largeCPP |> String.concat("\n");
let largeCPPArray = largeCPP |> Array.of_list;

let largeJS = read_file(dir ++ "/" ++ "react-dom.development.js");
let largeJSString = largeJS |> String.concat("\n");
let largeJSArray = largeJS |> Array.of_list;

let largeTS = read_file(dir ++ "/" ++ "vscode.d.ts");
let largeTSString = largeTS |> String.concat("\n");
let largeTSArray = largeTS |> Array.of_list;

let largePy = read_file(dir ++ "/" ++ "black.py");
let largePyString = largePy |> String.concat("\n");
let largePyArray = largePy |> Array.of_list;
print_endline("Finished loading.");
