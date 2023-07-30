struct Stack {
    int *v;
    size_t capacity;
    size_t size;
}

int data_init(Data *);
int data_deinit(Data *);
size_t data_size(Data *);
int data_inc_capacity(Data *);
int data_add(Data *);
