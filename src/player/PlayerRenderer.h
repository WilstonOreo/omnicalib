
#ifndef OMNI_PLAYERRENDERER_H_
#define OMNI_PLAYERRENDERER_H_

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

namespace omni {
  class PlayerRenderer : public QObject, protected QOpenGLFunctions {
    Q_OBJECT

    public:
      PlayerRenderer() : m_t(0), m_program(0) {}

      ~PlayerRenderer();

      void setT(qreal t) {
        m_t = t;
      }

      void setViewportSize(const QSize& size) {
        m_viewportSize = size;
      }

      void setWindow(QQuickWindow *window) {
        m_window = window;
      }

    public slots:
      void paint();

    private:
      QSize m_viewportSize;
      qreal m_t;
      QOpenGLShaderProgram *m_program;
      QQuickWindow *m_window;
  };
}

#endif /* OMNI_PLAYERRENDERER_H_ */
