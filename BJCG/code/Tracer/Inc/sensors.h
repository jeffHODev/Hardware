#ifndef SENSOR_H
#define SENSOR_H

#define SN		12
void sensors_Init(void);
void snesors_process(void);
float *GetSensorsData(void);
unsigned char* getGPS(void);//��ȡgpsλ����Ϣ

#endif

