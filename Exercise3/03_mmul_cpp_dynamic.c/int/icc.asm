operator*(std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&):
  push rbp #22.52
  mov rbp, rsp #22.52
  and rsp, -32 #22.52
  push r12 #22.52
  push r13 #22.52
  push r14 #22.52
  push r15 #22.52
  push rbx #22.52
  sub rsp, 88 #22.52
  mov rax, 0x2aaaaaaaaaaaaaab #656.52
  mov rcx, QWORD PTR [8+rsi] #656.26
  sub rcx, QWORD PTR [rsi] #656.26
  mov QWORD PTR [64+rsp], rdx #22.52[spill]
  imul rcx #656.52
  mov rbx, rdx #656.52
  shr rbx, 2 #656.52
  sar rcx, 63 #656.52
  sub rbx, rcx #656.52
  mov edx, ebx #23.17
  mov QWORD PTR [56+rsp], rsi #22.52[spill]
  mov QWORD PTR [48+rsp], rdi #22.52[spill]
  mov DWORD PTR [72+rsp], edx #23.17[spill]
  call std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >::vector() #10.9
  mov rsi, QWORD PTR [48+rsp] #656.26[spill]
  mov rax, 0x2aaaaaaaaaaaaaab #656.52
  mov ebx, ebx #11.13
  mov rdi, QWORD PTR [8+rsi] #656.26
  mov rcx, rdi #656.26
  mov rsi, QWORD PTR [rsi] #656.52
  sub rcx, rsi #656.26
  imul rcx #656.52
  mov r10, rdx #656.52
  mov r9, rcx #656.52
  sar r10, 2 #656.52
  sar r9, 63 #656.52
  mov r11, r10 #656.52
  sub r11, r9 #656.52
  cmp rbx, r11 #676.19
  ja ..B6.98 # Prob 5% #676.19
  jae ..B6.11 # Prob 78% #678.24
  lea rax, QWORD PTR [rbx+rbx*2] #679.20
  lea r12, QWORD PTR [rsi+rax*8] #679.20
  mov r13, r12 #126.7
  cmp r12, rdi #102.22
  je ..B6.10 # Prob 1% #102.22
  mov r15, rdi #
..B6.6: # Preds ..B6.8 ..B6.5
  mov rdi, QWORD PTR [r13] #160.23
  test rdi, rdi #177.6
  je ..B6.8 # Prob 72% #177.6
  call operator delete(void*) #110.9
..B6.8: # Preds ..B6.7 ..B6.6
  add r13, 24 #102.32
  cmp r13, r15 #102.22
  jne ..B6.6 # Prob 82% #102.22
..B6.10: # Preds ..B6.8 ..B6.4
  mov rax, QWORD PTR [48+rsp] #1437.2[spill]
  mov QWORD PTR [8+rax], r12 #1437.2
..B6.11: # Preds ..B6.3 ..B6.98 ..B6.134 ..B6.127 ..B6.10
  xor r12d, r12d #12.16
  cmp DWORD PTR [72+rsp], 0 #12.22[spill]
  jbe ..B6.42 # Prob 4% #12.22
  mov rax, QWORD PTR [48+rsp] #781.18[spill]
  mov r15d, DWORD PTR [72+rsp] #23.13[spill]
  shr r15d, 1 #23.13
  mov r10, QWORD PTR [rax] #781.18
..B6.13: # Preds ..B6.30 ..B6.12
  lea r14, QWORD PTR [r12+r12*2] #781.18
  lea r13, QWORD PTR [r10+r14*8] #781.18
  mov rax, QWORD PTR [8+r13] #656.26
  mov r9, rax #656.26
  mov rcx, QWORD PTR [r13] #656.52
  sub r9, rcx #656.26
  sar r9, 2 #656.52
  cmp rbx, r9 #676.19
  ja ..B6.43 # Prob 5% #676.19
  jae ..B6.16 # Prob 78% #678.24
  lea rax, QWORD PTR [rcx+rbx*4] #679.20
  mov QWORD PTR [8+r13], rax #1437.2
..B6.16: # Preds ..B6.67 ..B6.61 ..B6.43 ..B6.15 ..B6.14
  xor eax, eax #14.3
  mov ecx, 1 #14.3
  xor edx, edx #15.22
  test r15d, r15d #14.3
  jbe ..B6.26 # Prob 9% #14.3
..B6.18: # Preds ..B6.16 ..B6.24
  mov esi, edx #15.11
  cmp r12d, edx #15.22
  jne ..B6.20 # Prob 50% #15.22
  mov rcx, QWORD PTR [r10+r14*8] #781.18
  mov DWORD PTR [rcx+rsi*4], 1 #15.10
  jmp ..B6.21 # Prob 100% #15.10
..B6.20: # Preds ..B6.18
  mov rcx, QWORD PTR [r10+r14*8] #781.18
  mov DWORD PTR [rcx+rsi*4], 0 #15.10
..B6.21: # Preds ..B6.19 ..B6.20
  lea ecx, DWORD PTR [1+rdx] #14.26
  cmp r12d, ecx #15.22
  jne ..B6.23 # Prob 50% #15.22
  mov rsi, QWORD PTR [r10+r14*8] #781.18
  mov DWORD PTR [rsi+rcx*4], 1 #15.10
  jmp ..B6.24 # Prob 100% #15.10
..B6.23: # Preds ..B6.21
  mov rsi, QWORD PTR [r10+r14*8] #781.18
  mov DWORD PTR [rsi+rcx*4], 0 #15.10
..B6.24: # Preds ..B6.22 ..B6.23
  inc eax #14.3
  add edx, 2 #14.3
  cmp eax, r15d #14.3
  jb ..B6.18 # Prob 63% #14.3
  lea ecx, DWORD PTR [1+rax+rax] #15.22
..B6.26: # Preds ..B6.25 ..B6.16
  dec ecx #15.22
  cmp ecx, DWORD PTR [72+rsp] #14.3[spill]
  jae ..B6.30 # Prob 9% #14.3
  cmp r12d, ecx #15.22
  jne ..B6.29 # Prob 50% #15.22
  mov ecx, ecx #15.11
  mov r14, QWORD PTR [r10+r14*8] #781.18
  mov DWORD PTR [r14+rcx*4], 1 #15.10
  jmp ..B6.30 # Prob 100% #15.10
..B6.29: # Preds ..B6.27
  mov r14, QWORD PTR [r10+r14*8] #781.18
  mov DWORD PTR [r14+rcx*4], 0 #15.10
..B6.30: # Preds ..B6.26 ..B6.28 ..B6.29
  inc r12d #12.25
  cmp r12d, DWORD PTR [72+rsp] #12.22[spill]
  jb ..B6.13 # Prob 82% #12.22
  mov rsi, QWORD PTR [56+rsp] #796.18[spill]
  xor eax, eax #25.2
  mov rdx, QWORD PTR [64+rsp] #796.18[spill]
  xor edi, edi #25.2
  mov ebx, DWORD PTR [72+rsp] #25.2[spill]
  mov rsi, QWORD PTR [rsi] #796.18
  mov r8, QWORD PTR [rdx] #796.18
..B6.32: # Preds ..B6.40 ..B6.31
  xor edx, edx #26.3
  lea r9, QWORD PTR [rax+rax*2] #781.18
  xor ecx, ecx #26.3
..B6.33: # Preds ..B6.39 ..B6.32
  mov rax, QWORD PTR [r10+r9*8] #781.18
  mov r11d, 1 #28.4
  mov DWORD PTR [rax+rdx*4], 0 #27.8
  xor eax, eax #28.4
  test r15d, r15d #28.4
  jbe ..B6.37 # Prob 10% #28.4
..B6.35: # Preds ..B6.33 ..B6.35
  mov r11, QWORD PTR [r10+r9*8] #781.18
  lea r13d, DWORD PTR [rax+rax] #29.21
  lea r12, QWORD PTR [r13+r13*2] #796.18
  mov r14, QWORD PTR [r8+r12*8] #796.18
  mov r12, QWORD PTR [rsi+r9*8] #796.18
  mov r13d, DWORD PTR [r12+r13*4] #29.20
  lea r12d, DWORD PTR [1+rax+rax] #29.28
  imul r13d, DWORD PTR [r14+rdx*4] #29.30
  inc eax #28.4
  add DWORD PTR [r11+rdx*4], r13d #29.9
  lea r14, QWORD PTR [r12+r12*2] #796.18
  mov r11, QWORD PTR [r8+r14*8] #796.18
  mov r14, QWORD PTR [rsi+r9*8] #796.18
  mov r13, QWORD PTR [r10+r9*8] #781.18
  mov r12d, DWORD PTR [r14+r12*4] #29.20
  imul r12d, DWORD PTR [r11+rdx*4] #29.30
  add DWORD PTR [r13+rdx*4], r12d #29.9
  cmp eax, r15d #28.4
  jb ..B6.35 # Prob 64% #28.4
  lea r11d, DWORD PTR [1+rax+rax] #29.9
..B6.37: # Preds ..B6.36 ..B6.33
  dec r11d #29.9
  cmp r11d, ebx #28.4
  jae ..B6.39 # Prob 10% #28.4
  mov r13, QWORD PTR [rsi+r9*8] #796.18
  mov rax, QWORD PTR [r10+r9*8] #781.18
  lea r12, QWORD PTR [r11+r11*2] #796.18
  mov r11d, DWORD PTR [r13+r11*4] #29.20
  mov r14, QWORD PTR [r8+r12*8] #796.18
  imul r11d, DWORD PTR [r14+rdx*4] #29.30
  add DWORD PTR [rax+rdx*4], r11d #29.9
..B6.39: # Preds ..B6.37 ..B6.38
  inc ecx #26.3
  mov edx, ecx #26.3
  cmp ecx, ebx #26.3
  jb ..B6.33 # Prob 82% #26.3
  .byte 15 #25.2
  .byte 31 #25.2
  .byte 132 #25.2
  .byte 0 #25.2
  .byte 0 #25.2
  .byte 0 #25.2
  .byte 0 #25.2
  .byte 0 #25.2
  inc edi #25.2
  mov eax, edi #25.2
  cmp edi, ebx #25.2
  jb ..B6.32 # Prob 82% #25.2
..B6.42: # Preds ..B6.11 ..B6.40
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