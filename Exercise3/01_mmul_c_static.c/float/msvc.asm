$LL22@main:
        vmulps   ymm2, ymm3, YMMWORD PTR [rdx+rax-4]
        vaddps   ymm0, ymm2, YMMWORD PTR [rax]
        vmulps   ymm2, ymm3, YMMWORD PTR [rdx+rax+28]
        vmovups YMMWORD PTR [rax], ymm0
        vaddps   ymm0, ymm2, YMMWORD PTR [rax+32]
        add      r8, 16
        vmovups YMMWORD PTR [rax+32], ymm0
        lea      rax, QWORD PTR [rax+64]
        cmp      r8, rcx
        jl       SHORT $LL22@main
$LN58@main:
        cmp      r8, r10
        jge      $LN17@main
        mov      rax, r10
        sub      rax, r8
        cmp      rax, 4
        jl       SHORT $LC81@main
        lea      rax, QWORD PTR [rbx+r11*4]
        vbroadcastss xmm2, DWORD PTR [rax+r12]
        lea      rax, QWORD PTR [r8+rdi]
        lea      rcx, QWORD PTR [r15+rax*4]
        mov      rax, r10
        sub      rax, r8
        sub      rax, 4
        shr      rax, 2
        inc      rax
        lea      r8, QWORD PTR [r8+rax*4]
        npad     9
$LL82@main:
        vmulps   xmm1, xmm2, XMMWORD PTR [rdx+rcx-4]
        vaddps   xmm1, xmm1, XMMWORD PTR [rcx]
        vmovups XMMWORD PTR [rcx], xmm1
        lea      rcx, QWORD PTR [rcx+16]
        sub      rax, 1
        jne      SHORT $LL82@main
$LC81@main:
        cmp      r8, r10
        jge      SHORT $LN17@main
        lea      rax, QWORD PTR [rbx+r11*4]
        vmovss   xmm2, DWORD PTR [rax+r12]
        lea      rax, QWORD PTR [r8+rdi]
        lea      rcx, QWORD PTR [r15+rax*4]
        mov      rax, r10
        sub      rax, r8
$LC57@main:
        vmulss   xmm0, xmm2, DWORD PTR [rdx+rcx-4]
        vaddss   xmm1, xmm0, DWORD PTR [rcx]
        vmovss   DWORD PTR [rcx], xmm1