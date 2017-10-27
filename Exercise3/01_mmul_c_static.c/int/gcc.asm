.L9:
  mov edi, DWORD PTR [r9]
  xor eax, eax
.L8:
  mov ecx, DWORD PTR B[rsi+rax]
  imul ecx, edi
  add DWORD PTR C[rdx+rax], ecx
  add rax, 4