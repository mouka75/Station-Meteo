#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QTimer>

using namespace std;
class client : public QObject{
    Q_OBJECT
    QTimer m_timer;
    int compteur = 0 ; 
public:
    client();

private slots:
    void donneesRecues();
    void connecte();
    void deconnecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void start();
    void getTemperature();

private:
    
    int timerId;
    int timerRTDM_message;
    QTcpSocket *socket; // Represents the server
    quint16 tailleMessage;
    int i = 0;
    bool connecter = false;
    bool lancerB = false;
    bool stopB = false;
    float value_temperature;
    float value_humidity;

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // CLIENT_H




