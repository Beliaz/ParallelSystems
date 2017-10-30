..B1.103: # Preds ..B1.101 ..B1.82
  xor r9d, r9d #53.3
  test r12d, r12d #53.19
  jle ..B1.135 # Prob 9% #53.19
  movsxd r8, r12d #55.7
  mov r13, QWORD PTR [8+rsp] #55.7[spill]
..B1.105: # Preds ..B1.133 ..B1.104
  xor r10d, r10d #54.5
..B1.106: # Preds ..B1.132 ..B1.105
  test r12d, r12d #55.23
  jle ..B1.132 # Prob 50% #55.23
  cmp r12d, 8 #55.7
  jl ..B1.178 # Prob 10% #55.7
  mov rsi, QWORD PTR [r14+r9*8] #56.9
  mov r15, rsi #55.7
  and r15, 15 #55.7
  test r15d, r15d #55.7
  je ..B1.111 # Prob 50% #55.7
  test r15d, 7 #55.7
  jne ..B1.178 # Prob 10% #55.7
  mov r15d, 1 #55.7
..B1.111: # Preds ..B1.110 ..B1.108
  lea eax, DWORD PTR [8+r15] #55.7
  cmp r12d, eax #55.7
  jl ..B1.178 # Prob 10% #55.7
  mov r11d, r12d #55.7
  xor edx, edx #55.7
  sub r11d, r15d #55.7
  and r11d, 7 #55.7
  neg r11d #55.7
  add r11d, r12d #55.7
  mov eax, r15d #55.7
  mov rcx, QWORD PTR [r13+r10*8] #56.30
  test r15d, r15d #55.7
  jbe ..B1.177 # Prob 10% #55.7
  mov rdi, QWORD PTR [rbx+r9*8] #56.20
  movsd xmm0, QWORD PTR [rdi+r10*8] #56.20
..B1.114: # Preds ..B1.114 ..B1.113
  movsd xmm1, QWORD PTR [rcx+rdx*8] #56.30
  mulsd xmm1, xmm0 #56.30
  addsd xmm1, QWORD PTR [rsi+rdx*8] #56.9
  movsd QWORD PTR [rsi+rdx*8], xmm1 #56.9
  inc rdx #55.7
  cmp rdx, rax #55.7
  jb ..B1.114 # Prob 82% #55.7
..B1.116: # Preds ..B1.114 ..B1.177
  mov edi, DWORD PTR [r13+r10*8] #56.30
  movsxd rdx, r11d #55.7
  lea r15d, DWORD PTR [rdi+r15*8] #56.30
  test r15d, 15 #55.7
  je ..B1.120 # Prob 60% #55.7
  unpcklpd xmm0, xmm0 #56.20
..B1.118: # Preds ..B1.118 ..B1.117
  movups xmm1, XMMWORD PTR [rcx+rax*8] #56.30
  movups xmm3, XMMWORD PTR [16+rcx+rax*8] #56.30
  movups xmm5, XMMWORD PTR [32+rcx+rax*8] #56.30
  movups xmm7, XMMWORD PTR [48+rcx+rax*8] #56.30
  mulpd xmm1, xmm0 #56.30
  mulpd xmm3, xmm0 #56.30
  mulpd xmm5, xmm0 #56.30
  mulpd xmm7, xmm0 #56.30
  movups xmm2, XMMWORD PTR [rsi+rax*8] #56.9
  movups xmm4, XMMWORD PTR [16+rsi+rax*8] #56.9
  movups xmm6, XMMWORD PTR [32+rsi+rax*8] #56.9
  movups xmm8, XMMWORD PTR [48+rsi+rax*8] #56.9
  addpd xmm2, xmm1 #56.9
  addpd xmm4, xmm3 #56.9
  addpd xmm6, xmm5 #56.9
  addpd xmm8, xmm7 #56.9
  movups XMMWORD PTR [rsi+rax*8], xmm2 #56.9
  movups XMMWORD PTR [16+rsi+rax*8], xmm4 #56.9
  movups XMMWORD PTR [32+rsi+rax*8], xmm6 #56.9
  movups XMMWORD PTR [48+rsi+rax*8], xmm8 #56.9
  add rax, 8 #55.7
  cmp rax, rdx #55.7
  jb ..B1.118 # Prob 82% #55.7
  jmp ..B1.123 # Prob 100% #55.7
..B1.120: # Preds ..B1.116
  unpcklpd xmm0, xmm0 #56.20
..B1.121: # Preds ..B1.121 ..B1.120
  movups xmm1, XMMWORD PTR [rcx+rax*8] #56.30
  movups xmm3, XMMWORD PTR [16+rcx+rax*8] #56.30
  movups xmm5, XMMWORD PTR [32+rcx+rax*8] #56.30
  movups xmm7, XMMWORD PTR [48+rcx+rax*8] #56.30
  mulpd xmm1, xmm0 #56.30
  mulpd xmm3, xmm0 #56.30
  mulpd xmm5, xmm0 #56.30
  mulpd xmm7, xmm0 #56.30
  movups xmm2, XMMWORD PTR [rsi+rax*8] #56.9
  movups xmm4, XMMWORD PTR [16+rsi+rax*8] #56.9
  movups xmm6, XMMWORD PTR [32+rsi+rax*8] #56.9
  movups xmm8, XMMWORD PTR [48+rsi+rax*8] #56.9
  addpd xmm2, xmm1 #56.9
  addpd xmm4, xmm3 #56.9
  addpd xmm6, xmm5 #56.9
  addpd xmm8, xmm7 #56.9
  movups XMMWORD PTR [rsi+rax*8], xmm2 #56.9
  movups XMMWORD PTR [16+rsi+rax*8], xmm4 #56.9
  movups XMMWORD PTR [32+rsi+rax*8], xmm6 #56.9
  movups XMMWORD PTR [48+rsi+rax*8], xmm8 #56.9
  add rax, 8 #55.7
  cmp rax, rdx #55.7
  jb ..B1.121 # Prob 82% #55.7
..B1.123: # Preds ..B1.121 ..B1.118
  lea eax, DWORD PTR [1+r11] #55.7
  cmp eax, r12d #55.7
  ja ..B1.132 # Prob 50% #55.7
  mov rdi, r8 #55.7
  sub rdi, rdx #55.7
  cmp rdi, 2 #55.7
  jl ..B1.176 # Prob 10% #55.7
  mov r15d, edi #55.7
  xor eax, eax #55.7
  and r15d, -2 #55.7
  lea rcx, QWORD PTR [rcx+rdx*8] #56.30
  movsxd r15, r15d #55.7
  lea rdx, QWORD PTR [rsi+rdx*8] #56.9
..B1.126: # Preds ..B1.126 ..B1.125
  movups xmm1, XMMWORD PTR [rcx+rax*8] #56.30
  movups xmm2, XMMWORD PTR [rdx+rax*8] #56.9
  mulpd xmm1, xmm0 #56.30
  addpd xmm2, xmm1 #56.9
  movups XMMWORD PTR [rdx+rax*8], xmm2 #56.9
  add rax, 2 #55.7
  cmp rax, r15 #55.7
  jb ..B1.126 # Prob 82% #55.7
..B1.128: # Preds ..B1.126 ..B1.176 ..B1.179
  cmp r15, rdi #55.7
  jae ..B1.132 # Prob 10% #55.7
  movsxd r11, r11d #56.9
  mov rcx, QWORD PTR [r13+r10*8] #56.30
  mov rax, QWORD PTR [r14+r9*8] #56.9
  mov rdx, QWORD PTR [rbx+r9*8] #56.20
  lea rax, QWORD PTR [rax+r11*8] #56.9
  movsd xmm0, QWORD PTR [rdx+r10*8] #56.20
  lea rdx, QWORD PTR [rcx+r11*8] #56.30
..B1.130: # Preds ..B1.130 ..B1.129
  movsd xmm1, QWORD PTR [rdx+r15*8] #56.30
  mulsd xmm1, xmm0 #56.30
  addsd xmm1, QWORD PTR [rax+r15*8] #56.9
  movsd QWORD PTR [rax+r15*8], xmm1 #56.9
  inc r15 #55.7
  cmp r15, rdi #55.7
  jb ..B1.130 # Prob 82% #55.7
..B1.132: # Preds ..B1.130 ..B1.123 ..B1.106 ..B1.178 ..B1.128
  inc r10 #54.5
  cmp r10, r8 #54.5
  jb ..B1.106 # Prob 82% #54.5
  .byte 15 #53.3
  .byte 31 #53.3
  .byte 132 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  inc r9 #53.3
  cmp r9, r8 #53.3
  jb ..B1.105 # Prob 82% #53.3
