// 遍历I2C地址-检索i2c设备
#include <Wire.h>

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("nI2C Scanner");
}

void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address (without R/W)0x");
      if (address<16) 
        Serial.print("0");

      Serial.print(address,HEX);
      Serial.print("  ,");

       Serial.print("(with R/W)0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address<<1,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices foundn");
  else
    Serial.println("donen");

  delay(5000);           // wait 5 seconds for next scan
}
