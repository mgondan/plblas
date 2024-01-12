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

PREDICATE(column, 2)
{ vec v(A1.as_int32_t()) ;
  auto ref = std::unique_ptr<PlBlob>(new Column(v)) ;
  return A2.unify_blob(&ref);
}

static foreign_t plblas_portray(term_t a1, int arity, control_t ctx)
{ PL_blob_t* t ;
  if(static_cast<PlTerm>(a1).is_blob(&t) == false)
    return false ;

  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(static_cast<PlTerm>(a1), matrix) ;
    PlTerm cout = PlTerm_atom("current_output") ;
    PlStream s(cout, SIO_OUTPUT) ;
    ref->portray(s) ;
    return true ;
  }

  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex(static_cast<PlTerm>(a1), column) ;
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
  { for(uword j=0; j<m.n_cols; j++)
      s.printf(" %.3f", m(i, j)) ;
    s.printf("\n") ;
  }
  s.printf("\n") ;
}

void Column::portray(PlStream& s) const
{ s.printf("Column(rows=%u)\n", v.n_rows) ;
  for(uword i=0; i<v.n_rows; i++)
    s.printf(" %.3f\n", v(i)) ;
  s.printf("\n") ;
}

PREDICATE(zeros, 1)
{ PL_blob_t* t ;
  if(!A1.is_blob(&t))
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    ref->m.zeros() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex(A1, column) ;
    ref->v.zeros() ;
    return true ;
  }

  throw PlTypeError("Matrix or Column", A1) ;
}

PREDICATE(ones, 1)
{ PL_blob_t* t ;
  if(!A1.is_blob(&t))
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    ref->m.ones() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex(A1, column) ;
    ref->v.ones() ;
    return true ;
  }

  throw PlTypeError("Matrix or Column", A1) ;
}

PREDICATE(eye, 1)
{ PL_blob_t* t ;
  if(!A1.is_blob(&t))
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    ref->m.eye() ;
    return true ;
  }

  throw PlTypeError("Matrix", A1) ;
}

PREDICATE(randu, 1)
{ PL_blob_t* t ;
  if(!A1.is_blob(&t))
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    ref->m.randu() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex(A1, column) ;
    ref->v.randu() ;
    return true ;
  }

  throw PlTypeError("Matrix or Column", A1) ;
}

PREDICATE(randn, 1)
{ PL_blob_t* t ;
  if(!A1.is_blob(&t))
    return false ;
 
  if(t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    ref->m.randn() ;
    return true ;
  }
 
  if(t == &column)
  { auto ref = PlBlobV<Column>::cast_ex(A1, column) ;
    ref->v.randn() ;
    return true ;
  }

  throw PlTypeError("Matrix or Column", A1) ;
}

PREDICATE(fill, 2)
{ double f = A2.as_double() ;
  PL_blob_t* t ;
  if(A1.is_blob(&t) && t == &matrix)
  { auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    ref->m.fill(f) ;
    return true ;
  }

  if(A1.is_blob(&t) && t == &column)
  { auto ref = PlBlobV<Column>::cast_ex(A1, column) ;
    ref->v.fill(f) ;
    return true ;
  }

  throw PlTypeError("Matrix or Column", A1) ;
}

PREDICATE(sum, 3)
{ PL_blob_t *t1, *t2 ;
  if(A1.is_blob(&t1) && t1 == &matrix && A2.is_blob(&t2) && t2 == &matrix)
  { auto m1 = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    auto m2 = PlBlobV<Matrix>::cast_ex(A2, matrix) ;

    try
    { mat m = m1->m + m2->m ;
      auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
      return A3.unify_blob(&ref) ;
    }

    catch(std::exception& e)
    { throw PlDomainError(e.what(), PlCompound("+", PlTermv(A1, A2))) ;
    }
  }

  if(A1.is_blob(&t1) && t1 == &matrix && A2.is_blob(&t2) && t2 == &column)
  { auto m1 = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    auto m2 = PlBlobV<Column>::cast_ex(A2, column) ;
    mat m = m1->m + m2->v ;
    auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
    return A3.unify_blob(&ref) ;
  }

  if(A1.is_blob(&t1) && t1 == &column && A2.is_blob(&t2) && t2 == &matrix)
  { auto m1 = PlBlobV<Column>::cast_ex(A1, column) ;
    auto m2 = PlBlobV<Matrix>::cast_ex(A2, matrix) ;
    mat m = m1->v + m2->m ;
    auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
    return A3.unify_blob(&ref) ;
  }

  if(A1.is_blob(&t1) && t1 == &column && A2.is_blob(&t2) && t2 == &column)
  { auto m1 = PlBlobV<Column>::cast_ex(A1, column) ;
    auto m2 = PlBlobV<Column>::cast_ex(A2, column) ;
    mat m = m1->v + m2->v ;
    auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
    return A3.unify_blob(&ref) ;
  }

  if(!A1.is_blob(&t1) || (t1 != &matrix && t1 != &column))
    throw PlTypeError("Matrix or Column", A1) ;

  // if(!A2.is_blob(&t2) || (t2 != &matrix && t2 != &column))
  throw PlTypeError("Matrix or Column", A2) ;
}

PREDICATE(prod, 3)
{ PL_blob_t *t1, *t2 ;
  if(A1.is_blob(&t1) && t1 == &matrix && A2.is_blob(&t2) && t2 == &matrix)
  { auto m1 = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    auto m2 = PlBlobV<Matrix>::cast_ex(A2, matrix) ;

    try
    { mat m = m1->m * m2->m ;
      auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
      return A3.unify_blob(&ref) ;
    }

    catch(std::exception& e)
    { throw PlDomainError(e.what(), PlCompound("*", PlTermv(A1, A2))) ;
    }
  }

  if(A1.is_blob(&t1) && t1 == &matrix && A2.is_blob(&t2) && t2 == &column)
  { auto m1 = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
    auto m2 = PlBlobV<Column>::cast_ex(A2, column) ;
    mat m = m1->m * m2->v ;
    auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
    return A3.unify_blob(&ref) ;
  }

  if(A1.is_blob(&t1) && t1 == &column && A2.is_blob(&t2) && t2 == &matrix)
  { auto m1 = PlBlobV<Column>::cast_ex(A1, column) ;
    auto m2 = PlBlobV<Matrix>::cast_ex(A2, matrix) ;
    mat m = m1->v * m2->m ;
    auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
    return A3.unify_blob(&ref) ;
  }

  if(A1.is_blob(&t1) && t1 == &column && A2.is_blob(&t2) && t2 == &column)
  { auto m1 = PlBlobV<Column>::cast_ex(A1, column) ;
    auto m2 = PlBlobV<Column>::cast_ex(A2, column) ;
    mat m = m1->v * m2->v ;
    auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
    return A3.unify_blob(&ref) ;
  }

  if(!A1.is_blob(&t1) || (t1 != &matrix && t1 != &column))
    throw PlTypeError("Matrix or Column", A1) ;

  // if(!A2.is_blob(&t2) || (t2 != &matrix && t2 != &column))
  throw PlTypeError("Matrix or Column", A2) ;
}

PREDICATE(get0, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = ref->m(r, c) ;
  return A4.unify_float(x) ;
}

PREDICATE(get0, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = ref->v(i) ;
  return A3.unify_float(x) ;
}

PREDICATE(get1, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = ref->m(--r, --c) ;
  return A4.unify_float(x) ;
}

PREDICATE(get1, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = ref->v(--i) ;
  return A3.unify_float(x) ;
}

PREDICATE(put0, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = A4.as_double() ;
  ref->m(r, c) = x ;
  return true ;
}

PREDICATE(put0, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = A3.as_double() ;
  ref->v(i) = x ;
  return true ;
}

PREDICATE(put1, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto ref = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = A4.as_double() ;
  ref->m(--r, --c) = x ;
  return true ;
}

PREDICATE(put1, 3)
{ auto i = A1.as_int32_t() ;
  auto ref = PlBlobV<Column>::cast_ex(A2, column) ;
  double x = A3.as_double() ;
  ref->v(--i) = x ;
  return true ;
}
