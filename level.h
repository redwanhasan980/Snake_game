
#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SDL2/SDL_image.h>
#include "score.h"
using namespace std;
//-----vvvaaaarrriiaaaabbllleesssss-----
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
bool is_collision = false;
bool start = false;
int cell_num = 50;
int cell_size = SCREEN_WIDTH / cell_num;
bool gameover = false;
bool do_text = 0;
int t1 = 0, t2 = 0;
int first = 0;
int event_time = 0;
int high_score;
int bonus = 7;
bool bfood = 0;
int tb = 0;
int lv = 1;
int charge = 0;
int rasen = 0;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *textTexture;
TTF_Font *font;
SDL_Surface *textSurface;
SDL_Rect textRect;
SDL_Rect imgRect;
fstream inputFile("highest_score.txt");

bool maingame = false;
bool gamerun = false;
int score, next_score;
SDL_Color color;
char scores[] = {'\0'};
char high_scores[] = {'\0'};
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};
struct SnakeSegment
{
    int x, y;
};
Direction direction;
Direction direction_char;
vector<SnakeSegment> snake;
SnakeSegment food;
SnakeSegment bonusfood;
SnakeSegment character;
vector<SnakeSegment> snake3[10];
int avsnake[10] = {0};
///-------mmmaaaakkkkkinnnnng fooooooooooooood----------
void makeFood()
{

    food.x = rand() % cell_num;
    food.y = rand() % 27;
    while (food.y <= 1)
        food.y = rand() % 27;
}
/// ---making bonus fooooooodddddddddd----------
void bonus_Food()
{
    bfood = 1;
    bonusfood.x = rand() % cell_num;
    bonusfood.y = rand() % 27;
    while (bonusfood.y <= 1)
        bonusfood.y = rand() % 27;
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        SDL_Quit();
        return false;
    }

    Mix_Chunk *intro = Mix_LoadWAV("music/intro.mp3");
    Mix_Chunk *level1 = Mix_LoadWAV("music/level1.mp3");
    Mix_Chunk *level2 = Mix_LoadWAV("music/naruto_epic.mp3");
     Mix_Chunk *level3 = Mix_LoadWAV("music/level3.mp3");
    Mix_PlayChannel(1, intro, 0);
    Mix_Pause(1);
    Mix_PlayChannel(2, level1, 5);
    Mix_Pause(2);
    Mix_PlayChannel(3, level2, 0);
    Mix_Pause(3);
    Mix_PlayChannel(4, level3, 0);
    Mix_Pause(4);
    int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
    {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;

        return false;
    }

    direction = RIGHT;
    snake.push_back({0, 14});
    snake.push_back({-1, 14});
    makeFood();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    return true;
}
// music effect------
void music(char s[])
{
    Mix_Chunk *effect = Mix_LoadWAV(s);
    Mix_PlayChannel(1, effect, 0);
}
/// image load----------
void img(char img_path[], int x, int y, int w, int h)
{
    SDL_Surface *imgSurface = IMG_Load(img_path);
    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    imgRect.x = x;
    imgRect.y = y;
    imgRect.w = w;
    imgRect.h = h;
    SDL_FreeSurface(imgSurface);
    SDL_RenderCopy(renderer, imgTexture, NULL, &imgRect);
    SDL_DestroyTexture(imgTexture);
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
{
    if (event_time <= 2)
        event_time++;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN && event_time > 0)
        {
            event_time = 0;
            switch (event.key.keysym.sym)
            {
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
            if (SDLK_f == event.key.keysym.sym && first == 0)
            {
                start = true, do_text = 0;
                first = 1;
                Mix_Pause(1);
                Mix_Resume(2);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
            }

            else if (SDLK_r == event.key.keysym.sym)
            {
                start = true, do_text = 0;

                direction = RIGHT;
                snake.clear();
                snake.push_back({0, 14});
                snake.push_back({-1, 14});
                makeFood();
                if (lv == 1)
                {
                    score = 0;
                    next_score = 0;
                    bonus = 0;
                }
                else if (lv == 2)
                {
                    score = 12;
                    next_score = 12;
                    bonus = 14;
                }
                else if(lv==3)
                {     charge=0;
                    score = 25;
                    next_score = 25;
                      character.x = 25;
                      character.y = 13;
                for(int i=0;i<6;i++)
                {

                       int d;
                    int x;
                    int y;
                    d = rand() % 4;
                    if (d == 0)
                    {
                        y = rand() % 27;
                        while (y <= 1)
                            y = rand() % 27;
                        x = 0;
                    }
                    else if (d == 1)
                    {
                        x = rand() % cell_num;
                        y = 27;
                    }
                    else if (d == 3)
                    {
                        y = rand() % 27;
                        while (y <= 1)
                            y = rand() % 27;
                        x = cell_num;
                    }
                    else
                    {
                        x = rand() % cell_num;
                        y = 2;
                    }

                    snake3[i][0].x = x;
                    snake3[i][0].y = y;
                    snake3[i][1].x = x - 1;
                    snake3[i][1].y = y;
                }
                }
            }

            else if (SDLK_ESCAPE == event.key.keysym.sym)
                gamerun = false;
            else if (SDLK_SPACE == event.key.keysym.sym)
                rasen = 1;
        }
        else if (event.type == SDL_QUIT)
        {
            gamerun = false;
            break;
        }
    }
}

////measure collllisionnnnnn------

bool measure_collision()
{
    for (int i = 1; i < snake.size(); ++i)
    {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y)
        {
            return true;
        }
    }
    return false;
}

/// //update snake --------

void update_snake()
{

    SnakeSegment newHead = snake.front();

    switch (direction)
    {
    case UP:
        newHead.y -= 1;
        if (newHead.y == 1)
            newHead.y = 28;
        break;
    case DOWN:
        newHead.y += 1;
        if (newHead.y == 29)
            newHead.y = 2;
        break;
    case LEFT:
        newHead.x -= 1;
        if (newHead.x == -1)
            newHead.x = 50;
        break;
    case RIGHT:
        newHead.x += 1;
        if (newHead.x == 51)
            newHead.x = 0;
        break;
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y)
    {
        makeFood();
        score++;
        music("music/dis.mp3");
    }
    else if (((newHead.x == bonusfood.x) || (newHead.x - 1 == bonusfood.x)) && ((newHead.y == bonusfood.y) || (newHead.y - 1 == bonusfood.y)) && tb < 80 && bfood == 1)
    {
        score += 3;
        next_score += 2;
        music("music/dis.mp3");
        bfood = 0;
        tb = 0;
    }
    else
    {
        snake.pop_back();
    }
    if (score == bonus)
    {
        music("music/bonus.mp3");
        bonus_Food();
        bonus += 7;
    }
    if (measure_collision())
    {

        start = false;
        gameover = true;
    }
}

// clear the renderer

void clear_renderer(int x, int y, int w, int h, int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect Rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &Rect);
    SDL_RenderPresent(renderer);
}

// render--------
void render()
{
    // SDL_SetRenderDrawColor(renderer, 124, 179, 66, 255);
    // SDL_RenderClear(renderer);
    clear_renderer(0, 48, 1285, 725, 85, 139, 47);

    SDL_SetRenderDrawColor(renderer, 156, 39, 176, 255);

    for (const auto &segment : snake)
    {
        SDL_Rect rect = {segment.x * cell_size, segment.y * cell_size, cell_size, cell_size};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 106, 27, 154, 255);
    auto it = snake.begin();
    SDL_Rect rect = {(*it).x * cell_size, (*it).y * cell_size, cell_size, cell_size};
    SDL_RenderFillRect(renderer, &rect);

    img("image/naruto.png", food.x * cell_size, food.y * cell_size, cell_size + 10, cell_size + 10);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 47, 1300, 47);
     
    if (score == next_score)
    {
        drawText("Score : ", "zebulon/Zebulon Bold Italic.otf", 950, 5, 30, {255, 0, 0});
        drawText("let the show begin", "zebulon/Zebulon Bold Italic.otf", 330, 5, 30, {255, 0, 0});
        clear_renderer(1120, 0, 100, 40, 0, 0, 0);
        drawText(scores, "zebulon/Zebulon Bold Italic.otf", 1140, 5, 30, {255, 0, 0});
        drawText("Level 1", "zebulon/Zebulon Bold Italic.otf", 50, 5, 30, {255, 0, 0});
        next_score++;
    }
    if (bfood == 1 && tb < 80)
    {
        if (tb > 10)
            img("image/gamakichi.png", bonusfood.x * cell_size, bonusfood.y * cell_size, cell_size + 30, cell_size + 30);
        tb++;
        cout << tb << endl;
    }
    else
    {
        tb = 0;
        bfood = 0;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}
// calculation of score
void calcscore()
{
    int temp, i;
    string st;
    st = to_string(score);

    for (i = 0; i < st.size(); i++)
        scores[i] = st[i];
    scores[i] = '\0';
}
// calculation of high score --------
void calchighscore()
{
    int temp, i;
    string sk;
    sk = to_string(high_score);

    for (i = 0; i < sk.size(); i++)
        high_scores[i] = sk[i];
    high_scores[i] = '\0';
}
///------------
void check_high_score()
{

    int temp_high_score = 0;
    inputFile >> temp_high_score;
    cout << temp_high_score;
    if (score >= temp_high_score)
    {
        temp_high_score = score;
        ofstream file("highest_score.txt");
        file << score;
    }

    high_score = temp_high_score;
}
void destroyWindow(void)
{
    inputFile.close();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
// update snake for level 222222222
void update2()
{
    if ((snake[0].x == 25 || snake[0].y == 13))
    {
        start = false;
        gameover = true;
    }
    if (food.x == 25 || food.y == 13)
        makeFood();
    if (bfood == 1 && (bonusfood.x == 25 || bonusfood.y == 13))
        bonus_Food();
}
// render for level 222222222
void level2()
{
    clear_renderer(0, 48, 1285, 725, 85, 139, 47);

    SDL_SetRenderDrawColor(renderer, 121, 85, 72, 255);

    for (const auto &segment : snake)
    {
        SDL_Rect rect = {segment.x * cell_size, segment.y * cell_size, cell_size, cell_size};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 62, 39, 35, 255);
    auto it = snake.begin();
    SDL_Rect rect = {(*it).x * cell_size, (*it).y * cell_size, cell_size, cell_size};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 96, 125, 139, 255);
    SDL_Rect roct = {25 * cell_size, 2 * cell_size, cell_size, cell_size * 28};
    SDL_RenderFillRect(renderer, &roct);
    SDL_Rect rct = {0, 13 * cell_size, cell_size * 53, cell_size};
    SDL_RenderFillRect(renderer, &rct);

    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    /// SDL_Rect foodRect = { food.x * cell_size, food.y * cell_size, cell_size, cell_size };
    //  SDL_RenderFillRect(renderer, &foodRect);
    img("image/naruto.png", food.x * cell_size, food.y * cell_size, cell_size + 10, cell_size + 10);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 47, 1300, 47);
    if (score == next_score)
    {
        drawText("Score : ", "zebulon/Zebulon Bold Italic.otf", 950, 5, 30, {255, 0, 0});

        clear_renderer(1120, 0, 100, 40, 0, 0, 0);
        drawText(scores, "zebulon/Zebulon Bold Italic.otf", 1140, 5, 30, {255, 0, 0});
        drawText("Level 2", "zebulon/Zebulon Bold Italic.otf", 50, 5, 30, {255, 0, 0});
        drawText("Tale of Sadness", "zebulon/Zebulon Bold Italic.otf", 350, 5, 30, {255, 0, 0});
        next_score++;
    }
    if (bfood == 1 && tb < 80)
    {
        if (tb > 10)
            img("image/gamakichi.png", bonusfood.x * cell_size, bonusfood.y * cell_size, cell_size + 30, cell_size + 30);
        tb++;
        cout << tb << endl;
    }
    else
    {
        tb = 0;
        bfood = 0;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}
// game over sreen-------
void game_over_screen()
{
    check_high_score();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawText("GAME OVER", "zebulon/Zebulon Bold Italic.otf", 220, 100, 100, {255, 0, 0});
    drawText("SCORE : ", "font/GothamMedium.ttf", 400, 250, 65, {255, 255, 255});

    drawText(scores, "zebulon/Zebulon Bold Italic.otf", 710, 225, 80, {255, 0, 0});

    drawText("To restart press r", "font/GothamMedium.ttf", 400, 350, 50, {255, 255, 255});
    drawText("To Quit press ESC", "font/GothamMedium.ttf", 400, 450, 50, {255, 255, 255});
    drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 800, 600, 40, {255, 0, 0});
    drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 800, 650, 15, {255, 255, 255});
    drawText("Developed by: Redwan Hasan", "font/GothamMedium.ttf", 800, 680, 20, {255, 255, 255});
    SDL_RenderPresent(renderer);
    do_text = 1;
}

void update_character()
{
    if (direction == STOP)
        return;
    else if (direction == UP&&character.y>3)
    {
        character.y -= 2;
    }
    else if (direction == DOWN&&character.y<25)
        character.y += 2;
    else if (direction == RIGHT&&character.x<=47)
        character.x += 2;
    else if(direction == LEFT&&character.x>=1)
        character.x -= 2;
}
void create_snake(int slot)
{
    int d;
    int x;
    int y;
    d = rand() % 4;
    if (d == 0)
    {
        y = rand() % 27;
        while (y <= 1)
            y = rand() % 27;
        x = 0;
    }
    else if (d == 1)
    {
        x = rand() % cell_num;
        y = 27;
    }
    else if (d == 3)
    {
        y = rand() % 27;
        while (y <= 1)
            y = rand() % 27;
        x = cell_num;
    }
    else
    {
        x = rand() % cell_num;
        y = 2;
    }

    snake3[slot].push_back({x, y});
    snake3[slot].push_back({x - 1, y});
}
void update_snake3()
{
    for (int i = 0; i < 6; i++)
    {
        if (avsnake[i] == 0)
        {
            create_snake(i);
            avsnake[i] = 1;
        }
        else
        {
            SnakeSegment newHead = snake3[i].front();
            if (character.x < newHead.x)
                newHead.x -= 1;
            else if (character.y < newHead.y)
                newHead.y--;
            else if (character.x > newHead.x)
                newHead.x++;
            else if (character.y > newHead.y)
                newHead.y++;
            if ((newHead.x == character.x||newHead.x == character.x+1) && (newHead.y == character.y||newHead.y == character.y+1))
            {
                start = false;
                gameover = true;
            }

            snake3[i].insert(snake3[i].begin(), newHead);
            snake3[i].pop_back();
        }
    }
}
void distroy_snake()
{
    if (rasen == 1 && charge == 100)
    {
        cout << "Yes";
        for (int k = 0; k < 100; k++)
        {
            for (int i = 0; i < 6; i++)
            {
                SnakeSegment newHead = snake3[i].front();
                if (character.x - 4 <= newHead.x && character.x + 4 >= newHead.x && character.y - 4 <= newHead.y && character.y + 4 >= newHead.y)

                {   score++;
                next_score++;
                    cout << i << endl;
                    int d;
                    int x;
                    int y;
                    d = rand() % 4;
                    if (d == 0)
                    {
                        y = rand() % 27;
                        while (y <= 1)
                            y = rand() % 27;
                        x = 0;
                    }
                    else if (d == 1)
                    {
                        x = rand() % cell_num;
                        y = 27;
                    }
                    else if (d == 3)
                    {
                        y = rand() % 27;
                        while (y <= 1)
                            y = rand() % 27;
                        x = cell_num;
                    }
                    else
                    {
                        x = rand() % cell_num;
                        y = 2;
                    }

                    snake3[i][0].x = x;
                    snake3[i][0].y = y;
                    snake3[i][1].x = x - 1;
                    snake3[i][1].y = y;
                }
               
            }
        }
         next_score--;
    }
}
void level3rander()
{
    clear_renderer(0, 48, 1285, 725, 85, 139, 47);
    for (int i = 0; i < 6; i++)
    {
        if (avsnake[i] == 1)
        {
            for (int j = 0; j < 2; j++)
            {
                if (j == 0 && i % 2 == 0)
                    SDL_SetRenderDrawColor(renderer, 62, 39, 35, 255);
                else if (j == 1 && i % 2 == 0)
                    SDL_SetRenderDrawColor(renderer, 121, 85, 72, 255);
                else if (j == 1 && i % 2 == 1)
                    SDL_SetRenderDrawColor(renderer, 156, 39, 176, 255);
                else if (j == 0 && i % 2 == 1)
                    SDL_SetRenderDrawColor(renderer, 106, 27, 154, 255);

                SDL_Rect rect = {snake3[i][j].x * cell_size, snake3[i][j].y * cell_size, cell_size, cell_size};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    int temp, i;
    string st;
    char chr[10];
    st = to_string(charge);

    for (i = 0; i < st.size(); i++)
        chr[i] = st[i];
    chr[i] = '\0';
      drawText("chakra : ", "zebulon/Zebulon Bold Italic.otf", 950, 50, 30, {0, 0, 0});
     drawText(chr, "zebulon/Zebulon Bold Italic.otf", 1160, 50, 30, {0, 0, 0});
    img("image/naruto_tail.png", character.x * cell_size, character.y * cell_size, cell_size + 70, cell_size + 70);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 47, 1300, 47);
    if (score == next_score)
    {  cout<<"ssss";
        drawText("Score : ", "zebulon/Zebulon Bold Italic.otf", 950, 5, 30, {255, 0, 0});

        clear_renderer(1120, 0, 100, 40, 0, 0, 0);
        drawText(scores, "zebulon/Zebulon Bold Italic.otf", 1140, 5, 30, {255, 0, 0});
        drawText("Level 3", "zebulon/Zebulon Bold Italic.otf", 50, 5, 30, {255, 0, 0});
        drawText("A Failure's True Power", "zebulon/Zebulon Bold Italic.otf", 350, 5, 30, {255, 0, 0});
        next_score++;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(100);
}
// level 11111 gameeee
void game1()
{

    gamerun = initializeWindow();
    int time = 0;

    while (gamerun)
    {
        input();

        SDL_Delay(100);

        if (first == 0 && start == false && do_text == 0)
        {
            Mix_Resume(1);
            check_high_score();
            calchighscore();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            img("image/snake_sasuke.jpg", 0, 0, 1280, 720);
            drawText("SNAKE PRO", "zebulon/Zebulon Bold Italic.otf", 50, 80, 90, {255, 0, 0});
            drawText("HIGH SCORE : ", "font/SpaceMission.otf", 150, 240, 75, {255, 255, 255});
            drawText(high_scores, "zebulon/Zebulon Bold Italic.otf", 680, 220, 80, {255, 0, 0});
            drawText("To Start press F", "font/GothamMedium.ttf", 380, 350, 50, {255, 255, 255});
            drawText("To Quit press ESC", "font/GothamMedium.ttf", 320, 450, 50, {255, 255, 255});
            drawText("REDO GAMING", "font/SerpentineBoldItalic.ttf", 50, 600, 40, {255, 0, 0});
            drawText("©2023 REDO GAMING. All rights reserved.", "font/GothamMedium.ttf", 50, 650, 15, {255, 255, 255});
            SDL_RenderPresent(renderer);
            do_text = 1;
        }
        else if (do_text == 1 && start == false && gameover == false)
        {
            SDL_RenderPresent(renderer);
        }
        else if (start == true)
        {

            update_snake();
            calcscore();
            render();
            if (score == 12)
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