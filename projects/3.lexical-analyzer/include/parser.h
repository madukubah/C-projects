#ifndef simplex_parser_h
#define simplex_parser_h

typedef enum {
    OP_LITERAL,
    OP_CONCAT,
    OP_PIPE,
    OP_KLEEN,
}Opcode;

typedef struct {
    Opcode opcode;
    char value;
} Operation;

typedef struct OperationList {
    int count;
    Operation operations[1024];
} OperationList;

struct OperationList parse(const char *regex);

#endif
