#include "jlink_tool.h"
#include "qlibrary.h"
#include "qdebug.h"
#include "qfile.h"
#include "qdatastream.h"

jlink_tool::jlink_tool(QObject *parent) : QObject(parent)
{
    int rc= -1;
    device = new QString("STM32F103RC");

    QLibrary jlink_arm_dll("JLinkARM.dll");
    if (!jlink_arm_dll.load()) {
        emit emit jlink_tool_rsp(JLINK_TOOL_LOAD_DLL,-1,"加载JLinkARM.dll失败！");
        goto  err;
    }
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
            goto err;
        }
        if (!end_download) {
            goto err;
        }
        if (!halt) {
            goto err;
        }

        if (!open) {
            goto err;
        }
        if (!close) {
            goto err;
        }
        if (!is_opened) {
            goto err;
        }
        if (!is_connected) {
            goto err;
        }
        if (!exe_cmd) {
            goto err;
        }
        if (!set_pc) {
            goto err;
        }
        if (!go) {
            goto err;
        }
        if (!reset) {
            goto err;
        }
        if (!get_id) {
            goto err;
        }
        if (!unlock) {
            goto err;
        }
        if (!clear_err) {
            goto err;
        }
        if (!get_speed) {
            goto err;
        }
        if (!set_speed) {
            goto err;
        }
        if (!read) {
            goto err;
        }
        if (!write) {
            goto err;
        }
        if (!set_tif) {
            goto err;
        }
        if (!get_sn) {
            goto err;
        }

        rc = 0;
        inited = true;
        return;
err:
    inited = false;
}

/*打开jlink*/
int jlink_tool::open_jlink()
{
    /*jlink是否是打开和连接状态*/
    if (!is_opened()) {
        open();
        if (!is_opened()) {
            emit emit jlink_tool_rsp(JLINK_TOOL_SCAN, -1,"打开jlink失败！");
            return -1;
        }
    }

    emit emit jlink_tool_rsp(JLINK_TOOL_SCAN, 0,"打开jlink成功！");
    return 0;
}
/*扫描jlink并打开，设置速度4000khz*/
int jlink_tool::scan(void)
{
    int rc;
    uint32_t sn;

    rc = open_jlink();
    if (rc != 0) {
        return -1;
    }

    sn = get_sn();
    emit emit jlink_tool_rsp(JLINK_TOOL_SCAN,0,"JLINK-SN:" + QString::number(sn));

    set_speed(4000);
    emit emit jlink_tool_rsp(JLINK_TOOL_SCAN,0,"设置频率4000Khz成功！");


    return 0;
}

/*连接指定设备*/
int jlink_tool::connect_device(void)
{
    int rc;

    rc = open_jlink();
    if (rc != 0) {
        return -1;
    }

    rc = exe_cmd("Device = STM32F103RC",0,0);
    if (rc != 0) {
        emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"选择芯片失败！");
        return -1;
    }
    emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"选择芯片成功！");

    rc = set_tif(1);
    if (rc != 0) {
        emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"设置SWD模式失败！");
        return -1;
    }
    emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"设置SWD模式成功！");


    reset();
    emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"芯片复位成功！");

    rc = halt();
    if (rc != 0) {
       emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"芯片halt失败！");
        return -1;
    }
    emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"芯片halt成功！");

    if (!is_connected() ) {
       emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,-1,"JLINK连接失败！");
       return -1;
    }
    emit emit jlink_tool_rsp(JLINK_TOOL_CONNECT_DEVICE,0,"JLINK连接成功！");

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
    QFile bootloader_file;
    QDataStream *stream;
    QString version;
    int start,end;


    bootloader_path = str;
    if (bootloader_path.isEmpty()){
        return -1;
    }
    bootloader_file.setFileName(str);
    if (bootloader_file.open(QFile::ReadOnly)) {
        stream = new QDataStream(&bootloader_file);
        stream->device()->seek(0);
        bootloader_bin = stream->device()->readAll();
        if (bootloader_bin.contains("v1.")) {
            start = bootloader_bin.indexOf("v1.");
            end = bootloader_bin.indexOf(" ",start);
            version = QString::fromLocal8Bit(bootloader_bin.mid(start + 1,end - start));
            emit emit jlink_tool_rsp(JLINK_TOOL_OPEN_BOOTLOADER,0,version);

            return 0;
        }
    }
    bootloader_bin.clear();

    emit emit jlink_tool_rsp(JLINK_TOOL_OPEN_BOOTLOADER,-1,0);
    return -1;
}


/*打开application*/
int jlink_tool::open_application(QString str)
{

    QFile application_file;
    QDataStream *stream;
    QString version;
    int start,end;

    application_path = str;
    if (application_path.isEmpty()){
        return -1;
    }
    application_file.setFileName(str);
    if (application_file.open(QFile::ReadOnly)) {
        stream = new QDataStream(&application_file);
        stream->device()->seek(0);
        application_bin = stream->device()->readAll();
        if (application_bin.contains("v1.")) {
            goto exit;
        }

        if (application_bin.contains("1.0")) {
            start = application_bin.indexOf("1.0");
            end = application_bin.indexOf(" ",start);
            version = QString::fromLocal8Bit(application_bin.mid(start,end - start));
            emit emit jlink_tool_rsp(JLINK_TOOL_OPEN_APPLICATION,0,version);
            return 0;
        }
    }
exit:
    application_bin.clear();
    emit emit jlink_tool_rsp(JLINK_TOOL_OPEN_APPLICATION,-1,0);
    return -1;
}

/*读取SN*/
int jlink_tool::read_sn()
{
    int rc;
    QString sn;
    QByteArray *byte;
    uint8_t sn_byte[JLINK_TOOL_SN_SIZE] = { 0 };

    rc = connect_device();

    if (rc != 0) {
        return -1;
    }
    rc = read(JLINK_TOOL_SERIAL_NO_ADDR,JLINK_TOOL_SN_SIZE,sn_byte);
    if (rc != 0) {
        emit emit jlink_tool_rsp(JLINK_TOOL_READ_SN,-1,"读取SN失败！");
        return -1;
    }

    byte = new QByteArray((char *)sn_byte,JLINK_TOOL_SN_SIZE);
    sn = QString::fromLocal8Bit(*byte);
    emit emit jlink_tool_rsp(JLINK_TOOL_READ_SN,0,sn);

    return 0;
}


/*写入SN*/
int jlink_tool::write_sn(void)
{
    int rc;

    if (sn.size() != JLINK_TOOL_SN_SIZE) {
        emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,-1,"错误！SN长度=" + QString::number(sn.size()) + "错误");
        return -1;
    }
    rc = connect_device();

    if (rc != 0) {
        return -1;
    }

    start_download();
    rc = write(JLINK_TOOL_SERIAL_NO_ADDR,JLINK_TOOL_SN_SIZE,(uint8_t *)sn.toUtf8().data());

    if (rc != JLINK_TOOL_SN_SIZE) {
        rc = -1;
        goto exit;;
     }
    emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,100,"写入SN：" + sn + "成功！");

    end_download();
    rc = 0;

exit:
    if (rc != 0) {
        emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_SN,-1,"写入SN：" + sn + "失败！");
    }

    return rc;

}

/*烧写bootloader*/
int jlink_tool::write_bootloader()
{
    int rc = -1;
    int write_total = 0;
    int write_size;
    int percent = 0;

    if (bootloader_bin.size() == 0) {
        goto exit;
    }
    rc = connect_device();

    if (rc != 0) {
        return -1;
    }
    start_download();
    while (write_total < bootloader_bin.size()) {
    write_size = (JLINK_TOOL_WRITE_SIZE <= (bootloader_bin.size() - write_total) ? JLINK_TOOL_WRITE_SIZE : bootloader_bin.size() - write_total);
    rc = write(JLINK_TOOL_BOOTLOADER_ADDR + write_total,write_size,(uint8_t*)(bootloader_bin.data() + write_total));
    if (rc != write_size) {
        rc = -1;
        goto exit;
    }
    write_total += write_size;
    percent = write_total*100 /bootloader_bin.size();
    emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_BOOTLOADER,percent,0);
    }
    rc = 0;
exit:
     end_download();
     if (rc != 0) {
        emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_BOOTLOADER,-1,"烧写bootloader失败！");
     }
     return rc;
}

/*烧写应用程序*/
int jlink_tool::write_application()
{
    int rc = -1;
    int write_total = 0;
    int write_size;
    int percent;

    if (application_bin.size() == 0) {
        goto exit;
    }
    rc = connect_device();

    if (rc != 0) {
        return -1;
    }
    start_download();
    while (write_total < application_bin.size()) {
    write_size = (JLINK_TOOL_WRITE_SIZE <= (application_bin.size() - write_total) ? JLINK_TOOL_WRITE_SIZE : application_bin.size() - write_total);
    rc = write(JLINK_TOOL_APPLICATION_ADDR + write_total,write_size,(uint8_t*)(application_bin.data() + write_total));
    if (rc != write_size) {
        rc = -1;
        goto exit;
    }
    write_total += write_size;
    percent = write_total*100 /application_bin.size();
    emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_APPLICATION,percent,0);
    }
    rc = 0;
exit:
     end_download();
     if (rc != 0) {
        emit emit jlink_tool_rsp(JLINK_TOOL_WRITE_APPLICATION,-1,"烧写application失败！");
     }
     return rc;
}


/*执行烧录*/
int jlink_tool::execute()
{
    int rc;

    rc = write_sn();
    if (rc != 0) {
        return -1;
    }
    rc = write_bootloader();
    if (rc != 0) {
        return -1;
    }
    rc = write_application();
    if (rc != 0) {
        return -1;
    }

    return 0;

}

/*自动烧写*/
int jlink_tool::auto_execute()
{
    int rc;

    if (sn.size() != JLINK_TOOL_SN_SIZE) {
        emit emit jlink_tool_rsp(JLINK_TOOL_AUTO_EXECUTE,-2,"错误！SN长度=" + QString::number(sn.size()) + "错误");
        goto exit;
    }

    if ( bootloader_bin.size() == 0) {
         emit emit jlink_tool_rsp(JLINK_TOOL_AUTO_EXECUTE,-2,"bootloade文件没有打开或者不正确！");
         goto exit;
    }

    if ( application_bin.size() == 0) {
         emit emit jlink_tool_rsp(JLINK_TOOL_AUTO_EXECUTE,-2,"application文件没有打开或者不正确！");
         goto exit;
    }

    rc = execute();
    if (rc != 0) {
        goto exit;
    }

    emit emit jlink_tool_rsp(JLINK_TOOL_AUTO_EXECUTE,0,"SN："+ sn + " 自动烧录成功！");
    return 0;

exit:
    emit emit jlink_tool_rsp(JLINK_TOOL_AUTO_EXECUTE,-1,"自动烧录失败！");
    return -1;
}


/*手动烧写*/
int jlink_tool::manual_execute()
{
    int rc;

    if (sn.size() != JLINK_TOOL_SN_SIZE) {
        emit emit jlink_tool_rsp(JLINK_TOOL_MANUAL_EXECUTE,-2,"错误！SN长度=" + QString::number(sn.size()) + "错误");
        goto exit;
    }

    if ( bootloader_bin.size() == 0) {
         emit emit jlink_tool_rsp(JLINK_TOOL_MANUAL_EXECUTE,-2,"bootloade文件没有打开或者不正确！");
         goto exit;
    }

    if ( application_bin.size() == 0) {
         emit emit jlink_tool_rsp(JLINK_TOOL_MANUAL_EXECUTE,-2,"application文件没有打开或者不正确！");
         goto exit;
    }

    rc = execute();
    if (rc != 0) {
        goto exit;
    }

    emit emit jlink_tool_rsp(JLINK_TOOL_MANUAL_EXECUTE,0,"SN："+ sn + " 手动烧录成功！");
    return 0;

exit:
    emit emit jlink_tool_rsp(JLINK_TOOL_MANUAL_EXECUTE,-1,"手动烧录失败！");
    return -1;
}

/*芯片运行程序*/
int jlink_tool::start_go()
{
   set_pc(9,0x8000000);
   go();
   return 0;
}
/*执行命令*/
void jlink_tool::handle_jlink_tool_req(int req,QString str)
{
    int rc = -1;
    if (!inited) {
       emit jlink_tool_rsp(JLINK_TOOL_INIT,0,"驱动初始化失败！");
       return ;
    }
    emit jlink_tool_rsp(JLINK_TOOL_BUSY_STATUS,1,0);
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

    case JLINK_TOOL_AUTO_EXECUTE:
        rc = auto_execute();
        if (rc == 0) {
            start_go();
        }
        break;
    case JLINK_TOOL_MANUAL_EXECUTE:
        rc = manual_execute();
        if (rc == 0) {
            start_go();
        }
        break;
    }

    emit jlink_tool_rsp(JLINK_TOOL_BUSY_STATUS,0,0);
}





















