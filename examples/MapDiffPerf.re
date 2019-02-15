Js.log("Generating...");
let gen = max => {
  let arr = Array.make(max, (0, 0));
  for (i in 0 to max - 1) {
    arr->Array.set(i, (Random.int(max * 5), Random.int(max * 5)));
  };
  arr;
};

let a = Belt.Map.Int.fromArray @@ gen(500000);
let b = Belt.Map.Int.(a->set(420420, 54)->set(42, 51));
let a = Belt.Map.Int.(a->set(800421, 42)->set(42, 81));

Js.log("generated.");

let res =
  BeltDiff.Map.Int.symmetricDiff(a, b, ~veq=(==), ~acc=[], ~f=(either, acc) =>
    switch (either) {
    | (k, Left(a)) =>
      Js.log("Key " ++ string_of_int(k) ++ ", Left " ++ string_of_int(a));
      [a, ...acc];
    | (k, Right(a)) =>
      Js.log("Key " ++ string_of_int(k) ++ ", Right " ++ string_of_int(a));
      [a, ...acc];
    | (k, Unequal(a, b)) =>
      Js.log(
        "Key "
        ++ string_of_int(k)
        ++ ", Unequal "
        ++ string_of_int(a)
        ++ ","
        ++ string_of_int(b),
      );
      [a, b, ...acc];
    }
  );
Js.log(Array.of_list(res));