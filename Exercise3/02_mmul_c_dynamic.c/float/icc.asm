..B1.118: # Preds ..B1.220 ..B1.117
  xor edx, edx #53.3
  test r12d, r12d #53.19
  jle ..B1.142 # Prob 9% #53.19
  mov eax, r12d #55.7
  and eax, -8 #55.7
  movsxd rcx, r12d #55.7
  movsxd rax, eax #55.7
  mov rdi, QWORD PTR [8+rsp] #55.7[spill]
..B1.120: # Preds ..B1.140 ..B1.119
  xor r15d, r15d #54.5
..B1.121: # Preds ..B1.139 ..B1.120
  test r12d, r12d #55.23
  jle ..B1.139 # Prob 50% #55.23
  cmp rcx, 8 #55.7
  jl ..B1.183 # Prob 10% #55.7
  cmp rcx, 793 #55.7
  jl ..B1.185 # Prob 10% #55.7
  mov r11, QWORD PTR [r14+rdx*8] #56.9
  mov r10, r11 #55.7
  and r10, 31 #55.7
  test r10d, r10d #55.7
  je ..B1.127 # Prob 50% #55.7
  test r10d, 3 #55.7
  jne ..B1.183 # Prob 10% #55.7
  neg r10d #55.7
  add r10d, 32 #55.7
  shr r10d, 2 #55.7
..B1.127: # Preds ..B1.126 ..B1.124
  mov r9d, r10d #55.7
  lea rsi, QWORD PTR [8+r9] #55.7
  cmp rcx, rsi #55.7
  jl ..B1.183 # Prob 10% #55.7
  mov r13d, r12d #55.7
  xor esi, esi #55.7
  sub r13d, r10d #55.7
  and r13d, 7 #55.7
  neg r13d #55.7
  add r13d, r12d #55.7
  movsxd r13, r13d #55.7
  mov r8, QWORD PTR [rdi+r15*8] #56.30
  test r10d, r10d #55.7
  jbe ..B1.182 # Prob 10% #55.7
  mov r10, QWORD PTR [rbx+rdx*8] #56.20
  vmovss xmm0, DWORD PTR [r10+r15*4] #56.20
..B1.130: # Preds ..B1.130 ..B1.129
  vmovss xmm1, DWORD PTR [r8+rsi*4] #56.30
  vfmadd213ss xmm1, xmm0, DWORD PTR [r11+rsi*4] #56.9
  vmovss DWORD PTR [r11+rsi*4], xmm1 #56.9
  inc rsi #55.7
  cmp rsi, r9 #55.7
  jb ..B1.130 # Prob 82% #55.7
..B1.132: # Preds ..B1.130 ..B1.182 ..B1.185
  vbroadcastss ymm0, xmm0 #56.20
..B1.133: # Preds ..B1.133 ..B1.132
  vmovups ymm1, YMMWORD PTR [r8+r9*4] #56.30
  vfmadd213ps ymm1, ymm0, YMMWORD PTR [r11+r9*4] #56.9
  vmovups YMMWORD PTR [r11+r9*4], ymm1 #56.9
  add r9, 8 #55.7
  cmp r9, r13 #55.7
  jb ..B1.133 # Prob 82% #55.7
..B1.135: # Preds ..B1.133 ..B1.183
  cmp r13, rcx #55.7
  jae ..B1.139 # Prob 10% #55.7
  mov rsi, QWORD PTR [rbx+rdx*8] #56.20
  mov r8, QWORD PTR [r14+rdx*8] #56.9
  vmovss xmm0, DWORD PTR [rsi+r15*4] #56.20
  mov rsi, QWORD PTR [rdi+r15*8] #56.30
..B1.137: # Preds ..B1.137 ..B1.136
  vmovss xmm1, DWORD PTR [rsi+r13*4] #56.30
  vfmadd213ss xmm1, xmm0, DWORD PTR [r8+r13*4] #56.9
  vmovss DWORD PTR [r8+r13*4], xmm1 #56.9
  inc r13 #55.7
  cmp r13, rcx #55.7
  jb ..B1.137 # Prob 82% #55.7
..B1.139: # Preds ..B1.137 ..B1.135 ..B1.121
  inc r15 #54.5
  cmp r15, rcx #54.5
  jb ..B1.121 # Prob 82% #54.5
  .byte 15 #53.3
  .byte 31 #53.3
  .byte 132 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  .byte 0 #53.3
  inc rdx #53.3
  cmp rdx, rcx #53.3
  jb ..B1.120 # Prob 82% #53.3
  mov r13d, 1 #