/***************************************************************************
                          kvalueeditor.cpp  -  description
                             -------------------
    begin                : Sa Dez 4 2004
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


// qt specific
#include <QtGui/QKeyEvent>
// corelib specific
#include <kabstractbytearraymodel.h>
// lib specific
#include "kvaluecolumn.h"
#include "kdataranges.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
#include "kvalueeditor.h"


namespace KHEUI {

KValueEditor::KValueEditor( KValueColumn *VC, KDataCursor *BC, KByteArrayView* HE, KController *P )
  : KEditor( BC, HE, P ),
  ValueColumn( VC ),
  InEditMode( false ),
  EditModeByInsert( false )
{
}


bool KValueEditor::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = true;

  // TODO: for now we don't touch it if there are selections
  if( !HexEdit->BufferRanges->hasSelection() )
  {
    //
    switch( KeyEvent->key() )
    {
      case Qt::Key_Plus:
        doValueEditAction( IncValue );
        break;
      case Qt::Key_Minus:
        doValueEditAction( DecValue );
        break;
      case Qt::Key_Space:
        if( !InEditMode )
        {
          KeyUsed = false;
          break;
        }
      case Qt::Key_Enter:
      case Qt::Key_Return:
        doValueEditAction( InEditMode?LeaveValue:EnterValue );
        break;
      case Qt::Key_Escape:
        if( InEditMode )
          doValueEditAction( CancelValue );
        else
          KeyUsed = false;
        break;
      case Qt::Key_Backspace:
        if( InEditMode )
          doValueEditAction( ValueBackspace );
        else
          KeyUsed = false;
        break;
      default:
        // is plain char?
        if( KeyEvent->text().length() > 0
            && ( !(KeyEvent->modifiers()&( Qt::CTRL | Qt::ALT | Qt::META )) ) )
        {
          int Input = KeyEvent->text()[0].toAscii();
          // no usable char?
          if( Input < 32 )
          {
            KeyUsed = false;
            break;
          }

          if( InEditMode )
            doValueEditAction( ValueAppend, Input );
          else
          {
            unsigned char InputValue = 0;
            const KHECore::KByteCodec *ByteCodec = ValueColumn->byteCodec();
            // valid digit?
            if( ByteCodec->appendDigit(&InputValue,Input) )
            {
              if( HexEdit->OverWrite )
                doValueEditAction( ValueEdit, InputValue );
              else
              {
                int Index = BufferCursor->realIndex();
                if( HexEdit->ByteArrayModel->insert(Index,(char*)&InputValue,1) > 0 )
                {
                  InEditMode = true;
                  EditModeByInsert = true;
                  OldValue = EditValue = InputValue;
                  ByteCodec->encode( ByteBuffer, 0, EditValue );

                  BufferCursor->gotoRealIndex();
                  HexEdit->ensureCursorVisible();
                  HexEdit->updateCursors();
                }
              }
            }
          }
        }
        else
          KeyUsed = false;
    }
  }
  else
    KeyUsed = false;

  return KeyUsed ? true : KEditor::handleKeyPress(KeyEvent);
}


void KValueEditor::doValueEditAction( KValueEditAction Action, int Input )
{
  // we are not yet in edit mode?
  if( !InEditMode )
  {
    int ValidIndex = BufferCursor->validIndex();
    // no valid cursor position?
    if( ValidIndex == -1 || (!HexEdit->OverWrite && Input == -1) || BufferCursor->isBehind() )
      return;

    InEditMode = true;
    EditModeByInsert = false; // default, to be overwritten if so

    // save old value
    OldValue = EditValue = (unsigned char)HexEdit->ByteArrayModel->datum(ValidIndex);
  }

  const KHECore::KByteCodec *ByteCodec = ValueColumn->byteCodec();
  // 
  unsigned char NewValue = EditValue;
  bool StayInEditMode = true;
  bool MoveToNext = false;

  switch( Action )
  {
    case ValueEdit:
      NewValue = Input;
      EditValue = NewValue^255; // force update
      EditModeByInsert = true;
      break;
    case ValueBackspace:
      if( NewValue > 0 )
        ByteCodec->removeLastDigit( &NewValue );
      break;
    case EnterValue:
      EditValue ^= 255; // force update
      break;
    case IncValue:
      if( NewValue < 255 )
        ++NewValue;
      break;
    case DecValue:
      if( NewValue > 0 )
        --NewValue;
      break;
    case ValueAppend:
      if( ByteCodec->appendDigit(&NewValue,Input) )
        if( EditModeByInsert && NewValue >= ByteCodec->digitsFilledLimit() )
        {
          StayInEditMode = false;
          MoveToNext = true;
        }
      break;
    case LeaveValue:
      StayInEditMode = false;
      MoveToNext = EditModeByInsert;
      break;
    case CancelValue:
      NewValue = OldValue;
      StayInEditMode = false;
      break;
  }

  // change happened?
  if( NewValue != EditValue )
  {
    // sync value
    EditValue = NewValue;
    ByteCodec->encode( ByteBuffer, 0, EditValue );
    HexEdit->ByteArrayModel->replace( BufferCursor->index(), 1, (char*)&EditValue, 1 );
  }

  HexEdit->updateCursors();

  if( !StayInEditMode )
  {
    HexEdit->pauseCursor();
    InEditMode = false;
    if( MoveToNext )
      BufferCursor->gotoNextByte();
    HexEdit->unpauseCursor();
  }
}

}
