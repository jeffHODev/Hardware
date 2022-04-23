#include <jni.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <android/log.h>

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "GPIO_JNI", __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "GPIO_JNI", __VA_ARGS__)

#define GPIO_DEVICE "/dev/gpio_ctl"
int fd = -1;
int GPIO_NUM = 4;

extern "C"
{
jboolean
Java_com_hekangyuan_nivi1000_GpioNative_openGpioDev(
        JNIEnv *env,
        jobject obj) {
    if(fd != -1)
        return 1;

    fd = open(GPIO_DEVICE, O_RDWR);
    if (-1 == fd) {
        //LOGE("Can't Open %s[%d]: %s", GPIO_DEVICE, errno, strerror(errno));
        return 0;
    }
    return 1;
}

void
Java_com_hekangyuan_nivi1000_GpioNative_closeGpioDev(
        JNIEnv *env,
        jobject obj) {

    if(fd != -1){
        close(fd);
    }
    fd = -1;
}

void
Java_com_hekangyuan_nivi1000_GpioNative_setGpioMode(
        JNIEnv *env,
        jobject obj,
        jint index,
        jint mode) {
    if(fd == -1) {
        //LOGE("Gpio device is closing.");
        return;
    }

    if(index < 0 || index > (GPIO_NUM - 1))
        return;

    if (ioctl(fd, mode, index) < 0) {
        //LOGE("Can not set GPIO mode. error[%d]: %s", errno, strerror(errno));
    }
}

jint
Java_com_hekangyuan_nivi1000_GpioNative_getGpioLevel(
        JNIEnv *env,
        jobject obj,
        jint index) {
    if(fd == -1) {
        //LOGE("Gpio device is closing.");
        return -1;
    }

    if(index < 0 || index > (GPIO_NUM - 1))
        return -1;

    unsigned char gpios[GPIO_NUM];
    int l = 0;
    while(l != GPIO_NUM) {
        l = (int)read(fd, gpios, size_t(GPIO_NUM));
        if(l < 0){
            //LOGE("Can not read GPIO data. error[%d]: %s", errno, strerror(errno));
            return -1;
        }
    }

    return gpios[index];
}

void
Java_com_hekangyuan_nivi1000_GpioNative_setGpioLevel(
        JNIEnv *env,
        jobject obj,
        jint index,
        jboolean level) {

    if(fd == -1) {
        //LOGE("Gpio device is closing.");
        return;
    }

    if(index < 0 || index > (GPIO_NUM - 1))
        return;

    unsigned char gpios[GPIO_NUM];
    int l = 0;
    while(l != GPIO_NUM) {
        l = (int)read(fd, gpios, size_t(GPIO_NUM));
        if(l < 0){
            //LOGE("Can not read GPIO data. error[%d]: %s", errno, strerror(errno));
            return;
        }
    }

    for(int i = 0; i < GPIO_NUM; i++){
        //LOGI("GPIO[%d] = %d.", i, gpios[i]);
    }

    gpios[index] = level;
    if (write(fd, gpios, 6) < 0) {
        //LOGE("Can not write GPIO data. error[%d]: %s", errno, strerror(errno));
    }
}
}