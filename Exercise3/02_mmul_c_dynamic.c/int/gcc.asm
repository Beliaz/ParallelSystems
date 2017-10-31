.L19:
  mov rsi, QWORD PTR [r13+0+rax*8]
  mov rdi, QWORD PTR [r12+rax*8]
  xor r9d, r9d
.L18:
  mov r8, QWORD PTR [rbp+0+r9*8]
  xor edx, edx
.L17:
  mov ecx, DWORD PTR [rdi]
  imul ecx, DWORD PTR [r8+rdx*4]
  add DWORD PTR [rsi+rdx*4], ecx
  add rdx, 1
  cmp ebx, edx
  jg .L17
  add r9, 1
  add rdi, 4
  cmp ebx, r9d
  jg .L18
  add rax, 1
  cmp ebx, eax
  jg .L19
  xor edi, edi
  mov r14d, 1