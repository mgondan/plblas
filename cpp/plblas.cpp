#include <armadillo>
#include <SWI-cpp2.h>

using namespace arma;

struct Matrix;

static PL_blob_t matrix = PL_BLOB_DEFINITION(Matrix, "matrix");

struct Matrix : public PlBlob
{
  mat m ;

  explicit Matrix()
    : PlBlob(&matrix),
      m()
  { 
  }

  explicit Matrix(const mat& am)
    : PlBlob(&matrix),
      m(am)
  {
  }

  PL_BLOB_SIZE

  void portray(PlStream& strm) const ;

  // Fill
  void zeros() 
  { m.zeros() ; 
  }

  void ones()
  { m.ones() ;
  }

  void eye()
  { m.eye() ;
  }

  void randu()
  { m.randu() ;
  }

  void randn()
  { m.randn() ;
  }

  void fill(const double v)
  { m.fill(v) ;
  }

  // Element access
  double& operator()(uword r, uword c)
  { return m(r, c) ;
  }
} ;

PREDICATE(matrix, 3)
{ mat m(A1.as_int32_t(), A2.as_int32_t()) ;
  auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
  return A3.unify_blob(&ref);
}

PREDICATE(matrix_portray, 2)
{ auto ref = PlBlobV<Matrix>::cast_ex(A2, matrix);
  PlTerm a1(A1);
  PlStream s(a1, SIO_OUTPUT);
  ref->portray(s);
  return true;
}

void Matrix::portray(PlStream& s) const
{
  s.printf("Matrix(rows=%u cols=%u)\n", m.n_rows, m.n_cols);
  for(uword i=0; i<m.n_rows; i++)
  {
    for(uword j=0; j<m.n_cols; j++)
      s.printf(" %.3f", m(i, j));
    s.printf("\n");
  }
  s.printf("\n");
}

PREDICATE(zeros, 1)
{ auto m = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  m->zeros() ;
  return true ;
}

PREDICATE(ones, 1)
{ auto m = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  m->ones() ;
  return true ;
}

PREDICATE(eye, 1)
{ auto m = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  m->eye() ;
  return true ;
}

PREDICATE(randu, 1)
{ auto m = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  m->randu() ;
  return true ;
}

PREDICATE(randn, 1)
{ auto m = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  m->randn() ;
  return true ;
}

PREDICATE(fill, 2)
{ auto m = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  double v = A2.as_double() ;
  m->fill(v) ;
  return true ;
}

PREDICATE(mult, 3)
{ auto a = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  auto b = PlBlobV<Matrix>::cast_ex(A2, matrix) ;
  mat m = a->m * b->m ;
  auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
  return A3.unify_blob(&ref) ;
}

PREDICATE(get0, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto m = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = m->operator()(r, c) ;
  return A4.unify_float(x) ;
}

PREDICATE(get1, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto m = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = m->operator()(--r, --c) ;
  return A4.unify_float(x) ;
}

PREDICATE(put0, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto m = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = A4.as_double() ;
  m->operator()(r, c) = x ;
  return true ;
}

PREDICATE(put1, 4)
{ auto r = A1.as_int32_t() ;
  auto c = A2.as_int32_t() ;
  auto m = PlBlobV<Matrix>::cast_ex(A3, matrix) ;
  double x = A4.as_double() ;
  m->operator()(--r, --c) = x ;
  return true ;
}

