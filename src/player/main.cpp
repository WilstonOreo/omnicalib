
#include <QGuiApplication>

#include <QtQuick/QQuickView>

#include "Player.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<omni::Player>("OmniPlayer", 1, 0, "OmniPlayer");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.setFlags(Qt::WindowFullscreenButtonHint);
    view.show();

    return app.exec();
}
