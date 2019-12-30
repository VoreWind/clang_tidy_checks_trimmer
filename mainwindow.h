#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  QString GetFileContents();
 public slots:
  void OpenChecksFile();
  void OpenReportsFile();
  void TrimUnusedChecks();

 private:
  Ui::MainWindow* ui;
  QString checks_file_contents;
  QString reports_file_contents;
};
#endif  // MAINWINDOW_H
