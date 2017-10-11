#ifndef MMUL_H
#define MMUL_H

#include <vector>

using Matrix = std::vector<std::vector<double>>;

// initializes a square identity matrix of size n x n
Matrix id(unsigned n) {
	Matrix res;
	res.resize(n);
	for (unsigned i = 0; i<n; i++) {
		res[i].resize(n);
		for (unsigned j = 0; j<n; j++) {
			res[i][j] = (i == j) ? 1 : 0;
		}
	}
	return res;
}

// computes the product of two matrices
Matrix operator*(const Matrix& a, const Matrix& b) {
	auto n = static_cast<unsigned>(a.size());
	Matrix c = id(n);
	for (unsigned i = 0; i<n; ++i) {
		for (unsigned j = 0; j<n; ++j) {
			c[i][j] = 0;
			for (unsigned k = 0; k<n; ++k) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return c;
}

#endif
