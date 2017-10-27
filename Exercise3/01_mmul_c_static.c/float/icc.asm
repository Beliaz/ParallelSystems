  vbroadcastss ymm0, DWORD PTR [A+rcx+rdi*4] #40.20
  mov r10, rdx #39.7
  xor r9d, r9d #39.7
..B1.46: # Preds ..B1.46 ..B1.45
  vmovups ymm1, YMMWORD PTR [B+rsi+r9*4] #40.30
  vfmadd213ps ymm1, ymm0, YMMWORD PTR [C+rcx+r9*4] #40.9
  vmovups YMMWORD PTR [C+rcx+r9*4], ymm1 #40.9
  add r9, 8 #39.7
  cmp r9, rdx #39.7
  jb ..B1.46 # Prob 99% #39.7
..B1.48: # Preds ..B1.46 ..B1.79
  cmp r10, rax #39.7
  jae ..B1.52 # Prob 10% #39.7
  vmovss xmm0, DWORD PTR [A+rcx+rdi*4] #40.20
..B1.50: # Preds ..B1.50 ..B1.49
  vmovss xmm1, DWORD PTR [B+rsi+r10*4] #40.30
  vfmadd213ss xmm1, xmm0, DWORD PTR [C+rcx+r10*4] #40.9
  vmovss DWORD PTR [C+rcx+r10*4], xmm1 #40.9
  inc r10 #39.7
  cmp r10, rax #39.7
  jb ..B1.50 # Prob 99% #39.7