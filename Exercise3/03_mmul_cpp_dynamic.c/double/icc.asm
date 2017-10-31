xor eax, eax #25.2
  xor esi, esi #25.2
  mov r12, QWORD PTR [8+rsp] #49.11
  xor edx, edx #25.2
  mov r9, QWORD PTR [32+rsp] #49.15
  xor r13d, r13d #25.2
..B1.73: # Preds ..B1.81 ..B1.72
  mov DWORD PTR [rsp], eax #26.3[spill]
  mov r10, rsi #26.3
  mov r8d, r13d #26.3
  lea r11, QWORD PTR [rdx+rdx*2] #781.18
..B1.74: # Preds ..B1.80 ..B1.73
  mov rax, QWORD PTR [rdi+r11*8] #781.18
  mov edx, ecx #28.4
  mov QWORD PTR [rax+r10*8], rsi #27.8
  mov eax, r13d #28.4
  test r15d, r15d #28.4
  jbe ..B1.78 # Prob 9% #28.4
  mov rdx, QWORD PTR [r12+r11*8] #796.18
..B1.76: # Preds ..B1.76 ..B1.75
  mov r14, QWORD PTR [rdi+r11*8] #781.18
  lea r13d, DWORD PTR [rax+rax] #29.21
  vmovsd xmm1, QWORD PTR [rdx+r13*8] #29.20
  lea r13, QWORD PTR [r13+r13*2] #796.18
  mov r13, QWORD PTR [r9+r13*8] #796.18
  vmovsd xmm0, QWORD PTR [r13+r10*8] #29.30
  vfmadd213sd xmm1, xmm0, QWORD PTR [r14+r10*8] #29.9			// SIMD instruction
  vmovsd QWORD PTR [r14+r10*8], xmm1 #29.9
  mov r13, QWORD PTR [rdi+r11*8] #781.18
  lea r14d, DWORD PTR [1+rax+rax] #29.28
  vmovsd xmm3, QWORD PTR [rdx+r14*8] #29.20
  inc eax #28.4
  lea r14, QWORD PTR [r14+r14*2] #796.18
  mov r14, QWORD PTR [r9+r14*8] #796.18
  vmovsd xmm2, QWORD PTR [r14+r10*8] #29.30
  vfmadd213sd xmm3, xmm2, QWORD PTR [r13+r10*8] #29.9			// SIMD instruction
  vmovsd QWORD PTR [r13+r10*8], xmm3 #29.9
  cmp eax, r15d #28.4
  jb ..B1.76 # Prob 64% #28.4
  xor r13d, r13d #
  lea edx, DWORD PTR [1+rax+rax] #29.9
..B1.78: # Preds ..B1.77 ..B1.74
  dec edx #29.9
  cmp edx, ebx #28.4
  jae ..B1.80 # Prob 9% #28.4
  mov r14, QWORD PTR [r12+r11*8] #796.18
  mov rax, QWORD PTR [rdi+r11*8] #781.18
  vmovsd xmm1, QWORD PTR [r14+rdx*8] #29.20
  lea rdx, QWORD PTR [rdx+rdx*2] #796.18
  mov r14, QWORD PTR [r9+rdx*8] #796.18
  vmovsd xmm0, QWORD PTR [r14+r10*8] #29.30
  vfmadd213sd xmm1, xmm0, QWORD PTR [rax+r10*8] #29.9
  vmovsd QWORD PTR [rax+r10*8], xmm1 #29.9
..B1.80: # Preds ..B1.78 ..B1.79
  inc r8d #26.3
  mov r10d, r8d #26.3
  cmp r8d, ebx #26.3
  jb ..B1.74 # Prob 82% #26.3
  .byte 15 #
  .byte 31 #
  .byte 68 #
  .byte 0 #
  .byte 0 #
  mov eax, DWORD PTR [rsp] #[spill]
  inc eax #25.2
  mov edx, eax #25.2
  cmp eax, ebx #25.2
  jb ..B1.73 # Prob 82% #25.2
  
