open TestFramework;

open Treesitter;

describe("Parser", ({describe, _}) => {
  describe("allocation", ({test, _}) => {

    test("finalizer gets called", ({expect, _}) => {
      let jsonParser = Parser.json();
      let callCount = ref(0);
      Gc.finalise_last(() => incr(callCount), jsonParser);
      Gc.full_major();

      expect.int(callCount^).toBe(1);
    });
  });
});
