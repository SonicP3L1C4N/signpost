// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    // Before the QApplication, not after: the platform registers the
    // application with the portal while it is being constructed, so a name set
    // afterwards is a name nobody asked for. Claimed only when the .desktop
    // file is actually installed -- an id the portal cannot resolve is a
    // warning on every start.
    //
    // It has to be told to KAboutData as well, further down. KAboutData
    // carries a desktop file name of its own, defaulting to org.kde.<name>,
    // and setApplicationData writes it over whatever was set here -- which is
    // why this application spent a while calling itself org.kde.signpost to
    // the portal and to the window manager.
    const QString desktopId = QStringLiteral("io.github.sonicp3l1c4n.signpost");
    if (!QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                QStringLiteral("applications/") + desktopId
                                    + QStringLiteral(".desktop"))
             .isEmpty()) {
        QGuiApplication::setDesktopFileName(desktopId);
    }

    // QApplication rather than QGuiApplication: KIO's launcher job puts up
    // widget dialogs when a .desktop file wants a terminal or asks a question.
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("signpost"));
    // The installed icon when there is one; the copy compiled in otherwise,
    // so running out of the build directory still looks like the application.
    QIcon icon = QIcon::fromTheme(desktopId);
    if (icon.isNull()) {
        icon = QIcon(QStringLiteral(":/sc-apps-io.github.sonicp3l1c4n.signpost.svg"));
    }
    QApplication::setWindowIcon(icon);
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
    about.setDesktopFileName(desktopId);
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
