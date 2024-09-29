// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0) throw std::logic_error("TBitField::TBitField : len param must be greater than zero.");
    
    BitLen = len;
    MemLen = (len + (8 * sizeof(TELEM) - 1)) / (8 * sizeof(TELEM));
    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    if (bf == *this) return;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    copy(bf.pMem, bf.pMem + MemLen, pMem);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw range_error("TBitField::GetMemIndex : param n is out of bounds.");
    return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) throw range_error("TBitField::GetMemIndex : param n is out of bounds.");
    return 1 << n % (8 * sizeof(TELEM));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw range_error("TBitField::GetMemIndex : param n is out of bounds.");

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw range_error("TBitField::GetMemIndex : param n is out of bounds.");

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw range_error("TBitField::GetMemIndex : param n is out of bounds.");

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }

    BitLen = bf.BitLen;
    copy(bf.pMem, bf.pMem + MemLen, pMem);

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (bf.GetLength() != GetLength()) return 0;

    for (size_t i = 0; i < MemLen; i++)
    {
        if (bf.pMem[i] != pMem[i])
        {
            return 0;
        }
    }
    
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return operator==(bf) == 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField bitfield(max(GetLength(), bf.GetLength()));

    for (size_t i = 0; i < GetLength(); i++)
    {
        if (GetBit(i))
        {
            bitfield.SetBit(i);
        }
    }

    for (size_t i = 0; i < bf.GetLength(); i++)
    {
        if (bf.GetBit(i))
        {
            bitfield.SetBit(i);
        }
    }

    return bitfield;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField bitfield(max(GetLength(), bf.GetLength()));

    for (size_t i = 0; i < min(GetLength(), bf.GetLength()); i++)
    {
        if (GetBit(i) && bf.GetBit(i))
        {
            bitfield.SetBit(i);
        }
    }

    return bitfield;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField bitfield(GetLength());

    for (size_t i = 0; i < GetLength(); i++)
    {
        if (!GetBit(i))
        {
            bitfield.SetBit(i);
        }
    }

    return bitfield;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (size_t i = 0; i < bf.GetLength(); i++)
    {
        char bit;
        istr >> bit;

        if (bit == '1')
        {
            bf.SetBit(i);
        }
        else
        {
            bf.ClrBit(i);
        }
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (size_t i = 0; i < bf.GetLength(); i++)
    {
        ostr << bf.GetBit(i);
    }

    return ostr;
}
