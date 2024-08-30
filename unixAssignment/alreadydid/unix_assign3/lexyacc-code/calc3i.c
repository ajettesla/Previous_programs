#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int lbl;
int var = 7;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:       
        printf("\tpushq	$%d\n", p->con.value); 
	 
        break;
    case typeId:        
        printf("\tpushq\t%c\n", p->id.i + 'a'); 
        break;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
	    ex(p->opr.op[0]);
            printf("\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2) {
                /* if else */
                printf("\tL%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                /* if */
                printf("\tL%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:     
            ex(p->opr.op[0]);
            printf("\tmovq\t$format,\t%%rdi\n\tpopq\t%%rsi\n\tpushq\t$0\n\tcall\tprintf\n\tpopq\t%%rsi\n");
	    
            break;
        case '=':       
            ex(p->opr.op[1]);
            printf("\tpopq\t%c\n", p->opr.op[0]->id.i + 'a');
            break;
        case UMINUS:    
            ex(p->opr.op[0]);
            printf("\tpopq\t%%r8\n\tneg\t%%r8\n\tpushq\t%%r8\n");
            break;
	case FACT:
  	    ex(p->opr.op[0]);
	    printf("\tpopq\t%%rdi\n\tcall\tfact\n\tpushq\t%%rbp\n");
	    break;
	case LNTWO:
	    ex(p->opr.op[0]);
	    printf("\tpopq\t%%rdi\n\tcall\ttwo\n\tpushq\t%%rdi\n");
	    break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch(p->opr.oper) {
	    case GCD:   printf("\tpopq\t%%rdi\n\tpopq\t%%rsi\n\tcall\tgcd\n\tpushq\t%%rbp\n"); break;
            case '+': 
		printf("\tpopq\t%%rbx\n");
		printf("\tpopq\t%%rax\n");
		printf("\tadd\t%%rax,\t%%rbx\n");
		printf("\tpushq\t%%rbx\n");
		break;
            case '-':  
		printf("\tpopq\t%%rbx\n");
		printf("\tpopq\t%%rax\n");
		printf("\tsub\t%%rbx,\t%%rax\n");
		printf("\tpushq\t%%rax\n");
		break; 
            case '*':   
		printf("\tpopq\t%%rbx\n");
		printf("\tpopq\t%%rax\n");
		printf("\timulq\t%%rax,\t%%rbx\n");
		printf("\tpushq\t%%rbx\n");
		break;		
            case '/':  
		printf("\txor\t%%rax,\t%%rax\n");
		printf("\txor\t%%rbx,\t%%rbx\n");
		printf("\txor\t%%rdx,\t%%rdx\n");
		printf("\tpopq\t%%rbx\n");
		printf("\tpopq\t%%rax\n");
		printf("\tidivq\t%%rbx\n");
		printf("\tpushq\t%%rax\n");
		break;
            case '<':  
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcmp\t%%rax,\t%%rbx\n");		
				break;
			    case '>':   printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcmp\t%%rax,\t%%rbx\n");		
			    break;
			    case GE:  
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcmp\t%%rax,\t%%rbx\n");					
				break;
			    case LE:    
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcmp\t%%rax,\t%%rbx\n");		
			    break;
			    case NE:    
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcmp\t%%rax,\t%%rbx\n");		
				break;
			    case EQ:    
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcmp\t%%rax,\t%%rbx\n");		
			    break;
            }
        }
    }
    return 0;
}
