#include "mainwindow.h"
#include <QApplication>
#include "jlink_tool.h"
#include "qstring.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    jlink_tool jtool;

    QObject::connect(&w,SIGNAL(req_jlink_tool(int,QString)),&jtool,SLOT(handle_jlink_tool_req(int,QString)));
    QObject::connect(&jtool,SIGNAL(jlink_tool_rsp(int,int,QString)),&w,SLOT(handle_jlink_tool_rsp(int,int,QString)));
    w.show();
    return a.exec();
}
