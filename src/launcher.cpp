// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "launcher.h"

#include <KIO/ApplicationLauncherJob>
#include <KService>

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

Launcher::Launcher(QObject *parent)
    : QObject(parent)
{
}

void Launcher::launch(const QString &desktopId) const
{
    const KService::Ptr service = KService::serviceByStorageId(desktopId);
    if (!service) {
        qWarning() << "signpost: nothing installed for" << desktopId;
        return;
    }
    auto *job = new KIO::ApplicationLauncherJob(service);
    job->start();
}

void Launcher::showInDiscover(const QString &desktopId) const
{
    // Discover answers appstream: URLs, and a .desktop id is the appstream id
    // with the suffix taken off for the applications that ship both.
    QString appstreamId = desktopId;
    if (appstreamId.endsWith(QLatin1String(".desktop"))) {
        appstreamId.chop(QStringLiteral(".desktop").length());
    }
    QDesktopServices::openUrl(QUrl(QStringLiteral("appstream://") + appstreamId));
}
