/*
 *   SPDX-FileCopyrightText: 2010 Alex Fiestas <alex@eyeos.org>
 *   SPDX-FileCopyrightText: 2010 UFO Coders <info@ufocoders.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "wizardagent.h"
#include "debug_p.h"

#include <QDBusObjectPath>
#include <QFile>
#include <QStandardPaths>
#include <QXmlStreamReader>

#include <KLocalizedString>
#include <KRandom>

#include <BluezQt/Device>
#include <QDebug>

WizardAgent::WizardAgent(QObject *parent)
    : BluezQt::Agent(parent)
    , m_fromDatabase(false)
{
}

QString WizardAgent::pin()
{
    return m_pin;
}

void WizardAgent::setPin(const QString &pin)
{
    m_pin = pin;
    m_fromDatabase = false;
}

bool WizardAgent::isFromDatabase()
{
    return m_fromDatabase;
}

QString WizardAgent::getPin(BluezQt::DevicePtr device)
{
    m_fromDatabase = false;
    m_pin = QString::number(KRandom::random());
    m_pin = m_pin.left(6);

    const QString &xmlPath = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("pin-code-database.xml"));

    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCDebug(WIZARD) << "Can't open the pin-code-database.xml";
        return m_pin;
    }

    QXmlStreamReader xml(&file);

    QString deviceType = BluezQt::Device::typeToString(device->type());
    if (deviceType == QLatin1String("audiovideo")) {
        deviceType = QStringLiteral("audio");
    }

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.name() != QLatin1String("device")) {
            continue;
        }
        QXmlStreamAttributes attr = xml.attributes();

        if (attr.count() == 0) {
            continue;
        }

        if (attr.hasAttribute(QLatin1String("type")) && attr.value(QLatin1String("type")) != QLatin1String("any")) {
            if (deviceType != attr.value(QLatin1String("type")).toString()) {
                continue;
            }
        }

        if (attr.hasAttribute(QLatin1String("oui"))) {
            if (!device->address().startsWith(attr.value(QLatin1String("oui")).toString())) {
                continue;
            }
        }

        if (attr.hasAttribute(QLatin1String("name"))) {
            if (device->name() != attr.value(QLatin1String("name")).toString()) {
                continue;
            }
        }

        m_pin = attr.value(QLatin1String("pin")).toString();
        m_fromDatabase = true;
        if (m_pin.startsWith(QLatin1String("max:"))) {
            m_fromDatabase = false;
            int num = m_pin.rightRef(m_pin.length() - 4).toInt();
            m_pin = QString::number(KRandom::random()).left(num);
        }

        qCDebug(WIZARD) << "PIN: " << m_pin;
        return m_pin;
    }

    return m_pin;
}

QDBusObjectPath WizardAgent::objectPath() const
{
    return QDBusObjectPath(QStringLiteral("/agent"));
}

void WizardAgent::requestPinCode(BluezQt::DevicePtr device, const BluezQt::Request<QString> &req)
{
    qCDebug(WIZARD) << "AGENT-RequestPinCode" << device->ubi();

    Q_EMIT pinRequested(m_pin);
    req.accept(m_pin);
}

void WizardAgent::displayPinCode(BluezQt::DevicePtr device, const QString &pinCode)
{
    qCDebug(WIZARD) << "AGENT-DisplayPinCode" << device->ubi() << pinCode;

    Q_EMIT pinRequested(pinCode);
}

void WizardAgent::requestPasskey(BluezQt::DevicePtr device, const BluezQt::Request<quint32> &req)
{
    qCDebug(WIZARD) << "AGENT-RequestPasskey" << device->ubi();

    Q_EMIT pinRequested(m_pin);
    req.accept(m_pin.toUInt());
}

void WizardAgent::displayPasskey(BluezQt::DevicePtr device, const QString &passkey, const QString &entered)
{
    Q_UNUSED(entered);

    qCDebug(WIZARD) << "AGENT-DisplayPasskey" << device->ubi() << passkey;

    Q_EMIT pinRequested(passkey);
}

void WizardAgent::requestConfirmation(BluezQt::DevicePtr device, const QString &passkey, const BluezQt::Request<> &req)
{
    
    qCDebug(WIZARD) << "AGENT-RequestConfirmation " << device->ubi() << passkey;

    Q_EMIT confirmationRequested(passkey, req);
}
