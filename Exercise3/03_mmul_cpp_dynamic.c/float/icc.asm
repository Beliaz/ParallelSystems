  mov r11, QWORD PTR [56+rsp] #796.18[spill]
  xor ecx, ecx #25.2
  mov r14, QWORD PTR [64+rsp] #796.18[spill]
  xor ebx, ebx #25.2
  mov r9d, DWORD PTR [72+rsp] #25.2[spill]
  xor r8d, r8d #25.2
  mov r11, QWORD PTR [r11] #796.18
  mov r14, QWORD PTR [r14] #796.18
..B6.24: # Preds ..B6.32 ..B6.23
  mov DWORD PTR [rsp], ecx #26.3[spill]
  xor r10d, r10d #26.3
  mov r12d, r8d #26.3
  lea rdi, QWORD PTR [rbx+rbx*2] #781.18
..B6.25: # Preds ..B6.31 ..B6.24
  mov rcx, QWORD PTR [rdx+rdi*8] #781.18
  mov ebx, eax #28.4
  mov esi, r8d #28.4
  mov DWORD PTR [rcx+r10*4], r8d #27.8
  test r13d, r13d #28.4
  jbe ..B6.29 # Prob 10% #28.4
..B6.27: # Preds ..B6.25 ..B6.27
  mov rcx, QWORD PTR [r11+rdi*8] #796.18
  lea r15d, DWORD PTR [rsi+rsi] #29.21
  mov rbx, QWORD PTR [rdx+rdi*8] #781.18
  vmovss xmm1, DWORD PTR [rcx+r15*4] #29.20
  lea rcx, QWORD PTR [r15+r15*2] #796.18
  mov r15, QWORD PTR [r14+rcx*8] #796.18
  vmovss xmm0, DWORD PTR [r15+r10*4] #29.30
  vfmadd213ss xmm1, xmm0, DWORD PTR [rbx+r10*4] #29.9
  vmovss DWORD PTR [rbx+r10*4], xmm1 #29.9
  mov r15, QWORD PTR [r11+rdi*8] #796.18
  lea ebx, DWORD PTR [1+rsi+rsi] #29.28
  mov rcx, QWORD PTR [rdx+rdi*8] #781.18
  inc esi #28.4
  vmovss xmm3, DWORD PTR [r15+rbx*4] #29.20
  lea rbx, QWORD PTR [rbx+rbx*2] #796.18
  mov r15, QWORD PTR [r14+rbx*8] #796.18
  vmovss xmm2, DWORD PTR [r15+r10*4] #29.30
  vfmadd213ss xmm3, xmm2, DWORD PTR [rcx+r10*4] #29.9
  vmovss DWORD PTR [rcx+r10*4], xmm3 #29.9
  cmp esi, r13d #28.4
  jb ..B6.27 # Prob 64% #28.4
  lea ebx, DWORD PTR [1+rsi+rsi] #29.9
..B6.29: # Preds ..B6.28 ..B6.25
  dec ebx #29.9
  cmp ebx, r9d #28.4
  jae ..B6.31 # Prob 10% #28.4
  mov rsi, QWORD PTR [r11+rdi*8] #796.18
  mov rcx, QWORD PTR [rdx+rdi*8] #781.18
  vmovss xmm1, DWORD PTR [rsi+rbx*4] #29.20
  lea r15, QWORD PTR [rbx+rbx*2] #796.18
  mov rbx, QWORD PTR [r14+r15*8] #796.18
  vmovss xmm0, DWORD PTR [rbx+r10*4] #29.30
  vfmadd213ss xmm1, xmm0, DWORD PTR [rcx+r10*4] #29.9
  vmovss DWORD PTR [rcx+r10*4], xmm1 #29.9
..B6.31: # Preds ..B6.29 ..B6.30
  inc r12d #26.3
  mov r10d, r12d #26.3
  cmp r12d, r9d #26.3
  jb ..B6.25 # Prob 82% #26.3
  .byte 15 #
  .byte 31 #
  .byte 64 #
  .byte 0 #
  mov ecx, DWORD PTR [rsp] #[spill]
  inc ecx #25.2
  mov ebx, ecx #25.2
  cmp ecx, r9d #25.2
  jb ..B6.24 # Prob 82% #25.2
..B6.34: # Preds ..B6.11 ..B6.32
  vzeroupper #33.2
  add rsp, 88 #33.2
  pop rbx #33.2
  pop r15 #33.2
  pop r14 #33.2
  pop r13 #33.2
  pop r12 #33.2
  mov rsp, rbp #33.2
  pop rbp #33.2
  ret #33.2