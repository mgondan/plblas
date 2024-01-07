#include <armadillo>
#include <SWI-cpp2.h>

using namespace arma ;

struct Matrix ;
struct Column ;

static PL_blob_t matrix = PL_BLOB_DEFINITION(Matrix, "matrix") ;
static PL_blob_t column = PL_BLOB_DEFINITION(Column, "column") ;

struct Matrix : public PlBlob
{ mat m ;

  explicit Matrix()
    : PlBlob(&matrix),
      m()
  { }

  explicit Matrix(const mat& am)
    : PlBlob(&matrix),
      m(am)
  { }

  PL_BLOB_SIZE

  void portray(PlStream& strm) const ;

  double& operator()(uword r, uword c)
  { return m(r, c) ;
  }
} ;

struct Column : public PlBlob
{ vec v ;

  explicit Column()
    : PlBlob(&column),
      v()
  { }

  explicit Column(const vec& av)
    : PlBlob(&column),
      v(av)
  { }

  PL_BLOB_SIZE

  void portray(PlStream& strm) const ;

  double& operator()(uword i)
  { return v(i) ;
  }
} ;

PREDICATE(matrix, 3)
{ mat m(A1.as_int32_t(), A2.as_int32_t()) ;
  auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
  return A3.unify_blob(&ref) ;
}

static foreign_t plblas_portray(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(((PlTerm) a1).is_blob(&t) == false)
    return false ;

  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex((PlTerm) a1, matrix) ;
    PlTerm cout = PlTerm_atom("current_output") ;
    PlStream s(cout, SIO_OUTPUT) ;
    ref->portray(s) ;
    return true ;
  }

  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex((PlTerm) a1, column) ;
    PlTerm cout = PlTerm_atom("current_output") ;
    PlStream s(cout, SIO_OUTPUT) ;
    ref->portray(s) ;
    return true ;
  }

  return false ;
}

PlRegister x_plblas_portray_1("user", "portray", 1, plblas_portray) ;

void Matrix::portray(PlStream& s) const
{ s.printf("Matrix(rows=%u cols=%u)\n", m.n_rows, m.n_cols) ;
  for(uword i=0; i<m.n_rows; i++)
  {
    for(uword j=0; j<m.n_cols; j++)
      s.printf(" %.3f", m(i, j)) ;
    s.printf("\n") ;
  }
  s.printf("\n") ;
}

static foreign_t plblas_zeros(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(((PlTerm) a1).is_blob(&t) == false)
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex((PlTerm) a1, matrix) ;
    ref->m.zeros() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex((PlTerm) a1, column) ;
    ref->v.zeros() ;
    return true ;
  }
 
  return false ;
}

PlRegister x_plblas_zeros_1(NULL, "zeros", 1, plblas_zeros) ;

static foreign_t plblas_ones(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(((PlTerm) a1).is_blob(&t) == false)
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex((PlTerm) a1, matrix) ;
    ref->m.ones() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex((PlTerm) a1, column) ;
    ref->v.ones() ;
    return true ;
  }
 
  return false ;
}

PlRegister x_plblas_ones_1(NULL, "ones", 1, plblas_ones) ;

static foreign_t plblas_eye(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(((PlTerm) a1).is_blob(&t) == false)
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex((PlTerm) a1, matrix) ;
    ref->m.eye() ;
    return true ;
  }

  if(t == &column)
    PlTypeError("Matrix", (PlTerm) a1) ;
 
  return false ;
}

PlRegister x_plblas_eye_1(NULL, "eye", 1, plblas_eye) ;

static foreign_t plblas_randu(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(((PlTerm) a1).is_blob(&t) == false)
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex((PlTerm) a1, matrix) ;
    ref->m.randu() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex((PlTerm) a1, column) ;
    ref->v.randu() ;
    return true ;
  }
 
  return false ;
}

PlRegister x_plblas_randu_1(NULL, "randu", 1, plblas_randu) ;

static foreign_t plblas_randn(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(((PlTerm) a1).is_blob(&t) == false)
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex((PlTerm) a1, matrix) ;
    ref->m.randn() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex((PlTerm) a1, column) ;
    ref->v.randn() ;
    return true ;
  }
 
  return false ;
}

PlRegister x_plblas_randn_1(NULL, "randn", 1, plblas_randn) ;


PREDICATE(matrix_fill, 2)
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  double f = A2.as_double() ;
  ref->m.fill(f) ;
  return true ;
}

PREDICATE(column, 2)
{ vec v(A1.as_int32_t()) ;
  auto ref = std::unique_ptr<PlBlob>(new Column(v)) ;
  return A2.unify_blob(&ref);
}

void Column::portray(PlStream& s) const
{ s.printf("Column(rows=%u)\n", v.n_rows) ;
  for(uword i=0; i<v.n_rows; i++)
    s.printf(" %.3f\n", v(i)) ;
  s.printf("\n") ;
}

PREDICATE(column_fill, 2)
{ auto ref = PlBlobV<Column>::cast_ex(A1, column) ;
  double f = A2.as_double() ;
  ref->v.fill(f) ;
  return true ;
}

PREDICATE(mult, 3)
{ auto a = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  auto b = PlBlobV<Matrix>::cast_ex(A2, matrix) ;
  mat m = a->m * b->m ;
  auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
  return A3.unify_blob(&ref) ;
}

PREDICATE(matrix_get0, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = ref->m(r, c) ;
  return A4.unify_float(x) ;
}

PREDICATE(matrix_get1, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = ref->m(--r, --c) ;
  return A4.unify_float(x) ;
}

PREDICATE(matrix_put0, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = A4.as_double() ;
  ref->m(r, c) = x ;
  return true ;
}

PREDICATE(matrix_put1, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = A4.as_double() ;
  ref->m(--r, --c) = x ;
  return true ;
}

PREDICATE(column_get0, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = ref->v(i) ;
  return A3.unify_float(x) ;
}

PREDICATE(column_get1, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = ref->v(--i) ;
  return A3.unify_float(x) ;
}

PREDICATE(column_put0, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = A3.as_double() ;
  ref->v(i) = x ;
  return true ;
}

PREDICATE(column_put1, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = A3.as_double() ;
  ref->v(--i) = x ;
  return true ;
}
