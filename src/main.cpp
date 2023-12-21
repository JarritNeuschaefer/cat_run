#include "raylib.h"

class Player {
public:
    Rectangle collider;
    Vector2 position;
    Vector2 velocity;
    float jumpForce;
    bool isJumping;
    const Texture2D gatze = LoadTexture("textures/gatze.png");
    const Texture2D go_gatze = LoadTexture("textures/gatze_traurig.png");
    Sound sound = LoadSound("audio/yippee-tbh.mp3");


    Player(float x, float y) :collider({x - 40, y - 55, 75, 75}), position({x, y -50}), velocity({0, 0}), jumpForce(14.0f), isJumping(false) {}

    void Update(bool paused) {
        if(!paused)
        {
            velocity.y += 0.4f;

            // Jump when space is pressed
            if (IsKeyDown(KEY_SPACE) && !isJumping) {
                PlaySound(sound);
                velocity.y = -jumpForce;
                isJumping = true;
            }

            // Update position based on velocity
            position.y += velocity.y;
            collider.y = position.y - 40;

            // Simple ground collision (stop falling when hitting the ground)
            if (position.y >= GetScreenHeight() - 70) {
                position.y = GetScreenHeight() - 65;
                velocity.y = 0;
                isJumping = false;
            }
        }
    }

    void Draw(bool isPaused, bool isRunning) const {
        //DrawCircle(position.x, position.y, 20, BLACK);
        DrawRectangle(collider.x, collider.y, collider.width, collider.height, BLUE);
        //DrawTextureRec(gatze, { 0.0f, 0.0f, gatze.width / 2.0f, static_cast<float>(gatze.height)}, {collider.x, collider.y}, WHITE);
        if(isPaused && isRunning)
            {
                DrawTextureEx(go_gatze, (Vector2){collider.x, collider.y}, 0, 2.35f, WHITE);
            }
        else
        {
            if(!isJumping)
            {
                DrawTexturePro(
                gatze, 
                {0.0f, 0.0f, gatze.width / 2.0f, static_cast<float>(gatze.height)}, 
                {(Rectangle){collider.x, collider.y, gatze.width * 1.2f, gatze.height * 2.35f}}, 
                {0,0},
                0, WHITE);
            }
        else
            {
                DrawTexturePro(
                gatze, 
                {gatze.width / 2.0f, 0.0f, static_cast<float>(gatze.height), static_cast<float>(gatze.height)}, 
                {(Rectangle){collider.x, collider.y, gatze.width * 1.2f, gatze.height * 2.35f}}, 
                {0,0},
                0, WHITE);
            }
        }
    }
};

class Block {
public:
    Rectangle rect;
    float speed;
    float offset;

    Block(int screenWidth, int screenHeight, float blockSpeed) : rect({static_cast<float>(screenWidth - 40), static_cast<float>(screenHeight * 0.785f ), 70, 100}), speed(blockSpeed) {}

    void Update(bool paused) {
        if (!paused){
            rect.x -= speed;

        // Respawn the block if it goes off the left side of the screen
        if (rect.x + rect.width <= 0) {
                rect.x = GetScreenWidth();
                speed = GetRandomValue(5 + offset, 8 + offset);
                offset += 0.1f;
            }
        }
    }

    void Draw() const {
        DrawRectangleRec(rect, BROWN);
    }

};

class Floor {
public:
    Rectangle rect;

    Floor(int screenWidth, int screenHeight) : rect({0, static_cast<float>(screenHeight - 30), static_cast<float>(screenWidth), 30}) {}

    void Draw() const {
        DrawRectangleRec(rect, GREEN);
    }
};

int main() {
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitAudioDevice();
    InitWindow(screenWidth, screenHeight, "1 Spiel");

    Sound stains = LoadSound("audio/stains.mp3");
    Sound egg = LoadSound("audio/egg.wav");

    SetTargetFPS(60);

    Player player(screenWidth / 6, screenHeight - 50);
    Floor floor(screenWidth, screenHeight);
    Block block(screenWidth, screenHeight, 5.0f);

    bool paused = true;
    bool started = false;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        player.Update(paused);
        block.Update(paused);

        


        if (CheckCollisionRecs(player.collider, block.rect) && !paused) 
        {
            StopSound(stains);
            PlaySound(egg);
            paused = true;
        }
        if (paused) {
            if(started)
            {
                DrawText("verloren", screenWidth / 2 - MeasureText("verloren", 40) / 2, screenHeight / 2 - 20, 40, BLACK);
            }
            else{
                DrawText("press enter to start game", screenWidth / 2 - MeasureText("press enter to start game", 40) / 2, screenHeight / 2 - 20, 40, BLACK);
            }
        if(IsKeyPressed(KEY_ENTER))
        {
            PlaySound(stains);
            block.offset = 0;
            block.speed = 5;
            player.position.y = screenHeight - 65;
            block.rect.x = screenWidth - 40;
            paused = false;
            if(!started)
            {
                started = true;
            }
        }
        }

        // Draw
        BeginDrawing();

        ClearBackground(BLUE);
        floor.Draw();
        block.Draw();
        player.Draw(paused, started);

        EndDrawing();
    }

    UnloadSound(egg);
    UnloadSound(stains);
    // De-Initialization
    CloseWindow();

    return 0;
}