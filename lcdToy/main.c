#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "main.h"

unsigned char redrawScreen = 1;
char switches = 0;

static Ball ball = {
  { 60, 60 },
  { 60, 60 },
  { 4, 4 },
};

static Bar player = {
  0,
  { screenWidth / 2, screenHeight - 4 },
  { screenWidth / 2, screenHeight - 4 },
};

static Bar enemy = {
  0,
  { screenWidth / 2, 4 },
  { screenWidth / 2, 4 },
};

static unsigned char hasBallXMoved()
{
  return ball.drawPosition.x != ball.controlPosition.y;
}

static unsigned char hasBallYMoved()
{
  return ball.drawPosition.y != ball.controlPosition.y;
}

static void screenUpdateBall()
{
  if (hasBallXMoved || hasBallYMoved) {
    fillCircle(ball.drawPosition.x, ball.drawPosition.y, BALL_RADIUS, COLOR_BLACK);
    ball.drawPosition = ball.controlPosition;
    fillCircle(ball.drawPosition.x, ball.drawPosition.y, BALL_RADIUS, COLOR_WHITE);
  }
}

static void updateBallPosition()
{
  unsigned char newXPosition;
  unsigned char newYPosition;

  newXPosition = ball.controlPosition.x + ball.velocity.x;
  if (newXPosition <= BALL_RADIUS || newXPosition >= screenWidth - BALL_RADIUS)
    ball.velocity.x = -ball.velocity.x;
  else
    ball.controlPosition.x = newXPosition;

  newYPosition = ball.controlPosition.y + ball.velocity.y;
  if (newYPosition <= BALL_RADIUS) {
    if (newXPosition >= enemy.controlPosition.x
	&& newXPosition <= enemy.controlPosition.x + BAR_WIDTH) {
      if (newXPosition < enemy.controlPosition.x + BAR_WIDTH / 2)
	ball.velocity.x -= 1;
      else if (newXPosition > enemy.controlPosition.x + BAR_WIDTH / 2)
	ball.velocity.x += 1;

      ball.velocity.y = -ball.velocity.y;
    } else {
      player.score++;
    }
  } else if (newYPosition >= screenHeight - BALL_RADIUS) {
    if (newXPosition >= player.controlPosition.x
	&& newXPosition <= player.controlPosition.x + BAR_WIDTH) {
      if (newXPosition < player.controlPosition.x + BAR_WIDTH / 2)
	ball.velocity.x -= 1;
      else if (newXPosition > player.controlPosition.x + BAR_WIDTH / 2)
	ball.velocity.x += 1;

      ball.velocity.y = -ball.velocity.y;
    }
  } else {
    ball.controlPosition.y = newYPosition;
  }
  /*
  if (newYPosition <= BALL_RADIUS || newYPosition >= screenHeight - BALL_RADIUS)
    ball.velocity.y = -ball.velocity.y;
  else
    ball.controlPosition.y = newYPosition;
  */
}

static unsigned char hasPlayerXMoved()
{
  return player.drawPosition.x != player.controlPosition.x;
}

static unsigned char hasPlayerYMoved()
{
  return player.drawPosition.y != player.controlPosition.y;
}

static void screenUpdatePlayer()
{
  if (hasPlayerXMoved() || hasPlayerYMoved()) {
    fillRectangle(player.drawPosition.x, player.drawPosition.y, BAR_WIDTH,
		  BAR_HEIGHT, COLOR_BLACK);
    player.drawPosition = player.controlPosition;
    fillRectangle(player.drawPosition.x, player.drawPosition.y, BAR_WIDTH,
		  BAR_HEIGHT, COLOR_WHITE);
  }
}

static void updatePlayerPosition()
{
  if ((switches & SW1) && (player.controlPosition.x > 0))
    player.controlPosition.x -= BAR_SPEED;
  if ((switches & SW2) && (player.controlPosition.x < screenWidth - BAR_WIDTH))
    player.controlPosition.x += BAR_SPEED;
}

static unsigned char hasEnemyXMoved()
{
  return enemy.drawPosition.x != enemy.controlPosition.x;
}

static unsigned char hasEnemyYMoved()
{
  return enemy.drawPosition.y != enemy.controlPosition.y;
}

static void screenUpdateEnemy()
{
  if (hasEnemyXMoved() || hasEnemyYMoved()) {
    fillRectangle(enemy.drawPosition.x, enemy.drawPosition.y, BAR_WIDTH,
		  BAR_HEIGHT, COLOR_BLACK);
    enemy.drawPosition = enemy.controlPosition;
    fillRectangle(enemy.drawPosition.x, enemy.drawPosition.y, BAR_WIDTH,
		  BAR_HEIGHT, COLOR_WHITE);
  }
}

static void updateEnemyPosition()
{
  if ((switches & SW3) && (enemy.controlPosition.x > 0))
    enemy.controlPosition.x -= BAR_SPEED;
  if ((switches & SW4) && (enemy.controlPosition.x < screenWidth - BAR_WIDTH))
    enemy.controlPosition.x += BAR_SPEED;
}

static void updateShape()
{
  screenUpdateBall();
  screenUpdatePlayer();
  screenUpdateEnemy();
}

void main()
{
  configureClocks();
  switchInit();
  lcd_init();

  enableWDTInterrupts();
  or_sr(0x8);

  clearScreen(COLOR_BLACK);
  
  while (1) {
    if (redrawScreen) {
      redrawScreen = 0;
      updateShape();
    }
    or_sr(0x10);
  }
}
    
void wdt_c_handler()
{
  static int secondCount = 0;

  if (++secondCount >= 20) {
    updateBallPosition();
    updatePlayerPosition();
    updateEnemyPosition();
    redrawScreen = 1;
    secondCount = 0;
  }
}
