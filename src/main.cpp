<<<<<<< HEAD
#include "header.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


bool fps60 = false;
bool ismenuopen = false;
bool dropdownEditMode = false;
bool colorDropdownEditMode = false;

class Ball {
public:
    float x = 400;
    float y = 225;
    int radius = 10;
    int speedX = 4;
    int speedY = 3;

    void Update() {
        x += speedX;
        y += speedY;
        if (y <= radius || y >= GetScreenHeight() - radius) {
            speedY *= -1;
        }
    }

    void Reset() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        speedX = (GetRandomValue(0, 1) == 0) ? -4 : 4;
        speedY = (GetRandomValue(0, 1) == 0) ? -3 : 3;
    }
};

class Player1 {
public:
    float x = 10;
    float y = 200;
    int Width = 10;
    int Height = 80;
    int speedY = 5;

    void Update() {
        if (IsKeyDown(KEY_W)) y -= speedY;
        if (IsKeyDown(KEY_S)) y += speedY;
        if (y < 0) y = 0;
        if (y > GetScreenHeight() - Height) y = GetScreenHeight() - Height;
    }
};

class Player2 {
public:
    float y = 200;
    int Width = 10;
    int Height = 80;
    int speedY = 5;

    void Update() {
        if (IsKeyDown(KEY_UP)) y -= speedY;
        if (IsKeyDown(KEY_DOWN)) y += speedY;
        if (y < 0) y = 0;
        if (y > GetScreenHeight() - Height) y = GetScreenHeight() - Height;
    }
};

int main() {
    int screenWidth = 800;
    int screenHeight = 450;

    Ball ball;
    Player1 player1;
    Player2 player2;
    int score1 = 0;
    int score2 = 0;

    float volume = 0.5f;
    bool fullscreen = false;
    bool vsync = false;
    int difficulty = 0;
    bool showfps = false;
    int color = 0;

    Rectangle volumeRect = { 180, 100, 200, 20 };
    Rectangle fullscreenRect = { 180, 130, 20, 20 };
    Rectangle vsyncRect = { 180, 155, 20, 20 };
    Rectangle difficultyRect = { 180, 215, 200, 25 };
    Rectangle saveButtonRect = { 100, 340, 100, 30 };
    Rectangle backButtonRect = { 100, 390, 100, 30 };
    Rectangle fpsRect = { 180, 180, 20, 20 };
    Rectangle colorRect = { 570, 215, 200, 25 };

    raylib::Window window(screenWidth, screenHeight, "RayPong");

    window.SetTargetFPS(60);
    fps60 = true;
    ::SetExitKey(0);

    while (!window.ShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) ismenuopen = !ismenuopen;

        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        window.BeginDrawing();
        {
            if (ismenuopen) {
                window.ClearBackground(RAYWHITE);
                DrawText("Settings", 300, 50, 24, DARKGRAY);

                DrawText("Volume:", 100, 107, 16, DARKGRAY);
                volume = GuiSlider(volumeRect, NULL, NULL, &volume, 0.0f, 1.0f);

                DrawText("Fullscreen:", 93, 130, 15, DARKGRAY);
                GuiCheckBox(fullscreenRect, NULL, &fullscreen);

                DrawText("VSync:", 100, 150, 16, DARKGRAY);
                GuiCheckBox(vsyncRect, NULL, &vsync);

                DrawText("Fps", 100, 180, 16, DARKGRAY);
                GuiCheckBox(fpsRect, NULL, &showfps);

                DrawText("Difficulty:", 100, 215, 16, DARKGRAY);
                if (GuiDropdownBox(difficultyRect, "Easy;Medium;Hard", &difficulty, dropdownEditMode)) {
                    dropdownEditMode = !dropdownEditMode;
                }
                DrawText("Background:", 470, 215, 16, DARKGRAY);
                if (GuiDropdownBox(colorRect, "Pink-Red;Green;Purple;Blue", &color, colorDropdownEditMode)) {
                    colorDropdownEditMode = !colorDropdownEditMode;
                }
                if (GuiButton(saveButtonRect, "Save")) {
                    if (fullscreen && !IsWindowFullscreen()) ToggleFullscreen();
                    if (!fullscreen && IsWindowFullscreen()) ToggleFullscreen();

                    if (difficulty == 0) ::SetTargetFPS(60);
                    else if (difficulty == 1) ::SetTargetFPS(120);
                    else if (difficulty == 2) ::SetTargetFPS(240);
                }

                if (GuiButton(backButtonRect, "Back")) {
                    ismenuopen = false;
                }
            } else {
                    ball.Update();
                    player1.Update();
                    player2.Update();

                    float p2x = (float)(sw - player2.Width - 10);

                    if (ball.x - ball.radius < player1.x + player1.Width &&
                        ball.x + ball.radius > player1.x &&
                        ball.y + ball.radius > player1.y &&
                        ball.y - ball.radius < player1.y + player1.Height)
                    {
                        ball.speedX *= -1;
                    }
                    if (ball.x + ball.radius > p2x &&
                        ball.x - ball.radius < p2x + player2.Width &&
                        ball.y + ball.radius > player2.y &&
                        ball.y - ball.radius < player2.y + player2.Height)
                    {
                        ball.speedX *= -1;
                    }

                    if (ball.x < 0) {
                        score2++;
                        ball.Reset();
                    }
                    if (ball.x > sw) {
                        score1++;
                        ball.Reset();
                    }

                    if (color == 0) window.ClearBackground(raylib::Color(247, 64, 86));
                    else if (color == 1) window.ClearBackground(raylib::Color(0, 158, 96));
                    else if (color == 2) window.ClearBackground(raylib::Color(200, 180, 230));
                    else if (color == 3) window.ClearBackground(raylib::Color(0, 127, 255));

                    ::DrawRectangle(0, sh / 2 - 40, 5, 80, RED);
                    ::DrawRectangle(sw - 5, sh / 2 - 40, 5, 80, RED);

                    DrawText(TextFormat("%i", score1), sw / 4 - 10, 20, 40, BLACK);
                    DrawText(TextFormat("%i", score2), sw * 3 / 4 - 10, 20, 40, BLACK);

                    ::DrawLine((float)(sw / 2), 0.0f, (float)(sw / 2), (float)sh, BLACK);

                    DrawCircle((int)ball.x, (int)ball.y, ball.radius, WHITE);
                    DrawRectangle((int)player1.x, (int)player1.y, player1.Width, player1.Height, WHITE);
                    DrawRectangle((int)p2x, (int)player2.y, player2.Width, player2.Height, WHITE);


                    if (showfps) DrawFPS(10, 10);
            }
            window.EndDrawing();
        }
    }
    return 0;
}
=======

#include "raylib.h"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color tex tColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);
            textColor.DrawText("Congrats! You created your first window!", 190, 200, 20);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
>>>>>>> 5e03f88b881a6afaa8b0399828d589731e3f2fd9
