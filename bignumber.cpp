#include "bignumber.h"
#include <math.h>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"

BigNumber::BigNumber() : m_data(NULL), m_len(0), m_capacity(0)
{}

BigNumber::BigNumber( int len ) : m_len(len), m_capacity(len)
{
    m_data = new base[ m_capacity ];
    memset(m_data, 0, m_len * sizeof(base));
}

BigNumber::BigNumber(const char* text)
{

    int text_len = strlen(text);

    m_len = text_len * log2(10)/(sizeof(base)*8) + 1;

    m_data = new base [m_len];

    m_capacity = m_len;

    memset(m_data,0,sizeof(base)*m_len);

    BigNumber ten (1);

    BigNumber milliard (1);

    milliard.m_data[0] = 1000*1000*1000;

    ten.m_data[0] = 10;

    base current_base = 0;

    for(int i = 0; i < text_len / 9 ; i++)
    {
        *this = *this * milliard;

        current_base = 0;

        for(int j = 0; j < 9; j++)
        {
            current_base *= 10;

            current_base += text[i*9+j] - '0';
        }

        add_base(current_base);
    }

    int i = text_len%9;

    BigNumber ne_milliard (1);

    ne_milliard.m_data[0] = ceil(pow(10,i));


    *this = *this * ne_milliard;

    current_base = 0;

    for(int j = 0; j < i; j++)
    {
        current_base *= 10;

        current_base += text[strlen(text)/9*9+j] - '0';
    }

    add_base(current_base);

}

void BigNumber::add_base(base b)
{
    BigNumber _base(1);
    _base.m_data[0] = b;
    *this = *this + _base;
}

BigNumber::BigNumber(const BigNumber &in) : m_len(in.m_len), m_capacity(in.m_len)
{
    m_data = new base[m_len];
    memcpy(m_data, in.m_data, m_len*sizeof(base));
}

BigNumber::BigNumber(const BigNumber &in, int len) : m_len(len), m_capacity(len)
{
    m_data = new base[len];
    memcpy(m_data, in.m_data, in.m_len*sizeof(base));
    memset(m_data + in.m_len, 0, (len - in.m_len)*sizeof(base));
}

BigNumber::BigNumber(int NUM_BASES, filltype FillType) : m_len(NUM_BASES), m_capacity(NUM_BASES)
{
    m_data = new base[m_capacity];
    switch (FillType)
    {
    case FILL_TYPE_RANDOM:
        for( int i = 0; i < m_len; ++i )
        {
            for( unsigned int k = 0; k < 2*sizeof(base); k++ )
            {
                base r = rand()%15;
                base mask = r << (4*k);
                m_data[i] += mask;
            }
        }
        break;

    default:
        assert(!"Invalid fill type");
        break;
    }
}

base BigNumber::CharToInt(char s)
{
    if( s <= 57 && s >= 48 )
    {
        return s - 48;
    }

    if( s <= 70 && s >= 65 )
    {
        return s - 55;
    }

    assert(!"Failed symbol!!!");
}

char BigNumber::IntToChar(base x)
{
    if( x <= 9 )
    {
        return x + 48;
    }

    if( x <= 15 && x >= 10 )
    {
        return x + 55;
    }

    assert(!"Failed int!!!");
}

base BigNumber::Numb( int k , int j )
{
    base mask = m_data[j] & ((8*sizeof(s_base)-1) << k);
    mask = mask >> k;
    return mask;
}

void BigNumber::RewriteNumb( int k, int j, base digit )
{
    m_data[j] = m_data[j] | (digit << k);
}

BigNumber BigNumber::operator +( const BigNumber &in )
{
    int len = std::max( m_len , in.m_len ) + 1;

    int min = std::min( m_len, in.m_len );

    BigNumber Result( len );

    char overflow = 0;

    for( int j = 0; j < min; ++j )
    {
        Result.m_data[j] = m_data[j] + in.m_data[j] + overflow;

        if( Result.m_data[j] < m_data[j] )
            overflow = 1;
        else
            overflow = 0;
    }

    int j = min;
    if( m_len == min )
    {
        for( ; j < in.m_len; ++j )
        {
            Result.m_data[j] = in.m_data[j] + overflow;

            if( Result.m_data[j] < in.m_data[j] )
                overflow = 1;
            else
                overflow = 0;
        }

        if( overflow == 1 )
        {
            Result.m_data[j] += 1;
        }

    }
    else
    {
        for( ; j < m_len; ++j )
        {
            Result.m_data[j] = m_data[j] + overflow;

            if( Result.m_data[j] < m_data[j] )
                overflow = 1;
            else
                overflow = 0;
        }


        if( overflow == 1 )
        {
            Result.m_data[j] += 1;
        }
    }
    while( Result.m_data[Result.m_len - 1] == 0 && Result.m_len > 1 )
    {
        Result.m_len--;
    }

    return Result;
}

BigNumber BigNumber::operator -(const BigNumber &in)
{    
    int len = m_len;

    BigNumber Result(len);

    char loan = 0;

    int j = 0;
    for( ; j < in.m_len; ++j )
    {
        Result.m_data[j] = m_data[j] - in.m_data[j] - loan;

        if( Result.m_data[j] > m_data[j] )
            loan = 1;
        else
            loan = 0;
    }

    for( ; loan != 0 && j < m_len; ++j )
    {
        Result.m_data[j] = m_data[j] - loan;

        if( Result.m_data[j] > m_data[j] )
            loan = 1;
        else
            loan = 0;
    }

    for( ; j < m_len; ++j )
        Result.m_data[j] = m_data[j];

    while( Result.m_data[Result.m_len - 1] == 0 && Result.m_len > 1 )
    {
        Result.m_len--;
    }

    return Result;

}

BigNumber BigNumber::operator *(const BigNumber &in)
{
    BigNumber Result( in.m_len + m_len );

    base flag = 0, res;

    int k = 0;

    for( int j = 0; j < in.m_len; ++j )
    {
        k = j;

        for( int i = 0; i < m_len; ++i, ++k )
        {
            res = mul(m_data[i], in.m_data[j], &flag);
            Result.m_data[k] += res;

            if( Result.m_data[k] - res > Result.m_data[k] )
            {
                flag += 1;
            }
        }
        while( flag != 0 )
        {
            res = mul(0, 0, &flag);
            Result.m_data[k] += res;

            if( Result.m_data[k] - res > Result.m_data[k] )
            {
                flag += 1;
            }
            k++;
        }
    }
    Result.m_len = k;

    while( Result.m_data[Result.m_len - 1] == 0 && Result.m_len > 1 )
    {
        Result.m_len--;
    }

    return Result;
}

base BigNumber::mul(base a, base b, base *flag)
{

    base Result;
    Digit first, second, mulRes;

    first.Base = a;
    second.Base = b;

    mulRes.Base = first.halfBase[0] * second.halfBase[0] + *flag;

    if( mulRes.Base < (mulRes.Base - *flag) )
    {
        *flag = 1;
    }

    else
    {
        *flag = 0;
    }

    Result = mulRes.Base;



    mulRes.Base = first.halfBase[1] * second.halfBase[0] + first.halfBase[0] * second.halfBase[1];

    if( mulRes.Base < first.halfBase[0] * second.halfBase[1] )
    {
        *flag += 1 << 8*sizeof(s_base);
    }

    *flag += mulRes.halfBase[1] + first.halfBase[1] * second.halfBase[1];

    if( Result + (mulRes.halfBase[0] << 8*sizeof(s_base)) < Result)
    {
        *flag += 1;
    }

    if( Result == 0 && mulRes.Base == 0 )
    {
        Result += *flag;
        return Result;
    }

    Result += mulRes.halfBase[0] << 8*sizeof(s_base);

    return Result;

}

BigNumber BigNumber::shiftsub(const BigNumber &first, const BigNumber &second, bool &sub, int k)
{
    BigNumber _second(second, second.m_len + k);
    for( int i = _second.m_len - k - 1; i >= 0; i-- )
    {
        _second.m_data[i+k] = _second.m_data[i];
    }
    for( int i = 0; i < k; i++ )
    {
        _second.m_data[i] = 0;
    }

    int len = std::max(first.m_len, _second.m_len);
    int min = std::min(first.m_len, _second.m_len);

    BigNumber Result(len);

    char loan = 0;

    int j = 0;
    for( ; j < min; ++j )
    {
        Result.m_data[j] = first.m_data[j] - _second.m_data[j] - loan;

        if( Result.m_data[j] > first.m_data[j] )
            loan = 1;
        else
            loan = 0;
    }
    if(min == len)
    {
        if(loan != 0)
            sub = true;
        else
            sub = false;
        return Result;
    }

    if(min == first.m_len)
    {
        Result.m_data[j] = - _second.m_data[j] - loan;
        j++;

        for(; j < len; ++j)
            Result.m_data[j] = - _second.m_data[j];
        sub = true;
        return Result;
    }

    // min = second.m_len


    for( ; loan != 0 && j < first.m_len; ++j )
    {
        Result.m_data[j] = first.m_data[j] - loan;

        if( Result.m_data[j] > _second.m_data[j] )
            loan = 1;
        else
            loan = 0;
    }

    for( ; j < first.m_len; ++j )
        Result.m_data[j] = first.m_data[j];

    while( Result.m_data[Result.m_len - 1] == 0 && Result.m_len > 1 )
    {
        Result.m_len--;
    }

    sub = false;
    return Result;
}



BigNumber BigNumber::operator /(const BigNumber &in)
{
    if( m_len < in.m_len )
    {
        BigNumber result(1);
        return result;
    }

    BigNumber result( m_len - in.m_len + 1 );

    BigNumber d(1);
    d_base b = 1;
    b = b << 4*sizeof(d_base);
    if(in.m_data[in.m_len - 1] + 1 == 0)
        d.m_data[0] = 1;
    else
        d.m_data[0] = b / (in.m_data[in.m_len - 1] + 1);

    int m = m_len - in.m_len;
    int n = in.m_len;

    BigNumber u(*this,m_len + 1);
    BigNumber v(in);
    if(v.m_len == 1)
    {
        result = u.Div(v.m_data[0]);
        return result;
    }

    v = v * d;
    u = u * d;


    for( int j = m; j >= 0; j-- )
    {
        d_base _u = u.m_data[j+n];
        _u <<= 4*sizeof(d_base);
        _u |= u.m_data[j+n-1];

        d_base _r = _u % v.m_data[n-1];
        d_base _q = _u / v.m_data[n-1];

        assert(n-2 >= 0);

        d_base x = _q * v.m_data[n-2];
        d_base y = _r;
        y = y << 4*sizeof(d_base);
        y = y + u.m_data[j+n-2];
        if( (_q == b) || (x > y) )
        {
            _q--;
            _r = _r + v.m_data[n-1];

            if(_r < b)
            {
                d_base x = _q * v.m_data[n-2];
                d_base y = _r;
                y = y << 4*sizeof(d_base);
                y = y + u.m_data[j+n-2];
                if( (_q == b) || (x > y) )
                {
                    _q--;
                    _r = _r + v.m_data[n-1];
                }
            }
        }


        BigNumber q_(1);
        assert(_q >> 8*sizeof(base) == 0);
        q_.m_data[0] = _q;

        bool borrow;
        q_ = q_ * v;
        u = shiftsub(u,q_,borrow,j);

        result.m_data[j] = _q;

        if(borrow)
        {
            result.m_data[j]--;
            u = u + v;
            u.m_len--;
        }

    }
    while( result.m_data[result.m_len - 1] == 0 && result.m_len > 1 )
    {
        result.m_len--;
    }
    return result;

}

BigNumber BigNumber::operator %(const BigNumber &in)
{
    if( m_len - in.m_len < 0 )
        return *this;

    BigNumber d(1);
    d_base b = 1;
    b = b << 4*sizeof(d_base);
    if(in.m_data[in.m_len - 1] + 1 == 0)
        d.m_data[0] = 1;
    else
        d.m_data[0] = b / (in.m_data[in.m_len - 1] + 1);

    int m = m_len - in.m_len;
    int n = in.m_len;

    BigNumber u(*this,m_len + 1);
    BigNumber v(in);

    if(v.m_len == 1)
    {
        base reminder = 0;
        u.Div(v.m_data[0],&reminder);
        BigNumber Reminder(1);
        Reminder.m_data[0] = reminder;
        return Reminder;
    }

    v = v * d;
    u = u * d;



    for( int j = m; j >= 0; j-- )
    {
        d_base _u = u.m_data[j+n];
        _u <<= 4*sizeof(d_base);
        _u |= u.m_data[j+n-1];

        d_base _r = _u % v.m_data[n-1];
        d_base _q = _u / v.m_data[n-1];

        assert(n-2 >= 0);


        d_base x = _q * v.m_data[n-2];
        d_base y = _r;
        y = y << 4*sizeof(d_base);
        y = y + u.m_data[j+n-2];
        if( (_q == b) || (x > y) )
        {
            _q--;
            _r += v.m_data[n-1];

            if(_r < b)
            {
                d_base x = _q * v.m_data[n-2];
                d_base y = _r;
                y = y << 4*sizeof(d_base);
                y = y + u.m_data[j+n-2];
                if( (_q == b) || (x > y) )
                {
                    _q--;
                    _r = _r + v.m_data[n-1];
                }
            }
        }


        BigNumber q_(1);
        q_.m_data[0] = _q;

        bool borrow;
        q_ = q_ * v;
        u = shiftsub(u,q_,borrow,j);



        if(borrow)
        {
            u = u + v;
            u.m_len--;
        }

    }
    u = u.Div(d.m_data[0]);
    while( u.m_data[u.m_len - 1] == 0 && u.m_len > 1 )
    {
        u.m_len--;
    }
    return u;
}

bool BigNumber::operator !=(const BigNumber &in)
{
    if( m_len != in.m_len ) return 1;
    int i = 0;
    while(m_data[i] == in.m_data[i] && i < m_len)
        i++;
    if(i == m_len)
        return 0;
    else
        return 1;
}


BigNumber &BigNumber::operator =(const BigNumber& in)
{
    m_len = in.m_len;

    if( m_capacity < in.m_len )
    {
        delete [] m_data;

        m_data = new base[ m_len ];

        m_capacity = m_len;
    }

    memcpy( m_data, in.m_data, m_len * sizeof(base) );

    return *this;
}

bool BigNumber::operator ==(const BigNumber &in)
{
    if( m_len != in.m_len ) return 0;
    int i = 0;
    while(m_data[i] == in.m_data[i] && i < m_len)
        i++;
    if(i == m_len)
        return 1;
    else
        return 0;
}

bool BigNumber::operator >(const BigNumber &in)
{
    if(m_len > in.m_len) return 1;
    if(m_len < in.m_len) return 0;
    int i;
    for( i = m_len - 1; i >= 0 && m_data[i] == in.m_data[i]; --i );
    if( i < 0 ) return 0;
    if( m_data[i] > in.m_data[i] ) return 1;
    else return 0;
}


bool BigNumber::operator <(const BigNumber &in)
{
    if(m_len < in.m_len) return 1;
    if(m_len > in.m_len) return 0;
    int i;
    for( i = m_len - 1; i >= 0 && m_data[i] == in.m_data[i]; --i );
    if( i < 0 ) return 0;
    if( m_data[i] < in.m_data[i] ) return 1;
    else return 0;
}


BigNumber &BigNumber::operator +=(BigNumber &in)
{
    *this = in + *this;
    return *this;
}

base &BigNumber::operator [](int index)
{
    if(index > m_len - 1 || index < 0)
        assert(!"FAIL IN MEMORY ACCESS IN []");
    return m_data[index];
}

void BigNumber::In()
{
    std::string Str;
    puts("Введите число:");
    std::getline(std::cin, Str);   //std::Str (cin)
    m_len = Str.length();
    m_data = new base[ m_len ];

    memset(m_data, 0, m_len * sizeof(base));

    if(Str[0] == '0' && Str[1] == 'x')
    {
        int i = 0;
        for( ; i < Str.length() - 2; ++i )
        {
            Str[i] = Str[i+2];          //
        }
        Str.resize(Str.length() - 2);
    }


    int i = Str.length();

    if( i % (2*sizeof(base)) == 0 )
    {
        m_len = i / (2*sizeof(base));
    }
    else
    {
        m_len = i / (2*sizeof(base)) + 1;
    }
    i--;

    for( int j = 0, k = 0; i >= 0 ; --i )
    {
        base digit = CharToInt( Str[i] );
        this->RewriteNumb( k, j, digit );
        if( k == 8*sizeof(base) - 4 )
        {
            k=0; ++j;
        }
        else
            k += 4;
    }
}

void BigNumber::Out()
{
    int j = 8;
    base mask = 15 << (8*sizeof(base) - 4);

    if(m_len == 1 && m_data[0] == 0)
    {
        printf("\n0x0\n");
        return;

    }
    while ( !(mask & m_data[ m_len - 1 ]) )
    {
        mask = mask >> sizeof(base);
        j--;
    }

    int quantity = 8 * ( m_len - 1 ) + j;

    char* Str = new char[ quantity ];
    int i = 0;
    for( int j = 0, k = 0; i < quantity ; ++i )
    {
        base digit = this->Numb( k, j );
        Str[i] = IntToChar( digit );

        if( k == 8*(sizeof(base)) - 4 )
        {
            k = 0; ++j;
        }
        else k += sizeof(base);

    }

    printf("\n0x");

    i = quantity - 1;

    while( i >= 0 )
    {
        printf("%c", Str[i--]);
    }
    puts("");

}

base BigNumber::Max(base i, base j)
{
    if( i > j ) return i;
    else return j;
}


base BigNumber::Min(base i, base j)
{
    if( i < j ) return i;
    else return j;
}


BigNumber::~BigNumber()
{
    delete [] m_data;
}

BigNumber BigNumber::Div(const base &second, base *reminder)
{

    BigNumber Result(m_len);

    if( Result.m_len == 1 )
    {
        Result.m_data[0] = m_data[0] / second;
        if(reminder)
            *reminder = m_data[0] % second;
        return Result;
    }

    BigNumber u(*this);



    int m = u.m_len - 1;

    d_base dividend = 0;
    base q = 0;

    for( int j = m; j >= 0; j-- )
    {
        dividend = dividend << 4*sizeof(d_base);
        dividend |= u.m_data[j];

        q = dividend / second;

        Result.m_data[j] = q;

        dividend %= second;

    }

    while( Result.m_data[Result.m_len - 1] == 0 && Result.m_len > 1 )
    {
        Result.m_len--;
    }
    if(reminder)
        *reminder = dividend;
    return Result;


}
