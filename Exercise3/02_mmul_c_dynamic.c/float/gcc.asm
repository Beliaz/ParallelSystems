.L20:
  mov rcx, QWORD PTR [r13+0+rax*8]
  mov rsi, QWORD PTR [r12+rax*8]
  xor r8d, r8d
.L19:
  mov rdi, QWORD PTR [rbp+0+r8*8]
  xor edx, edx
.L18:
  vmovss xmm0, DWORD PTR [rsi]
  vmulss xmm0, xmm0, DWORD PTR [rdi+rdx*4]
  vaddss xmm0, xmm0, DWORD PTR [rcx+rdx*4]
  vmovss DWORD PTR [rcx+rdx*4], xmm0
  add rdx, 1
  cmp ebx, edx
  jg .L18
  add r8, 1
  add rsi, 4
  cmp ebx, r8d
  jg .L19
  add rax, 1
  cmp ebx, eax
  jg .L20
  xor esi, esi