#include <3ds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

const int FISH_COD_VALUE = 3;
const int FISH_SALMON_VALUE = 5;
float coolDown = 3.5;

int rando;
int userMoney = 0; 

int fish_cod = 0;
int fish_salmon = 0;

int fish_cod_buf;
int fish_salmon_buf;

void waitForKey(char key)
{
  while(true)
  {
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & key) break;
  }
}


void printHomeScreen()
{
  printf("\x1b[1;0H(A) Fish!");
  printf("\x1b[2;0H(B) Menu");
  printf("\x1b[3;0H(Y) Shop");
  printf("\x1b[30;0H[Start] Exit");
}

void printFishScreen()
{
  printf("\x1b[1;0H(A) Fish again!"); printf("\x1b[1;34HMoney: %d$", userMoney);
  printf("\x1b[2;0H(Y) Sell fishe");
  printf("\x1b[4;0H(B) Return to main menu");

  printf("\x1b[28;0HYou now have");
  printf("\x1b[29;0HCod: %d + %d", fish_cod, fish_cod_buf);
  printf("\x1b[30;0HSalmon: %d + %d", fish_salmon, fish_salmon_buf);
}

void doFish()
{
  rando = rand() % 6;
  fish_cod = fish_cod + rando;
  fish_cod_buf = rando;

  rando = rand() % 6;
  fish_salmon = fish_salmon + rando;
  fish_salmon_buf = rando;
}

u32 heldFishValue()
{
  u32 out = 0;
  out = out + (fish_cod * FISH_COD_VALUE);
  out = out + (fish_salmon * FISH_SALMON_VALUE);
  return out;
}

void clearFish()
{
  fish_cod = 0;
  fish_salmon = 0;

  fish_cod_buf = 0;
  fish_salmon_buf = 0;
}

void sellFish() 
{
  userMoney = userMoney + heldFishValue();
  clearFish();
}

void fishingMode()
{
  while(true)
  {
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_A) {
      doFish();
      consoleClear();
      printFishScreen();

      printf("\x1b[1;0H( ) Cooldown...");
      sleep(coolDown);
      printf("\x1b[1;0H(A) Fish again!");
    } else if (kDown & KEY_Y) {
      sellFish();
      consoleClear();
      printFishScreen();
    } else if (kDown & KEY_B) {
      consoleClear();
      printHomeScreen();
      break;
    }
  }
  return;
}

void menuMode()
{ 
  while(true)
  {
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_A) {
      consoleClear();
      doFish();
      printFishScreen();
      printf("\x1b[1;0H( ) Cooldown...");
      sleep(coolDown);
      printf("\x1b[1;0H(A) Fish again!");
      fishingMode();
    } else if (kDown & KEY_START) {
      break;
    }
  }
  return;
}

int main(int argc, char **argv)
{
	gfxInitDefault();

	consoleInit(GFX_TOP, NULL);

	printf("\x1b[15;13HPress (A) to start fishing!");
  
  waitForKey(KEY_A);

  consoleClear();

  srand(time(NULL));

  printHomeScreen();
  menuMode(); 

		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();

	gfxExit();
	return 0;
}
