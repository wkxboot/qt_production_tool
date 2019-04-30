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

/*jlink解锁芯片*/
typedef void (*jlink_unlock_t)(void);

/*jlink设置PC*/
typedef void (*jlink_set_pc_t)(uint32_t ,uint32_t);

/*jlink运行*/
typedef void (*jlink_go_t)(void);

/*jlink清除错误*/
typedef void (*jlink_clear_err_t)(void);

/*jlink读MEM*/
typedef void (*jlink_read_mem_t)(uint32_t,uint32_t,uint8_t *);

/*jlink写MEM*/
typedef void (*jlink_write_mem_t)(uint32_t,uint32_t,uint8_t *);


class jlink_tool : public QObject
{
    Q_OBJECT
public:
    explicit jlink_tool(QObject *parent = nullptr);
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

signals:
    void jlink_tool_rsp(int,QString);
public slots:
    void handle_scan_tool_req();

private:
    bool inited;
};

#endif // JLINK_TOOL_H
