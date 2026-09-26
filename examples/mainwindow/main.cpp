// Copyright (C) 2023-present Stdware Collections (https://www.github.com/stdware)
// Copyright (C) 2021-2023 wangwenx190 (Yuhang Zhao)
// SPDX-License-Identifier: Apache-2.0

#include <QtWidgets/QApplication>

#include "mainwindow.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/QWidget>
#include <QStyleFactory>
#include <QStyle>

class BreezeWithCustomMenuWidgetStyle : public QProxyStyle {
public:
    using QProxyStyle::QProxyStyle;

    void polish(QWidget *widget) override {
        if (auto *mainWindow = qobject_cast<QMainWindow *>(widget)) {
            if (mainWindow == mainWindow->window()) {
                QWidget *menuWidget = mainWindow->menuWidget();
                if (menuWidget && !qobject_cast<QMenuBar *>(menuWidget)) {
                    // WA_StyledBackground is the only QMainWindow-specific effect
                    // of Breeze::Style::polish(); replicate it without registering
                    // the window with ToolsAreaManager (which would call menuBar()).
                    widget->setAttribute(Qt::WA_StyledBackground);
                    return;
                }
            }
        }
        QProxyStyle::polish(widget);
    }
};

int main(int argc, char *argv[]) {
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
    qputenv("QSG_INFO", "1");
    // qputenv("QT_WIDGETS_HIGHDPI_DOWNSCALE", "1");
    // qputenv("QT_WIDGETS_RHI", "1");
    // qputenv("QSG_RHI_BACKEND", "d3d12");
    // qputenv("QSG_RHI_HDR", "scrgb");
    // qputenv("QT_QPA_DISABLE_REDIRECTION_SURFACE", "1");

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication a(argc, argv);

    if (QStyle *breezeStyle = QStyleFactory::create(QStringLiteral("breeze"))) {
        QApplication::setStyle(new BreezeWithCustomMenuWidgetStyle(breezeStyle));
    }

#ifdef Q_OS_LINUX
    // KDE Breeze calls QMainWindow::menuBar() while polishing the window. When a custom title bar
    // is installed through QMainWindow::setMenuWidget(), that call replaces it with an empty
    // QMenuBar and schedules the custom widget for deletion. Use Fusion on Linux so the WindowBar
    // remains installed and visible without changing the native styles on other platforms.
    QApplication::setStyle(QStringLiteral("Fusion"));
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
