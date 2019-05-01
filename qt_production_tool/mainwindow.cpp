#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfont.h"
#include "qpalette.h"
#include "qcolor.h"
#include "qdatetime.h"
#include "jlink_tool.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFont font;
    QPalette color;

    ui->setupUi(this);

    /*设置SN字体*/
    font.setPointSize(34);
    font.setFamily("Arial");
    ui->sn_display->setFont(font);

    color.setColor(QPalette::Text,QColor("blue"));
    ui->sn_display->setPalette(color);

    ui->sn_display->setText("DC1999988844411234567894");
    ui->sn_process_bar->setValue(100);

    /*设置日志字体*/
    font.setPointSize(12);
    ui->log_display->setFont(font);

    /*bootloader 地址*/
    bootloader_addr = new QString("0x00000000");
    ui->bootloader_addr_display->setText(*bootloader_addr);

    /*application 地址*/
    app_addr = new QString("0x08000000");
    ui->app_addr_display->setText(*app_addr);


    /*上次路径*/
    setting = new QSettings("./Setting.ini", QSettings::IniFormat);  //QSettings能记录一些程序中的信息，下次再打开时可以读取出来

    /*文件选择对话框*/
    file_dialog = new QFileDialog(this);

    file_dialog->setWindowTitle("选择烧录文件");

    file_dialog->setDirectory(setting->value("last_file_path").toString());

    file_dialog->setNameFilter(tr("二进制文件(*.bin)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    file_dialog->setFileMode(QFileDialog::ExistingFile);
    //设置视图模式
    file_dialog->setViewMode(QFileDialog::Detail);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scan_tool_button_clicked()
{
    emit req_jlink_tool(jlink_tool::JLINK_TOOL_SCAN,0);
}

/*当前时间*/
QString MainWindow::cur_time()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("  hh:mm:ss.zzz");
}

/*处理扫描工具结果*/
void MainWindow::handle_jlink_tool_rsp(int rsp,int rc,QString str)
{
    if (rc == 0) {
        ui->log_display->setTextColor(QColor::fromRgb(0,0,0));
    } else {
        ui->log_display->setTextColor(QColor::fromRgb(255,0,0));
    }

    if (rsp == jlink_tool::JLINK_TOOL_READ_SN && rc == 0) {

        ui->log_display->append("读取SN：" + str + "成功！"+ cur_time());
        ui->sn_display->setText(str);
    } else {
        ui->log_display->append(str + cur_time());
    }


}

void MainWindow::on_read_sn_button_clicked()
{
    emit  req_jlink_tool(jlink_tool::JLINK_TOOL_READ_SN,0);
}

void MainWindow::on_write_sn_button_clicked()
{
    emit req_jlink_tool(jlink_tool::JLINK_TOOL_WRITE_SN,0);
}

void MainWindow::on_manual_exe_button_clicked()
{

}


void MainWindow::on_bootloader_file_open_button_clicked()
{

    QString path;

    if (file_dialog->exec() ==  QDialog::Accepted) {
        path = file_dialog->selectedFiles()[0];
        if (!path.isEmpty()) {
            ui->bootloader_file_path_display->setText(path);
            setting->setValue("last_file_path",path);
        }

    } else {
        file_dialog->close();
    }
}

void MainWindow::on_app_file_open_button_clicked()
{
    QString path;

    if (file_dialog->exec() ==  QDialog::Accepted) {
        path = file_dialog->selectedFiles()[0];
        if (!path.isEmpty()) {
            ui->app_file_path_display->setText(path);
            setting->setValue("last_file_path",path);
        }

    } else {
        file_dialog->close();
    }
}

void MainWindow::on_sn_edit_check_stateChanged(int arg1)
{
    if (ui->sn_edit_check->isChecked()) {
        ui->sn_display->setReadOnly(false);
    } else {
        ui->sn_display->setReadOnly(true);
    }
}

void MainWindow::on_sn_display_textChanged(const QString &arg1)
{
    emit req_jlink_tool(jlink_tool::JLINK_TOOL_OPEN_SN,ui->sn_display->text());
}

void MainWindow::on_log_display_textChanged()
{
    ui->log_display->moveCursor(QTextCursor::End);
}
