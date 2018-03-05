int maze [14][14] = {
  {99,99,99,99,99,99,99,99,99,99,99,99,99,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99},
  {99,99,99,99,99,99,99,99,99,99,99,99,99,99}
  };
                                                //Motors pin_IO
#define IN_1 11
#define IN_2 10   
#define IN_3 8
#define IN_4 9

#define PWM_R 5   
#define PWM_L 6    
                                                //Sensors pin_IO
#define TRIGPIN_F 4
#define ECHOPIN_F 3
#define TRIGPIN_R 13
#define ECHOPIN_R 12
#define TRIGPIN_L 7
#define ECHOPIN_L 2

                                               //FACILITATING DEFINITIONS
#define TURN_DELAY 400
#define PWM 100

float t=0;                                           //Variables
int hata=0;
long cycle;
long distance_f;
long distance_r;
long distance_l;

long distance_fp;
long distance_rp;
long distance_lp;

int pid_pwm;

int x = 1;
int y = 1;

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
  pinMode(TRIGPIN_R, OUTPUT);
  pinMode(ECHOPIN_R,INPUT);
  pinMode(TRIGPIN_L, OUTPUT);
  pinMode(ECHOPIN_L,INPUT);
  
  Serial.begin(9600);                
}
    
                                                   //SENSORS

void f_sensor()
{
  digitalWrite(TRIGPIN_F, LOW); 
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_F, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_F, LOW);  
  cycle   = pulseIn(ECHOPIN_F, HIGH); 
  distance_f = cycle /29.1/2;
  distance_fp = distance_f%20; // I am try to prevent blocking pid.
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
  distance_rp = distance_r%25; // I am try to prevent blocking pid.
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
  distance_lp = distance_l%25; // I am try to prevent blocking pid.
}


                                                  //CONTROLS

void control_f()
{
  f_sensor();
  r_sensor();
  l_sensor();
  
  if(distance_f >= 10) //&& maze[y+1][y] != 99)
  {
    /*y+=1;
    maze[x][y] += 1;*/

    forward();
  }
  else if (distance_r >= 15) //&& maze[x-1][y] != 99)
  {
    /*x-=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_r();
    forward_r();
  }
  else if (distance_l >= 15 ) //&& maze[x+1][y] != 99)
  {
    /*x+=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_l();
    forward_l();
  }
  else
  {
    //maze[x][y] = 99;
    
    handbrake();
    turn_b();
    forward_b();
  }
}

void control_l()
{
    f_sensor();
  r_sensor();
  l_sensor();
  
  if(distance_f >= 10) //&& maze[x+1][y] != 99)
  {
    /*x+=1;
    maze[x][y] += 1;*/
    
    forward_l();
  }
  else if (distance_r >= 15) //&& maze[x][y+1] != 99)
  {
    /*y+=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_r();
    forward();
  }
  else if (distance_l >= 15) //&& maze[x][y-1] != 99)
  {
    /*y-=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_l();
    forward_b();
  }
  else
  {
    handbrake();
    turn_b();
    forward_r();
  }
  
}

void control_r()
{
  f_sensor();
  r_sensor();
  l_sensor();
  
  if(distance_f >= 10 )//&& maze[x-1][y] != 99)
  {
    /*x-=1;
    maze[x][y] += 1;*/
    
    forward_r();
  }
  else if (distance_r >= 10 )//&& maze[x][y-1] != 99)
  {
   /* y-=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_r();
    forward_b();
  }
  else if (distance_l >= 10)// && maze[x][y+1] != 99)
  {
    /*y+=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_l();
    forward();
  }
  else
  {
    handbrake();
    forward_l();
  }
}

void control_b()
{
    f_sensor();
  r_sensor();
  l_sensor();
  
  if(distance_f >= 10 && maze[x][y-1] != 99)
  {
    /*y-=1;
    maze[x][y] += 1;*/
    
    forward_b();
  }
  else if (distance_r >= 10 && maze[x+1][y] != 99)
  {
    /*x+=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_r();
    forward_l();
  }
  else if (distance_l >= 10 && maze[x-1][y] != 99)
  {
   /* x-=1;
    maze[x][y] += 1;*/
    
    handbrake();
    turn_l();
    forward_r();
  }
  else
  {
    handbrake();
    forward();
  }
}

                                                  //MOVES

void pid()
{
  l_sensor();
  r_sensor();
  t=millis();
  //int pid2= ((distance_lp-distance_rp)-(hata))/t;
  pid_pwm = ((distance_lp - distance_rp)*6);//+(pid2*0.6);
  //hata=distance_lp-distance_rp;

}

void forward()
{
  pid();           
  analogWrite(PWM_R,  PWM + pid_pwm);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  PWM - pid_pwm);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  control_f();

}

void forward_l()
{
  pid();           
  analogWrite(PWM_R,  PWM + pid_pwm);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  PWM - pid_pwm);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  control_l();
}

void forward_r()
{
  pid();           
  analogWrite(PWM_R,  PWM + pid_pwm);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  PWM - pid_pwm);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  control_r();
}

void forward_b()
{
  
  pid();           
  analogWrite(PWM_R,  PWM + pid_pwm);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  PWM - pid_pwm);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  control_b();
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

                                                    //TURNS

void turn_l()
{
  analogWrite(PWM_R,  PWM);  
  digitalWrite(IN_1, HIGH); 
  digitalWrite(IN_2,  LOW);  
                          
  analogWrite(PWM_L,  PWM);  
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(TURN_DELAY);
  
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

void turn_r()
{
  analogWrite(PWM_R,  PWM);  
  digitalWrite(IN_1, LOW); 
  digitalWrite(IN_2,  HIGH);  
                          
  analogWrite(PWM_L,  PWM);  
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  delay(TURN_DELAY);
  
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

void loop() 
{
  control_f();
}
