operator*(std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&):
  push r15
  push r14
  movabs rax, -6148914691236517205
  push r13
  push r12
  mov r13, rdx
  push rbp
  push rbx
  mov rbp, rsi
  sub rsp, 24
  mov rbx, QWORD PTR [rsi+8]
  sub rbx, QWORD PTR [rsi]
  mov QWORD PTR [rsp+8], rdi
  sar rbx, 3
  imul rbx, rax
  mov esi, ebx
  call id(unsigned int)
  test ebx, ebx
  mov rcx, QWORD PTR [rsp+8]
  je .L114
  lea eax, [rbx-1]
  mov rbx, QWORD PTR [r13+0]
  mov r15, QWORD PTR [rcx]
  mov r14, QWORD PTR [rbp+0]
  xor ebp, ebp
  lea rdx, [rax+rax*2]
  lea r12, [rax+3+rax*2]
  lea r13, [8+rax*8]
  lea r8, [rbx+24+rdx*8]
  sal r12, 3
.L110:
  mov r9, QWORD PTR [r15+rbp]
  mov r11, QWORD PTR [r14+rbp]
  mov rdi, r15
  lea r10, [r13+0+r9]
  mov rsi, r9
.L109:
  vxorpd xmm1, xmm1, xmm1
  mov r15, rsi
  mov QWORD PTR [rsi], 0x000000000
  sub r15, r9
  mov rdx, r11
  mov rax, rbx
  mov QWORD PTR [rsp+8], rcx
.L108:
  mov rcx, QWORD PTR [rax]
  add rax, 24
  add rdx, 8
  vmovsd xmm0, QWORD PTR [rcx+r15]
  vmulsd xmm0, xmm0, QWORD PTR [rdx-8]				// only multiplycation -> SISD
  cmp r8, rax
  vaddsd xmm1, xmm1, xmm0
  vmovsd QWORD PTR [rsi], xmm1
  jne .L108
  add rsi, 8
  mov rcx, QWORD PTR [rsp+8]
  cmp rsi, r10
  jne .L109
  add rbp, 24
  mov r15, rdi
  cmp r12, rbp
  jne .L110
