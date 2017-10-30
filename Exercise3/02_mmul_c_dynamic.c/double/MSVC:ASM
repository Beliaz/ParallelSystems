  test     ebx, ebx
        jle      $LN15@main
        mov      rdi, r12
        mov      rbx, r15
        sub      rdi, r15
        mov      rsi, r10
$LL16@main:
        xor      edx, edx
$LL19@main:
        xor      r11d, r11d
        cmp      r10, 4
        jl       $LC73@main
        mov      rax, QWORD PTR [rbx]
        mov      r8, QWORD PTR [r14+rdx*8]
        mov      r9, QWORD PTR [rdi+rbx]
        sub      r8, rax
        lea      rcx, QWORD PTR [rax+8]
        lea      rax, QWORD PTR [r10-4]
        shr      rax, 2
        inc      rax
        lea      r11, QWORD PTR [rax*4]
$LL74@main:
        vmovsd   xmm0, QWORD PTR [r8+rcx-8]
        lea      rcx, QWORD PTR [rcx+32]
        vmulsd   xmm1, xmm0, QWORD PTR [r9+rdx*8]
        vaddsd   xmm2, xmm1, QWORD PTR [rcx-40]
        vmovsd   QWORD PTR [rcx-40], xmm2
        vmovsd   xmm0, QWORD PTR [r8+rcx-32]
        vmulsd   xmm1, xmm0, QWORD PTR [r9+rdx*8]
        vaddsd   xmm2, xmm1, QWORD PTR [rcx-32]
        vmovsd   QWORD PTR [rcx-32], xmm2
        vmovsd   xmm0, QWORD PTR [r8+rcx-24]
        vmulsd   xmm1, xmm0, QWORD PTR [r9+rdx*8]
        vaddsd   xmm2, xmm1, QWORD PTR [rcx-24]
        vmovsd   QWORD PTR [rcx-24], xmm2
        vmovsd   xmm0, QWORD PTR [r8+rcx-16]
        vmulsd   xmm1, xmm0, QWORD PTR [r9+rdx*8]
        vaddsd   xmm2, xmm1, QWORD PTR [rcx-16]
        vmovsd   QWORD PTR [rcx-16], xmm2
        sub      rax, 1
        jne      SHORT $LL74@main
$LC73@main:
        cmp      r11, r10
        jge      SHORT $LN17@main
        mov      rax, QWORD PTR [rbx]
        mov      r8, QWORD PTR [r14+rdx*8]
        mov      r9, QWORD PTR [rdi+rbx]
        sub      r8, rax
        lea      rcx, QWORD PTR [rax+r11*8]
        mov      rax, r10
        sub      rax, r11
$LC22@main:
        vmovsd   xmm0, QWORD PTR [rcx+r8]
        lea      rcx, QWORD PTR [rcx+8]
        vmulsd   xmm1, xmm0, QWORD PTR [r9+rdx*8]
        vaddsd   xmm2, xmm1, QWORD PTR [rcx-8]
        vmovsd   QWORD PTR [rcx-8], xmm2
        sub      rax, 1
        jne      SHORT $LC22@main
$LN17@main:
        inc      rdx
        cmp      rdx, r10
        jl       $LL19@main
        add      rbx, 8
        sub      rsi, 1
        jne      $LL16@main
