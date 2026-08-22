// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "entrymodel.h"

#include <KRunner/AbstractRunner>

/**
 * The phrasebook, in KRunner.
 *
 * This is the form the answer wants to take. Someone who has just switched
 * presses Meta and types, because that is what the Start menu trained them to
 * do -- and what they type is the Windows word. Meeting them there costs them
 * nothing: no window to find, no application to remember having installed.
 *
 * The cost of being wrong is high, though. KRunner shows every runner's
 * guesses at once, so a phrasebook that answers loosely becomes noise in
 * everybody's launcher. Only a name or an alias earns a row here; a summary
 * that happens to contain the word does not.
 */
class SignpostRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    SignpostRunner(QObject *parent, const KPluginMetaData &metaData);

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context,
             const KRunner::QueryMatch &match) override;

private:
    EntryModel m_phrasebook;

    /**
     * The subtexts, translated once, in the thread that may do it.
     *
     * The first call into KLocalizedString installs an event filter on the
     * application, and a runner matches on its own thread -- so translating
     * lazily means doing that from the wrong one. They never change, so
     * building them with the phrasebook costs nothing and says nothing at
     * match time but a list lookup.
     */
    QStringList m_subtexts;
};
