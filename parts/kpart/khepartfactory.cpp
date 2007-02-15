/***************************************************************************
                          khepartfactory.h  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// kde specific
#include <kcomponentdata.h>
#include <kaboutdata.h>
#include <klocale.h>
// part specific
#include "khepart.h"
#include "khepartfactory.h"


// Part
static const char PartId[] =          "khexedit2part";
static const char PartName[] =        I18N_NOOP("KHexEdit2Part");
static const char PartDescription[] = I18N_NOOP("Embedded hex editor");
static const char PartVersion[] =     "0.3.0";
static const char PartCopyright[] =   "(C) 2003-2006 Friedrich W. H. Kossebau";
// Author
static const char FWHKName[] =         "Friedrich W. H. Kossebau";
static const char FWHKTask[] =         I18N_NOOP("Author");
static const char FWHKEmailAddress[] = "kossebau@kde.org";
// static const char FWHKWebAddress[] = "http://www.kossebau.de";


KComponentData *KHexEditPartFactory::s_instance = 0;
KAboutData* KHexEditPartFactory::s_about = 0;


KHexEditPartFactory::KHexEditPartFactory()
 : KParts::Factory()
{
}


KHexEditPartFactory::~KHexEditPartFactory()
{
  delete s_instance;
  delete s_about;

  s_instance = 0;
}


KParts::Part* KHexEditPartFactory::createPartObject( QWidget *ParentWidget, 
                                                     QObject *Parent, 
                                                     const char *CN, const QStringList &/*args*/ )
{
  QByteArray Classname( CN );
  bool BrowserViewWanted = ( Classname == "Browser/View" );
  //bool ReadOnlyWanted = (BrowserViewWanted || ( Classname == "KParts::ReadOnlyPart" ));

  // Create an instance of our Part
  KHexEditPart* HexEditPart = new KHexEditPart( ParentWidget, Parent, BrowserViewWanted );

  return HexEditPart;
}


const KComponentData &KHexEditPartFactory::componentData()
{
  if( !s_instance )
  {
    s_about = new KAboutData( PartId, PartName, PartVersion, PartDescription,
                              KAboutData::License_GPL_V2, PartCopyright, 0, 0, FWHKEmailAddress );
    s_about->addAuthor( FWHKName, FWHKTask, FWHKEmailAddress );
    s_instance = new KComponentData( s_about );
  }
  return *s_instance;
}


K_EXPORT_COMPONENT_FACTORY( libkhexedit2part, KHexEditPartFactory )

#include "khepartfactory.moc"
