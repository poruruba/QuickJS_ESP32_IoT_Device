#include <Arduino.h>
#include "main_config.h"

#include "endpoint_types.h"
#include "endpoint_imu.h"

#ifdef _IMU_ENABLE_

long endp_imu_getAccelData(JsonObject request, JsonObject response, int magic)
{
  float ax, ay, az;
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.getAccel(&ax, &ay, &az);
#else
  M5.IMU.getAccelData(&ax, &ay, &az);
#endif

  response["result"]["x"] = ax;
  response["result"]["y"] = ay;
  response["result"]["z"] = az;

  return 0;
}

long endp_imu_getGyroData(JsonObject request, JsonObject response, int magic)
{
  float gx, gy, gz;
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.getGyro(&gx, &gy, &gz);
#else
  M5.IMU.getGyroData(&gx, &gy, &gz);
#endif

  response["result"]["x"] = gx;
  response["result"]["y"] = gy;
  response["result"]["z"] = gz;

  return 0;
}

long endp_imu_getTempData(JsonObject request, JsonObject response, int magic)
{
  float t;  
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.getTemp(&t);
#else
  M5.IMU.getTempData(&t);
#endif
  
  response["result"] = t;

  return 0;
}

EndpointEntry imu_table[] = {
  EndpointEntry{ endp_imu_getAccelData, "/imu-getAccelData", -1 },
  EndpointEntry{ endp_imu_getGyroData, "/imu-getGyroData", -1 },
  EndpointEntry{ endp_imu_getTempData, "/imu-getTempData", -1 },
};

const int num_of_imu_entry = sizeof(imu_table) / sizeof(EndpointEntry);

#endif
