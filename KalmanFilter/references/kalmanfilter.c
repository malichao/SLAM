//卡尔曼滤波相关宏定义
#define dt               0.02//卡尔曼滤波采样周期 s 
#define R_angle          0.5 //测量噪声的协方差（即是测量偏差）
#define Q_angle          0.0001//过程噪声的协方差 
#define Q_gyro           0.0003 //过程噪声的协方差  过程噪声协方差为一个一行两列矩阵

float kalmanUpdate(const float gyro_m,const float incAngle)  {       
	float K_0;//含有卡尔曼增益的另外一个函数，用于计算最优估计值
	float K_1;//含有卡尔曼增益的函数，用于计算最优估计值的偏差
	float Y_0;
	float Y_1;

	float Rate;//去除偏差后的角速度
	float Pdot[4];//过程协方差矩阵的微分矩阵
	float angle_err;//角度偏量
	float E;//计算的过程量
	        
	static float angle = 0;            //下时刻最优估计值角度
	static float q_bias = 0;        //陀螺仪的偏差                 
	static float P[2][2] = {{ 1, 0 }, { 0, 1 }};//过程协方差矩阵
	         
	Rate = gyro_m - q_bias;

	//计算过程协方差矩阵的微分矩阵     

	Pdot[0] = Q_angle - P[0][1] - P[1][0];//？？？？？？         
	Pdot[1] = - P[1][1];                         
	Pdot[2] = - P[1][1];                                  
	Pdot[3] = Q_gyro;//？？？？？？                         

	angle += Rate * dt; //角速度积分得出角度

	P[0][0] += Pdot[0] * dt; //计算协方差矩阵
	P[0][1] += Pdot[1] * dt; 
	P[1][0] += Pdot[2] * dt; 
	P[1][1] += Pdot[3] * dt; 

	angle_err = incAngle - angle; //计算角度偏差

	E = R_angle + P[0][0];
	K_0 = P[0][0] / E; //计算卡尔曼增益
	K_1 = P[1][0] / E; 

	Y_0 = P[0][0];   
	Y_1 = P[0][1]; 

	P[0][0] -= K_0 * Y_0; //跟新协方差矩阵
	P[0][1] -= K_0 * Y_1; 
	P[1][0] -= K_1 * Y_0; 
	P[1][1] -= K_1 * Y_1; 

	angle += K_0 * angle_err; //给出最优估计值
	q_bias += K_1 * angle_err;//跟新最优估计值偏差 

	return angle; 
}