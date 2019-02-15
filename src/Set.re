// Inspired by symmetric_diff in https://github.com/janestreet/base/blob/master/src/set.ml

module N = Belt_internalAVLset;

type either('a) =
  | Left('a)
  | Right('a);

type enumeration('value) =
  | End
  | More('value, N.t('value), enumeration('value));

let rec cons = (s, e) =>
  switch (s->N.toOpt) {
  | None => e
  | Some(t) => cons(t->N.leftGet, More(t->N.valueGet, t->N.rightGet, e))
  };

let ofSet = t => cons(t, End);

let symmetricDiffAux:
  (
    N.t('value),
    N.t('value),
    ~f: (either('value), 'acc) => 'acc,
    ~cmp: (. 'value, 'value) => int,
    ~acc: 'acc
  ) =>
  'acc =
  (t1, t2, ~f, ~cmp, ~acc) => {
    let rec step = (~state, ~acc) => {
      switch (state) {
      | (End, End) => acc
      | (End, More(elt, tree, enum)) =>
        step(~acc=f(Right(elt), acc), ~state=(End, cons(tree, enum)))
      | (More(elt, tree, enum), End) =>
        step(~acc=f(Left(elt), acc), ~state=(cons(tree, enum), End))
      | (More(a1, tree1, enum1) as left, More(a2, tree2, enum2) as right) =>
        let compare_result = cmp(. a1, a2);
        if (compare_result == 0) {
          let next_state =
            if (tree1 === tree2) {
              (enum1, enum2);
            } else {
              (cons(tree1, enum1), cons(tree2, enum2));
            };

          step(~acc, ~state=next_state);
        } else if (compare_result < 0) {
          step(~acc=f(Left(a1), acc), ~state=(cons(tree1, enum1), right));
        } else {
          step(~acc=f(Right(a2), acc), ~state=(left, cons(tree2, enum2)));
        };
      };
    };
    step(~state=(ofSet(t1), ofSet(t2)), ~acc);
  };

let cmpFromId = (type k, type idx, id: Belt.Map.id(k, idx)) => {
  module M = (val id);
  M.cmp;
};

let symmetricDiff: (Belt.Set.t('v, 'id), Belt.Set.t('v, 'id)) => 'acc =
  (t1, t2) => {
    symmetricDiffAux(
      t1->Belt.Set.getData->Obj.magic,
      t2->Belt.Set.getData->Obj.magic,
      ~cmp=t1->Belt.Set.getId->cmpFromId->Belt_Id.getCmpInternal,
    );
  };

module String = {
  let symmetricDiff: (Belt.Set.String.t, Belt.Set.String.t) => 'acc =
    (t1, t2) => {
      symmetricDiffAux(t1->Obj.magic, t2->Obj.magic, ~cmp=(. a, b) =>
        Pervasives.compare(a, b)
      );
    };
};

module Int = {
  let symmetricDiff: (Belt.Set.Int.t, Belt.Set.Int.t) => 'acc =
    (t1, t2) => {
      symmetricDiffAux(t1->Obj.magic, t2->Obj.magic, ~cmp=(. a, b) =>
        Pervasives.compare(a, b)
      );
    };
};