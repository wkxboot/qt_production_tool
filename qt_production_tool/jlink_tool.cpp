#include "jlink_tool.h"
#include "qlibrary.h"
#include "qdebug.h"

jlink_tool::jlink_tool(QObject *parent) : QObject(parent)
{
    QLibrary jlink_arm_dll("JLinkARM.dll");
    if (jlink_arm_dll.load()) {
        qDebug("加载jlinkarm.dll成功.\r");
        /*加载库*/
        open = (jlink_open_t) jlink_arm_dll.resolve("JLINKARM_Open");
        close = (jlink_close_t) jlink_arm_dll.resolve("JLINKARM_Close");
        is_opened = (jlink_is_opened_t) jlink_arm_dll.resolve("JLINKARM_IsOpen");
        is_connected = (jlink_is_connected_t) jlink_arm_dll.resolve("JLINKARM_IsConnected");
        get_sn = (jlink_get_sn_t) jlink_arm_dll.resolve("JLINKARM_GetSN");
        exe_cmd = (jlink_exe_cmd_t) jlink_arm_dll.resolve("JLINKARM_ExecCommand");
        set_tif = (jlink_set_tif_t) jlink_arm_dll.resolve("JLINK_TIF_Select");
        set_pc = (jlink_set_pc_t) jlink_arm_dll.resolve("JLINKARM_WriteReg");
        go = (jlink_go_t) jlink_arm_dll.resolve("JLINKARM_Go");
        reset = (jlink_reset_t) jlink_arm_dll.resolve("JLINKARM_Reset");
        get_id = (jlink_get_id_t) jlink_arm_dll.resolve("JLINKARM_GetId");
        unlock = (jlink_unlock_t) jlink_arm_dll.resolve("JLINKARM_Unlock");
        clear_err = (jlink_clear_err_t) jlink_arm_dll.resolve("JLINKARM_ClrError");
        get_speed = (jlink_get_speed_t) jlink_arm_dll.resolve("JLINKARM_GetSpeed");
        set_speed = (jlink_set_speed_t) jlink_arm_dll.resolve("JLINKARM_SetSpeed");
        read = (jlink_read_mem_t) jlink_arm_dll.resolve("JLINKARM_ReadMem");
        write = (jlink_write_mem_t) jlink_arm_dll.resolve("JLINKARM_WriteMem");

        if (!open) {
            qDebug("reslove open err.\r\n");
        }
        if (!close) {
            qDebug("reslove close err.\r\n");
        }
        if (!is_opened) {
            qDebug("reslove is_opened err.\r\n");
        }
        if (!is_connected) {
            qDebug("reslove is_connected err.\r\n");
        }
        if (!exe_cmd) {
            qDebug("reslove exe_cmd err.\r\n");
        }
        if (!set_pc) {
            qDebug("reslove set_pc err.\r\n");
        }
        if (!go) {
            qDebug("reslove go err.\r\n");
        }
        if (!reset) {
            qDebug("reslove reset err.\r\n");
        }
        if (!get_id) {
            qDebug("reslove open err.\r\n");
        }
        if (!unlock) {
            qDebug("reslove open err.\r\n");
        }
        if (!clear_err) {
            qDebug("reslove clear_err err.\r\n");
        }
        if (!get_speed) {
            qDebug("reslove get_speed err.\r\n");
        }
        if (!set_speed) {
            qDebug("reslove set_speed err.\r\n");
        }
        if (!read) {
            qDebug("reslove read err.\r\n");
        }
        if (!write) {
            qDebug("reslove write err.\r\n");
        }
        if (!set_tif) {
            qDebug("reslove set_tif err.\r\n");
        }
        if (!get_sn) {
            qDebug("reslove get_sn err.\r\n");
        }

    } else {
        qDebug("load jlinkarm.dll err.\r\n");
        emit jlink_tool_rsp(-1,"加载JLinkARM.dll失败！");
    }
}



void jlink_tool::handle_scan_tool_req()
{
    int rc;
    uint32_t sn;
    /*jlink是否是打开和连接状态*/
    char *open_str;
    if (!is_opened()) {
        open_str = open();

        if (!is_opened()) {
            rc = -1;
            emit jlink_tool_rsp(-1,"打开jlink失败！");
            return;
        }
        emit jlink_tool_rsp(0,"打开jlink成功！");
    }

    sn = get_sn();
    emit jlink_tool_rsp(0,"JLINK-SN:" + QString::number(sn));

    set_speed(2000);
    emit jlink_tool_rsp(0,"设置频率2000Khz成功！");

    rc = set_tif(1);
    if (rc != 0) {
        emit jlink_tool_rsp(-1,"设置SWD失败！");
        return;
    }
    emit jlink_tool_rsp(0,"设置SWD成功！");

    rc = exe_cmd("Device = STM32F103C8",0,0);
    if (rc != 0) {
        emit jlink_tool_rsp(-1,"选择芯片STM32F103C8失败！");
        return;
    }
   emit jlink_tool_rsp(0,"选择芯片STM32F103C8成功！");
}





















