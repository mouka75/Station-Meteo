#include "client.h"

#include <iostream> 
#include <fstream>  
#include <string>
#include "RTIMULib.h"
#include <QFile>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

client::client()
{
    messageRTDM();
    QString ipAdresse = "169.254.100.12" ;
    int portQ = 50885 ;
    string msg ;
    
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    qDebug() << "Tentative de connexion en cours...";
    socket->abort(); // We disable the previous connections if there are any
    socket->connectToHost(ipAdresse, portQ); // We connect to the requested server 
    timerId = startTimer(1000);
    tailleMessage = 0;
}

void client::getTemperature()
{
    //  using RTIMULib here allows it to use the .ini file generated by RTIMULibDemo.
    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");
    RTIMU *imu = RTIMU::createIMU(settings);
    RTPressure *pressure = RTPressure::createPressure(settings);
    RTHumidity *humidity = RTHumidity::createHumidity(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL))
    {
        printf("No IMU found\n");
        exit(1);
    }
    //  This is an opportunity to manually override any settings before the call IMUInit
    //  set up IMU
    imu->IMUInit();
    //  this is a convenient place to change fusion parameters
    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);

    //  set up pressure sensor
    if (pressure != NULL)
        pressure->pressureInit();
    //  set up humidity sensor
    if (humidity != NULL)
        humidity->humidityInit();
    //  now just process data
    if(imu->IMURead())
    {
        RTIMU_DATA imuData = imu->getIMUData();
        //  add the pressure data to the structure
        if (pressure != NULL)
            pressure->pressureRead(imuData);
            //  add the humidity data to the structure
        if (humidity != NULL)
            humidity->humidityRead(imuData);
                if(pressure != NULL)
                {
                    value_temperature = imuData.temperature ; 
                }
                if(humidity != NULL)
                {
                    value_humidity = imuData.humidity;
                }
                printf("\n");
                fflush(stdout);
        }
    
}

void client::timerEvent(QTimerEvent *event){
    if (lancerB){
        getTemperature();
        compteur++;
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);
        QString messageAEnvoyer = QString::number(value_temperature) + ";" + QString::number(value_humidity);
        out << (quint16)0;
        out << messageAEnvoyer;
        out.device()->seek(0);
        out << (quint16)(paquet.size() - sizeof(quint16));
        socket->write(paquet);
        qDebug() << compteur;
    }
    if (stopB){
        deconnecte();
    }
}

void client::deconnecte(){
        qDebug() << "Déconnecté du serveur";
        killTimer(timerId);
        connecter = false;
}

// This slot is called when there is an error
void client::erreurSocket(QAbstractSocket::SocketError erreur){
    switch(erreur) // A different message is displayed depending on the error indicated
    {
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "ERREUR : le serveur n'a pas pu étre trouvé. Vérifiez l'IP et le port.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "ERREUR : le serveur a refuse la connexion. Vérifiez si le programme serveur a bien été lancé. Vérifiez aussi l'IP et le port.";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "ERREUR : le serveur a coupé la connexion.";
        break;
    default:
        qDebug() << "<em>ERREUR : " + socket->errorString() ;

    }
}

// This slot is called when the connection to the server is successful
void client::connecte(){
    qDebug() << "Connexion réussie !" ;
    connecter = true ;
}

// We received a package 
void client::donneesRecues()
{
    /* 
    Same principle as when the server receives a packet:
    We try to recover the size of the message
    Once we have it, we wait until we have received the whole message 
    (based on the announced message size sizeMessage)
    */
    QDataStream in(socket);
    if (tailleMessage == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> tailleMessage;
    }
    if (socket->bytesAvailable() < tailleMessage)
        return;
    //If we get to this line, we can retrieve the whole message
    QString messageRecu;
    in >> messageRecu;

    qDebug() << "BOOL : " + messageRecu;

    if(messageRecu == "9999"){
        qDebug() << "LANCER B";
        lancerB = true;
    }
    if (messageRecu == "1111"){
        qDebug() << "Stop B";
        stopB = true;
    }

    qDebug()<< "Message recu : " + messageRecu;
    // We reset the message size to 0 to be able to receive future messages
    tailleMessage = 0;
}

void client::start(){
    stopB = true;
    m_timer.start(1000);
    compteur++ ;
}

