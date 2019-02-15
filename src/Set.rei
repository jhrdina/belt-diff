type either('a) =
  | Left('a)
  | Right('a);

let symmetricDiff:
  (
    Belt.Set.t('v, 'id),
    Belt.Set.t('v, 'id),
    ~f: (either('v), 'acc) => 'acc,
    ~acc: 'acc
  ) =>
  'acc;

module Int: {
  let symmetricDiff:
    (
      Belt.Set.Int.t,
      Belt.Set.Int.t,
      ~f: (either('v), 'acc) => 'acc,
      ~acc: 'acc
    ) =>
    'acc;
};

module String: {
  let symmetricDiff:
    (
      Belt.Set.String.t,
      Belt.Set.String.t,
      ~f: (either('v), 'acc) => 'acc,
      ~acc: 'acc
    ) =>
    'acc;
};