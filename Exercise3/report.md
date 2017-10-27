# Report

## 01_mmul_c_static.c

| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SIMD (4), unroll (4) | SIMD(4, fma) | SIMD(4), unroll (2) |
| float  | SISD | SIMD (4), unroll (4) | SIMD(4, fma) | SIMD(4), unroll (2) |
| int    | SISD | SIMD (4), unroll (4) | SIMD(4) | SIMD(4), unroll (2) |