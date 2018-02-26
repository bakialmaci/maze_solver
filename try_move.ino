                      //Motors pin_IO
#define IN_1 10
#define IN_2 11   
#define IN_3 9
#define IN_4 8

#define PWM_R 5   
#define PWM_L 6    
                      //Sensors pin_IO
#define TRIGPIN_F 4
#define ECHOPIN_F 3
#define TRIGPIN_R 13
#define ECHOPIN_R 12
#define TRIGPIN_L 7
#define ECHOPIN_L 2
                      //Variables
long cycle;
long distance_f;
long distance_r;
long distance_l;
int pid_pwm;

void setup() 
{
  pinMode(IN_1, OUTPUT);  
  pinMode(IN_2, OUTPUT);  
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  
  pinMode(PWM_R , OUTPUT); 
  pinMode(PWM_L , OUTPUT);
  
  pinMode(TRIGPIN_F, OUTPUT);
  pinMode(ECHOPIN_F,INPUT);
  
  Serial.begin(9600);                
}

void f_sensor()
{
  digitalWrite(TRIGPIN_F, LOW); 
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_F, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_F, LOW);  
  cycle   = pulseIn(ECHOPIN_F, HIGH); 
  distance_f = cycle /29.1/2;
}

void r_sensor()
{
  digitalWrite(TRIGPIN_R, LOW); 
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_R, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_R, LOW);  
  cycle   = pulseIn(ECHOPIN_R, HIGH); 
  distance_r = cycle /29.1/2;
}

void l_sensor()
{
  digitalWrite(TRIGPIN_L, LOW); 
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_L, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_L, LOW);  
  cycle   = pulseIn(ECHOPIN_L, HIGH); 
  distance_l = cycle /29.1/2;
}

void handbrake()
{
  analogWrite(PWM_R,  255);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  HIGH);  
                          
  analogWrite(PWM_L,  255);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(30);
  
  analogWrite(PWM_R,  255);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  LOW);  
                          
   analogWrite(PWM_L,  255);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
delay(500);
}

void pid()
{
  l_sensor();
  r_sensor();
  pid_pwm = (distance_l - distance_r)*5;
}

void forward()
{
  pid();           
  analogWrite(PWM_R,  100 - pid_pwm);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  100 + pid_pwm);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
}

void turn_r()
{
  analogWrite(PWM_R,  100);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  100);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(500);
  
  analogWrite(PWM_R,  255);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  HIGH);  
                          
  analogWrite(PWM_L,  255);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  delay(30);
  
  analogWrite(PWM_R,  255);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  255);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
}

void turn_l()
{
  analogWrite(PWM_R,  100);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  HIGH);  
                          
  analogWrite(PWM_L,  100);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  delay(500);
  
  analogWrite(PWM_R,  255);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  255);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(30);
  
  analogWrite(PWM_R,  255);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  255);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
}

void turn_b()
{
  turn_l();
  delay(200);
  turn_l();
  delay(200);
}

void control()
{
  f_sensor();
  r_sensor();
  l_sensor();
  if(distance_f <=10)
  {
    handbrake();
    if(distance_r >= 15)
    {
      turn_r();
    }
    else if(distance_r >= 15)
    {
      turn_l();
    }
    else
    {
      turn_b();
    }
  }
  else
  {
    forward();
  }
}

void loop() 
{
  //control();
  f_sensor();
  Serial.println(distance_f);
  r_sensor();
   Serial.println(distance_r);
  l_sensor();
   Serial.println(distance_l);
   delay(1000);
  
}
