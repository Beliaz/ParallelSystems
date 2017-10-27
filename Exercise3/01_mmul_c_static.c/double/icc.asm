..B1.46: # Preds ..B1.46 ..B1.45
  vmovupd ymm1, YMMWORD PTR [B+rsi+r9*8] #40.30
  vfmadd213pd ymm1, ymm0, YMMWORD PTR [C+rcx+r9*8] #40.9
  vmovupd YMMWORD PTR [C+rcx+r9*8], ymm1 #40.9