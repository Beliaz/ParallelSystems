operator*(std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&):
  push r15
  push r14
  movabs rax, -6148914691236517205
  push r13
  push r12
  mov r14, rdi
  push rbp
  push rbx
  mov rbp, rsi
  mov r12, rdx
  sub rsp, 24
  mov rbx, QWORD PTR [rsi+8]
  sub rbx, QWORD PTR [rsi]
  mov QWORD PTR [rsp+8], rdi
  sar rbx, 3
  imul rbx, rax
  mov esi, ebx
  call id(unsigned int)
  test ebx, ebx
  je .L112
  lea eax, [rbx-1]
  mov r15, QWORD PTR [r14]
  mov r14, QWORD PTR [rbp+0]
  mov rbp, QWORD PTR [r12]
  xor r12d, r12d
  lea rdx, [rax+3+rax*2]
  lea r13, [4+rax*4]
  lea rbx, [0+rdx*8]
  lea rdx, [rax+rax*2]
  lea r9, [rbp+24+rdx*8]
  mov QWORD PTR [rsp], rbx
.L108:
  mov r10, QWORD PTR [r15+r12]
  mov rbx, QWORD PTR [r14+r12]
  lea r11, [r13+0+r10]
  mov rdi, r10
.L107:
  mov r8, rdi
  mov DWORD PTR [rdi], 0
  mov rsi, rbx
  sub r8, r10
  mov rax, rbp
  xor ecx, ecx
.L106:
  mov rdx, QWORD PTR [rax]
  add rax, 24
  add rsi, 4
  mov edx, DWORD PTR [rdx+r8]
  imul edx, DWORD PTR [rsi-4]
  add ecx, edx
  cmp r9, rax
  mov DWORD PTR [rdi], ecx
  jne .L106
  add rdi, 4
  cmp rdi, r11
  jne .L107
  add r12, 24
  cmp QWORD PTR [rsp], r12
  jne .L108
.L112:
  mov rax, QWORD PTR [rsp+8]
  add rsp, 24
  pop rbx
  pop rbp
  pop r12
  pop r13
  pop r14
  pop r15
  ret