/****************************************************************************
**
** Copyright (C) 2014 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMASKGENERATOR_H
#define QMASKGENERATOR_H

#include <QObject>

class  QMaskGenerator : public QObject
{
    Q_DISABLE_COPY(QMaskGenerator)

public:
    explicit QMaskGenerator(QObject *parent = 0);
    virtual ~QMaskGenerator();

    virtual bool seed() = 0;
    virtual quint32 nextMask() = 0;
};

class Url
{
public:
    Url():resourceName("/"),port(-1){}
    QString Protocol() const {return protocol;}
    QString Host() const {return host;}
    QString ResourceName() const {return resourceName;}
    qint16 Port(qint16 p = 80) const {if (port == -1) return p; else return port;}
    bool isInvalid() const {if (protocol.isEmpty() || host.isEmpty()) return true; else return false;}
    void clear() {protocol.clear();host.clear();resourceName.clear();port = -1;}
    static bool fromString(const QString & url, Url & mUrl);
private:
    QString protocol,host,resourceName;
    qint16 port;
};

#endif // QMASKGENERATOR_H
