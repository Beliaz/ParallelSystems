 jle .LBB2_77
  lea r9, [r15 - 1]
  mov rax, r15
  sub rax, qword ptr [rsp + 24] # 8-byte Folded Reload
  mov qword ptr [rsp + 32], rax # 8-byte Spill
  xor eax, eax
  mov qword ptr [rsp + 48], r14 # 8-byte Spill
  mov qword ptr [rsp + 40], r12 # 8-byte Spill
.LBB2_50: # =>This Loop Header: Depth=1
  mov rbp, qword ptr [r14 + 8*rax]
  mov qword ptr [rsp + 56], rax # 8-byte Spill
  mov r11, qword ptr [r12 + 8*rax]
  lea r12, [r11 + 4*r15]
  lea rax, [r11 + 96]
  mov qword ptr [rsp + 64], rax # 8-byte Spill
  lea r13, [r11 + 4]
  xor esi, esi
.LBB2_51: # Parent Loop BB2_50 Depth=1
  lea rdi, [rbp + 4*rsi]
  mov r14, qword ptr [r8 + 8*rsi]
  cmp r10d, 31
  ja .LBB2_53
.LBB2_52: # in Loop: Header=BB2_51 Depth=2
  xor eax, eax
  jmp .LBB2_58
.LBB2_53: # in Loop: Header=BB2_51 Depth=2
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
  jne .LBB2_52
  mov eax, 0
  and dl, bl
  jne .LBB2_58
  vbroadcastss ymm0, dword ptr [rdi]
  lea r8, [r14 + 96]
  mov rax, qword ptr [rsp + 32] # 8-byte Reload
  mov r10, qword ptr [rsp + 64] # 8-byte Reload
.LBB2_56: # Parent Loop BB2_50 Depth=1
  vmulps ymm1, ymm0, ymmword ptr [r8 - 96]
  vmulps ymm2, ymm0, ymmword ptr [r8 - 64]
  vmulps ymm3, ymm0, ymmword ptr [r8 - 32]
  vmulps ymm4, ymm0, ymmword ptr [r8]
  vaddps ymm1, ymm1, ymmword ptr [r10 - 96]
  vaddps ymm2, ymm2, ymmword ptr [r10 - 64]
  vaddps ymm3, ymm3, ymmword ptr [r10 - 32]
  vaddps ymm4, ymm4, ymmword ptr [r10]
  vmovups ymmword ptr [r10 - 96], ymm1
  vmovups ymmword ptr [r10 - 64], ymm2
  vmovups ymmword ptr [r10 - 32], ymm3
  vmovups ymmword ptr [r10], ymm4
  sub r8, -128
  sub r10, -128
  add rax, -32
  jne .LBB2_56
  mov rax, qword ptr [rsp + 32] # 8-byte Reload
  cmp dword ptr [rsp + 24], 0 # 4-byte Folded Reload
  mov r10, qword ptr [rsp + 8] # 8-byte Reload
  mov r8, qword ptr [rsp + 16] # 8-byte Reload
  je .LBB2_63
.LBB2_58: # in Loop: Header=BB2_51 Depth=2
  mov edx, r15d
  sub edx, eax
  mov rbx, rax
  test dl, 1
  je .LBB2_60
  vmovss xmm0, dword ptr [rdi] # xmm0 = mem[0],zero,zero,zero
  vmulss xmm0, xmm0, dword ptr [r14 + 4*rax]
  vaddss xmm0, xmm0, dword ptr [r11 + 4*rax]
  vmovss dword ptr [r11 + 4*rax], xmm0
  lea rbx, [rax + 1]
.LBB2_60: # in Loop: Header=BB2_51 Depth=2
  cmp r9, rax
  je .LBB2_63
  mov rax, r15
  sub rax, rbx
  lea rdx, [r14 + 4*rbx + 4]
  lea rbx, [r13 + 4*rbx]
.LBB2_62: # Parent Loop BB2_50 Depth=1
  vmovss xmm0, dword ptr [rdi] # xmm0 = mem[0],zero,zero,zero
  vmulss xmm0, xmm0, dword ptr [rdx - 4]
  vaddss xmm0, xmm0, dword ptr [rbx - 4]
  vmovss dword ptr [rbx - 4], xmm0
  vmovss xmm0, dword ptr [rdi] # xmm0 = mem[0],zero,zero,zero
  vmulss xmm0, xmm0, dword ptr [rdx]
  vaddss xmm0, xmm0, dword ptr [rbx]
  vmovss dword ptr [rbx], xmm0
  add rdx, 8
  add rbx, 8
  add rax, -2
  jne .LBB2_62
.LBB2_63: # in Loop: Header=BB2_51 Depth=2
  inc rsi
  cmp rsi, r15
  jne .LBB2_51
  mov rax, qword ptr [rsp + 56] # 8-byte Reload
  inc rax
  cmp rax, r15
  mov r14, qword ptr [rsp + 48] # 8-byte Reload
  mov r12, qword ptr [rsp + 40] # 8-byte Reload
  jne .LBB2_50