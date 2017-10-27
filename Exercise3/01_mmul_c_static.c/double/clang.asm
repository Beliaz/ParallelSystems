.LBB0_37: # Parent Loop BB0_23 Depth=1
  vmulpd ymm2, ymm1, ymmword ptr [rdx - 96]
  vmulpd ymm3, ymm1, ymmword ptr [rdx - 64]
  vmulpd ymm4, ymm1, ymmword ptr [rdx - 32]
  vmulpd ymm5, ymm1, ymmword ptr [rdx]
  vaddpd ymm2, ymm2, ymmword ptr [rcx - 96]
  vaddpd ymm3, ymm3, ymmword ptr [rcx - 64]
  vaddpd ymm4, ymm4, ymmword ptr [rcx - 32]
  vaddpd ymm5, ymm5, ymmword ptr [rcx]
  vmovupd ymmword ptr [rcx - 96], ymm2
  vmovupd ymmword ptr [rcx - 64], ymm3
  vmovupd ymmword ptr [rcx - 32], ymm4
  vmovupd ymmword ptr [rcx], ymm5
  sub rdx, -128
  sub rcx, -128
  add rbx, -16
  jne .LBB0_37
  mov rdi, r10
  test r8d, r8d
  je .LBB0_39
.LBB0_26: # in Loop: Header=BB0_24 Depth=2
  lea rcx, [rax + 8*rdi]
  lea rdx, [r11 + 8*rdi]
  mov rbx, r14
  sub rbx, rdi
.LBB0_27: # Parent Loop BB0_23 Depth=1
  vmulsd xmm1, xmm0, qword ptr [rcx]
  vaddsd xmm1, xmm1, qword ptr [rdx]
  vmovsd qword ptr [rdx], xmm1