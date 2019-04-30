#include "mainwindow.h"
#include <QApplication>
#include "jlink_tool.h"
#include "qstring.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    jlink_tool jtool;

    QObject::connect(&w,SIGNAL(req_scan_tool(void)),&jtool,SLOT(handle_scan_tool_req(void)));
    QObject::connect(&jtool,SIGNAL(jlink_tool_rsp(int,QString)),&w,SLOT(handle_jlink_tool_rsp(int,QString)));
    w.show();
    return a.exec();
}
