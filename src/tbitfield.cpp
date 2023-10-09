// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len)
{
    if (len < 0)
    {
        throw "wrong len";
    }
    if (len == 0)
    {
        bitLen = len;
        memLen = 0;
        pMem = nullptr;
    }
    else
    {
        bitLen = len;
        memLen = (bitLen / (sizeof(myuint) * 8)) + 1;
        pMem = new myuint[memLen];
        for (int i = 0; i < memLen; i++)
            pMem[i] = 0;

    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.getLength();
    memLen = (bitLen + sizeof(myuint) * 8 - 1) / sizeof(myuint);
    pMem = new myuint[memLen];
    for (int i = 0; i < memLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    if ((n < 0) || (n >= bitLen))
    {
        throw "wrong index";
    }
    return (n + (sizeof(myuint) * 8)) / (sizeof(myuint) * 8) - 1;
}

myuint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    if ((n < 0) || (n >= bitLen))
    {
        throw "wrong index";
    }
    else
    {
        return (1 << (n % (sizeof(myuint) * 8)));
    }
}

// доступ к битам битового поля
myuint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return bitLen / 8;//memLen * sizeof(myuint);
}

void TBitField::setBit(const size_t n) // установить бит
{
    if ((n < 0) || (n >= bitLen))
    {
        throw "wrong index";
    }
    else
    {
        int num = getIndex(n);
        myuint mask = getMask(n);
        pMem[num] = pMem[num] | mask;
    }
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if ((n < 0) || (n >= bitLen))
    {
        throw "wrong index";
    }
    else
    {
        int num = getIndex(n);
        myuint mask = getMask(n);
        pMem[num] = pMem[num] & ~mask;
    }
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    bool res;
    if ((n < 0) || (n >= bitLen))
    {
        throw "wrong index";
    }
    else
    {
        int num = getIndex(n);
        myuint mask = getMask(n);
        res = mask & pMem[num];
    }
    return res;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new myuint[memLen];
    for (int i = 0; i < memLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (bitLen != bf.bitLen)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < bitLen; i++)
        {
            if (getBit(i) != bf.getBit(i))
            {
                return false;
            }
        }
    }
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (bitLen != bf.bitLen)
    {
        return true;
    }
    else
    {
        for (int i = 0; i < bitLen; i++)
        {
            if (getBit(i) != bf.getBit(i))
            {
                return true;
            }
        }
    }
    return false;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (bitLen < bf.bitLen)
    {
        TBitField b(bf.bitLen);
        for (int i = 0; i < memLen; i++)
        {
            b.pMem[i] = pMem[i] | bf.pMem[i];
        }
        return b;
    }
    else
    {
        TBitField b(bitLen);
        for (int i = 0; i < bf.memLen; i++)
        {
            b.pMem[i] = pMem[i] | bf.pMem[i];
        }
        return b;
    }
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    if (bitLen < bf.bitLen)
    {
        TBitField b(bf.bitLen);
        for (int i = 0; i < bf.bitLen; i++)
        {
            b.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return b;
    }
    else
    {
        TBitField b(bitLen);
        for (int i = 0; i < bf.memLen; i++)
        {
            b.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return b;
    }
}

TBitField TBitField::operator~() // отрицание
{
    myuint num = 0;
    TBitField newbf(bitLen);
    for (int i = 0; i < memLen - 1; i++)
    {
        newbf.pMem[i] = ~pMem[i];
    }
    for (int i = 0; i < bitLen % (sizeof(myuint) * 8); i++)
    {
        num = num + (1 << i);
    }
    newbf.pMem[memLen - 1] = pMem[memLen - 1] ^ num;
    return newbf;

}

TBitField::~TBitField()
{
    delete[] pMem;
}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    char ss[255];
    istr.getline(ss, 255);
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.bitLen; i++)
    {
        ostr << bf.getBit(i);
    }
    return ostr;
}
