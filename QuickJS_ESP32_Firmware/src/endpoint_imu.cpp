#include <Arduino.h>
#include "main_config.h"

#include "endpoint_types.h"
#include "endpoint_imu.h"

#ifdef _IMU_ENABLE_

long endp_imu_getAccelData(JsonObject request, JsonObject response, int magic)
{
  float ax, ay, az;
  M5.IMU.getAccelData(&ax, &ay, &az);

  response["result"]["x"] = ax;
  response["result"]["y"] = ay;
  response["result"]["z"] = az;

  return 0;
}

long endp_imu_getGyroData(JsonObject request, JsonObject response, int magic)
{
  float gx, gy, gz;
  M5.IMU.getGyroData(&gx, &gy, &gz);

  response["result"]["x"] = gx;
  response["result"]["y"] = gy;
  response["result"]["z"] = gz;

  return 0;
}

long endp_imu_getTempData(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];
  uint32_t duty = request["duty"];

  ledcWrite(channel, duty);

  return 0;
}

EndpointEntry imu_table[] = {
  EndpointEntry{ endp_imu_getAccelData, "/imu-getAccelData", -1 },
  EndpointEntry{ endp_imu_getGyroData, "/imu-getGyroData", -1 },
  EndpointEntry{ endp_imu_getTempData, "/imu-getTempData", -1 },
};

const int num_of_imu_entry = sizeof(imu_table) / sizeof(EndpointEntry);

#endif
