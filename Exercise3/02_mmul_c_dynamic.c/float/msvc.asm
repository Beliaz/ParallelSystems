        vmovaps xmm6, XMMWORD PTR [rsp+32]
        test     ebx, ebx
        jle      $LN15@main
        mov      rdi, r12
        mov      QWORD PTR [rsp+80], rsi
        sub      rdi, r15
        mov      rbx, r15
        mov      rsi, r10
        npad     1
$LL16@main:
        xor      edx, edx
$LL19@main:
        xor      r11d, r11d
        cmp      r10, 4
        jl       $LC83@main
        mov      rax, QWORD PTR [rbx]
        mov      r8, QWORD PTR [r14+rdx*8]
        mov      r9, QWORD PTR [rdi+rbx]
        sub      r8, rax
        lea      rcx, QWORD PTR [rax+4]
        lea      rax, QWORD PTR [r10-4]
        shr      rax, 2
        inc      rax
        lea      r11, QWORD PTR [rax*4]
$LL84@main:
        vmovss   xmm0, DWORD PTR [r8+rcx-4]
        lea      rcx, QWORD PTR [rcx+16]
        vmulss   xmm1, xmm0, DWORD PTR [r9+rdx*4]
        vaddss   xmm2, xmm1, DWORD PTR [rcx-20]
        vmovss   DWORD PTR [rcx-20], xmm2
        vmovss   xmm0, DWORD PTR [r8+rcx-16]
        vmulss   xmm1, xmm0, DWORD PTR [r9+rdx*4]
        vaddss   xmm2, xmm1, DWORD PTR [rcx-16]
        vmovss   DWORD PTR [rcx-16], xmm2
        vmovss   xmm0, DWORD PTR [r8+rcx-12]
        vmulss   xmm1, xmm0, DWORD PTR [r9+rdx*4]
        vaddss   xmm2, xmm1, DWORD PTR [rcx-12]
        vmovss   DWORD PTR [rcx-12], xmm2
        vmovss   xmm0, DWORD PTR [r8+rcx-8]
        vmulss   xmm1, xmm0, DWORD PTR [r9+rdx*4]
        vaddss   xmm2, xmm1, DWORD PTR [rcx-8]
        vmovss   DWORD PTR [rcx-8], xmm2
        sub      rax, 1
        jne      SHORT $LL84@main
$LC83@main:
        cmp      r11, r10
        jge      SHORT $LN17@main
        mov      rax, QWORD PTR [rbx]
        mov      r8, QWORD PTR [r14+rdx*8]
        mov      r9, QWORD PTR [rdi+rbx]
        sub      r8, rax
        lea      rcx, QWORD PTR [rax+r11*4]
        mov      rax, r10
        sub      rax, r11
$LC22@main:
        vmovss   xmm0, DWORD PTR [rcx+r8]
        lea      rcx, QWORD PTR [rcx+4]
        vmulss   xmm1, xmm0, DWORD PTR [r9+rdx*4]
        vaddss   xmm2, xmm1, DWORD PTR [rcx-4]
        vmovss   DWORD PTR [rcx-4], xmm2
        sub      rax, 1
        jne      SHORT $LC22@main
$LN17@main:
        inc      rdx
        cmp      rdx, r10
        jl       $LL19@main
        add      rbx, 8
        sub      rsi, 1
        jne      $LL16@main
        mov      rsi, QWORD PTR [rsp+80]