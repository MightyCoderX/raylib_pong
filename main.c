#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#define PADDLE_HEIGHT 100.0
#define PADDLE_WIDTH 20.0
#define PADDLE_SPEED 10.0

typedef struct {
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    int radius;
} Ball;

typedef struct {
    Vector2 pos;
    float vy;
} Paddle;

static Ball ball;

static const Vector2 paddle_size = (Vector2) { PADDLE_WIDTH, PADDLE_HEIGHT };
static Paddle left_paddle;
static Paddle right_paddle;

static void update(void);
static void applyForce(Ball* p_ball, int forceX, int forceY);

int main(void) {
    const float screenWidth = 800;
    const float screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    ball = (Ball) {
        .pos = (Vector2) { screenWidth / 2, screenHeight / 2 },
        .vel = (Vector2) { 5, 0 },
        .acc = (Vector2) { 0, 0 },
        .radius = 10,
    };

    left_paddle = (Paddle) {
        { .x = 20, .y = (screenHeight / 2) - (PADDLE_HEIGHT / 2) },
        0,
    };

    right_paddle = (Paddle) {
        { .x = screenWidth - 40, .y = (screenHeight / 2) - (PADDLE_HEIGHT / 2) },
        0,
    };

    while(!WindowShouldClose()) {
        BeginDrawing();

        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mpos = GetMousePosition();
            ball.pos.x = mpos.x;
            ball.pos.y = mpos.y;
            ball.vel.x = 10;
            ball.vel.y = 0;
        }

        if(IsKeyDown(KEY_W)) {
            left_paddle.vy = -PADDLE_SPEED;
        } else if(IsKeyDown(KEY_S)) {
            left_paddle.vy = PADDLE_SPEED;
        }

        if(IsKeyDown(KEY_UP)) {
            right_paddle.vy = -PADDLE_SPEED;
        } else if(IsKeyDown(KEY_DOWN)) {
            right_paddle.vy = PADDLE_SPEED;
        }

        ClearBackground(BLACK);

        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

        DrawRectangleV(left_paddle.pos, paddle_size, WHITE);
        DrawRectangleV(right_paddle.pos, paddle_size, WHITE);

        DrawCircle(ball.pos.x, ball.pos.y, ball.radius, ORANGE);

        EndDrawing();
        update();
    }

    CloseWindow();

    return 0;
}

void edges() {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();

    bool left = ball.pos.x - ball.radius <= 0;
    bool right = ball.pos.x + ball.radius >= screenWidth;

    if(left || right) {
        if(left) {
            printf("point right\n");
        } else if(right) {
            printf("point left\n");
        }
        ball.pos.x = screenWidth / 2;
        ball.pos.y = screenHeight / 2;
    }

    bool top = ball.pos.y - ball.radius <= 0;
    bool bottom = ball.pos.y + ball.radius >= screenHeight;

    if(top || bottom) {
        if(top) {
            ball.pos.y = ball.radius;
        } else if(bottom) {
            ball.pos.y = screenHeight - ball.radius;
        }
        ball.vel.y = -ball.vel.y;
    }
}

static void paddle_collide(Paddle paddle) {
    const float bx = ball.pos.x;
    const float by = ball.pos.y;
    const int r = ball.radius;
    const int x1 = paddle.pos.x;
    const int y1 = paddle.pos.y;
    const int x2 = paddle.pos.x + paddle_size.x;
    const int y2 = paddle.pos.y + paddle_size.y;

    // left or right side of rect
    bool left = (bx + r >= x1 && bx + r < x2);
    bool right = (bx - r <= x2 && bx - r > x1);
    if((by + r >= y1 && by - r < y2) && (left || right)) {
        if(left) {
            ball.pos.x = x1 - r;
        } else if(right) {
            ball.pos.x = x2 + r;
        }
        ball.vel.x = -ball.vel.x * 1;
        ball.vel.y += paddle.vy * 0.1;
    }
}

void update(void) {
    edges();

    paddle_collide(left_paddle);
    paddle_collide(right_paddle);

    left_paddle.pos.y += left_paddle.vy;
    left_paddle.vy *= fabs(left_paddle.vy) > 0 ? 0.8 : 0;

    right_paddle.pos.y += right_paddle.vy;
    right_paddle.vy *= fabs(right_paddle.vy) > 0 ? 0.8 : 0;

    ball.vel.x += ball.acc.x;
    ball.vel.y += ball.acc.y;

    ball.pos.x += ball.vel.x;
    ball.pos.y += ball.vel.y;

    ball.acc.x = 0;
    ball.acc.y = 0;
}

void applyForce(Ball* p_ball, int forceX, int forceY) {
    p_ball->acc.x += forceX;
    p_ball->acc.y += forceY;
}
