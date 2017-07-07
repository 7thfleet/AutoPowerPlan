#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateStatus(void);

private slots:
    void onACComboBoxSelected(const QString& text);
    void onBatteryComboBoxSelected(const QString& text);
    void onSpinBoxValueChange(int i);
    void exitClicked(QAction *action);
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void closeEvent(QCloseEvent *event);


private:
    void writeToSettings(QString settingName, QString settingValue);
    void writeToSettings(QString settingName, int settingValue);

    Ui::MainWindow *ui;
};





#endif // MAINWINDOW_H
