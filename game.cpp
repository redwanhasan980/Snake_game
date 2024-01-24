#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SDL2/SDL_image.h>
#include "score.h"
#include "level.h"

using namespace std;
int main(int argc, char *argv[])
{ // level 111111
  if (score == 0)
    game1();

  // level 222222
  if (score == 12)
  {
    lv = 2;
    start = false;
    first = 0;
    do_text = 0;
    direction = RIGHT;
    snake.clear();
    snake.push_back({0, 14});
    snake.push_back({-1, 14});
    makeFood();
    Mix_Pause(2);
    next_score--;
    while (gamerun)
    {
      input();

      SDL_Delay(100);

      if (first == 0 && start == false && do_text == 0)
      {
        Mix_Resume(3);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        img("image/sad.jpg", 0, 0, 1280, 720);
        // drawText("SNAKE PRO", "zebulon/Zebulon Bold Italic.otf", 50, 80, 90, {255, 0, 0});
        drawText("Tale of Sadness", "font/SpaceMission.otf", 30, 120, 75, {255, 0, 0});
        drawText("LEVEL 2 ", "zebulon/Zebulon Bold Italic.otf", 730, 20, 75, {255, 0, 0});

        drawText("To Start press F", "font/GothamMedium.ttf", 730, 120, 50, {255, 255, 255});
        drawText("To Quit press ESC", "font/GothamMedium.ttf", 730, 200, 50, {255, 255, 255});
        drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 850, 600, 40, {255, 0, 0});
        drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 850, 650, 15, {255, 255, 255});
        SDL_RenderPresent(renderer);
        do_text = 1;
      }
      else if (do_text == 1 && start == false && gameover == false)
      {
        SDL_RenderPresent(renderer);
      }
      else if (start == true)
      {
        Mix_Pause(2);
        update_snake();
        update2();
        calcscore();
        level2();
        if (score == 25)
          break;
      }
      else if (start == false && gameover == true && do_text == 0)
      {
        game_over_screen();
      }
      else
        SDL_RenderPresent(renderer);
    }
  }

  // level 3333333333
  if (score == 25)
  { Mix_Resume(4);
    lv = 3;
    character.x = 25;
    character.y = 13;
    start = false;
    first = 0;
    do_text = 0;

    snake.clear();
    Mix_Pause(3);
    Mix_Pause(2);
    next_score--;
    while (gamerun)
    {
        rasen = 0;
      if (charge < 100)
        charge += 2;
      cout << score << endl;
      Mix_Pause(2);
      direction = STOP;
      input();
      SDL_Delay(100);

      if (first == 0 && start == false && do_text == 0)
      {
        // Mix_Resume(3);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        img("image/naruto_bg.jpg", 0, 0, 1280, 720);
        // drawText("SNAKE PRO", "zebulon/Zebulon Bold Italic.otf", 50, 80, 90, {255, 0, 0});
        drawText("A Failure's True Power", "font/SpaceMission.otf", 30, 620, 75, {255, 0, 0});
        drawText("LEVEL 3", "zebulon/Zebulon Bold Italic.otf", 30, 20, 75, {255, 0, 0});

        drawText("To Start press F", "font/GothamMedium.ttf", 30, 120, 50, {255, 255, 255});
        drawText("To Quit press ESC", "font/GothamMedium.ttf", 30, 200, 50, {255, 255, 255});
        drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 900, 20, 40, {255, 0, 0});
        drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 900, 60, 15, {255, 255, 255});
        SDL_RenderPresent(renderer);
        do_text = 1;
      }
      else if (do_text == 1 && start == false && gameover == false)
      {
        SDL_RenderPresent(renderer);
      }
      else if (start == true)
      {
        if (rasen == 1 && charge == 100)
        {     distroy_snake();
          charge=0;
        
          music("music/rasen.mp3");
           SDL_Delay(1500);
          img("image/rasen.png",(character.x-4)*cell_size,(character.y-4)*cell_size,9*cell_size,9*cell_size);
           SDL_RenderPresent(renderer);
          SDL_Delay(2500);
        }
        else
        {
          calcscore();
          Mix_Pause(2);
          update_snake3();
          update_character();
          level3rander();
        }
      }
      else if (start == false && gameover == true && do_text == 0)
      {
        game_over_screen();
      }
      else
        SDL_RenderPresent(renderer);
    }
  }

  destroyWindow();
}