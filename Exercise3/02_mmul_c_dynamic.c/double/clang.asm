 jle .LBB2_77
  lea r9, [r15 - 1]
  mov rax, r15
  sub rax, qword ptr [rsp + 8] # 8-byte Folded Reload
  mov qword ptr [rsp + 32], rax # 8-byte Spill
  xor eax, eax
  mov qword ptr [rsp + 48], r14 # 8-byte Spill
  mov qword ptr [rsp + 40], r12 # 8-byte Spill
.LBB2_50: # =>This Loop Header: Depth=1
  mov rbp, qword ptr [r14 + 8*rax]
  mov qword ptr [rsp + 56], rax # 8-byte Spill
  mov r11, qword ptr [r12 + 8*rax]
  lea r12, [r11 + 8*r15]
  lea rax, [r11 + 96]
  mov qword ptr [rsp + 64], rax # 8-byte Spill
  lea r13, [r11 + 8]
  xor esi, esi
.LBB2_51: # Parent Loop BB2_50 Depth=1
  lea rdi, [rbp + 8*rsi]
  mov r14, qword ptr [r10 + 8*rsi]
  cmp r8d, 15
  ja .LBB2_53
.LBB2_52: # in Loop: Header=BB2_51 Depth=2
  xor eax, eax
  jmp .LBB2_58
.LBB2_53: # in Loop: Header=BB2_51 Depth=2
  lea rax, [rbp + 8*rsi + 1]
  lea rdx, [r14 + 8*r15]
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
  vbroadcastsd ymm0, qword ptr [rdi]
  lea r8, [r14 + 96]
  mov rax, qword ptr [rsp + 32] # 8-byte Reload
  mov r10, qword ptr [rsp + 64] # 8-byte Reload
.LBB2_56: # Parent Loop BB2_50 Depth=1
  vmulpd ymm1, ymm0, ymmword ptr [r8 - 96]
  vmulpd ymm2, ymm0, ymmword ptr [r8 - 64]
  vmulpd ymm3, ymm0, ymmword ptr [r8 - 32]
  vmulpd ymm4, ymm0, ymmword ptr [r8]
  vaddpd ymm1, ymm1, ymmword ptr [r10 - 96]
  vaddpd ymm2, ymm2, ymmword ptr [r10 - 64]
  vaddpd ymm3, ymm3, ymmword ptr [r10 - 32]
  vaddpd ymm4, ymm4, ymmword ptr [r10]
  vmovupd ymmword ptr [r10 - 96], ymm1
  vmovupd ymmword ptr [r10 - 64], ymm2
  vmovupd ymmword ptr [r10 - 32], ymm3
  vmovupd ymmword ptr [r10], ymm4
  sub r8, -128
  sub r10, -128
  add rax, -16
  jne .LBB2_56
  mov rax, qword ptr [rsp + 32] # 8-byte Reload
  cmp dword ptr [rsp + 8], 0 # 4-byte Folded Reload
  mov r8, qword ptr [rsp + 24] # 8-byte Reload
  mov r10, qword ptr [rsp + 16] # 8-byte Reload
  je .LBB2_63
.LBB2_58: # in Loop: Header=BB2_51 Depth=2
  mov edx, r15d
  sub edx, eax
  mov rbx, rax
  test dl, 1
  je .LBB2_60
  vmovsd xmm0, qword ptr [rdi] # xmm0 = mem[0],zero
  vmulsd xmm0, xmm0, qword ptr [r14 + 8*rax]
  vaddsd xmm0, xmm0, qword ptr [r11 + 8*rax]
  vmovsd qword ptr [r11 + 8*rax], xmm0
  lea rbx, [rax + 1]
.LBB2_60: # in Loop: Header=BB2_51 Depth=2
  cmp r9, rax
  je .LBB2_63
  mov rax, r15
  sub rax, rbx
  lea rdx, [r14 + 8*rbx + 8]
  lea rbx, [r13 + 8*rbx]
.LBB2_62: # Parent Loop BB2_50 Depth=1
  vmovsd xmm0, qword ptr [rdi] # xmm0 = mem[0],zero
  vmulsd xmm0, xmm0, qword ptr [rdx - 8]
  vaddsd xmm0, xmm0, qword ptr [rbx - 8]
  vmovsd qword ptr [rbx - 8], xmm0
  vmovsd xmm0, qword ptr [rdi] # xmm0 = mem[0],zero
  vmulsd xmm0, xmm0, qword ptr [rdx]
  vaddsd xmm0, xmm0, qword ptr [rbx]
  vmovsd qword ptr [rbx], xmm0
  add rdx, 16
  add rbx, 16
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
