#include "raylib.h"
#include <math.h>
#include <stdio.h>

int main(void) {
  // Base Variables
  const int screenWidth = 1500;
  const int screenHeight = 800;
  char title[] = "Clicker";

  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  // Game Variables
  typedef enum { SCREEN_MAIN, SCREEN_SHORTCUTS } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  double money = 0;
  double moneyPerSecond = 0;
  double moneyPerClick = 0.1;

  // Upgrade Prices
  double genPrice = 10;
  double genItems = 0;

  while (!WindowShouldClose()) {
    // Colors
    Color neonGreen = (Color){57, 255, 20, 255};

    Vector2 mousePoint = GetMousePosition();

    if (IsKeyPressed(KEY_TAB)) {
      currentScreen =
          (currentScreen == SCREEN_MAIN) ? SCREEN_SHORTCUTS : SCREEN_MAIN;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleLines(6, 6, screenWidth - 12, screenHeight - 12, neonGreen);

    if (currentScreen == SCREEN_MAIN) {
      Rectangle button = {screenWidth / 6 - 100, screenHeight / 2 - 50, 200,
                          50};
      bool isHovering = CheckCollisionPointRec(mousePoint, button);
      bool isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      if (isClicked || IsKeyPressed(KEY_G)) {
        money += moneyPerClick;
      }

      if (moneyPerSecond > 0) {
        money += moneyPerSecond / 60;
      }

      // Drawing Base Environment
      DrawRectangleLines(screenWidth / 3, 6, screenWidth / 3, screenHeight - 12,
                         neonGreen);

      // Headers
      DrawText("Statistics:", screenWidth / 3 + 10, screenHeight * 0.04, 22,
               neonGreen);
      DrawText("Upgrades & Items:", (screenWidth / 3) * 2 + 10,
               screenHeight * 0.04, 22, neonGreen);
      DrawLine(screenWidth / 3, screenHeight * 0.05 + 20, screenWidth - 6,
               screenHeight * 0.05 + 20, neonGreen);

      // Drawing Statistics
      char moneyPerClickText[30];
      snprintf(moneyPerClickText, sizeof(moneyPerClickText),
               "Generating per click: %0.5f", moneyPerClick);
      DrawText(moneyPerClickText, screenWidth / 3 + 6, screenHeight * 0.05 + 30,
               22, neonGreen);

      // Format money into Text
      char moneyText[20];
      char moneyPerSecondText[20];
      snprintf(moneyText, sizeof(moneyText), "Balance: %0.5f", money);
      snprintf(moneyPerSecondText, sizeof(moneyPerSecondText), "%0.5f/s",
               moneyPerSecond);
      // Drawing Balance
      int textWidth = MeasureText(moneyText, 32);
      int textX = screenWidth / 6 - textWidth / 2;
      DrawText(moneyText, textX, screenHeight / 3, 32, neonGreen);

      // Drawing Balance per Second
      textWidth = MeasureText(moneyPerSecondText, 22);
      textX = screenWidth / 6 - textWidth / 2;
      DrawText(moneyPerSecondText, textX, screenHeight / 3 + 25, 22, neonGreen);

      bool isUpgradeHovering = false;
// Drawing Upgrades
#pragma region
      Rectangle generatorUpgrade = {(screenWidth / 3) * 2 + 6,
                                    screenHeight * 0.05 + 26,
                                    (screenWidth / 3) - 18, 75};

      DrawRectangleRec(generatorUpgrade, BLACK);

      isUpgradeHovering = CheckCollisionPointRec(mousePoint, generatorUpgrade);
      isClicked = isUpgradeHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(generatorUpgrade, isUpgradeHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 3) * 2 + 6, screenHeight * 0.05 + 26,
                         screenWidth / 3 - 18, 75, neonGreen);
      DrawText("Generator", (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 30, 22, neonGreen);
      char genPriceText[20];
      snprintf(genPriceText, sizeof(genPriceText), "Price: %0.5f", genPrice);
      DrawText(genPriceText, (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 50, 20, neonGreen);

      if ((isClicked || IsKeyPressed(KEY_ONE)) && money >= genPrice) {
        money -= genPrice;
        moneyPerSecond += 0.1;
        genItems += 1;
        genPrice *= (genItems * 1.1);
      }

#pragma endregion

      DrawRectangleRec(button, isHovering ? DARKGREEN : GREEN);
      DrawText("Generate", button.x + 50, button.y + 15, 20, WHITE);
    } else if (currentScreen == SCREEN_SHORTCUTS) {
      char titleSh[] = "Shortcuts:";
      int textWidth = MeasureText(titleSh, 20);
      int textX = screenWidth / 2 - textWidth / 2;
      DrawText("Shortcuts:", textX, screenHeight * 0.04, 20, neonGreen);
      DrawText("G: Generate",
               screenWidth / 6 - MeasureText("G: Generate", 20) / 2,
               screenHeight / 6, 20, neonGreen);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}