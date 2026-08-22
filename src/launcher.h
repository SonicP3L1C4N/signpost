// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QObject>
#include <QQmlEngine>

/**
 * Opening the thing the entry points at.
 *
 * A phrasebook that only tells you the name leaves you to go and find it; the
 * button that opens it is the reason this is an application rather than a wiki
 * page.
 */
class Launcher : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Launcher(QObject *parent = nullptr);

    /** Starts an application by .desktop id. */
    Q_INVOKABLE void launch(const QString &desktopId) const;

    /** Shows it in Discover, for the entries whose answer is not installed. */
    Q_INVOKABLE void showInDiscover(const QString &desktopId) const;
};
