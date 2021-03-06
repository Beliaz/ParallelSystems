#ifndef MMUL_H
#define MMUL_H

#include "matrix.h"
#include "matrix_factory.h"

#include "summa_seq.h"
#include "summa_dist.h"
#include "summa_dist_2.h"

using value_t = double;
using factory_t = matrix_factory<value_t>;
using matrix_t = matrix<value_t>;
using technique_t = summa_sequential;

#endif // MMUL_H
