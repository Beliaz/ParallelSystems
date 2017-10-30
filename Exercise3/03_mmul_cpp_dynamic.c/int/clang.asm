operator*(std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&): # @operator*(std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&)
  push rbp
  push r15
  push r14
  push r13
  push r12
  push rbx
  push rax
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
  mov dword ptr [rdx + 4*rbx], 0
  xor ecx, ecx
  cmp r15, 1
  jne .LBB1_10
  xor esi, esi
  test r13, r13
  jne .LBB1_6
  jmp .LBB1_7
.LBB1_10: # in Loop: Header=BB1_3 Depth=2
  mov rdi, r11
  xor esi, esi
.LBB1_11: # Parent Loop BB1_2 Depth=1
  mov rbp, qword ptr [rdi]
  mov ebp, dword ptr [rbp + 4*rbx]
  imul ebp, dword ptr [rax + 4*rsi]
  add ebp, ecx
  mov dword ptr [rdx + 4*rbx], ebp
  mov rcx, qword ptr [rdi + 24]
  mov ecx, dword ptr [rcx + 4*rbx]
  imul ecx, dword ptr [rax + 4*rsi + 4]
  add ecx, ebp
  mov dword ptr [rdx + 4*rbx], ecx
  add rsi, 2
  add rdi, 48
  cmp r12, rsi
  jne .LBB1_11
  test r13, r13
  je .LBB1_7
.LBB1_6: # in Loop: Header=BB1_3 Depth=2
  lea rdi, [rsi + 2*rsi]
  mov rdi, qword ptr [r11 + 8*rdi]
  mov edi, dword ptr [rdi + 4*rbx]
  imul edi, dword ptr [rax + 4*rsi]
  add ecx, edi
  mov dword ptr [rdx + 4*rbx], ecx
.LBB1_7: # in Loop: Header=BB1_3 Depth=2
  inc rbx
  cmp rbx, r15
  jne .LBB1_3
  inc r10
  cmp r10, r15
  jne .LBB1_2
.LBB1_9:
  mov rax, r14
  add rsp, 8
  pop rbx
  pop r12
  pop r13
  pop r14
  pop r15
  pop rbp
  ret