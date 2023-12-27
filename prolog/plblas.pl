:- module(plblas, []).

:- load_foreign_library(foreign(plblas)).

:- multifile user:portray/1.

user:portray(Matrix) :- 
    blob(Matrix, matrix), 
    !,
    matrix_portray(current_output, Matrix).

:- begin_tests(plblas).

test(mult) :-
  matrix(2, 3, A), ones(A),
  matrix(3, 2, B), ones(B),
  mult(A, B, X),
  get1(1, 1, X, 3.0).

:- end_tests(plblas).
