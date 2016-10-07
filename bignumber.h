#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include <inttypes.h>
#include <iostream>
#include <algorithm>
#include <string>

enum filltype
{
    FILL_TYPE_CONST,
    FILL_TYPE_RANDOM,
    FILL_TYPE_KEYBOARD
};



typedef unsigned int base;
typedef unsigned short s_base;
typedef unsigned long int d_base;


union Digit
{
    s_base halfBase[2];
    base Base;
};

class BigNumber
{
    base* m_data;

    int m_len;

    int m_capacity;

private:

    base CharToInt( char s );

    char IntToChar( base x );

    base Numb( int k , int j );

    void RewriteNumb( int k, int j, base digit );

    base mul( base a, base b, base *flag );

    base Max( base i, base j );

    base Min( base i, base j );

    BigNumber shiftsub(const BigNumber &first, const BigNumber &second, bool &sub, int j);

    void add_base(base b);

public:

    BigNumber();

    BigNumber( int len );

    BigNumber( const char* text );

    BigNumber( const BigNumber &in );

    BigNumber( const BigNumber &in, int len );

    BigNumber (int NUM_BASES, filltype FillType);

    BigNumber operator + ( const BigNumber& in );

    BigNumber operator - ( const BigNumber& in );

    BigNumber operator * ( const BigNumber& in );
    
    BigNumber operator / ( const BigNumber& in );

    BigNumber operator % ( const BigNumber& in );

    bool operator != ( const BigNumber& in );

    BigNumber &operator = ( const BigNumber& in );

    BigNumber& operator += ( BigNumber& in );

    BigNumber& operator -= ( BigNumber& in );

    BigNumber& operator *= ( BigNumber& in );

    BigNumber& operator /= ( BigNumber& in );

    base& operator [] ( int index );

    bool operator == ( const BigNumber& in );

    bool operator > (const BigNumber& in );

    bool operator < (const BigNumber& in );

    void In();

    void Out();

    ~BigNumber();

    BigNumber Div(const base &second, base *reminder = NULL);
};

#endif // BIGNUMBER_H
