#define MotorSpeedSet            0x82
#define PWMFrequenceSet          0x84
#define DirectionSet             0xaa
#define MotorSetA                0xa1
#define MotorSetB                0xa5
#define Nothing                  0x01

#define I2CMotorDriverAdd        0x0f    // Set the address of the I2CMotorDriver, set by onboard switch


void MotorSpeedSetAB(unsigned char MotorSpeedA , unsigned char MotorSpeedB)
{
    MotorSpeedA=map(MotorSpeedA,0,100,0,255);
    MotorSpeedB=map(MotorSpeedB,0,100,0,255);
    Wire.beginTransmission(I2CMotorDriverAdd);  //transmit to device I2CMotorDriverAdd
    Wire.write(MotorSpeedSet);                  //set pwm header 
    Wire.write(MotorSpeedA);                    //send pwma 
    Wire.write(MotorSpeedB);                    //send pwmb      
    Wire.endTransmission();                     //stop transmitting
}


void MotorPWMFrequenceSet(unsigned char Frequence)
{    
    Wire.beginTransmission(I2CMotorDriverAdd);  //transmit to device I2CMotorDriverAdd
    Wire.write(PWMFrequenceSet);                //set frequence header
    Wire.write(Frequence);                      //send frequence 
    Wire.write(Nothing);                        //need to send this byte as the third byte(no meaning)    
    Wire.endTransmission();                     //stop transmitting
}


void MotorDirectionSet(unsigned char Direction)
{                                               //Adjust the direction of the motors 0b0000 I4 I3 I2 I1
    Wire.beginTransmission(I2CMotorDriverAdd);  //transmit to device I2CMotorDriverAdd
    Wire.write(DirectionSet);                   //Direction control header
    Wire.write(Direction);                      //send direction control information
    Wire.write(Nothing);                        //need to send this byte as the third byte(no meaning)   
    Wire.endTransmission();                     //stop transmitting 
}


void MotorDriectionAndSpeedSet(unsigned char Direction,unsigned char MotorSpeedA,unsigned char MotorSpeedB)
{   //you can adjust the driection and speed together
    MotorDirectionSet(Direction);
    MotorSpeedSetAB(MotorSpeedA,MotorSpeedB); 
}


//runs for x steps (52steps/rev)
void run_for_steps (unsigned int steps)
{
    unsigned int current_step;
    for(current_step = 0; current_step < steps; ++current_step)
      {
          MotorSpeedSetAB(100,100);//when driving a stepper, the speed should be set to 100;
          delay(10);
          MotorDirectionSet(0b0001);//1  ({1} 3 6 12) [{1} 2 4 8]... 9 
          delay(4);
          MotorDirectionSet(0b0011);//3
          delay(4);
          MotorDirectionSet(0b0010);//2
          delay(4);
          MotorDirectionSet(0b0110);//6
          delay(4);
          MotorDirectionSet(0b0100);//4
          delay(4);
          MotorDirectionSet(0b1100);//12
          delay(4);
          MotorDirectionSet(0b1000);//8
          delay(4);
          MotorDirectionSet(0b1001);//9
          delay(4);
      }
}
