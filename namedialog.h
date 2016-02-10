#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(QWidget *parent = 0);
    ~NameDialog();
    QString getName() const;
    QString getSurname() const;
    QString getDestination() const;

private slots:
    void on_pushButtonConfirm_released();

private:
    Ui::NameDialog *ui;
};

#endif // NAMEDIALOG_H
