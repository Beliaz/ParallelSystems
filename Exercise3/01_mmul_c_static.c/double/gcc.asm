.L10:
  vmovsd xmm1, QWORD PTR [rdi]
  xor eax, eax
.L9:
  vmulsd xmm0, xmm1, QWORD PTR B[rcx+rax]
  vaddsd xmm0, xmm0, QWORD PTR C[rax+rdx]
  vmovsd QWORD PTR C[rdx+rax], xmm0
  add rax, 8