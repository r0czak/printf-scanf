#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "functions.h"


int my_printf(char* format,...){
    char *fmt;
    long int num;
    char *s;
    struct point_t point;
    double doub;
    int ret_value = 0;


    va_list ptr;
    va_start(ptr, format);

    for(fmt = format; *fmt != '\0'; fmt++)
    {
        while(*fmt != '%')
        {
            if(*fmt == '\0'){
                break;
            }
            putchar(*fmt);
            ret_value++;
            fmt++;
        }
        if(*fmt == '\0'){
            break;
        }

        fmt++;


        switch(*fmt){
            case 'd' :
                num = va_arg(ptr,int);

                if(num < 0)
                {
                    num = -num;
                    ret_value++;
                    putchar('-');
                }
                ret_value += itoa(num);
                break;
            case 's':
                s = va_arg(ptr,char *);
                ret_value += strlen(s);
                fwrite(s, 1, strlen(s), stdout);
                break;
            case 'p':
                point = va_arg(ptr, struct point_t);

                putc('(', stdout);
                ret_value++;
                if(point.x < 0)
                {
                    point.x = -point.x;
                    ret_value++;
                    putchar('-');
                }
                ret_value += itoa(point.x);

                putc(' ', stdout);
                ret_value++;
                if(point.y < 0)
                {
                    point.y = -point.y;
                    ret_value++;
                    putchar('-');
                }
                ret_value += itoa(point.y);
                putc(')', stdout);
                ret_value++;
                break;
            case 'f':
                doub = va_arg(ptr, double);

                if(doub < 0)
                {
                    doub = -doub;
                    ret_value++;
                    putchar('-');
                }
                ret_value += ftoa(doub);
                break;
        }
    }
    va_end(ptr);

    return ret_value;
}

int my_scanf(char *format, ...){
    char *fmt, c;
    int ret_value = 0;
    int load = 0;

    int tempint;
    double tempdoub;


    va_list ptr;
    va_start(ptr, format);

    for(fmt = format; *fmt != '\0'; fmt++)
    {
        while(*fmt != '%')
        {
            fmt++;
            if(*fmt != '%')
                fread(&c, 1, 1, stdin);
        }

        fmt++;



        switch(*fmt)
        {
            case 'd' :
                tempint = takeint(&load);
                if(load == 1){
                    *(int *)va_arg(ptr, int *) = tempint;
                    ret_value++;
                }
                tempint = 0;
                load = 0;
                break;
            case 'p':
                load = takepoint(va_arg(ptr, struct point_t*));
                if(load == 1){
                    ret_value += 2;
                }
                load = 0;
                break;
            case 'f':
                tempdoub = takedouble(&load);
                if(load == 1){
                    *(double *)va_arg(ptr, double *) = tempdoub;
                    ret_value++;
                }
                tempdoub = 0;
                load = 0;
                break;
        }
    }
    va_end(ptr);

    return ret_value;
}

int takeint(int *load_error){
    char c;
    int ret = 0, negative = 0, input_error = 1;

    do{
        fread(&c, 1, 1, stdin);
        if(isdigit(c) != 0){
            ret = ret * 10 + atoi(&c);
            input_error = 0;
        }
        else if(c == '-' && input_error == 1){
            negative = 1;
        }
        else if(input_error == 0){
            break;
        }
        else{
            *load_error = -1;
            return 0;
        }
    }while(c != ' ' && c != '\n');

    if(negative == 1){
        *load_error = 1;
        return -ret;
    }
    else{
        *load_error = 1;
        return ret;
    }
}

int takepoint(struct point_t *point){
    int negative = 0, x = 0, y = 0, input_error = 1;
    char c;
    fread(&c, 1, 1, stdin);
    if(c == '('){
        do{
            fread(&c, 1, 1, stdin);
            if(isdigit(c) != 0){
                x = x * 10 + atoi(&c);
                input_error = 0;
            }
            else if(c == '-' && input_error == 1){
                negative = 1;
            }
            else if(input_error == 0){
                break;
            }
            else{
                return -1;
            }
        }while(c != ' ');
        if(negative == 1){
            x = -x;
        }
        negative = 0;
        input_error = 1;

        do{
            fread(&c, 1, 1, stdin);
            if(isdigit(c) != 0){
                y = y * 10 + atoi(&c);
                input_error = 0;
            }
            else if(c == '-' && input_error == 1){
                negative = 1;
            }
            else if(input_error == 0){
                break;
            }
            else{
                return -1;
            }
        }while(c != ')');
    }
    if(negative == 1){
        y = -y;
    }
    fgetc(stdin);

    point->x = x;
    point->y = y;

    return 1;
}

double takedouble(int *load_error){
    char c;
    int negative = 0, input_error = 1, decimal_count = 1;
    double double_part = 0, temp, int_part = 0;

    do{
        fread(&c, 1, 1, stdin);
        if(isdigit(c) != 0){
            int_part = int_part * 10 + atoi(&c);
            input_error = 0;
        }
        else if(c == '-' && input_error == 1){
            negative = 1;
        }
        else if(input_error == 0){
            break;
        }
        else{
            *load_error = -1;
            return 0;
        }
    }while(c != '.');

    do{
        fread(&c, 1, 1, stdin);
        if(isdigit(c) != 0){
            temp = (double)(atoi(&c));
            double_part = double_part + temp/(pow(10, decimal_count));

            decimal_count++;
            input_error = 0;
        }
        else if(input_error == 0){
            break;
        }
        else    break;
    }while(c != ' ');


    if(negative == 1){
        int_part = -int_part;
        double_part = -double_part;
    }

    *load_error = 1;
    return double_part + int_part;
}


int itoa(int num){
    int ret_value = 0;
    if(num == 0){
        putc('0', stdout);
        ret_value++;
        return ret_value;
    }
    long temp = num, rem;
    int counter = 0;
    while(temp){
        temp /= 10;
        counter++;
    }

    temp = num;

    for(int i = counter; i > 0; i--){
        rem = temp % (int)pow(10, i-1);
        temp -= rem;
        temp /= pow(10, i-1);
        ret_value++;
        putc(temp + '0', stdout);
        temp = rem;
    }
    return ret_value;
}

int ftoa(double num){
    int ret_value = 0;
    if(num == 0){
        puts("0.00000");
        ret_value = 7;
        return ret_value;
    }
    int int_part, int_rem;
    int counter = 0;
    double double_part, double_rem;

    int_part = (int)num - fmod(num, 1);
    double_part = fmod(num, 1);

    if(int_part == 0){
        ret_value++;
        putc('0', stdout);
    }
    else{
        while(int_part){
            int_part /= 10;
            counter++;
        }

        int_part = num - fmod(num, 1);
        for(int i = counter; i > 0; i--){
            int_rem = int_part % (int)pow(10, i-1);
            int_part -= int_rem;
            int_part /= pow(10, i-1);
            putc(int_part + '0', stdout);
            ret_value++;
            int_part = int_rem;
        }
    }


    putc('.', stdout);
    ret_value++;

    for(int i = 0; i < 5; i++){
        double_part *= 10;
        double_rem = fmod(double_part, 1);
        double_part -= double_rem;
        putc((int)double_part + '0', stdout);
        ret_value++;
        double_part = double_rem;
    }

    return ret_value;
}