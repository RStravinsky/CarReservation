#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QtSql/QSqlQueryModel>
#include <QSystemTrayIcon>
#include <vector>
#include "carblock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadSqlModel();
    void setVisible(bool visible) Q_DECL_OVERRIDE;
    void showTrayIcon();

public slots:
    void updateView();
    void setIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;
    QVBoxLayout *scrollLayout{nullptr};
    QWidget *scrollWidget{nullptr};
    QSqlDatabase sqlDatabase;
    QSqlQueryModel * carTable{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    std::vector<CarBlock*> carBlockVector;
    QString login;
    QString password;

    bool connectToDatabase(QString &login, QString &password);
    void closeDatabase();


    QSystemTrayIcon * trayIcon;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *dcAction;
    QMenu *trayIconMenu;

    void createActions();
    void createTrayIcon();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showMessage();
    void setPopupMessage();
    void loadTrayIcon();
};

#endif // MAINWINDOW_H
