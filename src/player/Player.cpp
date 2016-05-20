
#include "Player.h"
#include "PlayerRenderer.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

namespace omni {

  Player::Player() : m_renderer(nullptr)
  {
      connect(this, &QQuickItem::windowChanged, this, &Player::handleWindowChanged);
  }

  void Player::handleWindowChanged(QQuickWindow *win)
  {
      if (win) {
          connect(win, &QQuickWindow::beforeSynchronizing, this, &Player::sync, Qt::DirectConnection);
          connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Player::cleanup, Qt::DirectConnection);
          // If we allow QML to do the clearing, they would clear what we paint
          // and nothing would show.
          win->setClearBeforeRendering(false);
      }
  }

  void Player::cleanup()
  {
      if (m_renderer) {
          delete m_renderer;
          m_renderer = 0;
      }
  }

  void Player::sync()
  {
      if (!m_renderer) {
          m_renderer = new PlayerRenderer();
          connect(window(), &QQuickWindow::beforeRendering, m_renderer, &PlayerRenderer::paint, Qt::DirectConnection);
      }
      m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
      m_renderer->setWindow(window());
  }

}
