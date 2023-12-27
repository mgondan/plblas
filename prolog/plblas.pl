:- module(plblas, [test_plblas/0]).

:- load_foreign_library(foreign(plblas)).

:- multifile user:portray/1.

user:portray(Matrix) :- 
    blob(Matrix, matrix), 
    !,
    matrix_portray(current_output, Matrix).

test_plblas :-
  true.

:- begin_tests(plblas).
:- use_module(library(plblas)).

test(plblas) :-
  test_plblas.

:- end_tests(plblas).
