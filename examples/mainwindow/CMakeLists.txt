project(QWKExample_MainWindow)

file(GLOB _src *.h *.cpp)

find_package(KF6 REQUIRED COMPONENTS WidgetsAddons)
find_package(KF6 REQUIRED COMPONENTS XmlGui)

qwk_add_example(${PROJECT_NAME}
    FEATURES cxx_std_17
    SOURCES ${_src} mainwindow.ui mainwindow.qrc ../shared/resources/shared.qrc
    QT_LINKS Core Gui Widgets # MultimediaWidgets
    LINKS QWKWidgets WidgetFrame KF6::WidgetsAddons KF6::XmlGui
)