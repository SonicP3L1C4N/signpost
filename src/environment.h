// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QObject>
#include <QQmlEngine>

/**
 * What to quote in a bug report.
 *
 * A Plasma bug report is unanswerable without the versions it happened on, and
 * nobody has those by heart -- least of all someone who moved to this desktop
 * last week and is reporting the bug because they cannot tell what is normal
 * yet.
 */
class Environment : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString qtVersion READ qtVersion CONSTANT)
    Q_PROPERTY(QString frameworksVersion READ frameworksVersion CONSTANT)
    Q_PROPERTY(QString plasmaVersion READ plasmaVersion CONSTANT)
    Q_PROPERTY(QString session READ session CONSTANT)
    Q_PROPERTY(QString homepage READ homepage CONSTANT)
    Q_PROPERTY(QString bugAddress READ bugAddress CONSTANT)

public:
    explicit Environment(QObject *parent = nullptr);

    QString version() const;
    QString qtVersion() const;
    QString frameworksVersion() const;

    /** "" when there is no plasmashell to ask, which is not an error. */
    QString plasmaVersion() const;

    QString session() const;
    QString homepage() const;
    QString bugAddress() const;

    /** The block the About page shows, with the entry count folded in. */
    Q_INVOKABLE QString report(int entries) const;

    /** The same block, on the clipboard, ready to paste into an issue. */
    Q_INVOKABLE void copyReport(int entries) const;
};
