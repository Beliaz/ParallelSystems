.LBB0_35: # Parent Loop BB0_21 Depth=1
  vpmulld ymm1, ymm0, ymmword ptr [rdx - 96]
  vpmulld ymm2, ymm0, ymmword ptr [rdx - 64]
  vpmulld ymm3, ymm0, ymmword ptr [rdx - 32]
  vpmulld ymm4, ymm0, ymmword ptr [rdx]
  vpaddd ymm1, ymm1, ymmword ptr [rcx - 96]
  vpaddd ymm2, ymm2, ymmword ptr [rcx - 64]
  vpaddd ymm3, ymm3, ymmword ptr [rcx - 32]
  vpaddd ymm4, ymm4, ymmword ptr [rcx]
  vmovdqu ymmword ptr [rcx - 96], ymm1
  vmovdqu ymmword ptr [rcx - 64], ymm2
  vmovdqu ymmword ptr [rcx - 32], ymm3
  vmovdqu ymmword ptr [rcx], ymm4
  sub rdx, -128
  sub rcx, -128
  add rdi, -32
  jne .LBB0_35
  mov rax, r10
  test r8d, r8d
  je .LBB0_37
.LBB0_24: # in Loop: Header=BB0_22 Depth=2
  lea rcx, [r13 + 4*rax]
  lea rdx, [r11 + 4*rax]
  mov rdi, r15
  sub rdi, rax
.LBB0_25: # Parent Loop BB0_21 Depth=1
  mov eax, dword ptr [rcx]
  imul eax, ebx
  add dword ptr [rdx], eax