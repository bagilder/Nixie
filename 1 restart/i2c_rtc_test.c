/*
bgilder
nixie clock i2c test for rx-8025sa RTC module
30jul2021
*/


//what if we run it in 24 hour mode always and just convert the time on our end? that will avoid mid-run 12/24 conversion errors


/*idea for precision adjustment:
take a millis() when time is first set, then check every time the menu button is held to see if precision offset has changed, and if it has, 
then measure difference between now and millis() and divide it out to send to the RTC to set the precision offset. or something*/

//if using 1hz interrupt /INTA control scheme instead of i2c, can use digital pin 2 as interrupt input (will need to move mux bin pin currently on 2)
//can have the interrupt increment a byte and then whenever the mux happens, can use that byte and just add to the seconds thing and ripple from there. and decrement byte after adding to seconds.
//this allows for multiple seconds during the hour change times, because it will increment the byte behind the scenes and can add things to the seconds at the end.
/*e.g.
while(theByte--)
{seconds_add_function_with_ripple_probably();}
*/

#include <Wire.h>


#define hourmode12 0b00000011  //control register E, sets up periodic iterrupt at 1hz
#define hourmode24  0b00100011  //control register E, sets up periodic interrupt at 1hz
#define hourmodereg 0xe0
#define startupreset 0b00100000 //control register F, 2.2v dropout mode
#define startupreg 0xf0
#define readaddr 0x65 //i2c slave address with read bit active
#define writeaddr 0b0110010  //i2c slave address in write configuration
#define secreg 0x00
#define minreg 0x10
#define hrreg 0x20
#define clockadjreg 0x70

uint8_t seconds=56;
uint8_t minutes=59;
uint8_t hours=21;
uint8_t RX8025_time[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t hourMode=hourmode24;


uint8_t bcd2bin (uint8_t val) 
{ return val - 6 * (val >> 4); 
}

uint8_t bin2bcd (uint8_t val) 
{ return val + 6 * (val / 10); 
}


void setup()
{
  uint8_t i2cSendData[16]={seconds,minutes,hours,0,0,0,0,0,0,0,0,0,0,0,bcd2bin(hourMode),bcd2bin(startupreset)};
  hourMode=hourmode24;
  unsigned char i=0;
  Serial.begin(115200);
  delay(1000);
  Wire.begin();
  Wire.beginTransmission(writeaddr);
  Wire.write(0xE0); //WHY THE FUCK ISN'T THIS WRITING TO THE E REGISTER????????????????? //it was. i just wasn't displaying it.
  Wire.write(0b00100011);
  Wire.endTransmission();
  Serial.write("\n\n\n\nbegan wire\n");

  Wire.beginTransmission(writeaddr);
  Wire.write(0x00);
  //Wire.endTransmission();//
  Wire.requestFrom(writeaddr,16);
  //RX8025_time[i]= Wire.read();//not use
  while(Wire.available())
  { 
    //RX8025_time[i]= bcd2bin(Wire.read());
    RX8025_time[i]= Wire.read();

    Serial.write("\n");
    Serial.print(RX8025_time[i]);
        i++;
  }
  Wire.endTransmission();//

  
  Wire.beginTransmission(writeaddr);
  Wire.write((byte)startupreg);
  Wire.write((byte)startupreset);
  Wire.endTransmission();
  Serial.write("\nreset write\n");
  Serial.print((byte)startupreg);
  Serial.write("- ");
  Serial.print((byte)startupreset);

  /*this goes after we set hour mode in the menu*/
  Wire.beginTransmission(writeaddr);
  Wire.write((byte)hourmodereg);
  Wire.write((byte)hourmode24);
  Wire.endTransmission();

  Serial.write("\n24 hour mode write\n");
  Serial.print(hourmodereg);
  Serial.write("- ");
  Serial.print(hourmode24);

  Wire.beginTransmission(writeaddr);
  Wire.write((byte)secreg);
for(byte f=0;f<16;f++)
{
  Wire.write(bin2bcd(i2cSendData[f]));
}
  Wire.endTransmission();

  
 /*   Serial.write("\nwrite 12:34:56\n");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(bin2bcd(12)>>(7-h)));}
    Serial.write(" ");
    Serial.print(bin2bcd(12));
    Serial.write("  ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(bin2bcd(34)>>(7-h)));}
    Serial.write(" ");
    Serial.print(bin2bcd(34));
    Serial.write("  ");
      for (byte h=0;h<8;h++)
  { Serial.print(0x1&(bin2bcd(56)>>(7-h)));}
    Serial.write(" ");
    Serial.print(bin2bcd(56));
    //Serial.write("\n");
    */

  Serial.write("\n\nattempting group read method-for loop ");

    i=0;
  Wire.beginTransmission(writeaddr);
  Wire.write(0x00);
 // Wire.endTransmission();//
  Wire.requestFrom(writeaddr,16);
  //RX8025_time[i]= Wire.read();//not use
  while(Wire.available())
  { 
    //RX8025_time[i]= bcd2bin(Wire.read());
    RX8025_time[i]= Wire.read();

    Serial.write("\n");
    Serial.print(RX8025_time[i]);
        i++;
  }
  Wire.endTransmission();//

  seconds=RX8025_time[0];
  minutes=RX8025_time[1];
  hours=RX8025_time[2];
  Serial.write("\nhours = ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(hours>>(7-h)));}
  Serial.write(" ");
  Serial.print(hours);
  Serial.write("\nminutes = ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(minutes>>(7-h)));}
  Serial.write(" ");
  Serial.print(minutes);
  Serial.write("\nseconds = ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(seconds>>(7-h)));}
  Serial.write(" ");
  Serial.print(seconds);

  /*delay(1000);
  Serial.write("\n\nattempting group read method-fast read address\n");
  Wire.beginTransmission(writeaddr);
  Wire.read();  //this is the garbage read from settings registerF, just getting it out of the way
  seconds=(Wire.read());
  minutes=(Wire.read());
  hours=(Wire.read());
  Wire.endTransmission();
  Serial.write("hours = ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(hours>>(7-h)));}
  Serial.write(" ");
  Serial.print(hours);
  Serial.write("\nminutes = ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(minutes>>(7-h)));}
  Serial.write(" ");
  Serial.print(minutes);
  Serial.write("\nseconds = ");
  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(seconds>>(7-h)));}
  Serial.write(" ");
  Serial.print(seconds);
*/


pinMode(2,INPUT_PULLUP);
pinMode(13,OUTPUT);
attachInterrupt(digitalPinToInterrupt(2),isr,RISING);

}

volatile byte hey=0;

void isr()
{
  hey++;
  Serial.print(hey);
  Serial.write(" oy!");

}

void loop()
{
  Serial.print(hey);
  delay(250);
    unsigned char  i=0;
  Wire.beginTransmission(writeaddr);
  Wire.write(0x00);
 // Wire.endTransmission();//
  Wire.requestFrom(writeaddr,16);
  //RX8025_time[i]= Wire.read();//not use
  while(Wire.available())
  { 
    //RX8025_time[i]= bcd2bin(Wire.read());
    RX8025_time[i]= Wire.read();

    Serial.write("\n");
    Serial.print((RX8025_time[i]));
        i++;
  }
  Wire.endTransmission();//
  Serial.write("\n");
  digitalWrite(13, digitalRead(3));

  if(hey>5)
  {hey=0;}

}







/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**************from the rtc library*******************
static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }


static uint8_t read_i2c_register(uint8_t addr, uint8_t reg,
                                 TwoWire *wireInstance) {
  wireInstance->beginTransmission(addr);
  wireInstance->send((byte)reg);
  wireInstance->endTransmission();

  wireInstance->requestFrom(addr, (byte)1);
  return wireInstance->_I2C_READ();
}


static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val,
                               TwoWire *wireInstance) {
  wireInstance->beginTransmission(addr);
  wireInstance->send((byte)reg);
  wireInstance->send((byte)val);
  wireInstance->endTransmission();
}


void RTC_DS1307::adjust(const DateTime &dt) {
  wireInstance->beginTransmission(DS1307_ADDRESS);
  wireInstance->send((byte)0); // start at location 0
  wireInstance->send(bin2bcd(dt.second()));
  wireInstance->send(bin2bcd(dt.minute()));
  wireInstance->send(bin2bcd(dt.hour()));
 /* wireInstance->send(bin2bcd(0));
  wireInstance->send(bin2bcd(dt.day()));
  wireInstance->send(bin2bcd(dt.month()));
  wireInstance->send(bin2bcd(dt.year() - 2000U));*
  wireInstance->endTransmission();
}


DateTime RTC_DS1307::now() {
  wireInstance->beginTransmission(DS1307_ADDRESS);
  wireInstance->_I2C_WRITE((byte)0);
  wireInstance->endTransmission();

  wireInstance->requestFrom(DS1307_ADDRESS, 7);
  uint8_t ss = bcd2bin(wireInstance->_I2C_READ() & 0x7F);
  uint8_t mm = bcd2bin(wireInstance->_I2C_READ());
  uint8_t hh = bcd2bin(wireInstance->_I2C_READ());
  wireInstance->_I2C_READ();
  uint8_t d = bcd2bin(wireInstance->_I2C_READ());
  uint8_t m = bcd2bin(wireInstance->_I2C_READ());
  uint16_t y = bcd2bin(wireInstance->_I2C_READ()) + 2000U;

  return DateTime(y, m, d, hh, mm, ss);





  /*************************now from the rx8025 library***************

  void setRtcTime(void)
{
  Wire.beginTransmission(RX8025_address);
  Wire.print(0x00);
  for(unsigned char i=0; i<7; i++)
  {
    Wire.write(RX8025_time[i]);
  }
  Wire.endTransmission();
}

//===============================================
uint8_t bcd2bin (uint8_t val) 
{ return val - 6 * (val >> 4); 
}

uint8_t bin2bcd (uint8_t val) 
{ return val + 6 * (val / 10); 
}


unsigned char RX8025_time[7];


unsigned char RX8025_Control[2]=
{
  0x20,0x00
};



void getRtcTime(void)
{
  unsigned char i=0;
  Wire.beginTransmission(RX8025_address);
  Wire.print(0x00);
  Wire.endTransmission();//
  Wire.requestFrom(RX8025_address,8);
  RX8025_time[i]= Wire.read();//not use
  while(Wire.available())
  { 
    RX8025_time[i]= Wire.read();
    i++;
  }
  Wire.endTransmission();//

  year   = bcd2bin(RX8025_time[6]&0xff);
  month  = bcd2bin(RX8025_time[5]&0x1f);
  date   = bcd2bin(RX8025_time[4]&0x3f);
  week   = bcd2bin(RX8025_time[3]&0x07);
  hour   = bcd2bin(RX8025_time[2]&0x3f);
  minute = bcd2bin(RX8025_time[1]&0x7f);
  second = bcd2bin(RX8025_time[0]&0x7f);
}


void RX8025_init(void)
{
  Wire.begin();
  Wire.beginTransmission(RX8025_address);//clear power on reset flag, set to 24hr format
  Wire.write(0xe0);
  for(unsigned char i=0; i<2; i++)
  {
    Wire.write(RX8025_Control[i]);
  }
  Wire.endTransmission();
  //setRtcTime();
}
*/