#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtNetwork>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QVector>
#include <QMainWindow>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QTimer m_timer;
    QTimer m_timer2;
    bool stopLoop = false;
    bool stopLoop2 = false;
    int i = 0 ;
    int y = 0 ;

public:
    MainWindow(QWidget *parent = nullptr);
    void envoyerATous(const QString &message);
    ~MainWindow();

private slots:
    void nouvelleConnexion();
    void donneesRecues();
    void deconnexionClient();
    void parsing(QString message);
    void sendString(QString message);

    QVector<QString> getListLocalIp();

    void plot() ;
    void plot_humidity();
    void addPoint(double x, double y) ;
    void addPoint2(double x, double y);

    void start();
    void start2();
    void acquisitionTemperature();
    void acquisitionHumidity();

    void on_btn_Stop_clicked();

    void on_btn_Clear_clicked();


    void on_btn_Stop_2_clicked();

    void on_btn_Clear_2_clicked();
    void readFile(QString fichier);

private:
    QComboBox *listeAdresse ;
    QLabel *ip;
    QLabel *StateServer;
    QPushButton *boutonQuitter;
    bool mtn = false ;
    QTcpServer *serveur;
    QList<QTcpSocket *> clients;
    quint16 tailleMessage;
    Ui::MainWindow *ui;

    QPushButton *btn_Lancer ;
    QPushButton *btn_Stop ;
    QPushButton *btn_Clear ;

    QPushButton *btn_Lancer_2 ;
    QPushButton *btn_Stop_2 ;
    QPushButton *btn_Clear_2 ;

    QVector<double> qv_x, qv_y, qv_x_2, qv_y_2;

    double value_temperature ;
    double value_humidity ;
};

#endif // MAINWINDOW_H

