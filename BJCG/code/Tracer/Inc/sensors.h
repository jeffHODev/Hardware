#ifndef SENSOR_H
#define SENSOR_H
#define GPS_COUNT       60
#define SENSORS_COUNT   12
#define SUM_COUNT   SENSORS_COUNT + GPS_COUNT

#define SN		SENSORS_COUNT
void sensors_Init(void);
void snesors_process(void);
float *GetSensorsData(void);
unsigned char* getGPS(void);//��ȡgpsλ����Ϣ

#endif

