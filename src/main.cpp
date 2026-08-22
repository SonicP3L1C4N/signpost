// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include <KService>

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    // QApplication rather than QGuiApplication: KIO's launcher job puts up
    // widget dialogs when a .desktop file wants a terminal or asks a question.
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("signpost"));
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("documentation")));
    // Claimed only once the .desktop file is installed: without it the portal
    // rejects the id and Qt says so on every start.
    const QString desktopId = QStringLiteral("io.github.sonicp3l1c4n.signpost");
    if (KService::serviceByDesktopName(desktopId)) {
        QGuiApplication::setDesktopFileName(desktopId);
    }
    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));

    KAboutData about(QStringLiteral("signpost"),
                     i18n("Signpost"),
                     QStringLiteral(SIGNPOST_VERSION),
                     i18n("What things are called on KDE, when you have come from Windows"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2026 Gary Bissett"));
    about.addAuthor(i18n("Gary Bissett"),
                    i18n("Author"),
                    QStringLiteral("gary.bissett@gmail.com"));
    about.setHomepage(QStringLiteral("https://github.com/SonicP3L1C4N/signpost"));
    about.setBugAddress(QByteArrayLiteral("https://github.com/SonicP3L1C4N/signpost/issues"));
    // Gives --version and --help for free, which is the lesson the last
    // project learned the hard way.
    KAboutData::setApplicationData(about);

    // `signpost "task manager"` opens with the answer already on screen, which
    // is what makes it worth putting on a shortcut or typing into KRunner.
    QCommandLineParser parser;
    about.setupCommandLine(&parser);
    parser.addPositionalArgument(QStringLiteral("term"),
                                 i18n("What the thing is called on Windows"));
    parser.process(app);
    about.processCommandLine(&parser);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.setInitialProperties({
        {QStringLiteral("initialSearch"), parser.positionalArguments().value(0)},
    });
    engine.loadFromModule("org.kde.signpost", "Main");
    if (engine.rootObjects().isEmpty()) {
        return 1;
    }

    return app.exec();
}
