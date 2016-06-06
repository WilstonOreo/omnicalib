
#include <QDebug>
#include <QApplication>

#include <fstream>

#include <omnic/qt/Output.h>

int main(int argc, char* argv[]) {

  // This line is absolutely mandatory for being able to have multiple
  // QOpenGLWidgets in different windows!!!
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
/*
  QSurfaceFormat _format;

  _format.setProfile(QSurfaceFormat::CompatibilityProfile);
#ifdef DEBUG
  _format.setOption(QSurfaceFormat::DebugContext);
#endif
  QSurfaceFormat::setDefaultFormat(_format);
*/
  QApplication _a(argc, argv);

  // Get filename of calibration from command line arguments
  QStringList&& _args = QCoreApplication::arguments();

  QString _filename;
  if (_args.size() < 2) {
    qDebug() << "Usage: ./bin/Debug/sampleApp calibration.omnic";
    return EXIT_FAILURE;
  }

  for (int i = 1; i < _args.size(); ++i) {
    _filename += " "+_args[i] ;
  }

  _filename = _filename.trimmed();

  omnic::Calibration _calibration;
  
  /// Load calibration from file
  std::ifstream _if(_filename.toStdString());
  _calibration.load(_if);
  _if.close();

  omnic::qt::Output _output;
  _output.setCalibration(_calibration);

  return _a.exec();
}
