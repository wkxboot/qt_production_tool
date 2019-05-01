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
        halt = (jlink_halt_t) jlink_arm_dll.resolve("JLINKARM_Halt");

        start_download = (jlink_start_download_t) jlink_arm_dll.resolve("JLINKARM_BeginDownload");
        end_download = (jlink_end_download_t) jlink_arm_dll.resolve("JLINKARM_EndDownload");

        if (!start_download) {
            qDebug("reslove start_download err.\r\n");
        }
        if (!end_download) {
            qDebug("reslove end_download err.\r\n");
        }
        if (!halt) {
            qDebug("reslove halt err.\r\n");
        }

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
        emit jlink_tool_rsp(JLINK_TOOL_LOAD_DLL,-1,"加载JLinkARM.dll失败！");
    }
}

/*扫描jlink并打开，设置速度4000khz*/
int jlink_tool::scan(void)
{
    uint32_t sn;

    /*jlink是否是打开和连接状态*/
    char *open_str;
    if (!is_opened()) {
        open_str = open();
        if (!is_opened()) {
            emit jlink_tool_rsp(JLINK_TOOL_SCAN, -1,"打开jlink失败！");
            return -1;
        }
    }
    qDebug(open_str);

    emit jlink_tool_rsp(JLINK_TOOL_SCAN, 0,"打开jlink成功！");

    sn = get_sn();
    emit jlink_tool_rsp(JLINK_TOOL_SCAN,0,"JLINK-SN:" + QString::number(sn));

    set_speed(4000);
    emit jlink_tool_rsp(JLINK_TOOL_SCAN,0,"设置频率4000Khz成功！");
    return 0;
}

/*连接设备特定STM32f103*/
int jlink_tool::connect_device(void)
{
    int rc;
    rc = is_connected();
    if (rc ) {
        emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"JLINK 是连接的！");
    } else {
       emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"JLINK 是断开的！");

       rc = exe_cmd("Device = STM32F103C8",0,0);
       if (rc != 0) {
           emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"选择芯片STM32F103C8失败！");
           return -1;
       }
       emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"选择芯片STM32F103C8成功！");

       rc = set_tif(1);
       if (rc != 0) {
           emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"设置SWD模式失败！");
           return -1;
       }
       emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"设置SWD模式成功！");
    }

    reset();
    emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"芯片复位成功！");

    rc = halt();
    if (rc != 0) {
       emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"芯片halt失败！");
        return -1;
    }
    emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"芯片halt成功！");

    rc = is_connected();

    if (rc ) {
        emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"JLINK 是连接的！");
    } else {
       emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"JLINK 是断开的！");
    }

    return 0;
}

/*打开SN*/
int jlink_tool::open_sn(QString str)
{
    sn = str;
}


/*打开bootloader*/
int jlink_tool::open_bootloader(QString str)
{
    bootloader_path = str;
}


/*打开application*/
int jlink_tool::open_application(QString str)
{
    application_path = str;
}
/*读取SN*/
int jlink_tool::read_sn()
{
    int rc;
    QString sn;
    QByteArray *byte;
    uint8_t sn_byte[25] = { 0 };

    rc = connect_device();

    if (rc != 0) {
        return -1;
    }
    rc = read(0x8000000,24,sn_byte);
    if (rc == 0) {
        for (int i = 0;i < 24;i ++ ) {
            qDebug("%d = %d",i,sn_byte[i]);
        }
    byte = new QByteArray((char *)sn_byte,24);
    sn = QString::fromLocal8Bit(*byte);
        emit jlink_tool_rsp(JLINK_TOOL_READ_SN,0,sn);
        return 0;
     }

    return -1;
}
/*写入SN*/

int jlink_tool::write_sn(void)
{
    int rc;

    if (sn.isEmpty()) {
        emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,-1,"错误！SN为空！");
        return -1;
    }

    if (sn.size() != 24) {
        emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,-1,"错误！SN长度=" + QString::number(sn.size()) + "错误");
        return -1;
    }
    rc = connect_device();

    if (rc != 0) {
        return -1;
    }

    rc = start_download();
    qDebug("start down rc = %d",rc);
    rc = write(0x8000000,256,(uint8_t *)sn.data());


    if (rc != 256) {
        emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,-1,"写入SN：" + sn + "失败！");
        return -1;
     }
    emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,0,"写入SN：" + sn + "成功！");

    rc = end_download();
    qDebug("end down rc = %d",rc);

    return 0;

}
/*执行自动化烧录*/
int jlink_tool::execute()
{

}

/*执行命令*/
void jlink_tool::handle_jlink_tool_req(int req,QString str)
{

    switch (req) {
    case JLINK_TOOL_SCAN:
        scan();
        break;
    case JLINK_TOOL_READ_SN:
        read_sn();
        break;
    case JLINK_TOOL_WRITE_SN:
        write_sn();
        break;
    case JLINK_TOOL_OPEN_SN:
        open_sn(str);
        break;
    case JLINK_TOOL_OPEN_BOOTLOADER:
        open_bootloader(str);
        break;
    case JLINK_TOOL_OPEN_APPLICATION:
        open_application(str);
        break;

    case JLINK_TOOL_EXECUTE:
        execute();
        break;

    }


}





















