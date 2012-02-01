#include <cmath>
#include <stdexcept>
#include <gtest/gtest.h>
#include <stan/maths/matrix.hpp>

using Eigen::Matrix;
using Eigen::Dynamic;

using stan::maths::matrix_d;
using stan::maths::vector_d;
using stan::maths::row_vector_d;

// typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> matrix_d;
// typedef Eigen::Matrix<double,Eigen::Dynamic,1> vector_d;
// typedef Eigen::Matrix<double,1,Eigen::Dynamic> row_vector_d;

TEST(matrix_test, resize_double) {
  double x = 5;
  std::vector<unsigned int> dims;
  stan::maths::resize(x,dims);
}
TEST(matrix_test, resize_svec_double) {
  std::vector<double> y;
  std::vector<unsigned int> dims;
  EXPECT_EQ(0U, y.size());

  dims.push_back(4U);
  stan::maths::resize(y,dims);
  EXPECT_EQ(4U, y.size());

  dims[0] = 2U;
  stan::maths::resize(y,dims);
  EXPECT_EQ(2U, y.size());
}
TEST(matrix_test, resize_vec_double) {
  Matrix<double,Dynamic,1> v(2);
  std::vector<unsigned int> dims;
  EXPECT_EQ(2U, v.size());

  dims.push_back(17U);
  stan::maths::resize(v,dims);
  EXPECT_EQ(17U, v.size());

  dims[0] = 3U;
  stan::maths::resize(v,dims);
  EXPECT_EQ(3U, v.size());
}
TEST(matrix_test, resize_rvec_double) {
  Matrix<double,1,Dynamic> rv(2);
  std::vector<unsigned int> dims;
  EXPECT_EQ(2U, rv.size());

  dims.push_back(17U);
  stan::maths::resize(rv,dims);
  EXPECT_EQ(17U, rv.size());

  dims[0] = 3U;
  stan::maths::resize(rv,dims);
  EXPECT_EQ(3U, rv.size());
}
TEST(matrix_test, resize_mat_double) {
  Matrix<double,Dynamic,Dynamic> m(2,3);
  std::vector<unsigned int> dims;
  EXPECT_EQ(2U, m.rows());
  EXPECT_EQ(3U, m.cols());

  dims.push_back(7U);
  dims.push_back(17U);
  stan::maths::resize(m,dims);
  EXPECT_EQ(7U, m.rows());
  EXPECT_EQ(17U, m.cols());
}
TEST(matrix_test, resize_svec_svec_double) {
  std::vector<std::vector<double> > xx;
  EXPECT_EQ(0U,xx.size());
  std::vector<unsigned int> dims;
  dims.push_back(4U);
  dims.push_back(5U);
  stan::maths::resize(xx,dims);
  EXPECT_EQ(4U,xx.size());
  EXPECT_EQ(5U,xx[0].size());

  dims[0] = 3U;
  dims[1] = 7U;
  stan::maths::resize(xx,dims);
  EXPECT_EQ(3U,xx.size());
  EXPECT_EQ(7U,xx[1].size());  
}
TEST(matrix_test, resize_svec_v_double) {
  std::vector<Matrix<double,Dynamic,1> > xx;
  EXPECT_EQ(0U,xx.size());
  std::vector<unsigned int> dims;
  dims.push_back(4U);
  dims.push_back(5U);
  stan::maths::resize(xx,dims);
  EXPECT_EQ(4U,xx.size());
  EXPECT_EQ(5U,xx[0].size());

  dims[0] = 3U;
  dims[1] = 7U;
  stan::maths::resize(xx,dims);
  EXPECT_EQ(3U,xx.size());
  EXPECT_EQ(7U,xx[1].size());  
}
TEST(matrix_test, resize_svec_rv_double) {
  std::vector<Matrix<double,1,Dynamic> > xx;
  EXPECT_EQ(0U,xx.size());
  std::vector<unsigned int> dims;
  dims.push_back(4U);
  dims.push_back(5U);
  stan::maths::resize(xx,dims);
  EXPECT_EQ(4U,xx.size());
  EXPECT_EQ(5U,xx[0].size());

  dims[0] = 3U;
  dims[1] = 7U;
  stan::maths::resize(xx,dims);
  EXPECT_EQ(3U,xx.size());
  EXPECT_EQ(7U,xx[1].size());  
}
TEST(matrix_test, resize_svec_svec_matrix_double) {
  std::vector<std::vector<Matrix<double,Dynamic,Dynamic> > > mm;
  std::vector<unsigned int> dims;
  dims.push_back(4U);
  dims.push_back(5U);
  dims.push_back(6U);
  dims.push_back(3U);
  stan::maths::resize(mm,dims);
  EXPECT_EQ(4U,mm.size());
  EXPECT_EQ(5U,mm[0].size());
  EXPECT_EQ(6U,mm[1][2].rows());
  EXPECT_EQ(3U,mm[3][4].cols());
}

TEST(matrix,get_base1_vec1) {
  using stan::maths::get_base1;
  std::vector<double> x(2);
  x[0] = 10.0;
  x[1] = 20.0;
  EXPECT_FLOAT_EQ(10.0,get_base1(x,1,"x[1]",0));
  EXPECT_FLOAT_EQ(20.0,get_base1(x,2,"x[1]",0));
  
  get_base1(x,2,"x[2]",0) = 5.0;
  EXPECT_FLOAT_EQ(5.0,get_base1(x,2,"x[1]",0));

  EXPECT_THROW(get_base1(x,0,"x[0]",0),
               std::out_of_range);
  EXPECT_THROW(get_base1(x,3,"x[3]",0),
               std::out_of_range);
}
TEST(matrix,get_base1_vec2) {
  using stan::maths::get_base1;
  using std::vector;
  size_t M = 3;
  size_t N = 4;

  vector<vector<double> > x(M,vector<double>(N,0.0));
  

  for (size_t m = 1; m <= M; ++m)
    for (size_t n = 1; n <= N; ++n)
      x[m - 1][n - 1] = (m * 10) + n;

  for (size_t m = 1; m <= M; ++m) {
    for (size_t n = 1; n <= N; ++n) {
      double expected = x[m - 1][n - 1];
      double found = get_base1(get_base1(x, m, "x[m]",1),
                               n, "x[m][n]",2);
      EXPECT_FLOAT_EQ(expected,found);
    }
  }

  get_base1(get_base1(x,1,"",-1),2,"",-1) = 112.5;
  EXPECT_FLOAT_EQ(112.5, x[0][1]);

  EXPECT_THROW(get_base1(x,0,"",-1),std::out_of_range);
  EXPECT_THROW(get_base1(x,M+1,"",-1),std::out_of_range);
  
  EXPECT_THROW(get_base1(get_base1(x,1,"",-1), 
                         12,"",-1),
               std::out_of_range);
}
TEST(matrix,get_base1_matrix) {
  using Eigen::Matrix;
  using Eigen::Dynamic;
  using stan::maths::get_base1;
  Matrix<double,Dynamic,Dynamic> x(4,3);
  for (size_t i = 0; i < 4; ++i)
    for (size_t j = 0; j < 3; ++j)
      x(i,j) = i * j;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_FLOAT_EQ(x(i,j),
                      get_base1(x,i+1,j+1,"x",1));
      EXPECT_FLOAT_EQ(x(i,j),
                      get_base1(x,i+1,"x",1)[j]);
      Matrix<double,Dynamic,1> xi
        = get_base1<double>(x,i+1,"x",1);
      EXPECT_FLOAT_EQ(x(i,j),xi[j]);
      EXPECT_FLOAT_EQ(x(i,j),get_base1(xi,j+1,"xi",2));
      // this is no good because can't get ref to inside val
      // could remedy by adding const versions, but don't need for Stan GM
      // double xij = get_base1<double>(get_base1<double>(x,i+1,"x",1),
      //                                j+1,"xi",2);
    }
  }
  EXPECT_THROW(get_base1(x,10,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,0,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,100,1,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,1,100,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,0,1,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,1,0,"x",1), std::out_of_range);
}
TEST(matrix,get_base1_vector) {
  using Eigen::Matrix;
  using Eigen::Dynamic;
  using stan::maths::get_base1;
  Matrix<double,1,Dynamic> x(3);
  x << 1, 2, 3;
  
  for (size_t i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(x(i), get_base1(x,i+1,"x",1));
  EXPECT_THROW(get_base1(x,0,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,100,"x",1), std::out_of_range);
}
TEST(matrix,get_base1_row_vector) {
  using Eigen::Matrix;
  using Eigen::Dynamic;
  using stan::maths::get_base1;
  Matrix<double,Dynamic,1> x(3);
  x << 1, 2, 3;
  
  for (size_t i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(x(i), get_base1(x,i+1,"x",1));
  EXPECT_THROW(get_base1(x,0,"x",1), std::out_of_range);
  EXPECT_THROW(get_base1(x,100,"x",1), std::out_of_range);
}
TEST(matrix,get_base1_8) {
  using stan::maths::get_base1;
  using std::vector;
  double x0(42.0);
  // ~ 4m entries ~ 32MB memory + sizes
  vector<double> x1(9,x0);
  vector<vector<double> > x2(8,x1);
  vector<vector<vector<double> > > x3(7,x2);
  vector<vector<vector<vector<double> > > > x4(6,x3);
  vector<vector<vector<vector<vector<double> > > > > x5(5,x4);
  vector<vector<vector<vector<vector<vector<double> > > > > > x6(4,x5);
  vector<vector<vector<vector<vector<vector<vector<double> > > > > > > x7(3,x6);
  vector<vector<vector<vector<vector<vector<vector<vector<double> > > > > > > > x8(2,x7);

  EXPECT_EQ(x0, x8[0][0][0][0][0][0][0][0]);
  
  for (size_t i1 = 0; i1 < x8.size(); ++i1)
    for (size_t i2 = 0; i2 < x8[0].size(); ++i2)
      for (size_t i3 = 0; i3 < x8[0][0].size(); ++i3)
        for (size_t i4 = 0; i4 < x8[0][0][0].size(); ++i4)
          for (size_t i5 = 0; i5 < x8[0][0][0][0].size(); ++i5)
            for (size_t i6 = 0; i6 < x8[0][0][0][0][0].size(); ++i6)
              for (size_t i7 = 0; i7 < x8[0][0][0][0][0][0].size(); ++i7)
                for (size_t i8 = 0; i8 < x8[0][0][0][0][0][0][0].size(); ++i8)
                  x8[i1][i2][i3][i4][i5][i6][i7][i8]
                    = i1 * i2 * i3 * i4 * i5 * i6 * i7 * i8;

  for (size_t i1 = 0; i1 < x8.size(); ++i1)
    for (size_t i2 = 0; i2 < x8[0].size(); ++i2)
      for (size_t i3 = 0; i3 < x8[0][0].size(); ++i3)
        for (size_t i4 = 0; i4 < x8[0][0][0].size(); ++i4)
          for (size_t i5 = 0; i5 < x8[0][0][0][0].size(); ++i5)
            for (size_t i6 = 0; i6 < x8[0][0][0][0][0].size(); ++i6)
              for (size_t i7 = 0; i7 < x8[0][0][0][0][0][0].size(); ++i7)
                for (size_t i8 = 0; i8 < x8[0][0][0][0][0][0][0].size(); ++i8)
                  EXPECT_FLOAT_EQ(x8[i1][i2][i3][i4][i5][i6][i7][i8],
                                  get_base1(x8,i1+1,i2+1,i3+1,i4+1,i5+1,i6+1,i7+1,i8+1,
                                            "x8",1));
}

TEST(matrix_test,add_v_exception) {
  vector_d d1, d2;

  d1.resize(3);
  d2.resize(3);
  EXPECT_NO_THROW(stan::maths::add(d1, d2));

  d1.resize(0);
  d2.resize(0);
  EXPECT_NO_THROW(stan::maths::add(d1, d2));

  d1.resize(2);
  d2.resize(3);
  EXPECT_THROW(stan::maths::add(d1, d2), std::invalid_argument);
}
TEST(matrix_test,add_rv_exception) {
  row_vector_d d1, d2;

  d1.resize(3);
  d2.resize(3);
  EXPECT_NO_THROW(stan::maths::add(d1, d2));

  d1.resize(0);
  d2.resize(0);
  EXPECT_NO_THROW(stan::maths::add(d1, d2));

  d1.resize(2);
  d2.resize(3);
  EXPECT_THROW(stan::maths::add(d1, d2), std::invalid_argument);
}
TEST(matrix_test,add_m_exception) {
  matrix_d d1, d2;

  d1.resize(2,3);
  d2.resize(2,3);
  EXPECT_NO_THROW(stan::maths::add(d1, d2));

  d1.resize(0,0);
  d2.resize(0,0);
  EXPECT_NO_THROW(stan::maths::add(d1, d2));

  d1.resize(2,3);
  d2.resize(3,3);
  EXPECT_THROW(stan::maths::add(d1, d2), std::invalid_argument);
}

TEST(matrix_test,subtract_v_exception) {
  vector_d d1, d2;

  d1.resize(3);
  d2.resize(3);
  EXPECT_NO_THROW(stan::maths::subtract(d1, d2));

  d1.resize(0);
  d2.resize(0);
  EXPECT_NO_THROW(stan::maths::subtract(d1, d2));

  d1.resize(2);
  d2.resize(3);
  EXPECT_THROW(stan::maths::subtract(d1, d2), std::invalid_argument);
}
TEST(matrix_test,subtract_rv_exception) {
  row_vector_d d1, d2;

  d1.resize(3);
  d2.resize(3);
  EXPECT_NO_THROW(stan::maths::subtract(d1, d2));

  d1.resize(0);
  d2.resize(0);
  EXPECT_NO_THROW(stan::maths::subtract(d1, d2));

  d1.resize(2);
  d2.resize(3);
  EXPECT_THROW(stan::maths::subtract(d1, d2), std::invalid_argument);
}
TEST(matrix_test,subtract_m_exception) {
  matrix_d d1, d2;

  d1.resize(2,3);
  d2.resize(2,3);
  EXPECT_NO_THROW(stan::maths::subtract(d1, d2));

  d1.resize(0,0);
  d2.resize(0,0);
  EXPECT_NO_THROW(stan::maths::subtract(d1, d2));

  d1.resize(2,3);
  d2.resize(3,3);
  EXPECT_THROW(stan::maths::subtract(d1, d2), std::invalid_argument);
}

TEST(matrix_test,multiply_rv_v_exception) {
  row_vector_d rv;
  vector_d v;
  
  rv.resize(3);
  v.resize(3);
  EXPECT_NO_THROW(stan::maths::multiply(rv, v));

  rv.resize(0);
  v.resize(0);
  EXPECT_NO_THROW(stan::maths::multiply(rv, v));

  rv.resize(2);
  v.resize(3);
  EXPECT_THROW(stan::maths::multiply(rv, v), std::invalid_argument);
}
TEST(matrix_test,multiply_m_v_exception) {
  matrix_d m;
  vector_d v;
  
  m.resize(3, 5);
  v.resize(5);
  EXPECT_NO_THROW(stan::maths::multiply(m, v));

  m.resize(3, 0);
  v.resize(0);
  EXPECT_NO_THROW(stan::maths::multiply(m, v));

  m.resize(2, 3);
  v.resize(2);
  EXPECT_THROW(stan::maths::multiply(m, v), std::invalid_argument);  
}
TEST(matrix_test,multiply_rv_m_exception) {
  row_vector_d rv;
  matrix_d m;
    
  rv.resize(3);
  m.resize(3, 5);
  EXPECT_NO_THROW(stan::maths::multiply(rv, m));

  rv.resize(0);
  m.resize(0, 3);
  EXPECT_NO_THROW(stan::maths::multiply(rv, m));

  rv.resize(3);
  m.resize(2, 3);
  EXPECT_THROW(stan::maths::multiply(rv, m), std::invalid_argument);
}
TEST(matrix_test,multiply_m_m_exception) {
  matrix_d m1, m2;
  
  m1.resize(1, 3);
  m2.resize(3, 5);
  EXPECT_NO_THROW(stan::maths::multiply(m1, m2));

  
  m1.resize(2, 0);
  m2.resize(0, 3);
  EXPECT_NO_THROW(stan::maths::multiply(m1, m2));

  m1.resize(4, 3);
  m2.resize(2, 3);
  EXPECT_THROW(stan::maths::multiply(m1, m2), std::invalid_argument);
}
TEST(matrix_test,cholesky_decompose_exception) {
  matrix_d m;
  
  m.resize(2,2);
  EXPECT_NO_THROW(stan::maths::cholesky_decompose(m));

  m.resize(0, 0);
  EXPECT_NO_THROW(stan::maths::cholesky_decompose(m));
  
  m.resize(2, 3);
  EXPECT_THROW(stan::maths::cholesky_decompose(m), std::invalid_argument);
}
TEST(matrix_test,std_vector_sum_double) {
  std::vector<double> x(3);
  EXPECT_FLOAT_EQ(0.0,stan::maths::sum(x));
  x[0] = 1.0;
  x[1] = 2.0;
  x[2] = 3.0;
  EXPECT_FLOAT_EQ(6.0,stan::maths::sum(x));
}
TEST(matrix_test,std_vector_sum_int) {
  std::vector<int> x(3);
  EXPECT_EQ(0,stan::maths::sum(x));
  x[0] = 1;
  x[1] = 2;
  x[2] = 3;
  EXPECT_EQ(6,stan::maths::sum(x));
}


