$LL22@main:
    vmulpd   ymm1, ymm0, YMMWORD PTR [r8+rax-8]
    vaddpd   ymm2, ymm1, YMMWORD PTR [rax]
    vmulpd   ymm1, ymm0, YMMWORD PTR [rax+r8+24]
    vmovupd YMMWORD PTR [rax], ymm2
    vaddpd   ymm2, ymm1, YMMWORD PTR [rax+32]
    add      rdx, 8
    vmovupd YMMWORD PTR [rax+32], ymm2
    lea      rax, QWORD PTR [rax+64]
    cmp      rdx, rcx
    jl       SHORT $LL22@main