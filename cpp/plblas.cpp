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

  void portray(PlStream& strm) const;

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
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  ref->zeros() ;
  return true ;
}

PREDICATE(ones, 1)
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  ref->ones() ;
  return true ;
}

PREDICATE(eye, 1)
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  ref->eye() ;
  return true ;
}

PREDICATE(randu, 1)
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  ref->randu() ;
  return true ;
}

PREDICATE(randn, 1)
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  ref->randn() ;
  return true ;
}

PREDICATE(fill, 2)
{ auto ref = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  double v = A2.as_double() ;
  ref->fill(v) ;
  return true ;
}

PREDICATE(matmult, 3)
{ auto m1 = PlBlobV<Matrix>::cast_ex(A1, matrix) ;
  auto m2 = PlBlobV<Matrix>::cast_ex(A2, matrix) ;
  mat m = m1->m * m2->m ;
  auto ref = std::unique_ptr<PlBlob>(new Matrix(m)) ;
  return A3.unify_blob(&ref);
}
