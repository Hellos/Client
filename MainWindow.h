#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QRandomGenerator64>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:

    void on_connectButton_clicked(bool checked);
    void on_requestResources_clicked();
    void onReadyRead();

    void on_lineEdit_textChanged(const QString& arg1);

private:
    Ui::MainWindow* ui;
    QTcpSocket*     _sock;
};
#endif // MAINWINDOW_H
