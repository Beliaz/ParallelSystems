$LL22@main:
        vpmulld ymm1, ymm3, YMMWORD PTR [rax+r8]
        vpaddd   ymm2, ymm1, YMMWORD PTR [rax]
        vpmulld ymm1, ymm3, YMMWORD PTR [rax+r8+32]
        vmovdqu YMMWORD PTR [rax], ymm2
        vpaddd   ymm2, ymm1, YMMWORD PTR [rax+32]
        add      r9, 16
        vmovdqu YMMWORD PTR [rax+32], ymm2
        lea      rax, QWORD PTR [rax+64]
        cmp      r9, rcx
        jl       SHORT $LL22@main
$LN58@main:
        cmp      r9, rdi
        jge      SHORT $LN17@main
        lea      rax, QWORD PTR [rsi+rbx*4]
        mov      rdx, rdi
        mov      r10d, DWORD PTR [rax+r12]
        lea      rax, QWORD PTR [r9+r14]
        lea      rax, QWORD PTR [rax*4]
        add      rax, r13
        sub      rdx, r9
        npad     12
$LL57@main:
        mov      ecx, DWORD PTR [rax+r8]
        lea      rax, QWORD PTR [rax+4]
        imul     ecx, r10d
        add      DWORD PTR [rax-4], ecx
        sub      rdx, 1
        jne      SHORT $LL57@main