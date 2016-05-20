

#ifndef OMNI_PLAYER_H_
#define OMNI_PLAYER_H_

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

namespace omni {
  class PlayerRenderer;

  class Player : public QQuickItem {
    Q_OBJECT
    //Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

    public:
      Player();

    public slots:
      void sync();
      void cleanup();

    private slots:
      void handleWindowChanged(QQuickWindow *win);

    private:
      PlayerRenderer *m_renderer;
  };
}


#endif // OMNI_PLAYER_H_
