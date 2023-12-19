:- module(plblas, []).

:- load_foreign_library(plblas).

:- multifile user:portray/1.

user:portray(Matrix) :- 
    blob(Matrix, matrix), 
    !,
    matrix_portray(current_output, Matrix).
