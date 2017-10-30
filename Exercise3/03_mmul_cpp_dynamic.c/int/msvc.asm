operator*, COMDAT PROC
        push     rbx
        push     rsi
        push     r14
        push     r15
        sub      rsp, 40              ; 00000028H
        mov      r9, QWORD PTR [rdx+8]
        mov      r14, rdx
        sub      r9, QWORD PTR [rdx]
        mov      rax, 3074457345618258603 ; 2aaaaaaaaaaaaaabH
        imul     r9
        mov      r15, r8
        mov      rsi, rcx
        mov      rbx, rdx
        sar      rbx, 2
        mov      rax, rbx
        shr      rax, 63              ; 0000003fH
        add      rbx, rax
        mov      edx, ebx
        call     id
        test     ebx, ebx
        je       $LN102@operator
        mov      QWORD PTR [rsp+80], rbp
        mov      QWORD PTR [rsp+88], rdi
        mov      QWORD PTR [rsp+96], r12
        mov      r12d, ebx
        xor      ebx, ebx
        mov      QWORD PTR [rsp+32], r13
        mov      r13d, r12d
        npad     12
$LL4@operator:
        xor      eax, eax
        mov      rbp, r12
        npad     11
$LL7@operator:
        mov      rcx, QWORD PTR [rsi]
        xor      r10d, r10d
        xor      r11d, r11d
        mov      rdi, r12
        mov      rdx, QWORD PTR [rbx+rcx]
        mov      DWORD PTR [rax+rdx], 0
        npad     9
$LL10@operator:
        mov      rcx, QWORD PTR [rsi]
        lea      r11, QWORD PTR [r11+4]
        lea      r10, QWORD PTR [r10+24]
        mov      r9, QWORD PTR [rbx+rcx]
        mov      rcx, QWORD PTR [r14]
        mov      r8, QWORD PTR [rbx+rcx]
        mov      rcx, QWORD PTR [r15]
        mov      rdx, QWORD PTR [r10+rcx-24]
        mov      ecx, DWORD PTR [r8+r11-4]
        imul     ecx, DWORD PTR [rdx+rax]
        add      DWORD PTR [r9+rax], ecx
        sub      rdi, 1
        jne      SHORT $LL10@operator
        add      rax, 4
        sub      rbp, 1
        jne      SHORT $LL7@operator
        add      rbx, 24
        sub      r13, 1
        jne      SHORT $LL4@operator
        mov      r13, QWORD PTR [rsp+32]
        mov      r12, QWORD PTR [rsp+96]
        mov      rdi, QWORD PTR [rsp+88]
        mov      rbp, QWORD PTR [rsp+80]
$LN102@operator:
        mov      rax, rsi
        add      rsp, 40              ; 00000028H
        pop      r15
        pop      r14
        pop      rsi
        pop      rbx
        ret      0
operator* ENDP