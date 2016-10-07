#define NDEBUG
#include "bignumber.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
/*
    srand(time(0));

   const int number_of_tests = 10;
   const int size_of_BigNumber = 8000000;
   filltype FillType = FILL_TYPE_RANDOM;

    for(int i = 0; i < number_of_tests; i++)
    {
        BigNumber a(size_of_BigNumber,FillType);
        BigNumber b(size_of_BigNumber,FillType);
        BigNumber c(size_of_BigNumber,FillType);

        if (a + b + c == c + a + b) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }



    for(int i = 0; i < number_of_tests; i++)
    {
        BigNumber a(size_of_BigNumber,FillType);
        BigNumber b(size_of_BigNumber,FillType);

        if (a+b-a-b == BigNumber(1)) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }


    for(int i = 0; i < number_of_tests; i++)
    {
        BigNumber a(size_of_BigNumber,FillType);
        BigNumber b(size_of_BigNumber,FillType);
        BigNumber c(size_of_BigNumber,FillType);
        if(a * b * c != a * (b * c) ) {printf("ERROR"); break;}
        if(a * b * c != c * b * a ) {printf("ERROR"); break;}
        if(a*(b*c-c) != a*b*c - a*c ) {printf("ERROR"); break;}
        if(a*(b*c+c) != a*b*c + a*c ) {printf("ERROR"); break;}
        printf("VSJO HOROSHO, %d \n", i);
    }

    for(int i = 0; i < number_of_tests; i++)
    {
        BigNumber a(2*size_of_BigNumber,FillType);
        BigNumber b(size_of_BigNumber,FillType);


        if (a*b/b == a) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }



    for(int i = 0; i < number_of_tests; i++)
    {
        BigNumber a(2*size_of_BigNumber,FillType);
        BigNumber b(size_of_BigNumber,FillType);


        if (a/b*b + a%b == a) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }

}
*/

    string test_root_dir = "/home/d1m/tests";

    string test_name = "bigint_div_tests";



    char test_num_str [4];

    clock_t t = clock();

    for(int i = 1; i <= 50; i++)
    {
        sprintf(test_num_str,"%03d",i);

        string datname =  test_root_dir + "/" + test_name + "/" + test_num_str + ".dat";

        ifstream in_dat_file(datname.c_str());

        if (!in_dat_file)
            cout << "cant open " << datname << endl;

        string num_1_str, num_2_str;

        in_dat_file >> num_1_str;

        in_dat_file >> num_2_str;

        BigNumber a (num_1_str.c_str());

        BigNumber b (num_2_str.c_str());

        string ansname =  test_root_dir + "/" + test_name + "/" + test_num_str + ".ans";

        ifstream in_ans_file(ansname.c_str());

        if (!in_ans_file)
            cout << "cant open " << datname << endl;

        string ans_str;

        in_ans_file >> ans_str;

        BigNumber c (ans_str.c_str());

        if(a / b == c)
            printf("TEST %d PASSED\n", i);
        else
        {
            printf("TEST %d FAILED\n", i);

            a.Out();
            b.Out();
            (a/b).Out();
            c.Out();
            break;
        }

    }
    t = clock() - t;
    printf("\n%f sec.\n", (double)t/CLOCKS_PER_SEC);

}

