/***************************************************************************
                          kfixedsizebytearraymodeltest.h  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/



#ifndef KFIXEDSIZEBUFFERTEST_H
#define KFIXEDSIZEBUFFERTEST_H

// qt specific
#include <QObject>

class KFixedSizeByteArrayModelTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testCompare();
    //void testCopy();

};

#endif
