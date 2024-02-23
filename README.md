# psychic-disco

### Usage 
Execute **_build.sh_** by running:  
`./build.sh`

### Instruction set description
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
- `BGE A B C`: if (data[B] >= data[C]) then PC = A
- `BLEI A B C`: if (data[B] <= C) then PC = A
- `BGEI A B C`: if (data[B] >= C) then PC = A
- `RET A`: data[0] = data[A]
- `RETI A`: data[0] = A
- `HALT`: End program

## Contributing

To contribbute to the codebase, our syntax rules have to be followed:
- snake_case for variables  
    ✔️ `int is_true = 1;`  
    ❌ `int isTrue = 1;`  

- camelCase for functions  
    ✔️ `int myFunc(void);`  
    ❌ `int my_func(void);`  

- if a function does not accept parameters, explicit it  
    ✔️ `int myFunc(void);`  
    ❌ `int myFunc();`  

- always add function declaration and at the top of file. Also respect the following syntax (just for declaration):  
    ✔️ 
    ```
        int 
        handlePrint(void);
    ```
    ❌ 
    ```
        int handlePrint(void);
    ```
    the reason is it facilitate regex to search function declarations and readability because the names are always at the start of the line
