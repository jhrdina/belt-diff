type diffResult('a) =
  | Left('a)
  | Right('a)
  | Unequal('a, 'a);

let symmetricDiff:
  (
    Belt.Map.t('k, 'v, 'id),
    Belt.Map.t('k, 'v, 'id),
    ~f: (('k, diffResult('v)), 'acc) => 'acc,
    ~veq: ('v, 'v) => bool,
    ~acc: 'acc
  ) =>
  'acc;

module Int: {
  let symmetricDiff:
    (
      Belt.Map.Int.t('v),
      Belt.Map.Int.t('v),
      ~f: (('k, diffResult('v)), 'acc) => 'acc,
      ~veq: ('v, 'v) => bool,
      ~acc: 'acc
    ) =>
    'acc;
};

module String: {
  let symmetricDiff:
    (
      Belt.Map.String.t('v),
      Belt.Map.String.t('v),
      ~f: (('k, diffResult('v)), 'acc) => 'acc,
      ~veq: ('v, 'v) => bool,
      ~acc: 'acc
    ) =>
    'acc;
};