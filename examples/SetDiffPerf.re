Js.log("Generating...");
let gen = max => {
  let arr = Array.make(max, 0);
  for (i in 0 to max - 1) {
    arr->Array.set(i, Random.int(max * 5));
  };
  arr;
};

let a = Belt.Set.Int.fromArray @@ gen(1000000);
let b = Belt.Set.Int.(a->add(800421)->add(420420));

Js.log("generated.");

let res =
  BeltDiff.Set.Int.symmetricDiff(a, b, ~acc=[], ~f=(either, acc) =>
    switch (either) {
    | Left(a) =>
      Js.log("Left " ++ string_of_int(a));
      [a, ...acc];
    | Right(a) =>
      Js.log("Right " ++ string_of_int(a));
      [a, ...acc];
    }
  );
Js.log(Array.of_list(res));