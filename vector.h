typedef struct Instruction Instruction;
typedef struct Vector Vector;
typedef struct Data Data;

struct Instruction {
    int instruction;
    int dest;
    int arg1;
    int arg2;
};

struct Data {
    int value;
    int is_char;
};

struct Vector {
    Instruction *instructions;
    size_t i_capacity; // maximum size before re-init
    size_t i_size; // current instructions size

    Data *data;
    size_t d_size;
    size_t d_capacity;
};

int vector_init(Vector *);
int vector_deinit(Vector *);

// Instructions
size_t instructions_size(Vector *);
int instructions_resize(Vector *);
int instructions_add(Vector *, Instruction *);
Instruction *instructions_get(Vector *, size_t);

// Data
size_t data_size(Vector *);
int data_resize(Vector *);
int add_add(Vector *, Data *);
int data_set(Vector *, size_t, Data *);
Data *data_get(Vector *, size_t);
