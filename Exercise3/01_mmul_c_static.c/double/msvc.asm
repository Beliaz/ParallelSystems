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
$LN53@main:
        cmp      rdx, r9
        jge      $LN17@main
        mov      rax, r9
        sub      rax, rdx
        cmp      rax, 4
        jl       SHORT $LC71@main
        lea      rax, QWORD PTR [rbx+r10*8]
        vbroadcastsd ymm2, QWORD PTR [rax+r12]
        lea      rax, QWORD PTR [rdi+rdx]
        lea      rcx, QWORD PTR [r15+rax*8]
        mov      rax, r9
        sub      rax, rdx
        sub      rax, 4
        shr      rax, 2
        inc      rax
        lea      rdx, QWORD PTR [rdx+rax*4]
        npad     7
$LL72@main:
        vmulpd   ymm1, ymm2, YMMWORD PTR [rcx+r8-8]
        vaddpd   ymm1, ymm1, YMMWORD PTR [rcx]
        vmovups YMMWORD PTR [rcx], ymm1
        lea      rcx, QWORD PTR [rcx+32]
        sub      rax, 1
        jne      SHORT $LL72@main
$LC71@main:
        cmp      rdx, r9
        jge      SHORT $LN17@main
        lea      rax, QWORD PTR [rbx+r10*8]
        vmovsd   xmm2, QWORD PTR [rax+r12]
        lea      rax, QWORD PTR [rdi+rdx]
        lea      rcx, QWORD PTR [r15+rax*8]
        mov      rax, r9
        sub      rax, rdx
$LC52@main:
        vmulsd   xmm0, xmm2, QWORD PTR [r8+rcx-8]
        vaddsd   xmm1, xmm0, QWORD PTR [rcx]
        vmovsd   QWORD PTR [rcx], xmm1