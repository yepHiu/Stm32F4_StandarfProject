
/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-18 12:43:48
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-08-03 15:32:34
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5.5_回滚_中断配置完毕RE_V1.3_任务运行测试\SYSTEM\para\para.h
 * @Description: 
 */
#ifndef _PARA_H_
#define _PARA_H_

/*
*********************************************************************************************************
*                                               系统级通用数据类型
*********************************************************************************************************
*/
typedef struct
{
	uint16_t len;
	uint8_t buf[1024];
} _Data_Rx;

#define RX_LEN 512
#define TX_LEN 512


/*
*********************************************************************************************************
*                                               飞行姿态相关数据类型
*********************************************************************************************************
*/
typedef struct 
{
	float height;
	float yaw;
	float pitch;
	float roll;
	
	float flowx;
	float flowy;
	uint8_t drone_start_flag;
}DroneRTInfo;

typedef struct
{
	int16_t mv_x;
	int16_t mv_y;
	uint16_t confidence;
	uint16_t fps;
}mvflow_t;

typedef struct
{
	float TOF_Height; 
	uint16_t tof_height_tmp;
	uint8_t height_valid;
	
	int16_t tmpx;
	int16_t tmpy;
	uint16_t flytime_tmp;
	uint8_t flow_valid;
	uint8_t flow_eff;
	
	mvflow_t mvflow;
}SensorDataSend;


typedef enum
{
	Data_Headmode = 0,
    Data_Point = 1,
    Data_Flow = 2,
	Data_Fly=3,
	Data_Line = 6
}fly_mode_e;

typedef enum
{
	grand = 0,
	flying = 1,
	land ,
	circle,
	tower_action,
	test,
	
	back
}state_e;

#define RT_INFO_FLOWX_BIAS 1.f  //1.2f
#define RT_INFO_FLOWY_BIAS 1.f  //1.2f

/*
*********************************************************************************************************
*                                               摄像机相关数据类型
*********************************************************************************************************
*/

typedef struct
{
	int16_t position_x;
	int16_t position_y;
	
	uint8_t valid;
	
	uint8_t mode; // 0 null 1 point 2 line 3color
	int16_t blacklineyaw;
}cam_data_t;

typedef struct{
	float px;
	float py;
	float h_line_distance;
	float v_line_distance;
	float h_line_angle;
	float v_line_angle;
	
	float tower_center;
	uint8_t green_valid;
	uint8_t green_q;
	
	uint8_t cross_valid;
	uint8_t cross_q;
	
	uint8_t A_valid;
	uint8_t A_q;
	
	uint8_t left_dot_valid; //上部线是否被识别
	uint8_t left_dot_q;
	
	uint8_t right_point_valid;
	uint8_t right_point_q;
	
	uint8_t right_dot_valid; //右侧线是否被识别
	uint8_t right_dot_q;
	uint8_t right_end_valid; //右侧终点
	uint8_t right_end_q;
	
	uint8_t tower_valid;
	uint8_t tower_q;
	uint8_t tower_enable;
	
	uint8_t color_valid;
	uint8_t color_q;
	
	uint8_t qc_valid;
	uint8_t qc_q;
	uint8_t qc_num;
	uint8_t qc_enable;
}cam_user_t;

typedef struct 
{
	uint8_t mode;
} cam_Info_t;

typedef enum
{
	null=0,
	calibration=1
	
}cam_mode_e_2;

typedef enum
{
	cam_null = 0,
	cam_cross = 1,
	cam_A = 2,
	cam_left_dot = 3,
	cam_right_point = 4, //右上角点 # angle + h_line
	cam_right_dot = 5,
	cam_tower = 6,
	
	cam_color = 7,
	cam_qc = 8,
	
	cam_right_dot_tmp = 9
}cam_mode_e;

/*
*********************************************************************************************************
*                                               TOF相关数据类型
*********************************************************************************************************
*/

typedef struct {
	float CurrentTime;
	uint64_t LastTime;
}FPS_TimeOfFly;

/*
*********************************************************************************************************
*                                               交互相关数据类型
*********************************************************************************************************
*/

typedef struct
{
    uint16_t Cmd_Msg;
    unsigned char beep_power;
    unsigned char flag_bits;
    uint16_t Cmd_Msg_Tofly;
    uint8_t Cmd_keyvalid;
} cmd_data_t;

/*
*********************************************************************************************************
*                                               控制模式相关数据类型
*********************************************************************************************************
*/
typedef enum
{
	onoff_cmd = 0,
	move_cmd 
}cmd_mode_e;

typedef enum
{
	ac = 0,
	fc = 1,
	afc = 2    //stm32控制模式
}control_mode_e;

typedef enum
{
	drone_remote = 0,
	drone_off = 1,
	drone_on  = 2,
	drone_land = 3 
}onoff_e;

typedef struct
{
	short x; //角度量 单位：°
	short y;
	
	short yaw_ver; //rad/s
	short z_ver; //m/s
	
	short fc_x;
	short fc_y;
	
	short afc_r; //angle
	short afc_p;
	
	unsigned char stop_flag;
	unsigned char x_flow_ban;
	unsigned char y_flow_ban;
	onoff_e onoff;
	control_mode_e ctr_mode;
	
	//这两个变量待观察
	unsigned char src_flow_ban;
	unsigned char tmp_flow_ban;
	
}ctr_t;


#endif // !_PARA_H_
