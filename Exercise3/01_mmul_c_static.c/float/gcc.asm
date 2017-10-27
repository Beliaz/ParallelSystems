.L10:
  vmovss xmm1, DWORD PTR [rdi]
  xor eax, eax
.L9:
  vmulss xmm0, xmm1, DWORD PTR B[rcx+rax]
  vaddss xmm0, xmm0, DWORD PTR C[rax+rdx]
  vmovss DWORD PTR C[rdx+rax], xmm0
  add rax, 4