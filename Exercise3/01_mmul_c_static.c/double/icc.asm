  vbroadcastsd ymm0, QWORD PTR [A+rcx+rdi*8] #40.20
  mov r10, rdx #39.7
  xor r9d, r9d #39.7
..B1.46: # Preds ..B1.46 ..B1.45
  vmovupd ymm1, YMMWORD PTR [B+rsi+r9*8] #40.30
  vfmadd213pd ymm1, ymm0, YMMWORD PTR [C+rcx+r9*8] #40.9
  vmovupd YMMWORD PTR [C+rcx+r9*8], ymm1 #40.9
  add r9, 4 #39.7
  cmp r9, rdx #39.7
  jb ..B1.46 # Prob 99% #39.7
..B1.48: # Preds ..B1.46 ..B1.79
  cmp r10, rax #39.7
  jae ..B1.52 # Prob 10% #39.7
  vmovsd xmm0, QWORD PTR [A+rcx+rdi*8] #40.20
..B1.50: # Preds ..B1.50 ..B1.49
  vmovsd xmm1, QWORD PTR [B+rsi+r10*8] #40.30
  vfmadd213sd xmm1, xmm0, QWORD PTR [C+rcx+r10*8] #40.9
  vmovsd QWORD PTR [C+rcx+r10*8], xmm1 #40.9