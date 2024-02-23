# Rules

To contribbute to the codebase, our syntax rules have to be followed:

- snake_case for functions  
    ✔️  `int myFunc(void);`  
    ❌ `int my_func(void);`  

- PascalCase for custom types (typedef)
    ✔️  `int myFunc(void);`    
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
