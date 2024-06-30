#include "MainWindow.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->connectButton->setCheckable(true);
    _sock = new QTcpSocket(this);

    connect(_sock, &QTcpSocket::disconnected, this, [&]() {
        ui->connectButton->toggle();
        ui->connectButton->click();
    });
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->ipEdit->setValidator(new QRegExpValidator(rx));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked(bool checked)
{
    if (checked == true)
    {
        _sock->connectToHost(ui->ipEdit->text(), ui->portEdit->text().toInt());
        connect(_sock, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
        QJsonObject json;
        json["username"] = ui->usernameEdit->text();
        QJsonDocument doc(json);

        _sock->write(doc.toJson());

        ui->usernameEdit->setEnabled(false);

        ui->connectButton->setText("Отключиться");

        ui->requestGroup->setEnabled(true);
    }
    else
    {
        _sock->disconnectFromHost();
        ui->connectButton->setText("Подключиться");
        ui->usernameEdit->setEnabled(true);
        ui->requestGroup->setEnabled(false);
    }
}

void MainWindow::on_requestResources_clicked()
{
    QRandomGenerator gen;
    uint32_t         req    = 0;
    uint8_t          arr[4] = { 0 };
    if (ui->resource1->isChecked())
    {
        arr[0] = static_cast<uint8_t>(gen.generate() % 256);
    }
    if (ui->resource2->isChecked())
    {
        arr[1] = static_cast<uint8_t>(gen.generate() % 256);
    }
    if (ui->resource3->isChecked())
    {
        arr[2] = static_cast<uint8_t>(gen.generate() % 256);
    }
    if (ui->resource4->isChecked())
    {
        arr[3] = static_cast<uint8_t>(gen.generate() % 256);
    }
    memcpy(&req, arr, sizeof(req));
    QJsonObject json;
    json["username"] = ui->usernameEdit->text();
    json["time"]     = QTime::currentTime().msecsSinceStartOfDay() / 1000;
    json["request"]  = ui->resourcesGroup->isEnabled() ? static_cast<int>(req) : ui->lineEdit->text().toInt();
    QJsonDocument doc(json);
    _sock->write(doc.toJson());
}

void MainWindow::onReadyRead()
{
    QByteArray data = _sock->readAll();
    ui->responsesBox->append(data);
}

void MainWindow::on_lineEdit_textChanged(const QString& arg1)
{
    if (!arg1.isEmpty())
    {
        ui->resourcesGroup->setEnabled(false);
    }
    else
    {
        ui->resourcesGroup->setEnabled(true);
    }
}
