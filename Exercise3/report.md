# Report

## 01_mmul_c_static.c

| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SIMD (4), unroll (4) | SIMD(4, fma) | SIMD(4), unroll (2) |
| float  | SISD | SIMD (4), unroll (4) | SIMD(4, fma) | SIMD(4), unroll (2) |
| int    | SISD | SIMD (4), unroll (4) | SIMD(4) | SIMD(4), unroll (2) |

## 02_mmul_c_dynamic.c
| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SISD | SIMD(4, fma) | SISD, unroll(4) |
| float  | SISD | SISD | SIMD(4, fma) | SISD, unroll(4) |
| int    | SISD | SISD | SIMD(4) | SISD |

## 03_mmul_cpp_dynamic.cpp
| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | unroll(2) | SIMD(4) | SISD, unroll(4) |
| float  | SISD | unroll(2) | SIMD(4) | SISD, unroll(4) |
| int    | SISD | unroll(2) | SISD | SISD |