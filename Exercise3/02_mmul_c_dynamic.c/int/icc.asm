..B1.131: # Preds ..B1.129 ..B1.86
  xor ecx, ecx #53.3
  test r12, r12 #53.19
  jle ..B1.155 # Prob 9% #53.19
  mov eax, ebx #55.7
  and eax, -8 #55.7
  movsxd rax, eax #55.7
  mov rdx, QWORD PTR [rsp] #55.7[spill]
..B1.133: # Preds ..B1.153 ..B1.132
  xor r8d, r8d #54.5
..B1.134: # Preds ..B1.152 ..B1.133
  test r12, r12 #55.23
  jle ..B1.152 # Prob 50% #55.23
  cmp r12, 8 #55.7
  jl ..B1.190 # Prob 10% #55.7
  cmp r12, 793 #55.7
  jl ..B1.192 # Prob 10% #55.7
  mov rsi, QWORD PTR [r13+rcx*8] #56.9
  mov rdi, rsi #55.7
  and rdi, 31 #55.7
  test edi, edi #55.7
  je ..B1.140 # Prob 50% #55.7
  test edi, 3 #55.7
  jne ..B1.190 # Prob 10% #55.7
  neg edi #55.7
  add edi, 32 #55.7
  shr edi, 2 #55.7
..B1.140: # Preds ..B1.139 ..B1.137
  mov r11d, edi #55.7
  lea r9, QWORD PTR [8+r11] #55.7
  cmp r12, r9 #55.7
  jl ..B1.190 # Prob 10% #55.7
  mov r9d, ebx #55.7
  xor r15d, r15d #55.7
  sub r9d, edi #55.7
  and r9d, 7 #55.7
  neg r9d #55.7
  add r9d, ebx #55.7
  movsxd r9, r9d #55.7
  mov r10, QWORD PTR [rdx+r8*8] #56.30
  test edi, edi #55.7
  jbe ..B1.189 # Prob 10% #55.7
  mov rdi, QWORD PTR [r14+rcx*8] #56.20
  mov QWORD PTR [8+rsp], r12 #56.20[spill]
  mov edi, DWORD PTR [rdi+r8*4] #56.20
..B1.143: # Preds ..B1.143 ..B1.142
  mov r12d, DWORD PTR [r10+r15*4] #56.30
  imul r12d, edi #56.30
  add DWORD PTR [rsi+r15*4], r12d #56.9
  inc r15 #55.7
  cmp r15, r11 #55.7
  jb ..B1.143 # Prob 82% #55.7
  mov r12, QWORD PTR [8+rsp] #[spill]
..B1.145: # Preds ..B1.144 ..B1.189 ..B1.192
  vmovd xmm0, edi #56.20
  vpbroadcastd ymm0, xmm0 #56.20
..B1.146: # Preds ..B1.146 ..B1.145
  vpmulld ymm1, ymm0, YMMWORD PTR [r10+r11*4] #56.30
  vpaddd ymm2, ymm1, YMMWORD PTR [rsi+r11*4] #56.9
  vmovdqu YMMWORD PTR [rsi+r11*4], ymm2 #56.9
  add r11, 8 #55.7
  cmp r11, r9 #55.7
  jb ..B1.146 # Prob 82% #55.7
..B1.148: # Preds ..B1.146 ..B1.190
  cmp r9, r12 #55.7
  jae ..B1.152 # Prob 10% #55.7
  mov rsi, QWORD PTR [r14+rcx*8] #56.20
  mov r11, QWORD PTR [r13+rcx*8] #56.9
  mov r10d, DWORD PTR [rsi+r8*4] #56.20
  mov rsi, QWORD PTR [rdx+r8*8] #56.30
..B1.150: # Preds ..B1.150 ..B1.149
  mov r15d, DWORD PTR [rsi+r9*4] #56.30
  imul r15d, r10d #56.30
  add DWORD PTR [r11+r9*4], r15d #56.9
  inc r9 #55.7
  cmp r9, r12 #55.7
  jb ..B1.150 # Prob 82% #55.7
..B1.152: # Preds ..B1.150 ..B1.148 ..B1.134
  inc r8 #54.5
  cmp r8, r12 #54.5
  jb ..B1.134 # Prob 82% #54.5
  .byte 15 #53.3
  .byte 31 #53.3
  .byte 132 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  inc rcx #53.3
  cmp rcx, r12 #53.3
  jb ..B1.133 # Prob 82% #53.3