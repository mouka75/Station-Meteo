#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <list>
#include <iostream>
#include <string>
#include <QVector>
#include <QHostAddress>
#include <QThread>
#include <QTime>
#include <QElapsedTimer>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph() ;
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot_humidity->addGraph() ;
    ui->plot_humidity->graph(0)->setPen(QPen(Qt::red));

    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssPlus) ;
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine) ;
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot->xAxis->setDateTimeSpec(Qt::UTC);
    ui->plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot->xAxis->setDateTimeFormat("mm:ss");
    ui->plot->yAxis->setLabel("Température C°");
    ui->plot->xAxis->setLabel("Temps");


    ui->plot_humidity->graph(0)->setScatterStyle(QCPScatterStyle::ssPeace) ;
    ui->plot_humidity->graph(0)->setLineStyle(QCPGraph::lsLine) ;
    ui->plot_humidity->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot_humidity->xAxis->setDateTimeSpec(Qt::UTC);
    ui->plot_humidity->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot_humidity->xAxis->setDateTimeFormat("mm:ss");
    ui->plot_humidity->yAxis->setLabel("Humidité g/m3");
    ui->plot_humidity->xAxis->setLabel("Temps");


    for (const QString &address: getListLocalIp()) {
        ui->listeAdresse->addItem(address);
    }

    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 50885)) {
        // Start the server on all available IPs and on port 50585
        // If the server has not been started correctly
        qDebug() << "Le serveur n'a pas pu etre demarre. Raison :" + serveur->errorString();
        ui->label_state_server->setStyleSheet("QLabel { background-color : red; color : white; }");
        ui->label_state_server->setText("   Stop") ;
    }
    else
    {
        // If the server has been started correctly
        qDebug() << "Le serveur a ete demarre sur le port " + QString::number(serveur->serverPort());
        ui->label_port->setText("Port : " + QString::number(serveur->serverPort())) ;
        ui->label_state_server->setStyleSheet("QLabel { background-color : green; color : white; }");
        ui->label_state_server->setText("   Running") ;
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }

    ui->btn_Lancer->setText("Lancer") ;
    ui->btn_Stop->setText("Stop") ;

    ui->btn_Lancer_2->setText("Lancer") ;
    ui->btn_Stop_2->setText("Stop") ;

    connect(ui->btn_Lancer,SIGNAL(clicked(bool)),this,SLOT( start() ));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT( acquisitionTemperature() ));

    connect(ui->btn_Lancer_2,SIGNAL(clicked(bool)),this,SLOT( start2() ));
    connect(&m_timer2,SIGNAL(timeout()),this,SLOT( acquisitionHumidity() ));

    tailleMessage = 0;
}


void MainWindow::sendString(QString message){
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));
    for (int i = 0; i < clients.size(); i++){
        clients[i]->write(paquet);
    }
}

void MainWindow::start(){
     ui->btn_Lancer->setEnabled(false);
     if(m_timer.isActive()){
         m_timer.disconnect();
     }
     m_timer.start(1000);
     sendString("9999");
}

void MainWindow::start2(){
     ui->btn_Lancer_2->setEnabled(false);
     if(m_timer2.isActive()){
         m_timer2.disconnect();
     }
     m_timer2.start(1000);
     sendString("9999");
}

void MainWindow::acquisitionTemperature(){
    i++;
    addPoint(i, value_temperature);
    plot();
    if (stopLoop){
        m_timer.stop();
        i = 0 ;
    }
}

void MainWindow::acquisitionHumidity(){
    y++;
    addPoint2(y, value_humidity);
    plot_humidity();
    if (stopLoop2){
        m_timer2.stop();
        y = 0 ;
    }
}

QVector<QString> MainWindow::getListLocalIp(){
    QVector<QString> ips;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            ips.push_back(address.toString()) ;
        }
    }
    return ips ;
}

void MainWindow::plot(){
    ui->plot->graph(0)->setData(qv_x,qv_y);
    ui->plot->axisRect()->axis(QCPAxis::atLeft)->rescale();
    ui->plot->axisRect()->axis(QCPAxis::atRight)->rescale();
    ui->plot->axisRect()->axis(QCPAxis::atBottom)->rescale();
    ui->plot->axisRect()->axis(QCPAxis::atTop)->rescale();
    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::plot_humidity(){
    ui->plot_humidity->graph(0)->setData(qv_x_2,qv_y_2);
    ui->plot_humidity->axisRect()->axis(QCPAxis::atLeft)->rescale();
    ui->plot_humidity->axisRect()->axis(QCPAxis::atRight)->rescale();
    ui->plot_humidity->axisRect()->axis(QCPAxis::atBottom)->rescale();
    ui->plot_humidity->axisRect()->axis(QCPAxis::atTop)->rescale();
    ui->plot_humidity->replot();
    ui->plot_humidity->update();
}

void MainWindow::addPoint(double x, double y){
    qv_x.append(x);
    qv_y.append(y);
}

void MainWindow::addPoint2(double x, double y){
    qv_x_2.append(x);
    qv_y_2.append(y);
}

void MainWindow::nouvelleConnexion(){
    envoyerATous("Un client vient de se connecter");
    qDebug() << "Connection";
    QTcpSocket *nouveauClient = serveur->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
}

void MainWindow::donneesRecues()
{
    // We receive a packet from one of the clients
    // We determine which client sends the message (search for the client's QTcpSocket)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // If by chance the client at the origin of the signal is not found, we stop the method
        return;
    // If by chance the client at the origin of the signal is not found, we stop the method
    QDataStream in(socket);

    if (tailleMessage == 0) // If we don't know the size of the message yet, we try to get it
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))  // We didn't receive the full size of the message
            return;

        in >> tailleMessage; // If we have received the entire size of the message, we retrieve it
    }

    // If we know the size of the message, we check if we have received the whole message
    if (socket->bytesAvailable() < tailleMessage) // If we haven't received everything yet, we stop the method
        return;


    // If these lines are executed, it means that we have received the whole message, we can recover it
    QString message;
    in >> message;
    envoyerATous(message);

    // reset the message size to 0 to allow the reception of future messages
    tailleMessage = 0;
}

void MainWindow::deconnexionClient()
{
    envoyerATous("Le client vient de se déconnecter");
    // We determine which client disconnects
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // If by chance we haven't found the client at the origin of the signal, we stop the method
        return;
    clients.removeOne(socket);
    socket->deleteLater();
}

void MainWindow::envoyerATous(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out << (quint16) 0; // We write 0 at the beginning of the package to reserve the place to write the size
    out << message; // We add the message after
    out.device()->seek(0); // We go back to the beginning of the package
    out << (quint16) (paquet.size() - sizeof(quint16)); // We overwrite the 0 we had reserved by the length of the message
    parsing(message);

    // Send the prepared packet to all clients connected to the server
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }
}

void MainWindow::parsing(QString message){
    QRegExp rx(";");
    QStringList list = message.split(rx);

    if(list.size() == 2){
        value_temperature = list.at(0).toDouble();
        value_humidity = list.at(1).toDouble();
    }
    qDebug() << value_temperature;
    qDebug() << value_humidity;
}


void MainWindow::on_btn_Stop_clicked(){
    if(stopLoop){
            ui->btn_Lancer->setEnabled(false);
            stopLoop = false;
            QString message_stop = "1111" ;
            QByteArray paquet;
            QDataStream out(&paquet, QIODevice::WriteOnly);
            out << (quint16) 0;
            out << message_stop;
            out.device()->seek(0);
            out << (quint16) (paquet.size() - sizeof(quint16));
            for (int i = 0; i < clients.size(); i++){
                 clients[i]->write(paquet);
            }
            }else {
            stopLoop = true ;
            }
}

void MainWindow::on_btn_Stop_2_clicked()
{
    if(stopLoop2){
            ui->btn_Lancer_2->setEnabled(false);
            stopLoop2 = false;
    }else {
            stopLoop2 = true ;
     }
}

void MainWindow::on_btn_Clear_clicked(){
    i = 0 ;
    qv_x.clear();
    qv_y.clear();
    ui->plot->replot();
    ui->btn_Lancer->setEnabled(true);
}

void MainWindow::on_btn_Clear_2_clicked(){
    y = 0 ;
    qv_x_2.clear();
    qv_y_2.clear();
    plot_humidity();
    ui->btn_Lancer_2->setEnabled(true);
    m_timer2.stop() ;
    stopLoop2 = true ;
}

void MainWindow::readFile(QString fichier){
    QFile file(fichier);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        qDebug() << " Could not open file for writing:" << file.errorString();
        return;
    }
    QTextStream instream(&file);
    QString line = instream.readLine();
    qDebug() << "first line : " << line;
    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


