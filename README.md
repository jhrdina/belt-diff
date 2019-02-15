# Symmetric Diffs for Belt Standard Library

ReasonML implementation of fast symmetric diffs for Sets and Maps. Library is designed to be used together with with [Belt](https://bucklescript.github.io/bucklescript/api/Belt.html) standard library that is shipped with [BuckleScript](https://bucklescript.github.io/).

Algorithm was inspired by Jane Street's [Base](https://github.com/janestreet/base) library _(that unfortunately cannot be used in BuckleScript directly)_.

## Installation

Install the package using `npm`:

```
npm install belt-diff
```

Add dependency to your `bsconfig.json`:

```
"bs-dependencies": [
  "belt-diff"
],
```

## Usage

For more examples see `examples` folder.

__Note:__ Diffing function is combined together with folding/reducing functionality to allow convenient direct accumulation of changes.

### Set

Following example for string set accumulates all added values into list:

```rust
let oldSet = Belt.Set.String.fromArray([|"A", "B", "C"|]);
let newSet = Belt.Set.String.(oldSet->remove("A")->remove("C")->add("D")->add("C"));
let addedItems = BeltDiff.Set.String.symmetricDiff(oldSet, newSet, ~acc=[], ~f=(either, acc) =>
  switch (either) {
  | Left(a) =>
    Js.log("Element " ++ a ++ " is only in the old set.");
    acc;
  | Right(a) =>
    Js.log("Element " ++ a ++ " is only in the new set.");
    [a, ...acc];
  }
);
// > Element A is only in the old set.
//   Element D is only in the new set.

Js.log(Array.of_list(addedItems));
// > [ 'D' ]
```

### Map

Argument `~veq` specifies function for detecting equal values with signature `('value, 'value) => bool`.

Following example for string map accumulates all added values:

```rust
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
// > Item with key A is only in the old map.
//   Item with key C is in both, but its value was changed from 3 to 5.
//   Item with key D is only in the new map.

Js.log(Array.of_list(addedItems));
// > [ 4 ]
```



## Development

### Build

```
npm run build
```

### Build + Watch

```
npm run start
```

### Run example

E.g.:

```
node lib/js/examples/MapDiffExample.js
```
