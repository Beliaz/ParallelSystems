# Report

## 01_mmul_c_static.c

| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SIMD (4), unrolled (4) | SIMD(4, fma) | SIMD(4), unrolled (2) |

