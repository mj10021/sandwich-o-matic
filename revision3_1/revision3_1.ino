// includes the servo and i2c (wire) library
#include <Servo.h>
#include <Wire.h>


//establishes swich input pins for the dispensers
#define MEATPIN1 2
#define CHEESEPIN1 4
#define MEATPIN2 7
#define CHEESEPIN2 8

//initialize servos
Servo meatServo1;
Servo cheeseServo1;
Servo meatServo2;
Servo cheeseServo2;
Servo bServo1;
Servo bServo2; 

//establishes defalt switch state
int meatState1 = 0;
int meatState2 = 0;
int cheeseState1 = 0;
int cheeseState2 = 0;

//count that matches each switch to a dispenser
int current_pin;

//count that goes up by one after each swich check
int conveyor_count;

//function to rotate dispenser 26*  FIX IT!!!!
void activate_dispenser (Servo servo, Servo* my_servos)
{   //stores servo position as a varaible
    int pos;
    for(pos = servo.read(); pos < 27; ++pos)
    {
        servo.write(pos);
        delay(20);
        Serial.write("rotated  ");
        if (pos == 26)
        {
            Serial.write("done");
            break;
        }
    }
}

//function to check switch input
boolean check_pin (int pin)
{
    int my_pins[] = {1, MEATPIN1, CHEESEPIN1, MEATPIN2, CHEESEPIN2, 1};
    Serial.write("checking  ");
    if(conveyor_count == pin && my_pins[conveyor_count] == 1)
    {
        Serial.write("yep.");
        return true;
    }
    else
    {
        Serial.write("nope.");
    }
}
    
void setup(void) 
{

    Serial.begin(9600);

    //attaches each servo to a PWM pin
    meatServo1.attach(3);
    cheeseServo1.attach(5);
    meatServo2.attach (6);
    meatServo2.attach (9);
    bServo1.attach (10);
    bServo2.attach (11);

    //initializes each swich as an input
    pinMode (MEATPIN1,INPUT);
    pinMode (CHEESEPIN1, INPUT);
    pinMode (MEATPIN2, INPUT);
    pinMode (CHEESEPIN2,INPUT);

    Wire.begin(); // join i2c bus
    delayMicroseconds(10000); //wait for motor driver to initialize
}

void loop(void)
{
    //checks which toppings are chosen
    meatState1 = digitalRead(MEATPIN1);
    cheeseState1 = digitalRead(CHEESEPIN1);
    meatState2 = digitalRead(MEATPIN2);
    cheeseState2 = digitalRead(CHEESEPIN2);
    
    //array that points to whichever dispenser corresponds with the conveyor count
    Servo my_servos[] = {bServo1, meatServo1, cheeseServo1, meatServo2, cheeseServo2, bServo2};


    Serial.write("begin  ");
    //increase conveyor position count by one after each topping dispenser
    for(conveyor_count = 0; conveyor_count < 7; ++conveyor_count)
    {
        //run conveyor belt and wait for dispensor
        run_for_steps(100);
        delay(1000);
        Serial.write("run stepper  ");
        
        if(conveyor_count == 6)
        {
            delay(429496294);//basically turn off until reset
        }
            
        //for each position
        for(current_pin = 0; current_pin < 5; ++current_pin)
        {   
            Serial.write("check pin  ");
            //if the switch is active
            if (check_pin(current_pin) == true)
            {   // dispense food item
                activate_dispenser(my_servos[conveyor_count], my_servos);
            }
            else
            {
                Serial.write("nope  ");
                break;
            }
        }
    }
}
