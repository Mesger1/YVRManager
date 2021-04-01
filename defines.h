#ifndef DEFINES_H
#define DEFINES_H


#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>


#define TRACE
#define DEBUG
#define INFO

struct yawvr_data {
    float yaw;
    float pitch;
    float roll;
    float battery;
};

typedef enum upd_run_status {
    yaw_run_loop_working = 0,
    yaw_not_reponding = 1,
    receiving_imu_data = 2
} yawvr_udp_run_status;

const int udp_yawvr_retries = 20;

#endif // DEFINES_H
