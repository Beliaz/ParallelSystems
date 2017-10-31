 jle .LBB2_75
  lea r9, [r15 - 1]
  mov rax, r15
  sub rax, qword ptr [rsp + 32] # 8-byte Folded Reload
  mov qword ptr [rsp + 40], rax # 8-byte Spill
  xor ecx, ecx
  mov qword ptr [rsp + 48], r12 # 8-byte Spill
.LBB2_48: # =>This Loop Header: Depth=1
  mov rbp, qword ptr [r12 + 8*rcx]
  mov rax, qword ptr [rsp + 8] # 8-byte Reload
  mov qword ptr [rsp + 56], rcx # 8-byte Spill
  mov r11, qword ptr [rax + 8*rcx]
  lea r12, [r11 + 4*r15]
  lea rax, [r11 + 96]
  mov qword ptr [rsp + 64], rax # 8-byte Spill
  lea r13, [r11 + 4]
  xor esi, esi
.LBB2_49: # Parent Loop BB2_48 Depth=1
  lea rdi, [rbp + 4*rsi]
  mov r14, qword ptr [r8 + 8*rsi]
  cmp r10d, 31
  ja .LBB2_51
.LBB2_50: # in Loop: Header=BB2_49 Depth=2
  xor eax, eax
  jmp .LBB2_56
.LBB2_51: # in Loop: Header=BB2_49 Depth=2
  lea rax, [rbp + 4*rsi + 1]
  lea rdx, [r14 + 4*r15]
  cmp rax, r11
  seta al
  cmp rdi, r12
  setb cl
  cmp r11, rdx
  setb dl
  cmp r14, r12
  setb bl
  test al, cl
  jne .LBB2_50
  mov eax, 0
  and dl, bl
  jne .LBB2_56
  vpbroadcastd ymm0, dword ptr [rdi]
  lea r8, [r14 + 96]
  mov rax, qword ptr [rsp + 40] # 8-byte Reload
  mov r10, qword ptr [rsp + 64] # 8-byte Reload
.LBB2_54: # Parent Loop BB2_48 Depth=1
  vpmulld ymm1, ymm0, ymmword ptr [r8 - 96]
  vpmulld ymm2, ymm0, ymmword ptr [r8 - 64]
  vpmulld ymm3, ymm0, ymmword ptr [r8 - 32]
  vpmulld ymm4, ymm0, ymmword ptr [r8]
  vpaddd ymm1, ymm1, ymmword ptr [r10 - 96]
  vpaddd ymm2, ymm2, ymmword ptr [r10 - 64]
  vpaddd ymm3, ymm3, ymmword ptr [r10 - 32]
  vpaddd ymm4, ymm4, ymmword ptr [r10]
  vmovdqu ymmword ptr [r10 - 96], ymm1
  vmovdqu ymmword ptr [r10 - 64], ymm2
  vmovdqu ymmword ptr [r10 - 32], ymm3
  vmovdqu ymmword ptr [r10], ymm4
  sub r8, -128
  sub r10, -128
  add rax, -32
  jne .LBB2_54
  mov rax, qword ptr [rsp + 40] # 8-byte Reload
  cmp dword ptr [rsp + 32], 0 # 4-byte Folded Reload
  mov r10, qword ptr [rsp + 16] # 8-byte Reload
  mov r8, qword ptr [rsp + 24] # 8-byte Reload
  je .LBB2_61
.LBB2_56: # in Loop: Header=BB2_49 Depth=2
  mov edx, r15d
  sub edx, eax
  mov rbx, rax
  test dl, 1
  je .LBB2_58
  mov edx, dword ptr [r14 + 4*rax]
  imul edx, dword ptr [rdi]
  add dword ptr [r11 + 4*rax], edx
  lea rbx, [rax + 1]
.LBB2_58: # in Loop: Header=BB2_49 Depth=2
  cmp r9, rax
  je .LBB2_61
  mov rax, r15
  sub rax, rbx
  lea rdx, [r14 + 4*rbx + 4]
  lea rbx, [r13 + 4*rbx]
.LBB2_60: # Parent Loop BB2_48 Depth=1
  mov ecx, dword ptr [rdx - 4]
  imul ecx, dword ptr [rdi]
  add dword ptr [rbx - 4], ecx
  mov ecx, dword ptr [rdx]
  imul ecx, dword ptr [rdi]
  add dword ptr [rbx], ecx
  add rdx, 8
  add rbx, 8
  add rax, -2
  jne .LBB2_60
.LBB2_61: # in Loop: Header=BB2_49 Depth=2
  inc rsi
  cmp rsi, r15
  jne .LBB2_49
  mov rcx, qword ptr [rsp + 56] # 8-byte Reload
  inc rcx
  cmp rcx, r15
  mov r12, qword ptr [rsp + 48] # 8-byte Reload
  jne .LBB2_48