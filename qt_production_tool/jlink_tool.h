#ifndef JLINK_TOOL_H
#define JLINK_TOOL_H

#include <QObject>

/*jlink打开*/
typedef char* (*jlink_open_t)(void);

/*jlink关闭*/
typedef void (*jlink_close_t)(void);

/*jlink执行命令*/
typedef int (*jlink_exe_cmd_t)(char *,int,int);

/*jlink是否打开*/
typedef int (*jlink_is_opened_t)(void);

/*jlink是否连接*/
typedef int (*jlink_is_connected_t)(void);



/*jlink获取SN*/
typedef uint32_t (*jlink_get_sn_t)(void);

/*jlink设置速度*/
typedef void (*jlink_set_speed_t)(int);

/*jlink获取速度*/
typedef uint32_t (*jlink_get_speed_t)(void);

/*jlink设置接口*/
typedef int (*jlink_set_tif_t)(int);

/*jlink获取接口*/
typedef int (*jlink_get_tif_t)(void);

/*jlink获取芯片ID*/
typedef uint32_t (*jlink_get_id_t)(void);

/*jlink解锁芯片*/
typedef void (*jlink_unlock_t)(void);

/*jlink复位芯片*/
typedef void (*jlink_reset_t)(void);

/*jlink暂停*/
typedef int (*jlink_halt_t)(void);

/*jlink解锁芯片*/
typedef void (*jlink_unlock_t)(void);

/*jlink设置PC*/
typedef void (*jlink_set_pc_t)(uint32_t ,uint32_t);

/*jlink运行*/
typedef void (*jlink_go_t)(void);

/*jlink清除错误*/
typedef void (*jlink_clear_err_t)(void);

/*jlink读MEM*/
typedef int (*jlink_read_mem_t)(uint32_t,uint32_t,uint8_t *);

/*jlink写MEM*/
typedef int (*jlink_write_mem_t)(uint32_t,uint32_t,uint8_t *);

/*jlink写MEM8*/
typedef int (*jlink_write_mem8_t)(uint32_t,uint8_t);

/*开始下载*/
typedef int (*jlink_start_download_t)(void);
/*结束下载*/
typedef int (*jlink_end_download_t)(void);


class jlink_tool : public QObject
{
    Q_OBJECT
public:
    explicit jlink_tool(QObject *parent = nullptr);

    enum {
        JLINK_TOOL_LOAD_DLL,
        JLINK_TOOL_OPEN_SN,
        JLINK_TOOL_OPEN_BOOTLOADER,
        JLINK_TOOL_OPEN_APPLICATION,

        JLINK_TOOL_SCAN,
        JLINK_TOOL_CONNECT_DEVICE,
        JLINK_TOOL_READ_SN,
        JLINK_TOOL_WRITE_SN,
        JLINK_TOOL_EXECUTE,
    };

    jlink_open_t open;
    jlink_close_t close;
    jlink_get_sn_t get_sn;
    jlink_exe_cmd_t exe_cmd;
    jlink_go_t go;
    jlink_reset_t reset;
    jlink_get_id_t get_id;
    jlink_read_mem_t read;
    jlink_write_mem_t write;
    jlink_set_pc_t set_pc;
    jlink_set_tif_t set_tif;
    jlink_unlock_t unlock;
    jlink_clear_err_t clear_err;
    jlink_get_speed_t get_speed;
    jlink_set_speed_t set_speed;
    jlink_is_opened_t is_opened;
    jlink_is_connected_t is_connected;
    jlink_halt_t halt;
    jlink_start_download_t start_download;
    jlink_end_download_t end_download;


    int scan(void);
    int connect_device(void);

    int open_sn(QString);
    int open_bootloader(QString);
    int open_application(QString);

    int read_sn();
    int write_sn(void);
    int write_bootloader(void);
    int write_application(void);
    int execute();

signals:
    void jlink_tool_rsp(int,int,QString);
public slots:
    void handle_jlink_tool_req(int,QString);

private:
    bool inited;
    QString bootloader_path;
    QString application_path;
    QString sn;
};

#endif // JLINK_TOOL_H
