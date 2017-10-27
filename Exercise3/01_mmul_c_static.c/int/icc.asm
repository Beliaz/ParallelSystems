  vpbroadcastd ymm0, DWORD PTR [A+rcx+r8*4] #40.20
  mov r10, rsi #39.7
  xor r9d, r9d #39.7
..B1.69: # Preds ..B1.69 ..B1.68
  vpmulld ymm1, ymm0, YMMWORD PTR [B+rdi+r9*4] #40.30
  vpaddd ymm2, ymm1, YMMWORD PTR [C+rcx+r9*4] #40.9
  vmovdqu YMMWORD PTR [C+rcx+r9*4], ymm2 #40.9
  add r9, 8 #39.7
  cmp r9, rsi #39.7
  jb ..B1.69 # Prob 99% #39.7
..B1.71: # Preds ..B1.69 ..B1.102
  cmp r10, rdx #39.7
  jae ..B1.75 # Prob 10% #39.7
  mov r9d, DWORD PTR [A+rcx+r8*4] #40.20
..B1.73: # Preds ..B1.73 ..B1.72
  mov r11d, DWORD PTR [B+rdi+r10*4] #40.30
  imul r11d, r9d #40.30
  add DWORD PTR [C+rcx+r10*4], r11d #40.9