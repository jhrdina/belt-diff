// Inspired by symmetric_diff in https://github.com/janestreet/base/blob/master/src/map.ml

module N = Belt_internalAVLtree;

type diffResult('a) =
  | Left('a)
  | Right('a)
  | Unequal('a, 'a);

type enumeration('k, 'v) =
  | End
  | More('k, 'v, N.t('k, 'v), enumeration('k, 'v));

let rec cons = (s, e) =>
  switch (s->N.toOpt) {
  | None => e
  | Some(t) =>
    cons(t->N.leftGet, More(t->N.keyGet, t->N.valueGet, t->N.rightGet, e))
  };

let ofTree = t => cons(t, End);

let symmetricDiffAux:
  (
    N.t('k, 'v),
    N.t('k, 'v),
    ~f: (('k, diffResult('v)), 'acc) => 'acc,
    ~kcmp: (. 'k, 'k) => int,
    ~veq: ('v, 'v) => bool,
    ~acc: 'acc
  ) =>
  'acc =
  (t1, t2, ~f, ~kcmp, ~veq, ~acc) => {
    let rec step = (~state, ~acc) => {
      switch (state) {
      | (End, End) => acc
      | (End, More(key, data, tree, enum)) =>
        step(
          ~acc=f((key, Right(data)), acc),
          ~state=(End, cons(tree, enum)),
        )
      | (More(key, data, tree, enum), End) =>
        step(
          ~acc=f((key, Left(data)), acc),
          ~state=(cons(tree, enum), End),
        )
      | (
          More(k1, v1, tree1, enum1) as left,
          More(k2, v2, tree2, enum2) as right,
        ) =>
        let compare_result = kcmp(. k1, k2);
        if (compare_result == 0) {
          let next_state =
            if (tree1 === tree2) {
              (enum1, enum2);
            } else {
              (cons(tree1, enum1), cons(tree2, enum2));
            };

          if (veq(v1, v2)) {
            step(~acc, ~state=next_state);
          } else {
            step(~acc=f((k1, Unequal(v1, v2)), acc), ~state=next_state);
          };
        } else if (compare_result < 0) {
          step(
            ~acc=f((k1, Left(v1)), acc),
            ~state=(cons(tree1, enum1), right),
          );
        } else {
          step(
            ~acc=f((k2, Right(v2)), acc),
            ~state=(left, cons(tree2, enum2)),
          );
        };
      };
    };
    step(~state=(ofTree(t1), ofTree(t2)), ~acc);
  };

let cmpFromId = (type k, type idx, id: Belt.Map.id(k, idx)) => {
  module M = (val id);
  M.cmp;
};

let symmetricDiff: (Belt.Map.t('k, 'v, 'id), Belt.Map.t('k, 'v, 'id)) => 'acc =
  (t1, t2) => {
    symmetricDiffAux(
      t1->Belt.Map.getData->Obj.magic,
      t2->Belt.Map.getData->Obj.magic,
      ~kcmp=t1->Belt.Map.getId->cmpFromId->Belt_Id.getCmpInternal,
    );
  };

module String = {
  let symmetricDiff: (Belt.Map.String.t('v), Belt.Map.String.t('v)) => 'acc =
    (t1, t2) => {
      symmetricDiffAux(t1->Obj.magic, t2->Obj.magic, ~kcmp=(. a, b) =>
        Pervasives.compare(a, b)
      );
    };
};

module Int = {
  let symmetricDiff: (Belt.Map.Int.t('v), Belt.Map.Int.t('v)) => 'acc =
    (t1, t2) => {
      symmetricDiffAux(t1->Obj.magic, t2->Obj.magic, ~kcmp=(. a, b) =>
        Pervasives.compare(a, b)
      );
    };
};