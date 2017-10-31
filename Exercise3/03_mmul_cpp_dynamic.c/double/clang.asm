operator*(std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&): # @operator*(std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > const&)
  push r15
  push r14
  push r13
  push r12
  push rbx
  mov r15, rdx
  mov r12, rsi
  mov r14, rdi
  mov rax, qword ptr [r12 + 8]
  sub rax, qword ptr [r12]
  sar rax, 3
  movabs r13, -6148914691236517205
  imul r13, rax
  mov esi, r13d
  call id(unsigned int)
  test r13d, r13d
  je .LBB1_9
  mov r8, qword ptr [r12]
  mov r11, qword ptr [r15]
  mov r9, qword ptr [r14]
  mov r15d, r13d
  and r13d, 1
  mov r12, r15
  sub r12, r13
  xor r10d, r10d
.LBB1_2: # =>This Loop Header: Depth=1
  lea rax, [8*r10]
  lea rax, [rax + 2*rax]
  mov rdx, qword ptr [r9 + rax]
  mov rax, qword ptr [r8 + rax]
  xor ebx, ebx
.LBB1_3: # Parent Loop BB1_2 Depth=1
  mov qword ptr [rdx + 8*rbx], 0
  vxorpd xmm0, xmm0, xmm0
  cmp r15, 1
  jne .LBB1_10
  xor ecx, ecx
  test r13, r13
  jne .LBB1_6
  jmp .LBB1_7
.LBB1_10: # in Loop: Header=BB1_3 Depth=2
  mov rsi, r11
  xor ecx, ecx
.LBB1_11: # Parent Loop BB1_2 Depth=1
  vmovsd xmm1, qword ptr [rax + 8*rcx] # xmm1 = mem[0],zero
  mov rdi, qword ptr [rsi]
  vmulsd xmm1, xmm1, qword ptr [rdi + 8*rbx]	// Multiply1
  vaddsd xmm0, xmm0, xmm1
  vmovsd qword ptr [rdx + 8*rbx], xmm0
  vmovsd xmm1, qword ptr [rax + 8*rcx + 8] # xmm1 = mem[0],zero
  mov rdi, qword ptr [rsi + 24]
  vmulsd xmm1, xmm1, qword ptr [rdi + 8*rbx]	// MuLtiply2
  vaddsd xmm0, xmm0, xmm1
  vmovsd qword ptr [rdx + 8*rbx], xmm0
  add rcx, 2								// unroll k+=2
  add rsi, 48
  cmp r12, rcx
  jne .LBB1_11
  test r13, r13
  je .LBB1_7
.LBB1_6: # in Loop: Header=BB1_3 Depth=2
  vmovsd xmm1, qword ptr [rax + 8*rcx] # xmm1 = mem[0],zero
  lea rcx, [rcx + 2*rcx]
  mov rcx, qword ptr [r11 + 8*rcx]
  vmulsd xmm1, xmm1, qword ptr [rcx + 8*rbx]
  vaddsd xmm0, xmm0, xmm1
  vmovsd qword ptr [rdx + 8*rbx], xmm0
.LBB1_7: # in Loop: Header=BB1_3 Depth=2
  inc rbx									// ++j
  cmp rbx, r15								// i<n
  jne .LBB1_3
  inc r10                  					// ++i
  cmp r10, r15								// j<n
  jne .LBB1_2
.LBB1_9:
  mov rax, r14
  pop rbx
  pop r12
  pop r13
  pop r14
  pop r15
  ret
