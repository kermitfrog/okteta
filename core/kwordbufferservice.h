/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2005 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KHE_CORE_KWORDBUFFERSERVICE_H
#define KHE_CORE_KWORDBUFFERSERVICE_H

// lib
#include "oktetacore_export.h"
// commonlib
#include <ksection.h>


namespace KHECore
{

class KAbstractByteArrayModel;
class KCharCodec;

/**
  *
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT KWordBufferService
{
  public:
    KWordBufferService( KAbstractByteArrayModel *B, KCharCodec *C );
    ~KWordBufferService();


  public:
    /** searches for the start of the word including the given index.
      * if no other nonwordchar preceds this is 0;
      * If the byte at the given Index is already a nonword char the given index is returned.
      * @param Index index to start with
      * @param CharType
      * @return index of the first char of the current word or the given index if there is none
      */
    int indexOfWordStart( unsigned int Index ) const;
    int indexOfLeftWordSelect( unsigned int Index ) const;
    /** searches for the end of the word including the given index.
      * If the byte at the given Index is already a nonword char the given index is returned.
      * if no other nonwordchar follows, that of the last byte;
      * @param Index index to start with
      * @param CharType
      * @return index of the last char of the current word or the given index if there is none
      */
    int indexOfWordEnd( unsigned int Index ) const;
    /** searches for the first char after the end of the word including the given index.
      * If the byte at the given Index is already a nonword char the given index is returned.
      * if no other nonwordchar follows that of behind the last byte;
      * @param Index index to start with
      * @param CharType
      * @return index of the first char after the current word or the given index if there is none
      */
    int indexOfRightWordSelect( unsigned int Index ) const;
    /** searches for the first char after the end of the word including the given index.
      * If the byte at the given Index is already a nonword char the given index is returned.
      * if no other nonwordchar follows that of behind the last byte;
      * @param Index index to start with
      * @param CharType
      * @return index of the first char after the current word or the given index if there is none
      */
//    int indexOfBehindLeftWordEnd( unsigned int Index ) const;
    /** searches for the first char after the end of the word including the given index.
      * If the byte at the given Index is already a nonword char the given index is returned.
      * if no other nonwordchar follows that of behind the last byte;
      * @param Index index to start with
      * @param CharType
      * @return index of the first char after the current word or the given index if there is none
      */
//    int indexOfBehindRightWordEnd( unsigned int Index ) const;
    /** searches the start of the next previous word that does not include the given index,
      * if no further word is found 0 is returned.
      * if the index is out of range the behaviour is undefined.
      * @param Index
      * @param CharType
      * @return index of the next previous word start or 0
      */
    int indexOfPreviousWordStart( unsigned int Index ) const;
    /** searches for the start of the next word not including the given index.
      * if there isn't a next word the index behind end is returned
      * @param Index
      * @param CharType
      * @return index of the start of the next word or behind end
      */
    int indexOfNextWordStart( unsigned int Index ) const;
    /** searches for the start of the next word not including the given index.
      * if there isn't a next word the index of the end is returned
      * @param Index index to start with
      * @param CharType
      * @return index of the last nonword char before the next word or the last index
      */
    int indexOfBeforeNextWordStart( unsigned int Index ) const;

    /** if Index is out of range the behaviour is undefined
      * @param Index
      * @param CharType
      * @return @c true if the byte at position i is a char of type CharType 
      */
    bool isWordChar( unsigned int Index ) const;

    /** returns the section with a word around index.
      * if there is no word the section is empty
      * @param Index
      * @param CharType
      * @return the section with a word around index.
      */
    KHE::KSection wordSection( unsigned int Index ) const;

  protected:
    KAbstractByteArrayModel *Buffer;
    KCharCodec *CharCodec;
};

}

#endif
