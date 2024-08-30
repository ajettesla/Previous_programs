#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:       
        printf("\tpush\t%d\n", p->con.value); 
        break;
    case typeId:        
        printf("\tpush\t%c\n", p->id.i + 'a'); 
        break;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0]);
            printf("\tjz\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2) {
                /* if else */
                printf("\tjz\tL%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                /* if */
                printf("\tjz\tL%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:     
            ex(p->opr.op[0]);
            printf("\tpop rax\n");
            printf("\tcall printdigit\n");
            break;
        case '=':       
            ex(p->opr.op[1]);
            printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a');
            break;
        case UMINUS:    
            ex(p->opr.op[0]);
             printf("\tpop\t rax\n");
             printf("\tneg\t rax\n");
             printf("\tpush\t rax\n");
            break;
	case FACT:
  	    ex(p->opr.op[0]);
        printf("\tpop rax\n");
	    printf("\tcall fact\n");
	    break;
	case LNTWO:
	    ex(p->opr.op[0]);
        printf("\t pop rax\n");
	    printf("\tcall lntwo\n");
	    break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch(p->opr.oper) {
	    case GCD: 
            printf("\tpop rbx\n");
            printf("\tpop rax\n");
            printf("\tcall gcd\n");            
        break;
            case '+': 
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tadd\t rax  rbx\n");
            printf("\tpush\t rax\n");
            break;
            case '-': 
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tsub\t rax rbx\n");
            printf("\tpush\t rax\n"); 
            break; 
            case '*':   
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\timul\t rax  rbx\n");
            printf("\tpush\t rax\n"); 
            break;
            case '/':
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tdiv\t rax rbx\n");
            printf("\tpush\t rax\n"); 
            break;
            case '<':  
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tcmp\t rax rbx\n");
            printf("\tjl\n");
            break;
            case '>':
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tcmp\t rax rbx\n");
            printf("\tjg\n");
            break;
            case GE:   
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tcmp\t rax  rbx\n");
            printf("\tjge\n");
             break;
            case LE:
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tcmp\t rax  rbx\n");
            printf("\tjle\n");   
             break;
            case NE:   
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tcmp\t rax  rbx\n");
            printf("\tjne\n"); 
             break;
            case EQ:   
            printf("\tpop\t rbx\n");
            printf("\tpop\t rax\n");
            printf("\tcmp\t rax  rbx\n");
            printf("\tje\n"); 
             break;
            }
        }
    }
    return 0;
}
