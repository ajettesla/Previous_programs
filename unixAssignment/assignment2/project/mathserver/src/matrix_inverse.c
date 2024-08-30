/***************************************************************************
 * Sequential version of Matrix Inverse
 * An adapted version of the code by H�kan Grahn
 ***************************************************************************/
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../include/matrix_inverse.h"

void startcalulation(char *buffer,int argc,char** argv){


    FILE *outputF = fopen(buffer, "w");
    if(outputF == NULL){
        printf("error with file open\n");
        exit(0);
    }
  
    fprintf(outputF,"Matrix Inverse\n");
   
    fclose(outputF);

 

    int i, timestart, timeend, iter;
    Init_Default();		/* Init default values	*/
    Read_Options(argc, argv, buffer);	/* Read arguments	*/
    Init_Matrix(buffer);		/* Init the matrix	*/
    find_inverse();
 
    if (PRINT == 1)
    {
        //Print_Matrix(A, "End: Input");
        Print_Matrix(I, "Inversed",buffer);
    }

   

}


void find_inverse()
{
    int row, col, p; // 'p' stands for pivot (numbered from 0 to N-1)
    double pivalue; // pivot value

    /* Bringing the matrix A to the identity form */
    for (p = 0; p < N; p++) { /* Outer loop */
        pivalue = A[p][p];
        for (col = 0; col < N; col++)
        {
            A[p][col] = A[p][col] / pivalue; /* Division step on A */
            I[p][col] = I[p][col] / pivalue; /* Division step on I */
        }
        assert(A[p][p] == 1.0);

        double multiplier;
        for (row = 0; row < N; row++) {
            multiplier = A[row][p];
            if (row != p) // Perform elimination on all except the current pivot row 
            {
                for (col = 0; col < N; col++)
                {
                    A[row][col] = A[row][col] - A[p][col] * multiplier; /* Elimination step on A */
                    I[row][col] = I[row][col] - I[p][col] * multiplier; /* Elimination step on I */
                }      
                assert(A[row][p] == 0.0);
            }
        }
    }
}

void
Init_Matrix(char *buffer)
{
    FILE *f = fopen(buffer, "a");
    int row, col;

    // Set the diagonal elements of the inverse matrix to 1.0
    // So that you get an identity matrix to begin with
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (row == col)
                I[row][col] = 1.0;
        }
    }

    fprintf(f,"\nsize      = %dx%d ", N, N);
    fprintf(f,"\nmaxnum    = %d \n", maxnum);
    fprintf(f,"Init	  = %s \n", Init);
    fprintf(f,"Initializing matrix...");

    if (strcmp(Init, "rand") == 0) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = (double)(rand() % maxnum) + 5.0;
                else
                    A[row][col] = (double)(rand() % maxnum) + 1.0;
            }
        }
    }
    if (strcmp(Init, "fast") == 0) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = 5.0;
                else
                    A[row][col] = 2.0;
            }
        }
    }

    fprintf(f,"done \n\n");
    if (PRINT == 1)
    {
        //Print_Matrix(A, "Begin: Input");
        //Print_Matrix(I, "Begin: Inverse");
    }
    fclose(f);
}

void
Print_Matrix(matrix M, char name[],char *buffer)
{
    FILE *f = fopen(buffer, "a");
    int row, col;

    fprintf(f,"%s Matrix:\n", name);
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++)
            fprintf(f," %5.2f", M[row][col]);
        fprintf(f,"\n");
    }
    fprintf(f,"\n\n");
    fclose(f);
}

void
Init_Default()
{
    N = 5;
    Init = "fast";
    maxnum = 15.0;
    PRINT = 1;
}

int
Read_Options(int argc, char** argv, char *buffer)
{
    FILE *f = fopen(buffer, "a");
    char* prog;
    prog = *argv;
    while (++argv, --argc > 0)
        if (**argv == '-')
            switch (*++ * argv) {
            case 'n':
                --argc;
                N = atoi(*++argv);
                break;
            case 'h':
                fprintf(f,"\nHELP: try matinv -u \n\n");
                fclose(f);
                exit(0);
                break;
            case 'u':
                fprintf(f,"\nUsage: matinv [-n problemsize]\n");
                fprintf(f,"           [-D] show default values \n");
                fprintf(f,"           [-h] help \n");
                fprintf(f,"           [-I init_type] fast/rand \n");
                fprintf(f,"           [-m maxnum] max random no \n");
                fprintf(f,"           [-P print_switch] 0/1 \n");
                fclose(f);
             
                break;
            case 'D':
                fprintf(f,"\nDefault:  n         = %d ", N);
                fprintf(f,"\n          Init      = rand");
                fprintf(f,"\n          maxnum    = 5 ");
                fprintf(f,"\n          P         = 0 \n\n");
                fclose(f);
                exit(0);
                break;
            case 'I':
                --argc;
                Init = *++argv;
             
                break;
            case 'm':
                --argc;
                maxnum = atoi(*++argv);
             
                break;
            case 'P':
                --argc;
                PRINT = atoi(*++argv);
             
                break;
            default:
                fprintf(f,"%s: ignored option: -%s\n", prog, *argv);
                fprintf(f,"HELP: try %s -u \n\n", prog);
              
                break;
            }
    fclose(f);

 return 0;
    
}
