#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QRegExp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->checks_file_button, SIGNAL(clicked()), SLOT(OpenChecksFile()));
  connect(ui->report_file_button, SIGNAL(clicked()), SLOT(OpenReportsFile()));
  connect(ui->trim_button, SIGNAL(clicked()), SLOT(TrimUnusedChecks()));
}

MainWindow::~MainWindow() { delete ui; }

QString MainWindow::GetFileContents() {
  auto file_name = QFileDialog::getOpenFileName(
      this, tr("Open Image"),
      "/home/jenkins/Repos/OtherProjects/ClangTidyCheckRemover/delette_this",
      tr("Any Files (*)"));

  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";

  QString resulting_string;
  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    resulting_string += line;
  }
  return resulting_string;
}

void MainWindow::OpenChecksFile() { checks_file_contents = GetFileContents(); }

void MainWindow::OpenReportsFile() {
  reports_file_contents = GetFileContents();
}

void MainWindow::TrimUnusedChecks() {
  if (checks_file_contents.isEmpty() || reports_file_contents.isEmpty()) {
    return;
  }
  int pos = 0;

  QStringList list;
  QRegExp regexp("\\[[^\n,\\]]+,\\-warnings-as-errors\\]");

  regexp.setMinimal(true);
  while ((pos = regexp.indexIn(reports_file_contents, pos)) != -1) {
    list << regexp.cap(0);
    pos += regexp.matchedLength();
  }

  list = list.toSet().toList();
  QStringList new_list;
  for (auto element : list) {
    QString trimmed_element = element.remove(",-warnings-as-errors]");
    trimmed_element = trimmed_element.remove("[");
    new_list << trimmed_element;

    QString joined_element = "," + trimmed_element + ",";
    checks_file_contents.remove(joined_element);
  }
  while (checks_file_contents.contains("\n    \n")) {
    checks_file_contents.replace("\n    \n", "\n");
  }

  //  ui->result_text->setPlainText(checks_file_contents);
  ui->result_text->setPlainText(new_list.join("\n"));
}
