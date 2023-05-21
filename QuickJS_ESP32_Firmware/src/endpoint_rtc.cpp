#include <Arduino.h>
#include "main_config.h"

#ifdef _RTC_ENABLE_

#include "endpoint_types.h"
#include "endpoint_rtc.h"

long endp_rtc_setTime(JsonObject request, JsonObject response, int magic)
{
  uint32_t hours = request["Hours"];
  uint32_t minutes = request["Minutes"];
  uint32_t seconds = request["Seconds"];

  RTC_TimeTypeDef def;
  def.Hours = hours;
  def.Minutes = minutes;
  def.Seconds = seconds;
  M5.Rtc.SetTime(&def);

  return 0;
}

long endp_rtc_setDate(JsonObject request, JsonObject response, int magic)
{
  uint32_t year = request["Year"];
  uint32_t month = request["Month"];
  uint32_t date = request["Date"];
  uint32_t weekday = request["WeekDay"];

  RTC_DateTypeDef def;
  def.Year = year;
  def.Month = month;
  def.Date = date;
  def.WeekDay = weekday;
#if defined(ARDUINO_M5Stick_C)
  M5.Rtc.SetData(&def);
#else
  M5.Rtc.SetDate(&def);
#endif

  return 0;
}

long endp_rtc_getTime(JsonObject request, JsonObject response, int magic)
{
  RTC_TimeTypeDef def;
  M5.Rtc.GetTime(&def);

  response["result"]["Hours"] = def.Hours;
  response["result"]["Minutes"] = def.Minutes;
  response["result"]["Seconds"] = def.Seconds;

  return 0;
}

long endp_rtc_getDate(JsonObject request, JsonObject response, int magic)
{
  RTC_DateTypeDef def;
#if defined(ARDUINO_M5Stick_C)
  M5.Rtc.GetData(&def);
#else
  M5.Rtc.GetDate(&def);
#endif
  response["result"]["Year"] = def.Year;
  response["result"]["Month"] = def.Month;
  response["result"]["Date"] = def.Date;
  response["result"]["WeekDay"] = def.WeekDay;

  return 0;
}

EndpointEntry rtc_table[] = {
  EndpointEntry{ endp_rtc_setTime, "/rtc-setTime", -1 },
  EndpointEntry{ endp_rtc_setDate, "/rtc-setDate", -1 },
  EndpointEntry{ endp_rtc_getTime, "/rtc-getTime", -1 },
  EndpointEntry{ endp_rtc_getDate, "/rtc-getDate", -1 },
};

const int num_of_rtc_entry = sizeof(rtc_table) / sizeof(EndpointEntry);

#endif
