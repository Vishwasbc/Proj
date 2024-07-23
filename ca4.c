#include <stdio.h>
#include <omp.h>
#define NUM_UNITS 2
#define NUM_INSTRUCTIONS 6

typedef struct {
    char type;      
    int src1;      
    int src2;       
    int dest;       
} Instruction;

typedef struct {
    char type;     
    int op1;       
    int op2;        
    int result;    
    int status;     
} FunctionalUnit;

Instruction instructions[NUM_INSTRUCTIONS] = {
    {'A', 2, 3, 1}, 
    {'M', 4, 5, 2}, 
    {'A', 1, 2, 3}, 
    {'M', 3, 3, 4}, 
    {'A', 5, 4, 5}, 
    {'M', 2, 2, 6}  
};

FunctionalUnit units[NUM_UNITS];

void execute_instruction(int unit_id, Instruction inst) {
    for (int i = 0; i < 100000000; ++i); 
    if (inst.type == 'A') {
        units[unit_id].result = inst.src1 + inst.src2;
    } else if (inst.type == 'M') {
        units[unit_id].result = inst.src1 * inst.src2;
    }
    units[unit_id].status = 0;
}

int main() {
    for (int i = 0; i < NUM_UNITS; ++i) {
        units[i].status = 0; 
    }

    #pragma omp parallel for num_threads(NUM_INSTRUCTIONS)
    for (int i = 0; i < NUM_INSTRUCTIONS; ++i) {
        int unit_id = -1;
        #pragma omp critical
        {
            for (int j = 0; j < NUM_UNITS; ++j) {
                if (units[j].status == 0) {
                    unit_id = j;
                    units[j].status = 1; 
                    break;
                }
            }
        }

        if (unit_id != -1) {
            execute_instruction(unit_id, instructions[i]);
            printf("Executed instruction %d: Result = %d\n", i + 1, units[unit_id].result);
        } else {
            printf("No free functional unit available for instruction %d\n", i + 1);
        }
    }

    return 0;
}
