let oldMap = Belt.Map.String.fromArray([|("A", 1), ("B", 2), ("C", 3)|]);
let newMap =
  Belt.Map.String.(oldMap->remove("A")->set("D", 4)->set("C", 5));

let addedItems =
  BeltDiff.Map.String.symmetricDiff(
    oldMap, newMap, ~acc=[], ~veq=(==), ~f=(diffRes, acc) =>
    switch (diffRes) {
    | (key, Left(_a)) =>
      Js.log("Item with key " ++ key ++ " is only in the old map.");
      acc;
    | (key, Right(a)) =>
      Js.log("Item with key " ++ key ++ " is only in the new map.");
      [a, ...acc];
    | (key, Unequal(a, b)) =>
      Js.log(
        "Item with key "
        ++ key
        ++ " is in both, but its value was changed from "
        ++ string_of_int(a)
        ++ " to "
        ++ string_of_int(b)
        ++ ".",
      );
      acc;
    }
  );
/*
 > Item with key A is only in the old map.
   Item with key C is in both, but its value was changed from 3 to 5.
   Item with key D is only in the new map.
 */

Js.log(Array.of_list(addedItems));
/*
 > [ 4 ]
 */