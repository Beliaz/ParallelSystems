.LBB0_37: # Parent Loop BB0_23 Depth=1
  vmulps ymm2, ymm1, ymmword ptr [rdx - 96]
  vmulps ymm3, ymm1, ymmword ptr [rdx - 64]
  vmulps ymm4, ymm1, ymmword ptr [rdx - 32]
  vmulps ymm5, ymm1, ymmword ptr [rdx]
  vaddps ymm2, ymm2, ymmword ptr [rcx - 96]
  vaddps ymm3, ymm3, ymmword ptr [rcx - 64]
  vaddps ymm4, ymm4, ymmword ptr [rcx - 32]
  vaddps ymm5, ymm5, ymmword ptr [rcx]
  vmovups ymmword ptr [rcx - 96], ymm2
  vmovups ymmword ptr [rcx - 64], ymm3
  vmovups ymmword ptr [rcx - 32], ymm4
  vmovups ymmword ptr [rcx], ymm5
  sub rdx, -128
  sub rcx, -128
  add rbx, -32
  jne .LBB0_37
  mov rdi, r10
  test r8d, r8d
  je .LBB0_39
.LBB0_26: # in Loop: Header=BB0_24 Depth=2
  lea rcx, [rax + 4*rdi]
  lea rdx, [r11 + 4*rdi]
  mov rbx, r14
  sub rbx, rdi
.LBB0_27: # Parent Loop BB0_23 Depth=1
  vmulss xmm1, xmm0, dword ptr [rcx]
  vaddss xmm1, xmm1, dword ptr [rdx]
  vmovss dword ptr [rdx], xmm1