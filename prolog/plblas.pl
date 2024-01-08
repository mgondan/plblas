:- module(plblas, [zeros/1, ones/1, eye/1, randu/1, randn/1, fill/2, get0/4, get1/4, put0/4, put1/4, get0/3, get1/3, put0/3, put1/3, sum/3, prod/3]).

:- multifile user:portray/1.
:- load_foreign_library(foreign(plblas)).

:- begin_tests(plblas).

test(mult) :-
  matrix(2, 3, A), ones(A),
  matrix(3, 2, B), ones(B),
  prod(A, B, X),
  get1(1, 1, X, 3.0).

:- end_tests(plblas).
