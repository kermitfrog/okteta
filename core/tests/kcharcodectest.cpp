/***************************************************************************
                          kcharcodectest.cpp  -  description
                            -------------------
    begin                : Sa Jan 15 2005
    copyright            : (C) 2005 by Friedrich W. H. Kossebau
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
#include <QtTest>
// kde specific
#include <kcomponentdata.h>
// lib specific
#include <khechar.h>
#include <kcharcodec.h>
// test specific
#include "kcharcodectest.h"

// namespaces
using namespace KHECore;


void KCharCodecTest::initTestCase()
{
  Instance = new KComponentData( "KTextCharCodecTest" );
}

void KCharCodecTest::cleanupTestCase()
{
  delete Instance;
}

//---------------------------------------------------------------------------- Tests -----


void KCharCodecTest::testCodecNames()
{
  QStringList List( KCharCodec::codecNames() );

  for( int i=0; i<List.size(); ++i )
  {
    // name exists
    const QString &Name = List[i];
    KCharCodec* Codec = KCharCodec::createCodec( Name );
    QCOMPARE( Codec->name(), Name );
    delete Codec;
  }
}


QTEST_MAIN( KCharCodecTest )

#include "kcharcodectest.moc"
