#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qfiledialog.h"
#include "qsettings.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void req_scan_tool(void);
    void req_show_file_dialog(void);
public slots:
    void handle_jlink_tool_rsp(int,QString);

private slots:
    void on_scan_tool_button_clicked();

    void on_read_sn_button_clicked();

    void on_write_sn_button_clicked();

    void on_manual_exe_button_clicked();

    void on_bootloader_file_open_button_clicked();

    void on_app_file_open_button_clicked();

private:
    Ui::MainWindow *ui;
    QString cur_time();
    QSettings *setting;
    QFileDialog *file_dialog;
    QString *bootloader_addr;
    QString *app_addr;

};

#endif // MAINWINDOW_H
