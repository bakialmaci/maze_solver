#include <math.h>

int maze [14][14] =
{
  {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
  {99,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99},
  {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}
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
#define TURN_DELAY 1000
#define PWM 100
#define LED_PIN A5
#define DISTANCE_DIFF_TOL 1
//Variables
long cycle;
long distance_f;
long distance_r;
long distance_l;
long distance_mf;
long distance_mr;
long distance_ml;
int pid_pwm;
int distance_diff;

int x = 1;
int y = 1;

//void f_sensor();
//void r_sensor();
//void l_sensor();
//void adjuster();
//int adjuster_turn(int dist);
//void control_f();
//void control_l();
//void control_r();
//void control_b();
//void pid();
//void manuel();
//void forward();
//void forward_r();
//void forward_l();
//void forward_b();
//void handbrake();
//void turn_l();
//void turn_r();
//void turn_b();
//void led();


void setup()
{
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  pinMode(PWM_R , OUTPUT);
  pinMode(PWM_L , OUTPUT);

  pinMode(TRIGPIN_F, OUTPUT);
  pinMode(ECHOPIN_F, INPUT);
  pinMode(TRIGPIN_R, OUTPUT);
  pinMode(ECHOPIN_R, INPUT);
  pinMode(TRIGPIN_L, OUTPUT);
  pinMode(ECHOPIN_L, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

//********************************SENSORS***********************************

void f_sensor()
{
  digitalWrite(TRIGPIN_F, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_F, LOW);
  cycle   = pulseIn(ECHOPIN_F, HIGH);
  distance_f = cycle / 29.1 / 2;
  distance_mf = distance_f % 20;
  Serial.println(distance_f);
  delay(10);

}

void r_sensor()
{
  digitalWrite(TRIGPIN_R, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_R, LOW);
  cycle   = pulseIn(ECHOPIN_R, HIGH);
  distance_r = cycle / 29.1 / 2;
  distance_mr = distance_r % 20;
  delay(10);
}

void l_sensor()
{

  digitalWrite(TRIGPIN_L, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_L, LOW);
  cycle   = pulseIn(ECHOPIN_L, HIGH);
  distance_l = cycle / 29.1 / 2;
  distance_ml = distance_l % 20;
  delay(10);
}


//********************************CONTROLS***********************************


void control_f()
{
  f_sensor();
  l_sensor();
  r_sensor();

  if (distance_f >= 10 && maze[x][y + 1] != 99) //önü açıksa ve çıkmaz yol olmadığı biliniyorsa
  {
    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      forward();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa
    {
      if ((maze[x][y + 1] <= maze[x - 1][y]) && (maze[x][y + 1] <= maze[x + 1][y])) ///**********************
      {
        forward();
      }
      else if (maze[x + 1][y] <= maze[x - 1][y]) ///////////sağadön/////////////////////////
      {
        handbrake();
        turn_r();
        back_pull();
        forward_r();
      }
      else if (maze[x - 1][y] < maze[x + 1][y]) ///////////soladön/////////////////////////
      {
        handbrake();
        turn_l();
        back_pull();
        forward_l();
      }
    }
    else if (distance_r <= 10)//sağ duvar var sol açik
    {
      if (maze[x - 1][y] < maze[x][y + 1]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_l();
        back_pull();
        forward_l();
      }
      else
      {
        forward();
      }
    }
    else if (distance_l <= 10)//sol duvar var sağ açik
    {
      if (maze[x + 1][y] < maze[x][y + 1]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_r();
        back_pull();
        forward_r();
      }
      else
      {
        forward();
      }
    } 
  }
  else if (distance_f < 10 || maze[x][y + 1] == 99) //ön dolu veya 99 ise
  {
    handbrake();

    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      turn_b();
      back_pull();
      forward_b();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafından biri açıksa(ön kapali)
    {
      if (maze[x + 1][y] <= maze[x - 1][y]) ///////////sağadön/////////////////////////
      {
        turn_r();
        back_pull();
        forward_r();
      }
      else if (maze[x - 1][y] < maze[x + 1][y]) ///////////soladön/////////////////////////
      {
        turn_l();
        back_pull();
        forward_l();
      }
    }
    else if (distance_r >= 10)
    {
      turn_r();
      back_pull();
      forward_r();
    }
    else if (distance_l >= 10)
    {
      turn_l();
      back_pull();
      forward_l();
    }

  }
}
void control_l()
{
  f_sensor();
  l_sensor();
  r_sensor();

  if (distance_f >= 10 && maze[x - 1][y] != 99)//önü açıksa ve çıkmaz yol olmadığı biliniyorsa
  {
    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      forward_l();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa
    {
      if ((maze[x - 1][y] <= maze[x][y - 1]) && (maze[x - 1][y] <= maze[x][y + 1])) ///**********************
      {
        forward_l();
      }
      else if (maze[x][y + 1] <= maze[x][y - 1]) ///////////sağadön/////////////////////////
      {
        handbrake();
        turn_r();
        back_pull();
        forward();
      }
      else if (maze[x][y - 1] < maze[x][y + 1]) ///////////soladön/////////////////////////
      {
        handbrake();
        turn_l();
        back_pull();
        forward_b();
      }
    }
    else if (distance_r <= 10)//sağ duvar var sol açik
    {
      if (maze[x][y - 1] < maze[x - 1][y]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_l();
        back_pull();
        forward_b();
      }
      else
      {
        forward_l();
      }
    }
    else if (distance_l <= 10)//sol duvar var sağ açik
    {
      if (maze[x][y + 1] < maze[x - 1][y]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_r();
        back_pull();
        forward();
      }
      else
      {
        forward_l();
      }
    }
  }
  else if (distance_f < 8 || maze[x - 1][y] == 99) //ön dolu ve 99 ise
  {
    handbrake();
    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      turn_b();
      back_pull();
      forward_r();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa(ön kapali)
    {

      if (maze[x][y + 1] <= maze[x][y - 1]) ///////////sağadön/////////////////////////
      {
        turn_r();
        back_pull();
        forward();
      }
      else if (maze[x][y - 1] < maze[x][y + 1]) ///////////soladön/////////////////////////
      {
        turn_l();
        back_pull();
        forward_b();
      }
    }
     else if(distance_r>=10)
      {
        turn_r();
        back_pull();
        forward();
      }
     else if(distance_l>=10)
      {
        turn_l();
        back_pull();
        forward_b();
      }
  }
}
void control_r()
{
  f_sensor();
  l_sensor();
  r_sensor();

  if (distance_f >= 6 && maze[x + 1][y] != 99)//önü açıksa ve çıkmaz yol olmadığı biliniyorsa
  {
    if (distance_l <= 6 && distance_r <= 10)//iki tarafında duvar varsa
    {
      forward_r();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa
    {
      if ((maze[x + 1][y] <= maze[x][y - 1]) && (maze[x + 1][y] <= maze[x][y + 1])) ///**********************
      {
        forward_r();
      }
      else if (maze[x][y - 1] <= maze[x][y + 1]) ///////////sağadön/////////////////////////
      {
        handbrake();
        turn_r();
        back_pull();
        forward_b();
      }
      else if (maze[x][y + 1] < maze[x][y - 1]) ///////////soladön/////////////////////////
      {
        handbrake();
        turn_l();
        back_pull();
        forward();
      }
    }
    else if (distance_r <= 10)//sağ duvar var sol açik
    {
      if (maze[x][y + 1] < maze[x + 1][y]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_l();
        back_pull();
        forward();
      }
      else
      {
        forward_r();
      }
    }
    else if (distance_l <= 10)//sol duvar var sağ açik
    {
      if (maze[x][y - 1] < maze[x + 1][y]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_r();
        back_pull();
        forward_b();
      }
      else
      {
        forward_r();
      }
    }
  }
  else if (distance_f < 8 || maze[x + 1][y] == 99) //ön dolu ve 99 ise
  {
    handbrake();
    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      turn_b();
      back_pull();
      forward_l();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa(ön kapali)
    {
      if (maze[x][y - 1] <= maze[x][y + 1]) ///////////sağadön/////////////////////////
      {
        turn_r();
        back_pull();
        forward_b();
      }
      else if (maze[x][y + 1] < maze[x][y - 1]) ///////////soladön/////////////////////////
      {
        turn_l();
        back_pull();
        forward();
      }
    }
    else if (distance_r >= 10)
    {
      turn_r();
      back_pull();
      forward_b();
    }
    else if (distance_l >= 10)
    {
      turn_l();
      back_pull();
      forward();
    }
  }
}

void control_b()
{
  f_sensor();
  l_sensor();
  r_sensor();
  if (distance_f >= 6 && maze[x][y - 1] != 99) //önü açıksa ve çıkmaz yol olmadığı biliniyorsa
  {
    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      forward_b();

    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa
    {
      if ((maze[x][y - 1] <= maze[x + 1][y]) && (maze[x][y - 1] <= maze[x - 1][y])) ///**********************
      {
        forward_b();
      }
      else if (maze[x - 1][y] <= maze[x + 1][y]) ///////////sağadön/////////////////////////
      {
        handbrake();
        turn_r();
        back_pull();
        forward_l();
      }
      else if (maze[x + 1][y] < maze[x - 1][y]) ///////////soladön/////////////////////////
      {
        handbrake();
        turn_l();
        back_pull();
        forward_r();
      }
    }
    else if (distance_r <= 10)//sağ duvar var sol açik
    {
      if (maze[x + 1][y] < maze[x][y - 1]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_l();
        back_pull();
        forward_r();
      }
      else
      {
        forward_b();
      }
    }
    else if (distance_l <= 10)//sol duvar var sağ açik
    {
      if (maze[x - 1][y] < maze[x][y - 1]) //gitmediğin noktaya dön
      {
        handbrake();
        turn_r();
        back_pull();
        forward_l();
      }
      else
      {
        forward_b();
      }
    }
  }

  else if (distance_f < 8 || maze[x][y - 1] == 99) //ön dolu veya 99 ise
  {
    if (distance_l <= 10 && distance_r <= 10)//iki tarafında duvar varsa
    {
      handbrake();
      turn_b();
      back_pull();
      forward();
    }
    else if (distance_l >= 10 && distance_r >= 10)//iki tarafı da açıksa(ön kapali)
    {

      if (maze[x - 1][y] <= maze[x + 1][y]) ///////////sağadön/////////////////////////
      {
        handbrake();
        turn_r();
        back_pull();
        forward_l();
      }
      else if (maze[x + 1][y] < maze[x - 1][y]) ///////////soladön/////////////////////////
      {
        handbrake();
        turn_l();
        back_pull();
        forward_r();
      }
    }
    else if(distance_r>=10)
    {
         handbrake();
        turn_r();
        back_pull();
        forward_l();
    }
    else if(distance_l>=10)
    {
        handbrake();
        turn_l();
        back_pull();
        forward_r(); 
    }
  }
}
//**********************************MOVES**********************************

void pid()
{
  l_sensor();
  r_sensor();
  pid_pwm = (distance_ml - distance_mr) * 5;
}

void manuel()
{
  for (int i = 0; i <= 11; i++)
  {
    pid();
    analogWrite(PWM_R,  PWM + pid_pwm); //SAG
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2,  LOW);

    analogWrite(PWM_L,  PWM - pid_pwm); //SOL
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4,  LOW);
    delay(25);
  }
}

void forward()
{

  if (distance_mf <= 6 && distance_mf >= 4)
  {
    //handbrake();
    if (distance_f >= 7 && maze[x][y + 1] != 99)
    {
      y += 1;
      maze[x][y] += 1;
      led();
      manuel();
    }
  }
  else
  {
    pid();
    analogWrite(PWM_R,  PWM + pid_pwm); //SAG
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2,  LOW);

    analogWrite(PWM_L,  PWM - pid_pwm); //SOL
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4,  LOW);
  }
  control_f();

}

void forward_l()
{
  if (distance_mf >= 4 && distance_mf <= 6   )
  {
    if (distance_f >= 7 && maze[x - 1][y] != 99)
    {
      x -= 1;
      maze[x][y] += 1;
      led();
      manuel();
    }
  }
  else
  {
    pid();
    analogWrite(PWM_R,  PWM + pid_pwm); //SAG
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2,  LOW);

    analogWrite(PWM_L,  PWM - pid_pwm); //SOL
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4,  LOW);
  }
  control_l();
}

void forward_r()
{
  if (distance_mf >= 4 && distance_mf <= 6)
  {
    if (distance_f >= 7 && maze[x + 1][y] != 99)
    {
      x += 1;
      maze[x][y] += 1;
      led();
      manuel();
    }
  }
  else
  {
    pid();
    analogWrite(PWM_R,  PWM + pid_pwm); //SAG
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2,  LOW);

    analogWrite(PWM_L,  PWM - pid_pwm); //SOL
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4,  LOW);
  }
  control_r();
}

void forward_b()
{
  if (distance_mf <= 6 && distance_mf >= 4 )
  {
    if (distance_f >= 7 && maze[x][y - 1] != 99)
    {
      y -= 1;
      maze[x][y] += 1;
      led();
      manuel();
    }
  }
  else
  {
    pid();
    analogWrite(PWM_R,  PWM + pid_pwm); //SAG
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2,  LOW);

    analogWrite(PWM_L,  PWM - pid_pwm); //SOL
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4,  LOW);
  }
  control_b();
}

void handbrake()
{
  analogWrite(PWM_R,  250);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  HIGH);

  analogWrite(PWM_L,  250);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(30);

  analogWrite(PWM_R,  50);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  LOW);

  analogWrite(PWM_L,  50);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
  delay(500);
}

void back_pull()
{
  analogWrite(PWM_R,  50);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  HIGH);

  analogWrite(PWM_L,  50);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(100);

  analogWrite(PWM_R,  100);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  LOW);

  analogWrite(PWM_L,  100);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
  delay(500);
}

//**************************TURNS******************************************

void turn_l()
{
  analogWrite(PWM_R,  PWM/2);
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2,  LOW);

  analogWrite(PWM_L,  PWM/2);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(TURN_DELAY);

  analogWrite(PWM_R,  PWM/2);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  HIGH);

  analogWrite(PWM_L,  PWM/2);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  delay(70);

  analogWrite(PWM_R,  0);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  LOW);

  analogWrite(PWM_L,  0);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
  delay(250);
}

void turn_r()
{
  analogWrite(PWM_R,  PWM/2);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  HIGH);

  analogWrite(PWM_L,  PWM/2);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4,  LOW);
  delay(TURN_DELAY);

  analogWrite(PWM_R,  PWM/2);
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2,  LOW);

  analogWrite(PWM_L,  PWM/2);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  HIGH);
  delay(70);

  analogWrite(PWM_R,  0);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2,  LOW);

  analogWrite(PWM_L,  0);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4,  LOW);
  delay(250);
}

void turn_b()
{
  turn_l();
  delay(200);
  turn_l();
  delay(200);
}

void led() {
  analogWrite(LED_PIN, 1000);
  delay(30);
  analogWrite(LED_PIN, 0);
}

void loop()
{
  control_f();
}

