#ifndef __functions_h
#define __functions_h


struct point_t{

    int x, y;
};
    

int my_printf(char* format,...);
int my_scanf(char *format, ...);

int itoa(int num);
int ftoa(double doub);
int takeint(int *load_error);
int takepoint(struct point_t *point);
double takedouble(int *load_error);

#endif