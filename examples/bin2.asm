SECTION TEXT
S_INPUT N, 3
S_OUTPUT N, 3
LOAD N
COPY AUX, N
FAT: SUB ONE
JMPZ FIM
STORE AUX
MULT N
STORE N
LOAD AUX
JMP FAT
FIM: OUTPUT N
STOP
SECTION DATA
ONE: CONST 1
AUX: SPACE 10
N: SPACE 2
