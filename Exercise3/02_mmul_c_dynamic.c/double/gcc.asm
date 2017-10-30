.L20:
  mov rcx, QWORD PTR [r13+0+rax*8]
  mov rsi, QWORD PTR [r12+rax*8]
  xor r8d, r8d
.L19:
  mov rdi, QWORD PTR [rbp+0+r8*8]
  xor edx, edx
.L18:
  vmovsd xmm0, QWORD PTR [rsi]
  vmulsd xmm0, xmm0, QWORD PTR [rdi+rdx*8]
  vaddsd xmm0, xmm0, QWORD PTR [rcx+rdx*8]
  vmovsd QWORD PTR [rcx+rdx*8], xmm0
  add rdx, 1
  cmp ebx, edx
  jg .L18
  add r8, 1
  add rsi, 8
  cmp ebx, r8d
  jg .L19
  add rax, 1
  cmp ebx, eax
  jg .L20
  xor esi, esi
  mov r14d, 1
