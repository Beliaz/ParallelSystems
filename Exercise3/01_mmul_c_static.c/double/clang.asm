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