:- module(plblas, [zeros/1, ones/1, eye/1, randu/1, randn/1, fill/2, get0/4, get1/4, put0/4, put1/4, get0/3, get1/3, put0/3, put1/3]).

:- multifile user:portray/1.
:- load_foreign_library(foreign(plblas)).

%user:portray(X) :-
%    ( blob(X, matrix)
%    ; blob(X, column)
%    ),
%    !, plblas_portray(X).

zeros(Matrix) :-
    blob(Matrix, matrix),
    !, matrix_zeros(Matrix).

zeros(Column) :-
    blob(Column, column),
    !, column_zeros(Column).

ones(Matrix) :-
    blob(Matrix, matrix),
    !, matrix_ones(Matrix).

ones(Column) :-
    blob(Column, column),
    !, column_ones(Column).

eye(Matrix) :-
    blob(Matrix, matrix),
    !, matrix_eye(Matrix).

randu(Matrix) :-
    blob(Matrix, matrix),
    !, matrix_randu(Matrix).

randu(Column) :-
    blob(Column, column),
    !, column_randu(Column).

randn(Matrix) :-
    blob(Matrix, matrix),
    !, matrix_randn(Matrix).

randn(Column) :-
    blob(Column, column),
    !, column_randn(Column).

fill(Matrix, Value) :-
    blob(Matrix, matrix),
    !, matrix_fill(Matrix, Value).

fill(Column, Value) :-
    blob(Column, column),
    !, column_fill(Column, Value).

get0(Row, Col, Matrix, Value) :-
    blob(Matrix, matrix),
    !, matrix_get0(Row, Col, Matrix, Value).

get1(Row, Col, Matrix, Value) :-
    blob(Matrix, matrix),
    !, matrix_get1(Row, Col, Matrix, Value).

put0(Row, Col, Matrix, Value) :-
    blob(Matrix, matrix),
    !, matrix_put0(Row, Col, Matrix, Value).

put1(Row, Col, Matrix, Value) :-
    blob(Matrix, matrix),
    !, matrix_put1(Row, Col, Matrix, Value).

get0(Index, Column, Value) :-
    blob(Column, column),
    !, column_get0(Index, Column, Value).

get1(Index, Column, Value) :-
    blob(Column, column),
    !, column_get1(Index, Column, Value).

put0(Index, Column, Value) :-
    blob(Column, column),
    !, column_put0(Index, Column, Value).

put1(Index, Column, Value) :-
    blob(Column, column),
    !, column_put1(Index, Column, Value).

:- begin_tests(plblas).

test(mult) :-
  matrix(2, 3, A), ones(A),
  matrix(3, 2, B), ones(B),
  mult(A, B, X),
  get1(1, 1, X, 3.0).

:- end_tests(plblas).
