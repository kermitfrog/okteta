/***************************************************************************
                          kabstractdocumentfilesystemsynchronizer.cpp  -  description
                             -------------------
    begin                : Mon Nov 12 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kabstractdocumentfilesystemsynchronizer.h"

// lib
#include <kabstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KMessageBox>
#include <KDirWatch>

class KAbstractDocumentFileSystemSynchronizer::Private
{
  public:
    Private();
  public:
    QWidget *widget() const;
    void setWidget( QWidget *widget );
  protected:
    QWidget *mWidget;
};
inline KAbstractDocumentFileSystemSynchronizer::Private::Private() : mWidget( 0 ) {}
inline QWidget *KAbstractDocumentFileSystemSynchronizer::Private::widget() const { return mWidget; }
inline void KAbstractDocumentFileSystemSynchronizer::Private::setWidget( QWidget *widget) { mWidget = widget; }


KAbstractDocumentFileSystemSynchronizer::KAbstractDocumentFileSystemSynchronizer()
 : d( new Private() )
{}

KAbstractDocument *KAbstractDocumentFileSystemSynchronizer::loadFromUrl( const KUrl &url )
{
    KAbstractDocument *document;
    QString workFileName;
    if( KIO::NetAccess::download(url.url(),workFileName,d->widget()) )
    {
        document = loadFromFile( workFileName );
        if( document )
        {
            setUrl( url );
            if( url.isLocalFile() )
            {
                connect( KDirWatch::self(), SIGNAL(dirty( const QString & )),
                         SLOT(onFileDirty( const QString & )) );

                connect( KDirWatch::self(), SIGNAL(created( const QString & )),
                         SLOT(onFileCreated( const QString & )) );

                connect( KDirWatch::self(), SIGNAL(deleted( const QString & )),
                          SLOT(onFileDeleted( const QString & )) );
                KDirWatch::self()->addFile( url.path() );
            }
            document->setSynchronizer( this );
        }
        else
            // TODO: these reports should go to a notification system, for log or popup
            KMessageBox::error( d->widget(), i18n("Problem when loading from local filesystem.") );

        KIO::NetAccess::removeTempFile( workFileName );
    }
    else
    {
        document = 0;
        KMessageBox::error( d->widget(), KIO::NetAccess::lastErrorString() );
    }

   return document;
}

bool KAbstractDocumentFileSystemSynchronizer::reloadFromUrl()
{
    bool result;
    QString workFileName;
    if( KIO::NetAccess::download(url().url(),workFileName,d->widget()) )
    {
        result = reloadFromFile( workFileName );
        if( !result )
            KMessageBox::error( d->widget(), i18n("Problem when loading from local filesystem.") );

        KIO::NetAccess::removeTempFile( workFileName );
    }
    else
    {
        result = false;
        KMessageBox::error( d->widget(), KIO::NetAccess::lastErrorString() );
    }

   return result;
}

bool KAbstractDocumentFileSystemSynchronizer::writeToUrl()
{
    bool result;

    QString workFileName;
    KTemporaryFile temporaryFile;

    if( url().isLocalFile() )
        workFileName = url().path();
    else
    {
        temporaryFile.open();
        workFileName = temporaryFile.fileName();
    }

    if( writeToFile(workFileName) )
    {
        if( !url().isLocalFile() )
        {
            result = KIO::NetAccess::upload( workFileName, url(), d->widget() );
            if( !result )
                KMessageBox::error( d->widget(), KIO::NetAccess::lastErrorString() );
        }
        else
            result = true;
    }
    else
    {
        result = false;
        KMessageBox::error( d->widget(), i18n("Problem when saving to local filesystem.") );
    }

    return result;
}

bool KAbstractDocumentFileSystemSynchronizer::syncWithUrl( const KUrl &newUrl,
                                                           KAbstractDocumentSynchronizer::ConnectOption option )
{
// Comment: here we play tricks to reuse the temporary file
// KIO::NetAccess::removeTempFile only removes tempfiles created by KIO::NetAccess::download
// So if replaceRemote and workFileName is temporaryFile both don't conflict -> no problem (now)

    bool result = false;

    QString workFileName;
    KTemporaryFile temporaryFile;

    bool isWorkFileOk;
    if( option == ReplaceRemote )
    {
        if( newUrl.isLocalFile() )
            workFileName = newUrl.path();
        else
        {
            temporaryFile.open();
            workFileName = temporaryFile.fileName();
        }
        isWorkFileOk = true;
    }
    else
        isWorkFileOk = KIO::NetAccess::download( newUrl.url(), workFileName, d->widget() );

    if( isWorkFileOk )
    {
        if( syncWithFile(workFileName,option) )
        {
            // care for old url
            if( url().isLocalFile() )
            {
                disconnect( KDirWatch::self() );
                KDirWatch::self()->removeFile( url().path() );
            }

            if( !newUrl.isLocalFile() )
            {
                result = KIO::NetAccess::upload( workFileName, newUrl, d->widget() );
                if( !result )
                    KMessageBox::error( d->widget(), KIO::NetAccess::lastErrorString() );
            }
            else
            {
                connect( KDirWatch::self(), SIGNAL(dirty( const QString & )),
                         SLOT(onFileDirty( const QString & )) );

                connect( KDirWatch::self(), SIGNAL(created( const QString & )),
                         SLOT(onFileCreated( const QString & )) );

                connect( KDirWatch::self(), SIGNAL(deleted( const QString & )),
                          SLOT(onFileDeleted( const QString & )) );
                KDirWatch::self()->addFile( workFileName );
                result = true;
            }
            setUrl( newUrl );
            // TODO; in path of both constructor by url and synchWithRemote
            // only needed for the first, so constructor writers can forget about this
            // for now we just check in setSynchronizer that new != old before deleteing old
            document()->setSynchronizer( this );
        }
        else
            KMessageBox::error( d->widget(), i18n("Problem when synching with local filesystem.") );
        KIO::NetAccess::removeTempFile( workFileName );
    }
    else
        KMessageBox::error( d->widget(), KIO::NetAccess::lastErrorString() );

    return result;
}

bool KAbstractDocumentFileSystemSynchronizer::syncFromRemote()
{
    return reloadFromUrl();
}

bool KAbstractDocumentFileSystemSynchronizer::syncToRemote()
{
    return writeToUrl();
}

bool KAbstractDocumentFileSystemSynchronizer::syncWithRemote( const KUrl &url,
                                                              KAbstractDocumentSynchronizer::ConnectOption option )
{
    return syncWithUrl( url, option );
}


void KAbstractDocumentFileSystemSynchronizer::onFileDirty( const QString &fileName )
{
    if( url().url() == fileName )
        ;//document()->setRemoteHasChanges(); TODO: needs a control interface? 
}
void KAbstractDocumentFileSystemSynchronizer::onFileCreated( const QString &fileName )
{
  Q_UNUSED( fileName )
  //TODO: could happen after a delete, what to do?
}
void KAbstractDocumentFileSystemSynchronizer::onFileDeleted( const QString &fileName )
{
    if( url().url() == fileName )
        ;//document()->setRemoteHasChanges(); TODO: needs a control interface? 
}

KAbstractDocumentFileSystemSynchronizer::~KAbstractDocumentFileSystemSynchronizer()
{
    if( url().isLocalFile() )
        KDirWatch::self()->removeFile( url().path() );

    delete d;
}
