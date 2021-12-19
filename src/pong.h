#include "stuff.h"
#include <sys/time.h>
#define PONG_BALL_SPEED 60
#define PONG_BALL_SIZE 1
#define PONG_PADDLE_SPEED 40
#define PONG_PADDLE_SIZE 3

struct timeval FRAME_begin,FRAME_end;

long total=0;
double ballX=30;
double ballY=10;
double ballAngle=0;
int PONG_Fps=0;
int PONG_Frames=0;
int PONG_P1_Score=0;
int PONG_P2_Score=0;
double PONG_P1_Y=0;
double PONG_P2_Y=0;

void PONG_Render();
void PONG_Update();
void PONG_Initialize();

void PONG_Collision(int side);
double distance(double x1, double y1, double x2, double y2);
double tempTime=0;

int PONG_Run()
{
  PONG_Initialize();
  while (running)
  {
    gettimeofday(&FRAME_begin, NULL);
    PONG_Render();
    PONG_Update();
    if (GetAsyncKeyState(VK_ESCAPE))
    {
      running = 0;
    }
    if (GetAsyncKeyState(0x57)) // W
    {
      PONG_P1_Y -= PONG_PADDLE_SPEED*dt;
    }
    if (GetAsyncKeyState(0x53)) // S
    {
      PONG_P1_Y += PONG_PADDLE_SPEED*dt;
    }
    if (GetAsyncKeyState(0x26)) // UP
    {
      PONG_P2_Y -= PONG_PADDLE_SPEED*dt;
    }
    if (GetAsyncKeyState(0x28)) // DOWN
    {
      PONG_P2_Y += PONG_PADDLE_SPEED*dt;
    }
    dt=0;
    while (dt<0.000001)
    {
      gettimeofday(&FRAME_end, NULL);
      dt = ((FRAME_end.tv_sec-FRAME_begin.tv_sec)*1000000 
      + (FRAME_end.tv_usec-FRAME_begin.tv_usec));
      dt/=1000000;
    }
    tempTime+=dt;
    if (tempTime>=1)
    {
      tempTime-=1;
      PONG_Fps=PONG_Frames;
      PONG_Frames=0;
      setCursorPosition(0,0);
      printf("FPS: %d", PONG_Fps);
    }
    setCursorPosition(0,0);
    PONG_Frames++;
  }
  return 0;
}

void PONG_Collision(int side)
{
  if (side == 1 || side == 3)
  {
    ballAngle -= ballAngle*2;
  }
  else
  {
    ballAngle = side/2*M_PI + ((double)(rand()%51)/100) - 0.25;
  }
}

void PONG_Initialize()
{
  running = 1;
  free(buffer);
  buffer = malloc(sizeof(char*)*HEIGHT);
  for (int i=0; i<HEIGHT; i++)
  {
    buffer[i] = malloc(sizeof(char)*WIDTH);
  }
  cls();
  srand(time(NULL));

  PONG_P1_Score = 0;
  PONG_P2_Score = 0;

  setConsoleColour(CYAN);
  setCursorPosition(WIDTH/4, 1);
  printf("%d", PONG_P1_Score);
  setConsoleColour(BLUE);
  setCursorPosition(WIDTH-WIDTH/4, 1);
  printf("%d", PONG_P2_Score);
  setConsoleColour(WHITE);

}

void PONG_Update()
{
  if (ballX+PONG_BALL_SPEED*cos(ballAngle)*dt>=WIDTH)
  {
    ballX = WIDTH/2;
    ballY = HEIGHT/2;
    PONG_Collision(2);
    PONG_P1_Score++;
    setConsoleColour(CYAN);
    setCursorPosition(WIDTH/4, 1);
    printf("%d", PONG_P1_Score);
    setConsoleColour(WHITE);
  }
  if (ballX+PONG_BALL_SPEED*cos(ballAngle)*dt<=0)
  {
    ballX = WIDTH/2;
    ballY = HEIGHT/2;
    PONG_Collision(0);
    PONG_P2_Score++;
    setConsoleColour(BLUE);
    setCursorPosition(WIDTH-WIDTH/4, 1);
    printf("%d", PONG_P2_Score);
    setConsoleColour(WHITE);
  }
  if (ballY+PONG_BALL_SPEED*sin(ballAngle)*dt>=HEIGHT)
  {
    PONG_Collision(1);
  }
  if (ballY+PONG_BALL_SPEED*sin(ballAngle)*dt<=2)
  {
    PONG_Collision(3);
  }
  if (ballX>=WIDTH-3)
  {
    if (ballY>=PONG_P2_Y && ballY<=(int)PONG_P2_Y+PONG_PADDLE_SIZE)
    {
      PONG_Collision(2);
    }
  }
  if (ballX<=2)
  {
    if (ballY>=PONG_P1_Y && ballY<=(int)PONG_P1_Y+PONG_PADDLE_SIZE)
    {
      PONG_Collision(0);
    }
  }
  ballX += PONG_BALL_SPEED*cos(ballAngle)*dt;
  ballY += PONG_BALL_SPEED*sin(ballAngle)*dt;
}

void PONG_Render()
{
  for (int y=2; y<HEIGHT; y++)
  {
    for (int x=0; x<WIDTH; x++)
    {
      if (distance(x,y,ballX,ballY) <= PONG_BALL_SIZE)
      {
        if (buffer[y][x] != '#')
        {
          setCursorPosition(x,y);
          printf("%c", BLOCK);
        }
        buffer[y][x] = '#';
      }
      else if (y >= PONG_P1_Y && y <= PONG_P1_Y+PONG_PADDLE_SIZE && x == 2)
      {
        if (buffer[y][x] != '1')
        {
          setConsoleColour(CYAN);
          setCursorPosition(x,y);
          printf("%c", BLOCK);
          setConsoleColour(WHITE);
        }
        buffer[y][x] = '1';
      }
      else if (y >= PONG_P2_Y && y <= PONG_P2_Y+PONG_PADDLE_SIZE && x == WIDTH-3)
      {
        if (buffer[y][x] != '2')
        {
          setConsoleColour(BLUE);
          setCursorPosition(x,y);
          printf("%c", BLOCK);
          setConsoleColour(WHITE);
        }
        buffer[y][x] = '2';
      }
      else
      {
        if (buffer[y][x] == 'L')
        {
          setCursorPosition(x,y);
          printf(" ");
          buffer[y][x] = ' ';
        }
        if (buffer[y][x] == '#' || buffer[y][x] == '1' || buffer[y][x] == '2')
        {
          setCursorPosition(x,y);
          printf(" ");
          buffer[y][x]='L';
        }
      }
    }
  }
}

double distance(double x1, double y1, double x2, double y2)
{
  return sqrt(pow(x1-x2, 2)+pow(y1-y2,2));
}