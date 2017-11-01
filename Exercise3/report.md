# Report

## 01_mmul_c_static.c

| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SIMD (4), unroll (4) | SISD(4, fma) | SIMD(4), unroll (2) |
| float  | SISD | SIMD (8), unroll (4) | SIMD(8, fma) | SIMD(8), unroll (2) |
| int    | SISD | SIMD (8), unroll (4) | SIMD(8) | SIMD(8), unroll (2) |

## 02_mmul_c_dynamic.c
| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SIMD(4), unroll(4) | SIMD(4, fma) | SISD, unroll(4) |
| float  | SISD | SIMD(8), unroll(4) | SIMD(8, fma) | SISD, unroll(4) |
| int    | SISD | SIMD(8), unroll(4) | SIMD(8) | SISD |

## 03_mmul_cpp_dynamic.cpp
| datatype / compiler | gcc | clang | icc | msvc |
| --- | --- | --- | --- | --- |
| double | SISD | SISD, unroll(2) | SISD(fma), unroll(2) | SISD, unroll(4) |
| float  | SISD | SISD, unroll(2) | SISD(fma), unroll(2) | SISD, unroll(4) |
| int    | SISD | SISD, unroll(2) | SISD, unroll(2) | SISD |