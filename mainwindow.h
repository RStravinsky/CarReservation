#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QtSql/QSqlQueryModel>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QScrollBar>
#include <QMovie>
#include <vector>
#include <memory>
#include "carblock.h"
#include "database.h"
#include "helpdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setVisible(bool visible) Q_DECL_OVERRIDE;
    void showTrayIcon();
    void loadTrayIcon();

public slots:
    void updateView(bool isCopyEnable);
    void reloadNotes();

private slots:
    void onTimerOverflow();
    void setIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void noteActionClicked(QAction *act);
    void poupMessageClicked();
    void setPopupMessage();

signals:
    void trayMenuNoteClicked(int _idNotes, int _idCar);

private:
    Ui::MainWindow *ui;
    QVBoxLayout *scrollLayout{nullptr};
    QWidget *scrollWidget{nullptr};
    Database * database;
    QSqlQueryModel * carTable{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    QSqlQueryModel * notesTable{nullptr};
    QTimer *timer{nullptr};
    std::vector<CarBlock*> carBlockVector{nullptr};
    QString login;
    QString password;
    bool copyEnable{false};
    QPushButton * backupButton{nullptr};

    QSystemTrayIcon * trayIcon{nullptr};
    QAction *minimizeAction{nullptr};
    QAction *maximizeAction{nullptr};
    QAction *restoreAction{nullptr};
    QAction *quitAction{nullptr};
    QAction *dcAction{nullptr};
    QMenu *trayIconMenu{nullptr};
    QMenu * notesMenu{nullptr};
    int newMessagesNumber;
    std::vector<QAction*> notesActionsVector;
    int lastRowCount;

    void createActions(bool _isAdmin);
    void createTrayIcon(bool _isAdmin);
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    void createLoginOption();
    void createUpdateButton();
    void createHelpButton();
    void createBackupButton();

    void createBackup();
    void setBackupButtonVisible();
};

#endif // MAINWINDOW_H
