#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qfiledialog.h"
#include "qsettings.h"
#include "qtimer.h"
#include "qfont.h"
#include "qpalette.h"
#include "qcolor.h"
#include "qdatetime.h"
#include "QKeyEvent"
#include "jlink_tool.h"
#include "qmessagebox.h"
#include "data_base.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

signals:
    void req_jlink_tool(int,QString);
public slots:
    void handle_jlink_tool_rsp(int,int,QString);
    void stop_scanner_timer(void);

private slots:
    void on_scan_tool_button_clicked();

    void on_read_sn_button_clicked();

    void on_write_sn_button_clicked();

    void on_manual_exe_button_clicked();

    void on_bootloader_file_open_button_clicked();

    void on_app_file_open_button_clicked();

    void on_sn_edit_check_stateChanged(int arg1);

    void on_sn_display_textChanged(const QString &arg1);

    void on_log_display_textChanged();

    void on_all_on_top_check_box_stateChanged(int arg1);

    void on_actionabout_triggered();

    void on_query_today_button_clicked();

    void on_query_day_button_clicked();

private:
    Ui::MainWindow *ui;
    QString cur_time();
    data_base *record_data_base;
    QSettings *setting;
    QFileDialog *file_dialog;
    QString *bootloader_addr;
    QString *application_addr;
    QTimer *scanner_timer;
    bool is_scanner_input;

    bool busy;
    bool clear;
    int success_cnt;
    int fail_cnt;
};

#endif // MAINWINDOW_H
