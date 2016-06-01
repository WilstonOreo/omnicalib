
#ifndef OMNIC_QT_WRAPPER_H_
#define OMNIC_QT_WRAPPER_H_

#include <QRect>
#include <omnic/Rect.h>

namespace omnic {

  namespace qt {
    /// Convert OMNIC rectangle to Qt rectangle
    static inline void wrap(omnic::Rect const& _in, QRect& _out) {
      _out = QRect(QPoint(_in.offsetX(),_in.offsetY()),QSize(_in.width(),_in.height()));
    }
    
    /// Convert Qt rectangle to OMNIC rectangle
    static inline void wrap(QRect const& _in, omnic::Rect& _out) {  
      _out = omnic::Rect(_in.left(),_in.top(),_in.width(),_in.height());
    }
  }
}


#endif /* OMNIC_QT_WRAPPER_H_ */

