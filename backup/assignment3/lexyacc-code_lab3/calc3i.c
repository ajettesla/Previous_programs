#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:       
        printf("\tpush\t$%d\n", p->con.value); 
        break;
    case typeId:        
        printf("\tpush\t%c\n", p->id.i + 'a'); 
        break;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0]);
            printf("L%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2) {
                /* if else */
                printf("L%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                /* if */
                printf("\n");
                printf("L%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:     
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\tcall\tprintdigit\n");
            break;
        case '=':       
            ex(p->opr.op[1]);
            printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a');
            break;
        case UMINUS:    
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rcx\n");
            printf("\tneg\t%%rcx\n");
            printf("\tpushq\t%%rcx\n");
            break;
	case FACT:
  	    ex(p->opr.op[0]);
        printf("\tpopq\t%%rax\n");
	    printf("\tcall\tfact\n");  
        printf("\tpushq\t%%rax\n"); 
	    break;
	case LNTWO:
	    ex(p->opr.op[0]);
        printf("\tpopq\t%%rax\n");
	    printf("\tcall\tlntwo\n");
        printf("\tpushq\t%%rax\n"); 
	    break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch(p->opr.oper) {
	    case GCD:   
            printf("\tpopq\t%%rax\n");
            printf("\tpopq\t%%rbx\n");
            printf("\tcall\tgcd\n");
            printf("\tpushq\t%%rax\n");
            break;
            case '+': 
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tadd\t%%rbx,%%rax\n");
             printf("\tpushq\t%%rax\n");
             break;
            case '-':   
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tsub\t%%rbx,%%rax\n");
             printf("\tpushq\t%%rax\n");
            break; 
            case '*':
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tmovq\t$0,%%rdx\n");
             printf("\timul\t%%rbx,%%rax\n");
             printf("\tpushq\t%%rax\n");
            break;
            case '/': 
             printf("\tmovq\t$0,%%rax\n");
             printf("\tmovq\t$0,%%rbx\n");
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tmovq\t$0,%%rdx\n");
             printf("\tidiv\t%%rbx\n");
             printf("\tpushq\t%%rax\n");
             break;
            case '<': 
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tcmp\t%%rax,%%rbx\n");
             printf("\tjle\t");
            break;
            case '>': 
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tcmp\t%%rax,%%rbx\n");
             printf("\tjge\t");  
            break;
            case GE:  
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tcmp\t%%rax,%%rbx\n");
             printf("\tjg\t");
             break;
            case LE:   
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tcmp\t%%rax,%%rbx\n");
             printf("\tjl\t"); break;
            case NE:    
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tcmp\t%%rax,%%rbx\n");
             printf("\tje\t");
             break;
            case EQ:   
             printf("\tpopq\t%%rbx\n");
             printf("\tpopq\t%%rax\n");
             printf("\tcmp\t%%rax,%%rbx\n");
             printf("\tjne\t"); 
             break;
            }
        }
    }
    return 0;
}
