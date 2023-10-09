// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Множество - реализация через битовые поля

#include <iostream>
#include "tbitfield.h"
#include "tset.h"

TSet::TSet(size_t mp) : bitField(mp) {
    maxPower = mp;
    bitField = TBitField(mp);
}

// конструктор копирования
TSet::TSet(const TSet& s) : bitField(s.bitField) {
    maxPower = s.maxPower;
    bitField = s.bitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : bitField(bf) {
    bitField = bf;
    maxPower = bf.getLength();
}

TSet::operator TBitField()
{
    return bitField;
}

size_t TSet::getMaxPower() const // получить макс. к-во эл-тов
{
    return maxPower;
}

bool TSet::isMember(const myuint elem) const // элемент множества?
{
    return bitField.getBit(elem);
}

void TSet::insElem(const myuint elem) // включение элемента множества
{
    bitField.setBit(elem);
}

void TSet::delElem(const myuint elem) // исключение элемента множества
{
    bitField.clrBit(elem);
}

// теоретико-множественные операции
TSet& TSet::operator=(const TSet& s) // присваивание
{
    bitField = s.bitField;
    maxPower = s.maxPower;
    return *this;
}

bool TSet::operator==(const TSet& s) const // сравнение
{
    if (maxPower != s.maxPower)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < maxPower; i++)
        {
            if (bitField.getBit(i) != s.bitField.getBit(i))
            {
                return false;
            }
        }
    }
    return true;
}

bool TSet::operator!=(const TSet& s) const // сравнение
{
    if (maxPower != s.maxPower) { return true; }
    else
    {
        for (int i = 0; i < maxPower; i++)
        {
            if (bitField.getBit(i) != s.bitField.getBit(i))
            {
                return true;
            }
        }
    }
    return false;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    if (maxPower > s.getMaxPower())
    {
        TBitField nbf(maxPower);
        nbf = bitField | s.bitField;
        TSet news(nbf);
        return news;
    }
    else
    {
        TBitField nbf(s.getMaxPower());
        nbf = bitField | s.bitField;
        TSet news(nbf);
        return news;
    }
}

TSet TSet::operator+(const myuint elem) // объединение с элементом
{
    TSet news(*this);
    news.bitField.setBit(elem);
    return news;
}

TSet TSet::operator-(const myuint elem) // разность с элементом
{
    TSet news(*this);
    news.bitField.clrBit(elem);
    return news;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    myuint new_mp = maxPower;
    if (s.maxPower > maxPower)
    {
        new_mp = s.maxPower;
    }
    TSet news(new_mp);
    news = bitField & s.bitField;
    return news;
}

TSet TSet::operator~() // дополнение
{
    TSet news(maxPower);
    news.bitField = ~bitField;
    return news;
}

// перегрузка ввода/вывода
std::istream& operator>>(std::istream& istr, TSet& s) // ввод
{
    int i = 0;
    while ((i >= 0) && (i < s.getMaxPower()))
    {
        s.insElem(i);
        istr >> i;
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) // вывод
{
    for (int i = 0; i < s.getMaxPower(); i++)
        ostr << i;
    return ostr;
}