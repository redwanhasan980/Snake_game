
#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SDL2/SDL_image.h>
using namespace std;
//-----vvvaaaarrriiaaaabbllleesssss-----
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
bool is_collision = false;
bool start = false;
int cell_num=50;
int cell_size=SCREEN_WIDTH/cell_num;
bool gameover= false;
bool do_text=0;
int  t1 = 0,t2=0;
int first = 0;
int event_time=0;
int high_score;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *textTexture;
TTF_Font *font;
  SDL_Surface *textSurface;
   SDL_Rect textRect;
   SDL_Rect imgRect;
    fstream inputFile("highest_score.txt");
  
   bool maingame=false;
bool gamerun = false;
int score=0,next_score=0;
SDL_Color color;
  char scores[]={'\0'};
  char high_scores[]={'\0'};
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
struct SnakeSegment {
    int x, y;
};
    Direction direction;
    vector<SnakeSegment> snake;
    SnakeSegment food;
///-------mmmaaaakkkkkinnnnng fooooooooooooood----------
void makeFood() {
  
    food.x =rand() % cell_num;
    food.y =rand() % 27;
    while( food.y<=1)
     food.y =rand() % 27;
}

// initialization----------------------
bool initializeWindow(void)
{
 
     
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error : SDL initialization failed." << endl
             << "SDL Error: " << SDL_GetError();
        return false;
    }
    window = SDL_CreateWindow("SNAKE ULTRA",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL)
    {
        cout << "Error: Failed to open window" << endl
             << "SDL Error: " << SDL_GetError();
        return false;
    }
    if (TTF_Init() != 0)
    {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        SDL_Quit();
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        cout << "Error: Failed to open renderer" << endl
             << "SDL Error: " << SDL_GetError();
        return false;
    }
     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
       cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        SDL_Quit();
        return false;
    }

     Mix_Chunk *sound = Mix_LoadWAV("music/opening.wav");
    if (sound == nullptr) {
       std::cerr << "Failed to load sound file! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return false;
    }
     Mix_PlayChannel(1, sound, 0);
      Mix_Pause(1);
   int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
      
        return false;
    }
   
       direction = RIGHT;
      snake.push_back({ 0, 14});
      snake.push_back({ -1, 14});
    makeFood();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    return true;
}

///image load----------
void img(char img_path[],int x,int y,int w,int h)
{
  SDL_Surface* imgSurface = IMG_Load(img_path);
  SDL_Texture* imgTexture = SDL_CreateTextureFromSurface(renderer,imgSurface);
       imgRect.x = x;
    imgRect.y = y;
    imgRect.w = w;
    imgRect.h = h; 
              SDL_FreeSurface(imgSurface);
                SDL_RenderCopy(renderer, imgTexture, NULL, &imgRect);
}

//////// drawing tttteeeeeeeexxxtttttt---------


void drawText(char s[], char ss[], int x, int y, int size, SDL_Color textColor)
{
    
     font = TTF_OpenFont(ss, size);
    if (!font)
    {
        SDL_Log("TTF_OpenFont Error: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    textSurface = TTF_RenderText_Solid(font, s, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
   
    textRect.x = x;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

///////taaaakkkiiiinnngggggg   inpppuuuuttttttt-----

void input(void)
{ if(event_time<=2)
 event_time++;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN&&event_time>0) {
          event_time=0;
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (direction != DOWN)
                  
                    direction = UP;
                break;
            case SDLK_DOWN:
                if (direction != UP)
                    direction = DOWN;
                  
                break;
            case SDLK_LEFT:
                if (direction != RIGHT)
                    direction = LEFT;
                   
                break;
            case SDLK_RIGHT:
                if (direction != LEFT)
                    direction = RIGHT;
                
                break;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            if (SDLK_f == event.key.keysym.sym&&first==0)
            {
                    start = true,do_text=0;
                    first=1;
                   Mix_Pause(1);
                   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);

            }
                
             else if (SDLK_r == event.key.keysym.sym)
             {
                start = true,do_text=0;
                 
                direction = RIGHT;
                snake.clear();
     snake.push_back({ 0, 14});
      snake.push_back({ -1, 14});
    makeFood();
    score=0;
    next_score=0;

                   
             }
                
            else if (SDLK_ESCAPE == event.key.keysym.sym)
                gamerun = false;
        }
        else if (event.type == SDL_QUIT)
        {
            gamerun = false;
            break;
        }
    }
}

////measure collllisionnnnnn------

bool measure_collision() {
    for (int i = 1; i < snake.size(); ++i) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
            return true;
        }
    }
    return false;
}


/// //update snake --------

void update_snake() {

    SnakeSegment newHead = snake.front();
    
    switch (direction) {
    case UP:
        newHead.y -= 1;
        if( newHead.y==1)
          newHead.y=28;
        break;
    case DOWN:
        newHead.y += 1;
         if( newHead.y==29)
          newHead.y=2;
        break;
    case LEFT:
        newHead.x -= 1;
         if( newHead.x==-1)
          newHead.x=50;
        break;
    case RIGHT:
        newHead.x += 1;
         if( newHead.x==51)
          newHead.x=0;
        break;
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y) {
        makeFood();
        score++;
    }
    else {
        snake.pop_back();
    }
 
    if (measure_collision()) {
       
       start=false;
       gameover=true;
    }
}

// clear the renderer 

void clear_renderer(int x,int y,int w,int h)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
     SDL_Rect Rect = { x, y, w, h};
    SDL_RenderFillRect(renderer, &Rect);
    SDL_RenderPresent(renderer);
}

// render--------
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  clear_renderer(0,48,1285,725);

    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);

    for (const auto& segment : snake) {
        SDL_Rect rect = { segment.x * cell_size, segment.y * cell_size, cell_size, cell_size };
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = { food.x * cell_size, food.y * cell_size, cell_size, cell_size };
    SDL_RenderFillRect(renderer, &foodRect);
     SDL_RenderDrawLine( renderer,0, 47,1300, 47);
     if(score==next_score)
     { 
   clear_renderer(610,0,100,40);
      drawText(scores, "zebulon/Zebulon Bold Italic.otf", 630, 5, 30, {255, 0, 0});
      next_score++;
     }
     
    SDL_RenderPresent(renderer);
}
// calculation of score
void calcscore()
{
    int temp,i;
    string st;
   st=to_string(score);

  for( i=0;i<st.size();i++)
    scores[i]=st[i];
     scores[i]='\0';
}
void calchighscore()
{
    int temp,i;
    string sk;
   sk=to_string(high_score);

  for( i=0;i<sk.size();i++)
    high_scores[i]=sk[i];
    high_scores[i]='\0';
}
void check_high_score()
{
 

  int temp_high_score=0;
  inputFile>>temp_high_score;
  cout<<temp_high_score;
  if(score>=temp_high_score)
  {
  temp_high_score=score;
  ofstream file("highest_score.txt");
    file<<score;
  }
 
   high_score=temp_high_score;
   

  
   
}
void destroyWindow(void)
{ 
 inputFile.close();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[])
{


  
    gamerun = initializeWindow();
    int time = 0;
    
    
    while (gamerun)
    {
         input();
   
    SDL_Delay(100);

   
      if (first == 0 && start == false&&do_text==0)
        {     Mix_Resume(1);
              check_high_score();
              calchighscore();
              
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            img("image/snake_sasuke.jpg",0,0,1280,720);
            drawText("SNAKE PRO", "zebulon/Zebulon Bold Italic.otf", 50, 80, 90, {255, 0, 0});
        drawText("HIGH SCORE : ", "font/SpaceMission.otf", 150, 240, 75, {255, 255, 255});
             drawText(high_scores, "zebulon/Zebulon Bold Italic.otf", 680, 220, 80, {255, 0, 0});
            drawText("To Start press F", "font/GothamMedium.ttf", 380, 350, 50, {255, 255, 255});
            drawText("To Quit press ESC", "font/GothamMedium.ttf", 320, 450, 50, {255, 255, 255});
            drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 50, 600, 40, {255, 0, 0});
            drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 50, 650, 15, {255, 255, 255});
            SDL_RenderPresent(renderer);
            do_text=1;
           
        }
        else if(do_text==1&&start == false&&gameover==false)
        {
            SDL_RenderPresent(renderer);
        }
       else if (start == true)
        {
          
         update_snake();
         calcscore();
          render();
        }
        else if (start == false&&gameover==true&&do_text==0)
        {     
                 check_high_score();
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderClear(renderer);
                drawText("GAME OVER", "zebulon/Zebulon Bold Italic.otf", 220, 100, 100, {255, 0, 0});
                drawText("SCORE : ", "font/GothamMedium.ttf", 400, 250, 65, {255, 255, 255});
               
               drawText(scores,"zebulon/Zebulon Bold Italic.otf", 710, 225, 80, {255, 0, 0});

                 drawText("To restart press r", "font/GothamMedium.ttf", 400, 350, 50, {255, 255, 255});
                drawText("To Quit press ESC", "font/GothamMedium.ttf", 400, 450, 50, {255, 255, 255});
                drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 800, 600, 40, {255, 0, 0});
                drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 800, 650, 15, {255, 255, 255});
                drawText("Developed by: Redwan Hasan", "font/GothamMedium.ttf", 800, 680, 20, {255, 255, 255});
                SDL_RenderPresent(renderer);
               do_text =1;
            
        
        }
        else
        SDL_RenderPresent(renderer);
         
    }
   

    destroyWindow();
    return 0;
}