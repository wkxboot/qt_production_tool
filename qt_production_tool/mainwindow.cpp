#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFont font;
    QPalette color;

    ui->setupUi(this);

    /*设置SN字体*/
    font.setPointSize(30);
    font.setFamily("Arial");
    ui->sn_display->setFont(font);

    color.setColor(QPalette::Text,QColor("blue"));
    ui->sn_display->setPalette(color);

    /*设置日志字体*/
    font.setPointSize(10);
    ui->log_display->setFont(font);

    /*bootloader 地址*/
    bootloader_addr = new QString("0x" + QString::number(jlink_tool::JLINK_TOOL_BOOTLOADER_ADDR,16));
    ui->bootloader_addr_display->setText(*bootloader_addr);

    /*application 地址*/
    application_addr = new QString("0x" + QString::number(jlink_tool::JLINK_TOOL_APPLICATION_ADDR,16));
    ui->app_addr_display->setText(*application_addr);


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


    /*扫描枪定时器 取来区别键盘*/
    scanner_timer = new QTimer(this);
    scanner_timer->setInterval(50);
    busy = false;
    clear = false;
    success_cnt = 0;
    is_scanner_input = false;
    fail_cnt = 0;

    QObject::connect(scanner_timer,SIGNAL(timeout()),this,SLOT(stop_scanner_timer()));
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
    if (rc >= 0) {
        ui->log_display->setTextColor(QColor::fromRgb(0,0,0));
    } else {
        ui->log_display->setTextColor(QColor::fromRgb(255,0,0));
    }

    /*处理SN*/
    if (rsp == jlink_tool::JLINK_TOOL_READ_SN && rc == 0) {
        ui->sn_display->setText(str);
        ui->log_display->append("读取SN：" + str + "成功！" + cur_time());
    } else if (rsp == jlink_tool::JLINK_TOOL_WRITE_SN && rc == 100) {
        ui->log_display->append(str + cur_time());
        ui->sn_process_bar->setValue(100);

   /*处理bootloader版本号*/
    } else if (rsp == jlink_tool::JLINK_TOOL_OPEN_BOOTLOADER) {
        if (rc == 0) {
            ui->bootloader_version_display->setText(str);
            ui->log_display->append("获取bootloader版本号：" + str + "成功！");
        } else {
            ui->bootloader_version_display->setText("");
            ui->log_display->append("获取bootloader版本号失败！");
        }

   /*处理application版本号*/
    }else if (rsp == jlink_tool::JLINK_TOOL_OPEN_APPLICATION) {
        if (rc == 0) {
            ui->app_version_display->setText(str);
            ui->log_display->append("获取application版本号：" + str + "成功！");
        } else {
            ui->app_version_display->setText("");
            ui->log_display->append("获取application版本号失败！");
        }

    /*处理bootloader进度*/
    }else if (rsp == jlink_tool::JLINK_TOOL_WRITE_BOOTLOADER && rc >= 0) {
        ui->bootloader_process_bar->setValue(rc);

    /*处理application进度*/
    } else if (rsp == jlink_tool::JLINK_TOOL_WRITE_APPLICATION && rc >= 0) {
         ui->application_process_bar->setValue(rc);

    /*处理烧录结果*/
    }else if (rsp == jlink_tool::JLINK_TOOL_AUTO_EXECUTE || rsp == jlink_tool::JLINK_TOOL_MANUAL_EXECUTE ){
        if (rc == 0) {
            success_cnt += 1;
            ui->success_cnt_display->setNum(success_cnt);
            ui->log_display->setTextColor(QColor::fromRgb(0,0,255));
        } else if (rc == - 1) {
            fail_cnt += 1;
            ui->fail_cnt_display->setNum(fail_cnt);
        }
        ui->log_display->append(str + cur_time());

    /*处理烧写冲突*/
    } else if (rsp == jlink_tool::JLINK_TOOL_BUSY_STATUS) {
        if (rc == 1) {
            busy = true;
        } else {
            busy = false;
        }
    /*处理其他*/
    } else {

        ui->log_display->append(str + cur_time());
    }


}

void MainWindow::stop_scanner_timer()
{
    scanner_timer->stop();
    is_scanner_input = false;
}

/*扫码枪处理扫描处理*/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    scanner_timer->start();
    is_scanner_input = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    scanner_timer->stop();

    if (!is_scanner_input) {
        qDebug("键盘输入，忽略！");
        return;
    }


    if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9 ) ||\
        (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z)) {
        if (clear) {
            clear = false;
            ui->sn_display->clear();
        }
        ui->sn_display->insert((QString)event->key());
    }
    /*回车 扫描完毕*/
    if (event->key() == Qt::Key_Return) {
        if (busy) {
           QMessageBox::warning(this,"错误","上个烧写/读写正在进行，请等待...",QMessageBox::Ok);
           return;
        }
        /*开始自动写程序*/
        clear = true;
        ui->sn_process_bar->setValue(0);
        ui->bootloader_process_bar->setValue(0);
        ui->application_process_bar->setValue(0);
        emit req_jlink_tool(jlink_tool::JLINK_TOOL_AUTO_EXECUTE,0);
    }
}
void MainWindow::on_read_sn_button_clicked()
{
    if (busy) {
       QMessageBox::warning(this,"错误","上个烧写/读写正在进行，请等待...",QMessageBox::Ok);
       return;
    }
    emit  req_jlink_tool(jlink_tool::JLINK_TOOL_READ_SN,0);
}

void MainWindow::on_write_sn_button_clicked()
{
    if (busy) {
       QMessageBox::warning(this,"错误","上个烧写/读写正在进行，请等待...",QMessageBox::Ok);
       return;
    }
    ui->sn_process_bar->setValue(0);

    emit req_jlink_tool(jlink_tool::JLINK_TOOL_WRITE_SN,0);
}

void MainWindow::on_manual_exe_button_clicked()
{
    if (busy) {
       QMessageBox::warning(this,"错误","上个烧写/读写正在进行，请等待...",QMessageBox::Ok);
       return;
    }
    /*开始手动写程序*/
    ui->sn_process_bar->setValue(0);
    ui->bootloader_process_bar->setValue(0);
    ui->application_process_bar->setValue(0);
    emit req_jlink_tool(jlink_tool::JLINK_TOOL_MANUAL_EXECUTE,0);
}


void MainWindow::on_bootloader_file_open_button_clicked()
{

    QString path;

    if (file_dialog->exec() ==  QDialog::Accepted) {
        path = file_dialog->selectedFiles()[0];
        if (!path.isEmpty()) {
            ui->bootloader_file_path_display->setText(path);
            setting->setValue("last_file_path",path);
            emit req_jlink_tool(jlink_tool::JLINK_TOOL_OPEN_BOOTLOADER,path);
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
            emit req_jlink_tool(jlink_tool::JLINK_TOOL_OPEN_APPLICATION,path);
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


void MainWindow::on_all_on_top_check_box_stateChanged(int arg1)
{
    (void)arg1;
    if (ui->all_on_top_check_box->isChecked()) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    this->show();
}
