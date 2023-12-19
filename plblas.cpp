#include <armadillo>
#include <SWI-cpp2.h>

using namespace arma;

struct Matrix;

static PL_blob_t matrix = PL_BLOB_DEFINITION(Matrix, "matrix");

struct Matrix : public PlBlob
{
  mat m;

  explicit Matrix()
    : PlBlob(&matrix) 
  { 
  }

  explicit Matrix(uword rows, uword cols)
    : PlBlob(&matrix)
  {
    m.set_size(rows, cols);
    m.zeros();
  }

  explicit Matrix(const mat& am)
    : PlBlob(&matrix)
  {
    m = am;
  }

  PL_BLOB_SIZE

  void portray(PlStream& strm) const;
};

PREDICATE(matrix, 3)
{
  auto ref = std::unique_ptr<PlBlob>(new Matrix(A1.as_int32_t(), A2.as_int32_t()));
  return A3.unify_blob(&ref);
}

PREDICATE(matrix_portray, 2)
{ auto ref = PlBlobV<Matrix>::cast_ex(A2, matrix);
  PlTerm a1(A1);
  PlStream s(a1, SIO_OUTPUT);
  ref->portray(s);
  return true;
}

PREDICATE(matmult, 3)
{
  auto m1 = PlBlobV<Matrix>::cast_ex(A1, matrix);
  auto m2 = PlBlobV<Matrix>::cast_ex(A2, matrix);
  mat m = m1->m * m2->m;
  auto ref = std::unique_ptr<PlBlob>(new Matrix(m));
  return A3.unify_blob(&ref);
}

void Matrix::portray(PlStream& s) const
{
  s.printf("Matrix(rows=%ul cols=%ul)\n", m.n_rows, m.n_cols);
  for(uword i=0; i<m.n_rows; i++)
  {
    for(uword j=0; j<m.n_cols; j++)
      s.printf(" %.3f", m(i, j));
    
    s.printf("\n");
  }
  s.printf("\n");
}
