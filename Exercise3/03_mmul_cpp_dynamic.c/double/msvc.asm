operator*, COMDAT PROC
        push     rbp
        push     rsi
        push     rdi
        push     r12
        sub      rsp, 40              ; 00000028H
        mov      r9, QWORD PTR [rdx+8]
        mov      rsi, rdx
        sub      r9, QWORD PTR [rdx]
        mov      rax, 3074457345618258603 ; 2aaaaaaaaaaaaaabH
        imul     r9
        mov      r12, r8
        mov      rdi, rcx
        mov      rbp, rdx
        sar      rbp, 2
        mov      rax, rbp
        shr      rax, 63              ; 0000003fH
        add      rbp, rax
        mov      edx, ebp
        call     id
        test     ebp, ebp
        je       $LN106@operator
        mov      QWORD PTR [rsp+88], rbx
        xor      r9d, r9d
        mov      QWORD PTR [rsp+96], r13
        mov      eax, ebp
        mov      QWORD PTR [rsp+104], r14
        mov      QWORD PTR [rsp+32], r15
        mov      QWORD PTR tv1044[rsp], rax
        mov      r13d, ebp
        npad     8
$LL4@operator:
        xor      r8d, r8d
        mov      r15, rax
$LL7@operator:
        mov      rax, QWORD PTR [rdi]
        xor      ebx, ebx
        mov      rcx, QWORD PTR [rax+r9]
        mov      QWORD PTR [r8+rcx], 0
        cmp      ebp, 4
        jb       $LC100@operator
        mov      r10, QWORD PTR [r12]
        lea      eax, DWORD PTR [rbp-4]
        add      r10, 48              ; 00000030H
        shr      eax, 2
        xor      r11d, r11d
        inc      eax
        mov      r14d, eax
        lea      ebx, DWORD PTR [rax*4]
        npad     3
$LL101@operator:
        mov      rax, QWORD PTR [rdi]
        lea      r11, QWORD PTR [r11+32]
        lea      r10, QWORD PTR [r10+96]
        mov      rdx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [rsi]
        mov      rcx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [r10-144]
        vmovsd   xmm0, QWORD PTR [rcx+r11-32]
        vmulsd   xmm1, xmm0, QWORD PTR [rax+r8]				//unroll 1
        vaddsd   xmm2, xmm1, QWORD PTR [rdx+r8]
        vmovsd   QWORD PTR [rdx+r8], xmm2
        mov      rax, QWORD PTR [rdi]
        mov      rdx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [rsi]
        mov      rcx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [r10-120]
        vmovsd   xmm0, QWORD PTR [rcx+r11-24]
        vmulsd   xmm1, xmm0, QWORD PTR [rax+r8]				//unroll 2
        vaddsd   xmm2, xmm1, QWORD PTR [rdx+r8]
        vmovsd   QWORD PTR [rdx+r8], xmm2
        mov      rax, QWORD PTR [rdi]
        mov      rdx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [rsi]
        mov      rcx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [r10-96]
        vmovsd   xmm0, QWORD PTR [rcx+r11-16]
        vmulsd   xmm1, xmm0, QWORD PTR [r8+rax]				//unroll 3
        vaddsd   xmm2, xmm1, QWORD PTR [rdx+r8]
        vmovsd   QWORD PTR [rdx+r8], xmm2
        mov      rax, QWORD PTR [rdi]
        mov      rdx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [rsi]
        mov      rcx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [r10-72]
        vmovsd   xmm0, QWORD PTR [rcx+r11-8]
        vmulsd   xmm1, xmm0, QWORD PTR [rax+r8]				//unroll 4
        vaddsd   xmm2, xmm1, QWORD PTR [rdx+r8]
        vmovsd   QWORD PTR [rdx+r8], xmm2
        sub      r14, 1
        jne      $LL101@operator
$LC100@operator:
        cmp      ebx, ebp
        jae      SHORT $LN5@operator
        mov      rax, QWORD PTR [r12]
        lea      rcx, QWORD PTR [rbx+rbx*2]
        lea      r10, QWORD PTR [rbx*8]
        lea      r11, QWORD PTR [rax+rcx*8]
        mov      eax, ebp
        sub      eax, ebx
        mov      ebx, eax
$LC10@operator:
        mov      rax, QWORD PTR [rdi]
        lea      r10, QWORD PTR [r10+8]
        lea      r11, QWORD PTR [r11+24]
        mov      rdx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [rsi]
        mov      rcx, QWORD PTR [rax+r9]
        mov      rax, QWORD PTR [r11-24]
        vmovsd   xmm0, QWORD PTR [rcx+r10-8]
        vmulsd   xmm1, xmm0, QWORD PTR [r8+rax]
        vaddsd   xmm2, xmm1, QWORD PTR [rdx+r8]
        vmovsd   QWORD PTR [rdx+r8], xmm2
        sub      rbx, 1
        jne      SHORT $LC10@operator
$LN5@operator:
        add      r8, 8
        sub      r15, 1
        jne      $LL7@operator
        mov      rax, QWORD PTR tv1044[rsp]
        add      r9, 24
        sub      r13, 1
        jne      $LL4@operator
        mov      r15, QWORD PTR [rsp+32]
        mov      r14, QWORD PTR [rsp+104]
        mov      r13, QWORD PTR [rsp+96]
        mov      rbx, QWORD PTR [rsp+88]
$LN106@operator:
        mov      rax, rdi
        add      rsp, 40              ; 00000028H
        pop      r12
        pop      rdi
        pop      rsi
        pop      rbp
        ret      0
operator* ENDP
