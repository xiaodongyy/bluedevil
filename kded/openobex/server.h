/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef OPENOBEX_SERVER_H
#define OPENOBEX_SERVER_H

#include <QtCore/QObject>
#include <QtDBus>


namespace OpenObex
{

class Server : public QObject
{
    Q_OBJECT
public:
    Server(const QString& addr);
    virtual ~Server();

public Q_SLOTS:
    void start();
    void stop();
    void close();

protected Q_SLOTS:
    void slotStarted();
    void slotStopped();
    void slotClosed();
    void slotErrorOccured(const QString& error_name, const QString& error_message);
    void slotSessionCreated(QDBusObjectPath path);
    void slotSessionRemoved(QDBusObjectPath path);

    void serverCreated(QDBusObjectPath path);
    void serverCreatedError(QDBusError error);

Q_SIGNALS:
    void started();
    void closed();
    void stopped();
    void serverReady();
    void openObexError();

private:
    struct Private;
    Private *d;
};

}

#endif // OPENOBEX_SERVER_H
