#ifndef MMUL_H
#define MMUL_H

#include <vector>

#if defined(NESTED_VECTOR)

struct Matrix : std::vector<std::vector<double>> {
	unsigned matSize;
	Matrix() : matSize(0) {}
	Matrix(unsigned n) : matSize(n) {
		resize(n);
		for(unsigned i = 0; i < n; ++i) {
			(*this)[i].resize(n);
		}
	}
	double& operator()(unsigned i, unsigned j) {
		return (*this)[i][j];
	}
	const double& operator()(unsigned i, unsigned j) const {
		return (*this)[i][j];
	}
};

#elif defined(CONTIGUOUS_WITH_MULTIPLICATION)

struct Matrix : std::vector<double> {
	unsigned matSize;
	Matrix() : matSize(0) {}
	Matrix(unsigned n) : matSize(n) {
		resize(n*n);
	}
	double& operator()(unsigned i, unsigned j) {
		return (*this)[i*matSize + j];
	}
	const double& operator()(unsigned i, unsigned j) const {
		return (*this)[i*matSize + j];
	}
};

#elif defined(CONTIGUOUS_WITH_INDIRECTION)

struct Matrix : std::vector<double> {
	unsigned matSize;
	std::vector<double*> indirection;
	Matrix() : matSize(0) {}
	Matrix(unsigned n) : matSize(n) {
		resize(n*n);
		indirection.resize(n);
		for(unsigned i = 0; i < n; ++i) {
			indirection[i] = &(*this)[i*n];
		}
	}
	double& operator()(unsigned i, unsigned j) {
		return indirection[i][j];
	}
	const double& operator()(unsigned i, unsigned j) const {
		return indirection[i][j];
	}
};

#endif

// initializes a matrix of size n x n
template<typename F>
Matrix init(unsigned n, F initFun) {
  Matrix res(n);
  for (unsigned i = 0; i < n; i++) {
	for (unsigned j = 0; j < n; j++) {
	  res(i, j) = initFun(i, j);
	}
  }
  return res;
}

#ifdef TRIANGLE

inline Matrix operator*(const Matrix& a, const Matrix& b) {
  auto n = a.matSize;
  Matrix c = init(n, [](unsigned, unsigned) { return 0; });
  //Parallelization
  //Improves approx. by factor of used CPU-Cores
  for (unsigned i = 0; i < n; ++i) {
	for (unsigned k = 0; k < n; ++k) {
	  //Upper triangle matrix only -> j = k
	  //Results in a speedup of approx. x2 (sequential as well as parallel) as only half the multiplications are needed
	  for (auto j = k; j < n; ++j) {
		//Check if value of right matrix equals zero and skip if so
		//if (b(k, j) == 0)
		// continue;
		c(i, j) += a(i, k) * b(k, j);
	  }
	}
  }
  return c;
}

#else

#ifdef TRIANGLE_PARALLEL

// computes the product of two matrices
inline Matrix operator*(const Matrix& a, const Matrix& b) {
  auto n = static_cast<int>(a.matSize);
  Matrix c = init(n, [](unsigned, unsigned) { return 0; });
  //Parallelization
  //Improves approx. by factor of used CPU-Cores
#pragma omp parallel for
  for (int i = 0; i < n; ++i) {
	for (int k = 0; k < n; ++k) {
	  //Upper triangle matrix only -> j = k
	  //Results in a speedup of approx. x2 (sequential as well as parallel) as only half the multiplications are needed
	  for (auto j = k; j < n; ++j) {
		//Check if value of right matrix equals zero and skip if so
		//if (b(k, j) == 0)
		//  continue;
		c(i, j) += a(i, k) * b(k, j);
	  }
	}
  }
  return c;
}

#else

// computes the product of two matrices
inline Matrix operator*(const Matrix& a, const Matrix& b) {
	unsigned n = a.matSize;
	Matrix c = init(n, [](unsigned, unsigned) { return 0; });
	for(unsigned i = 0; i < n; ++i) {
		for(unsigned k = 0; k < n; ++k) {
			for(unsigned j = 0; j < n; ++j) {
				c(i,j) += a(i,k) * b(k,j);
			}
		}
	}
	return c;
}


#endif

#endif

#endif