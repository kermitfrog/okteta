/***************************************************************************
                          koctalbytecodec.h  -  description
                             -------------------
    begin                : Mo Nov 29 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KOCTALBYTECODEC_H
#define KHE_CORE_KOCTALBYTECODEC_H


// lib specific
#include <kbytecodec.h>


namespace KHECore
{

class KOctalByteCodec : public KByteCodec
{
  public:
    virtual ~KOctalByteCodec() {}

  public: // KByteCodec API
    virtual unsigned int encodingWidth() const { return 3; }
    virtual unsigned char digitsFilledLimit() const { return 64; }

    virtual void encode( QString &Digits, unsigned  int Pos, const unsigned char Char ) const;
    virtual void encodeShort( QString &Digits, unsigned  int Pos, const unsigned char Char ) const;
    virtual bool appendDigit( unsigned char *Byte, const unsigned char Digit ) const;
    virtual void removeLastDigit( unsigned char *Byte ) const;
    virtual bool isValidDigit( const unsigned char Digit ) const;
    virtual bool turnToValue( unsigned char *Digit ) const;
};

}

#endif
