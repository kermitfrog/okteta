/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

// lib
#include "abstractview.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QObject>


namespace Kasten
{

class ModelCodecViewManager;
class DocumentManager;
class AbstractViewFactory;


class KASTENGUI_EXPORT ViewManager : public QObject
{
  Q_OBJECT

  public:
    explicit ViewManager( DocumentManager* documentManager );
    virtual ~ViewManager();

  public:
    void setViewFactory( AbstractViewFactory* factory );

  public:
    QList<AbstractView*> views() const;
    AbstractView *viewByWidget( QWidget* widget ) const;
    AbstractView* viewOfDocument( AbstractDocument* document ) const;

  public:
    ModelCodecViewManager* codecViewManager();

  Q_SIGNALS:
    // view was created and already added to the list
    void opened( const QList<Kasten::AbstractView*>& views );
    // view will be closed, already removed from list
    void closing( const QList<Kasten::AbstractView*>& views );

  protected Q_SLOTS:
    void createViewsFor( const QList<Kasten::AbstractDocument*>& documents );
    void removeViewsFor( const QList<Kasten::AbstractDocument*>& documents );
    /**
    * asks the manager to close and delete the view
    * may fail if the process if cancelled due to user input
    */
//     void closeView( AbstractView *view );

  protected:
    QList<AbstractView*> mViewList;
    AbstractViewFactory* mFactory;
    DocumentManager* mDocumentManager;

    // TODO: remove into own singleton
    ModelCodecViewManager* mCodecViewManager;
};


inline ModelCodecViewManager* ViewManager::codecViewManager() { return mCodecViewManager; }

}

#endif
