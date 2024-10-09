#include "sLib_Filter.h"

//#include "sHMI_Debug.h"

bool flag = 1;

void sLib_6AxisCompFilter(sLIB_6AXIS_INPUT_t* input,sLIB_ATTITUDE_RESULT_t* result){

    static uint32_t prev_time;
    static uint32_t now_time;
    static float dts;
    prev_time = now_time;
    now_time = HAL_GetTick();
    dts = (now_time - prev_time) / 1000.0f;

    if(flag){
        dts = 0;
        flag = 0;
    }

    //sLogOK("dts:%.4f", dts);

	const float Kp = 0.4f;
	const float Ki = 0.001f;
	//四元数
	static float q0 = 1.0f;
	static float q1 = 0.0f;
	static float q2 = 0.0f;
	static float q3 = 0.0f;	
	//误差积分累计
	float exInt = 0.0f;
	float eyInt = 0.0f;
	float ezInt = 0.0f;
    //方向余弦矩阵
	static float rMat[3][3];
    //用于标准化
    float normalise;
	float ex, ey, ez;
	float accBuf[3] = {0.f};

    //用于缓存数据,便于修改
    static sLIB_6AXIS_INPUT_t data;
    //拷贝数据
    memcpy(&data, input, sizeof(sLIB_6AXIS_INPUT_t));

    //把陀螺仪的角度单位换算成弧度
    data.gyro_x *= DEG2RAD;
	data.gyro_y *= DEG2RAD;
	data.gyro_z *= DEG2RAD;

	//标准化加速计测量值
	normalise = sLib_InvSqrt(data.acc_x * data.acc_x + data.acc_y * data.acc_y + data.acc_z * data.acc_z);
	data.acc_x *= normalise;
	data.acc_y *= normalise;
	data.acc_z *= normalise;

	//加速计读取的方向与重力加速计方向的差值，用向量叉乘计算
	ex = (data.acc_y * rMat[2][2] - data.acc_z * rMat[2][1]);
	ey = (data.acc_z * rMat[2][0] - data.acc_x * rMat[2][2]);
	ez = (data.acc_x * rMat[2][1] - data.acc_y * rMat[2][0]);

	//误差累计，与积分常数相乘
	exInt += Ki * ex * dts ;  
	eyInt += Ki * ey * dts ;
	ezInt += Ki * ez * dts ;

	//用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量
	data.gyro_x += Kp * ex + exInt;
	data.gyro_y += Kp * ey + eyInt;
	data.gyro_z += Kp * ez + ezInt;

	//一阶近似算法，四元数运动学方程的离散化形式和积分
	float q0Last = q0;
	float q1Last = q1;
	float q2Last = q2;
	float q3Last = q3;
	q0 += (-q1Last * data.gyro_x - q2Last * data.gyro_y - q3Last * data.gyro_z) * 0.5f * dts;
	q1 += ( q0Last * data.gyro_x + q2Last * data.gyro_z - q3Last * data.gyro_y) * 0.5f * dts;
	q2 += ( q0Last * data.gyro_y - q1Last * data.gyro_z + q3Last * data.gyro_x) * 0.5f * dts;
	q3 += ( q0Last * data.gyro_z + q1Last * data.gyro_y - q2Last * data.gyro_x) * 0.5f * dts;

	//标准化四元数
	normalise = sLib_InvSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;

	//计算方向余弦矩阵
    float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;

    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q3 = q2 * q3;

    rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    rMat[0][1] = 2.0f * (q1q2 + -q0q3);
    rMat[0][2] = 2.0f * (q1q3 - -q0q2);

    rMat[1][0] = 2.0f * (q1q2 - -q0q3);
    rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    rMat[1][2] = 2.0f * (q2q3 + -q0q1);

    rMat[2][0] = 2.0f * (q1q3 + -q0q2);
    rMat[2][1] = 2.0f * (q2q3 - -q0q1);
    rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;

    //计算roll pitch yaw 欧拉角
	result->roll  = -asinf(rMat[2][0]) * RAD2DEG; 
	result->pitch =  atan2f(rMat[2][1], rMat[2][2]) * RAD2DEG;
	result->yaw   =  atan2f(rMat[1][0], rMat[0][0]) * RAD2DEG;
    //保存四元数
    result->q0 = q0;
    result->q1 = q1;
    result->q2 = q2;
    result->q3 = q3;
}

//初始化SWF,传入滤波器句柄,窗口大小
void sLib_SWFInit(sLIB_SWF_t* pFliter,uint8_t win_size){
    pFliter->size = win_size;
    pFliter->data = (float*)sLib_PortMalloc(win_size * sizeof(float));
    memset(pFliter->data, 0, win_size * sizeof(float));
    pFliter->index = 0;
    pFliter->sum = 0.0;
    pFliter->average = 0.0;
}

//释放SWF
void sLib_SWFDeInit(sLIB_SWF_t* pFliter){
    sLib_PortFree(pFliter->data);
    pFliter->data = NULL;
    pFliter->size = 0;
    pFliter->index = 0;
    pFliter->sum = 0.0;
    pFliter->average = 0.0;
}

//传入滤波器和新数据,返回平均值
float sLib_SWFUpdate(sLIB_SWF_t* pFliter,float newData){
    //减去即将被覆盖的数据点的值
    pFliter->sum -= pFliter->data[pFliter->index];
    //添加新的数据到窗口并更新索引
    pFliter->data[pFliter->index] = newData;
    pFliter->sum += newData;
    pFliter->index = (pFliter->index + 1) % pFliter->size;
    //计算新的平均值
    pFliter->average = pFliter->sum / pFliter->size;
    return pFliter->average;
}

//中值滤波,传入滤波器句柄和窗口大小
void sLib_MWFInit(sLIB_MWF_t* pFilter, uint8_t size){
    pFilter->size = size;
    pFilter->data = (float*)sLib_PortMalloc(size * sizeof(float));
    memset(pFilter->data, 0, size * sizeof(float));
    pFilter->index = 0;
}

//传入滤波器和新数据, 返回中值
float sLib_MWFUpdate(sLIB_MWF_t* pFilter, float newData){
    //更新数据
    pFilter->data[pFilter->index] = newData;
    pFilter->index = (pFilter->index + 1) % pFilter->size;
    
    //创建一个用于排序的临时数组
    float sortedData[pFilter->size];
    memcpy(sortedData, pFilter->data, pFilter->size * sizeof(float));
    qsort(sortedData, pFilter->size, sizeof(float), sLib_FCmp);
    
    //返回中值
    if (pFilter->size % 2 == 1) {
        return sortedData[pFilter->size / 2];
    } else {
        //如果数据数量是偶数，则取中间两个数的平均值
        return (sortedData[pFilter->size / 2 - 1] + sortedData[pFilter->size / 2]) / 2.0;
    }
}

//一阶指数移动平均滤波初始化
void sLib_EMA1Init(sLib_EMA1_t* pFilter, float alpha){
    pFilter->alpha = alpha;
    pFilter->lastValue = 0.0; //初始值设为0,或者可以设为第一个数据点的值
}

//传入滤波器和新数据返回滤波结果
float sLib_EMA1Update(sLib_EMA1_t* pFilter, float newData){
    //更新滤波值:EMA公式为 EMA_t = alpha * newData + (1 - alpha) * EMA_{t-1}
    pFilter->lastValue = pFilter->alpha * newData + (1 - pFilter->alpha) * pFilter->lastValue;
    return pFilter->lastValue;
}


