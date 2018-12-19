/*
   This allows you to run various functions on a command-line like interface.
   Enter `config` to see the configuration of the device. Type 'ec' to get a
   measurement in mS, 'data' to see all the data members of the class like uS,
   and S, 'temp' to read the temperature, 'sal' to use salinity parameters for
   a measurment.

   Calibration:
    low <calibration solution in mS eg 0.7>
    high <calibration solution in mS eg 2.0>
    <measurements will automatically use points as soon as both low and high are received>

   Using Temperature compensation with attached temperature sensor:
    tc 1 25 <to adjust readings as if they were at 25 C>
    ec
    tc 0 <to disable compensation>

   Set a temperature to use
    t 20 <to use 20 C as the temperature rather than using the attached temp. sensor>

   Measure saltwater with a configured device:
    sal
 */

#include <Arduino.h>
#include "ECSalinity.h"
#include <ISE_pH.h>
#include <ISE_ORP.h>
#include <ArduinoJson.h>

EC_Salinity EC;
ISE_pH  pH;
ISE_ORP ORP(0x3d);
String  buffer, cmd, p1, p2, json;

void ph_reset()
{
  pH.reset();
}

void ec_reset()
{
  EC.reset();
}

void ph_connected()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  root["pH_connected"] = pH.connected();

  root.printTo(json);
  Serial.println(json);
}

void ec_connected()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  root["EC_connected"] = EC.connected();

  root.printTo(json);
  Serial.println(json);
}

void orp_connected()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  root["ORP_connected"] = ORP.connected();

  root.printTo(json);
  Serial.println(json);
}

void ph_cal()
{
  if (p1.length())
  {
    pH.calibrateSingle(p1.toFloat());
  }

  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  json = "";

  float offset = pH.getCalibrateOffset();
  if (isnan(offset)) {
    root["pH_offset"] = "-";
  }
  else {
    root["pH_offset"] = pH.getCalibrateOffset();
  }

  root.printTo(json);
  Serial.println(json);
}

void temperature()
{
  if (p1.length())
  {
    pH.setTemp(p1.toFloat());
  }
  else
  {
    pH.measureTemp();
  }

  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  json              = "";
  root["temp"]      = floor(pH.tempC * 100.0 + 0.5) / 100.0;
  root["temp_unit"] = "C";
  root.printTo(json);
  Serial.println(json);
}

void mv()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json                 = "";
  root["slot_3_value"] = floor(pH.measuremV() * 100.0 + 0.5) / 100.0;
  root["slot_3_unit"]  = "mV";
  root.printTo(json);
  Serial.println(json);
}

void ph()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json                 = "";
  root["slot_1_value"] = floor(pH.measurepH() * 100.0 + 0.5) / 100.0;
  root["slot_1_unit"]  = "pH";
  root.printTo(json);
  Serial.println(json);
}

void orp()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json                 = "";
  root["slot_3_value"] = floor(ORP.measuremV() * 100.0 + 0.5) / 100.0;
  root["slot_3_unit"]  = "mV";
  root.printTo(json);
  Serial.println(json);
}

void low()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  if (p1.length())
  {
    pH.calibrateProbeLow(p1.toFloat());
  }

  float lowReading = pH.getCalibrateLowReading();
  if (isnan(lowReading)) {
    root["pH_Low_Reading"] = "-";
  }
  else {
    root["pH_Low_Reading"] = lowReading;
  }

  float lowReference = pH.getCalibrateLowReference();
  if (isnan(lowReference)) {
    root["pH_Low_Reference"] = "-";
  }
  else {
    root["pH_Low_Reference"] = lowReference;
  }

  root.printTo(json);
  Serial.println(json);
}

void high()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  if (p1.length())
  {
    pH.calibrateProbeHigh(p1.toFloat());
  }

  float highReading = pH.getCalibrateHighReading();
  if (isnan(highReading)) {
    root["pH_High_Reading"] = "-";
  }
  else {
    root["pH_High_Reading"] = highReading;
  }

  float highReference = pH.getCalibrateHighReference();
  if (isnan(highReference)) {
    root["pH_High_Reference"] = "-";
  }
  else {
    root["pH_High_Reference"] = highReference;
  }

  root.printTo(json);
  Serial.println(json);
}

void dual_point()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  if (p1.length())
  {
    pH.useDualPoint(p1.toInt());
  }

  root["pH_DP_Enabled"] = pH.usingDualPoint(); // ? "yes" : "no";
  root.printTo(json);
  Serial.println(json);
}

void cal_ec()
{
  if (p1.length())
  {
    EC.calibrateEC(p1.toFloat());
  }

  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  json = "";

  root["EC_calibration"] = EC.getCalibrationEC();
  root.printTo(json);
  Serial.println(json);
}

void cal_sw()
{
  if (p1.length())
  {
    EC.calibrateSW(p1.toFloat());
  }
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  json = "";

  root["SW_calibration"] = EC.getCalibrationSW();
  root.printTo(json);
  Serial.println(json);
}

void ph_temp_comp()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  if (p1.length())
  {
    pH.useTemperatureCompensation(p1.toInt());
  }

  root["pH_TC_Enabled"] = pH.usingTemperatureCompensation(); // ? "yes" : "no";
  root.printTo(json);
  Serial.println(json);
}

void ec_temp_comp()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  if (p1.length())
  {
    EC.useTemperatureCompensation(p1.toInt());
  }

  root["EC_TC_Enabled"] = EC.usingTemperatureCompensation();
  root.printTo(json);
  Serial.println(json);
}

void ec_temp_const()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json = "";

  if (p1.length())
  {
    EC.setTempConstant(p1.toFloat());
    Serial.println(p1.toFloat());
  }

  root["EC_temp_const"] = EC.getTempConstant();
  root.printTo(json);
  Serial.println(json);
}

void ec()
{
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  json                 = "";
  root["slot_2_value"] = floor(EC.measureEC(false) * 100.0 + 0.5) / 100.0;
  root["slot_2_unit"]  = "mS";
  root.printTo(json);
  Serial.println(json);
}

void cmd_run()
{
  if (cmd == "ph_reset")
    ph_reset();
  if (cmd == "ec_reset")
    ec_reset();
  if ((cmd == "temp") || (cmd == "t"))
    temperature();
  if (cmd == "ph_tc")
    ph_temp_comp();
  if (cmd == "ec_tc")
    ec_temp_comp();
  if (cmd == "ec_temp_const")
    ec_temp_const();
  if (cmd == "ec")
    ec();
  if (cmd == "cal-ec")
    cal_ec();
  if (cmd == "cal-sw")
    cal_sw();
  if (cmd == "mv")
    mv();
  if (cmd == "ph")
    ph();
  if (cmd == "low")
    low();
  if (cmd == "high")
    high();
  if (cmd == "dp")
    dual_point();
  if (cmd == "ph_cal")
    ph_cal();
  if (cmd == "ph_connected")
    ph_connected();
  if (cmd == "ec_connected")
    ec_connected();
  if (cmd == "orp_connected")
    orp_connected();
  if (cmd == "orp")
    orp();
}

void cli_process()
{
  while (Serial.available())
  {
    char c = Serial.read();

    switch (c)
    {
    case '\n': // new line
      // Serial.println();
      cmd = buffer.substring(0, buffer.indexOf(" ", 0));
      cmd.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p1 = buffer.substring(0, buffer.indexOf(" ", 0));
      p1.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p2 = buffer.substring(0, buffer.indexOf(" ", 0));
      p2.trim();
      cmd_run();
      buffer = "";
      break;

    case '\b': // backspace
      buffer.remove(buffer.length() - 1);
      break;

    default: // everything else
      buffer += c;
    }
  }
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  cli_process();
}
