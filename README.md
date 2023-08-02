# psychic-disco

## Usage 
Execute **_build.sh_** by running:  
`./build.sh`

## Instruction set description
- `ADD A B C`: data[A] = data[B] + data[C]
- `ADDI A B C`: data[A] = data[B] + C
- `SUB A B C`: data[A] = data[B] - data[C]
- `SUBI A B C`: data[A] = data[B] - C
- `MUL A B C`: data[A] = data[B] * data[C]
- `MULI A B C`: data[A] = data[B] * C
- `DIV A B C`: data[A] = data[B] / data[C]
- `DIVI A B C`: data[A] = data[B] / C
- `MOV A B`: data[A] = data[B]
- `MOVI A B`: data[A] = B
- `B A`: PC = A
- `BEQ A B C`: if (data[B] == data[C]) then PC = A
- `BEQI A B C`: if (data[B] == C) then PC = A
- `BNE A B C`: if (data[B] != data[C]) then PC = A
- `BNEI A B C`: if (data[B] != C) then PC = A
- `BLEI A B C`: if (data[B] <= C) then PC = A
- `BGEI A B C`: if (data[B] >= C) then PC = A
- `RET A`: data[0] = data[A]
- `RETI A`: data[0] = A
- `HALT`: End program
