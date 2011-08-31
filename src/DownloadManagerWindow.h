/****************************************************************************
 *   Copyright (C) 2011  Instituto Nokia de Tecnologia (INdT)               *
 *                                                                          *
 *   This file may be used under the terms of the GNU Lesser                *
 *   General Public License version 2.1 as published by the Free Software   *
 *   Foundation and appearing in the file LICENSE.LGPL included in the      *
 *   packaging of this file.  Please review the following information to    *
 *   ensure the GNU Lesser General Public License version 2.1 requirements  *
 *   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.   *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 ****************************************************************************/

#ifndef DownloadManagerWindow_h
#define DownloadManagerWindow_h

#include <QtDeclarative/QSGView>
#include <QtGui/QMainWindow>

class DownloadManagerWindow : public QMainWindow {
    Q_OBJECT

public:
    DownloadManagerWindow(QWidget* parent = 0);
    virtual ~DownloadManagerWindow();

private:
    void setupDeclarativeEnvironment();
    QSGView* m_view;
};

#endif
