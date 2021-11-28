# include "raylib.h"
# include <math.h>

int Difficulty = 0;
float sprint = 200.0f;
int GameScreen = 0;

class player
{
    private:
        Texture2D shrimp = LoadTexture("assets/spritesheet.png");
        float rot = 0.0f;
        float DX = 0.0f;
        float DY = 0.0f;
        float speed;
        float animation = 0.0f;
        int frame = 0;
        int j = 0;
    
    public:
        float x = 0.0f;
        float y = 0.0f;
        float frameWidth = shrimp.width / 4;
        float frameHeight = shrimp.height / 2;

        void move()
        {
            if (IsKeyDown(KEY_A) or IsKeyDown(KEY_LEFT))
            {
                rot -= 1 * GetFrameTime() * 60 * speed;
            }
            else if (IsKeyDown(KEY_D) or IsKeyDown(KEY_RIGHT))
            {
                rot += 1 * GetFrameTime() * 60 * speed;
            };
            
            float radian = rot * (3.14 / 180);

            DX = sin(radian) * 3;
            DY = cos(radian) * -3;

            if (IsKeyDown(KEY_SPACE) and sprint > 0)
            {
                speed = 2.5f;
                sprint -= 0.75f;
                animation += GetFrameTime() * 1.5;
                
            }
            else
            {
                speed = 1;
                animation += GetFrameTime();
                j = 0;
            }

            if (animation >= 0.1)
            {
                animation = 0;
                frame += 1;

                if (frame >= 4)
                {
                    frame = 0;
                };
                
            };

            DX *= speed;
            DY *= speed;

            x += DX * GetFrameTime() * 60;
            y += DY * GetFrameTime() * 60;
        };

        void draw()
        {
            if (speed == 2.5f)
            {
                DrawTexturePro(shrimp, Rectangle{frame * frameWidth, frameHeight, frameWidth, frameHeight}, Rectangle{x, y, frameWidth, frameHeight}, Vector2{frameWidth/2, frameHeight/2}, rot, (Color){255, 255, 255, 255});
            }
            else
            {
                DrawTexturePro(shrimp, Rectangle{frame * frameWidth, 0, frameWidth, frameHeight}, Rectangle{x, y, frameWidth, frameHeight}, Vector2{frameWidth/2, frameHeight/2}, rot, (Color){255, 255, 255, 255});
            }
        };

        void sprintBar()
        {
            DrawRectangleRounded(Rectangle{600, 20, sprint, 50}, 25, 12, (Color){224, 140, 30, 255});
            DrawRectangleRoundedLines(Rectangle{600, 20, 200, 50}, 25, 12, 3, (Color){125, 125, 125, 255});
            if (sprint >= 200)
            {
                sprint = 200;
            }
            else if (sprint <= 0)
            {
                sprint = 0;
            }
        }

        void reset()
        {
            x = 0.0f;
            y = 0.0f;
            rot = 0.0f;
        }
};

class enemy
{
    private:
        Texture2D load = LoadTexture("assets/hunter.png");
        float frameWidth = load.width / 4;
        float frameHeight = load.height;
        float DX;
        float DY;
        float rot = 0.0f;
        float radian;
        bool danger = false;
        int frame = 0;
        float animation = 0.0f;
        float x;
        float y;
        int pos;

        void position()
        {
            pos = GetRandomValue(0, 3);
            switch(pos)
            {
            case 0:
                x = GetRandomValue(-1200, -500);
                y = GetRandomValue(-1200, -500);
                break;
            case 1:
                x = GetRandomValue(500, 1200);
                y = GetRandomValue(-1200, -500);
                break;
            case 2:
                x = GetRandomValue(500, 1200);
                y = GetRandomValue(500, 1200);
                break;
            case 3:
                x = GetRandomValue(-1200, -500);
                y = GetRandomValue(500, 1200);
                break;
            default:
                x = 0;
                y = 0;
                break;
            }
        };

    public:

        bool attack = false;

        void move(player obj, Sound fx)
        {
            DX = obj.x - x;
            DY = obj.y - y;

            radian = atan2(DY, DX);

            rot = radian * (180/ 3.142);

            if (CheckCollisionCircleRec(Vector2{obj.x, obj.y}, 600, Rectangle{x, y, frameWidth, frameHeight}))
            {
                rot += 90;

                radian = rot * (3.142/180);

                x += sin(radian) * 4 * GetFrameTime() * 60;
                y -= cos(radian) * 4 * GetFrameTime() * 60;
                if (danger == false)
                {
                    attack = true;
                    danger = true;
                };
                if (CheckCollisionRecs(Rectangle{obj.x - obj.frameWidth/2, obj.y - obj.frameHeight/2, obj.frameWidth, obj.frameHeight}, Rectangle{x - frameWidth/2, y - frameHeight/2, frameWidth, frameHeight}))
                {
                    GameScreen = 3;
                    if (Difficulty >= 1)
                    {
                        PlaySound(fx);
                    };
                }
                animation += GetFrameTime() * 1.2;
                
            }
            else
            {
                rot += 90;
                radian = rot * (3.142/180);

                x += sin(radian) * GetFrameTime() * 60;
                y -= cos(radian) * GetFrameTime() * 60;

                danger = false;
                attack = false;
                animation += GetFrameTime();
            }
            if (animation >= 0.2)
            {
                animation = 0.0f;
                frame++;
                if (frame >= 4)
                {
                    frame = 0;
                }
                
            }
            
        };

        void draw()
        {
            DrawTexturePro(load, Rectangle{frameWidth * frame, 0, frameWidth, frameHeight}, Rectangle{x, y, frameWidth, frameHeight}, Vector2{frameWidth/2, frameHeight/2}, rot, (Color){255, 255, 255, 255});
        };

        void reset()
        {
            position();
            rot = 0.0f;
            danger = false;
        };
};

class food
{
    private:
        float x = GetRandomValue(-2000, 2000);
        float y = GetRandomValue(-2000, 2000);
        float radius = GetRandomValue(2, 5);
        float radie = radius;
        unsigned char grayScale = GetRandomValue(0, 100);
    
    public:
        void eat(player obj, Sound fx)
        {
            if (CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{obj.x - obj.frameWidth/2, obj.y - obj.frameHeight/2, obj.frameWidth, obj.frameHeight}))
            {
                radie += 0.5;
                if (radie >= radius*4)
                {
                    if (IsKeyDown(KEY_SPACE) == false and sprint < 200)
                    {
                        sprint += radius;
                    };
                    
                    Difficulty++;
                    PlaySound(fx);
                    x = GetRandomValue(-2000 * Difficulty * 0.5, 2000 * Difficulty * 0.5);
                    y = GetRandomValue(-2000 * Difficulty * 0.5, 2000 * Difficulty * 0.5);
                }
            }
            else
            {
                radie = radius;
            }
        };

        void draw()
        {
            unsigned char alpha = 255 * (radius/radie);
            unsigned char transparent = alpha * 0.5;
            DrawCircle(x, y, radie * 1.5, (Color){26, 85, 76, transparent});
            DrawCircle(x, y, radie, (Color){27, 85, 76, alpha});
        };

        void reset()
        {
            x = GetRandomValue(-2000, 2000);
            y = GetRandomValue(-2000, 2000);
            radius = GetRandomValue(2, 5);
        };
};

class particles
{
    private:
        const float wide = GetRandomValue(2000, 2750);
        float x = GetRandomValue(wide * -1, wide);
        float y = GetRandomValue(wide * -1, wide);
        float dx = GetRandomValue(-5, 5) / 5;
        float dy = GetRandomValue(-5, 5) / 5;
    
    public:
        void update()
        {
            x += dx * GetFrameTime() * 60;
            y += dy * GetFrameTime() * 60;

            if (x >= wide)
            {
                dx *= -1;
            }
            else if (x <= wide * -1)
            {
                dx *= -1;
            };

            if (y >= wide)
            {
                dy *= -1;
            }
            else if (y <= wide * -1)
            {
                dy *= -1;
            };

            if (dx == 0)
            {
                dx = GetRandomValue(-5, 5) / 5;
            }
            else if (dy == 0)
            {
                dy = GetRandomValue(-5, 5) / 5;
            };
            
            
        };

        void draw(Texture2D bubble)
        {
            DrawTextureRec(bubble, Rectangle{0, 0, (float)bubble.width, (float)bubble.height}, Vector2{x, y}, (Color){255, 255, 255, 255});
        };
};
struct random
{
    float val = GetRandomValue(-350, 350);
};

void DrawBoundry(Texture2D shell)
{
    for (int i = 0; i < 100; i++)
    {
        DrawTexture(shell, i * 70 + 20, 20, (Color){255, 255, 255, 255});
        DrawTexture(shell, i * 70 + 20, 800 - 20 - shell.height, (Color){255, 255, 255, 255});
    };
    for (int i = 0; i < 12; i++)
    {
        DrawTexture(shell, 20, i * 60 + 60, (Color){255, 255, 255, 255});
        DrawTexture(shell, 1400 - 20 - shell.width, i * 60 + 60, (Color){255, 255, 255, 255});
    }
    
    
};

int main()
{
    int Width = 1400;
    int Height = 800;
    
    InitWindow(Width, Height, "Plankton Simulator");

    player obj;
    enemy shark[7];
    const int foodDensity = GetRandomValue(400, 750);
    food plakton[foodDensity];
    const int particleDense = 1000;
    particles party[particleDense];
    random offset[8];
    random SeaShell[16];

    Camera2D camera = { 0 };
    camera.offset = Vector2{(float)Width/2, (float)Height/2};
    camera.target = Vector2{obj.x, obj.y};
    camera.zoom = 1.0f;
    
    bool dangerZone = false;
    int frames = 0;

    Texture2D logo = LoadTexture("assets/logo.png");
    float logoWidth = logo.width;
    float logoHeight = logo.height;
    InitAudioDevice();
    SetMasterVolume(0.3f);

    Texture2D bubble = LoadTexture("assets/bubble.png");
    Texture2D shell = LoadTexture("assets/shell.png");
    Texture2D flower = LoadTexture("assets/marine plant.png");
    Texture2D home = LoadTexture("assets/home.png");

    Sound fxEat = LoadSound("assets/plankton.wav");
    Sound fxLaunch = LoadSound("assets/start.wav");
    Sound fxDie = LoadSound("assets/die.wav");

    float subY = 0;
    
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        switch (GameScreen)
        {
        case 0:
            frames++;
            if (frames >= 120)
            {
                GameScreen = 1;
            };
            
            break;
        
        case 1:
            subY += 0.08;

            if (IsMouseButtonReleased(0))
            {
                if (GetMouseX() > 600 and GetMouseX() < 800)
                {
                    if (GetMouseY() > 300 and GetMouseY() < 380)
                    {
                        GameScreen = 2;
                    }
                    else if (GetMouseY() > 530 and GetMouseY() < 630)
                    {
                        CloseWindow();
                    }
                    
                }
                
            }
            for (int i = 0; i < particleDense; i++)
            {
                party[i].update();
            };
            break;
        
        case 2:
            obj.move();
            camera.target = Vector2{obj.x, obj.y};

            dangerZone = false;

            for (int i = 0; i < foodDensity; i++)
            {
                plakton[i].eat(obj, fxEat);

                if (i < 7)
                {
                    shark[i].move(obj, fxDie);

                    if (shark[i].attack == true)
                    {
                        dangerZone = true;
                    };
                    
                };
                                
            };
            
            for (int i = 0; i < particleDense; i++)
            {
                party[i].update();
            };
            
            

            if (dangerZone == true)
            {
                if (camera.zoom > 0.75)
                {
                    camera.zoom -= 0.01f;
                };
                    
            }
            else
            {
                if (camera.zoom < 1.0f)
                {
                    camera.zoom += 0.01f;
                };
                    
            };
            
            break;
        
        case 3:
            if (IsKeyPressed(KEY_SPACE))
            {
                PlaySound(fxLaunch);
                GameScreen = 2;
                obj.reset();
                Difficulty = 0;
                sprint = 200;
                
                for (int i = 0; i < foodDensity; i++)
                {
                    plakton[i].reset();

                    if (i < 7)
                    {
                        shark[i].reset();
                    };
                    
                };
                
            };
            if (IsMouseButtonReleased(0))
            {
                if (GetMouseX() > 1300 and GetMouseX() < 1350)
                {
                    if (GetMouseY() > 50 and GetMouseY() < 100)
                    {
                        GameScreen = 1;
                    };
                    
                };
                
            };
            
            for (int i = 0; i < particleDense; i++)
            {
                party[i].update();
            };
            break;
        
        default:
            break;
        }
        BeginDrawing();

            switch (GameScreen)
            {
                case 0:
                {
                    if (frames >= 60)
                    {
                        unsigned char alpha = (120.0f / (float)frames) * 255;

                        ClearBackground((Color){245, 245, 245, alpha});

                        DrawTexture(logo, 700 - logoWidth/2, 400 - logoHeight/2, (Color){255, 255, 255, alpha});
                        DrawCircleLines(700, 400, 250, (Color){125, 125, 125, alpha});
                        DrawCircleLines(700, 400, 249, (Color){125, 125, 125, alpha});
                    }
                    else
                    {
                        ClearBackground(RAYWHITE);

                        DrawTexture(logo, 700 - logoWidth/2, 400 - logoHeight/2, (Color){255, 255, 255, 255});
                        DrawCircleLines(700, 400, 250, (Color){125, 125, 125, 255});
                        DrawCircleLines(700, 400, 249, (Color){125, 125, 125, 255});
                    }
                    
                }break;
                case 1:
                {
                    ClearBackground((Color){71, 140, 191, 255});
                    
                    for (int i = 0; i < particleDense; i++)
                    {
                        party[i].draw(bubble);
                    };
                    DrawBoundry(shell);

                    DrawRectangleLinesEx(Rectangle{400, 100 + sin(subY)*20, 600, 80}, 8, (Color){25, 25, 25, 255});
                    DrawRectangleLinesEx(Rectangle{600, 300, 200, 80}, 8, (Color){25, 25, 25, 255});
                    DrawRectangleLinesEx(Rectangle{600, 530, 200, 80}, 8, (Color){25, 25, 25, 255});

                    DrawText("Start", 620, 320, 60, (Color){25, 25, 25, 255});
                    DrawText("Close", 620, 550, 60, (Color){25, 25, 25, 255});
                    DrawText("Plankton Simulator", 420, 120 + sin(subY)*17, 60, (Color){25, 25, 25, 255});
                }break;
                case 2:
                {
                    ClearBackground((Color){71, 140, 191, 255});
                    BeginMode2D(camera);
                        for (int i = -7; i < 7; i++)
                        {
                            for (int j = -7; j < 8; j++)
                            {
                                float XOffset = SeaShell[i + 8].val * j;
                                float YOffset = SeaShell[j + 8].val * i;
                                DrawTextureRec(shell, Rectangle{0, 0, (float)shell.width, (float)shell.height}, Vector2{i * 400.0f + XOffset, j * 400.0f + YOffset}, (Color){255, 255, 255, 255});
                            };
                            
                        };
                        for (int i = -3; i < 4; i++)
                        {
                            for (int j = -4; j < 3; j++)
                            {
                                float offsetX = offset[i+3].val * j;
                                float offsetY = offset[j+4].val * i;
                                DrawTextureRec(flower, Rectangle{0, 0, (float)flower.width, (float)flower.height}, Vector2{i * 600.0f + offsetX, j * 700.0f + offsetY}, (Color){255, 255, 255, 255});
                            }
                            
                        }
                        
                        obj.draw();
                        for (int i = 0; i < foodDensity; i++)
                        {
                            plakton[i].draw();

                            if (i < 7)
                            {
                                shark[i].draw();
                            };
                            
                        };
                        for (int i = 0; i < particleDense; i++)
                        {
                            party[i].draw(bubble);
                        };
                        
                    EndMode2D();

                    DrawText(TextFormat("%i", Difficulty), 20, 20, 30, (Color){0, 0, 0, 255});
                    obj.sprintBar();
                }break;
                case 3:
                {
                    ClearBackground((Color){72, 140, 191, 255});
                    for (int i = 0; i < particleDense; i++)
                    {
                        party[i].draw(bubble);
                    };
                    DrawBoundry(shell);
                    DrawText(TextFormat("%i", Difficulty), 680, 250, 50, (Color){0, 0, 0, 255});
                    if (Difficulty >= 1)
                    {
                        DrawText("You were eaten by a Predator", 330, 400, 50, (Color){0, 0, 0, 255});
                        DrawText("Press Space to Restart", 400, 550, 50, (Color){0, 0, 0, 255});
                    }
                    else
                    {
                        DrawText("Press Space to Start", 400, 450, 50, (Color){0, 0, 0, 255});
                    };
                    DrawTexture(home, 1300, 50, (Color){255, 255, 255, 255});
                }break;
                default: break;
            }

        EndDrawing();
    };
    
    CloseWindow();
};
