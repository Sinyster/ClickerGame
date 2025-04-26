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


#pragma region Prices
  // Upgrade Prices
  double genPrice = 1;
  int genItems = 0;

  double clickUpgradePrice = 1;
  int clickUpgItems = 0;

#pragma endregion

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
      DrawRectangleRec(button, isHovering ? DARKGREEN : GREEN);
      DrawText("Generate", button.x + 50, button.y + 15, 20, WHITE);

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
               "Generating per click: %0.2f", moneyPerClick);
      DrawText(moneyPerClickText, screenWidth / 3 + 6, screenHeight * 0.05 + 30,
               22, neonGreen);
#pragma region Balance
      // Format money into Text
      char moneyText[20];
      char moneyPerSecondText[20];
      snprintf(moneyText, sizeof(moneyText), "Balance: %0.2f", money);
      snprintf(moneyPerSecondText, sizeof(moneyPerSecondText), "%0.2f/s",
               moneyPerSecond);
      // Drawing Balance
      int textWidth = MeasureText(moneyText, 32);
      int textX = screenWidth / 6 - textWidth / 2;
      DrawText(moneyText, textX, screenHeight / 3, 32, neonGreen);

      // Drawing Balance per Second
      textWidth = MeasureText(moneyPerSecondText, 22);
      textX = screenWidth / 6 - textWidth / 2;
      DrawText(moneyPerSecondText, textX, screenHeight / 3 + 25, 22, neonGreen);
#pragma endregion

      // Drawing Upgrades
      bool isUpgradeHovering = false;
      const int blockHeight = 75;
#pragma region Generator  Upgrade
      Rectangle generatorUpgrade = {(screenWidth / 3) * 2 + 6,
                                    screenHeight * 0.05 + 26,
                                    (screenWidth / 3) - 18, blockHeight};

      DrawRectangleRec(generatorUpgrade, BLACK);

      isUpgradeHovering = CheckCollisionPointRec(mousePoint, generatorUpgrade);
      isClicked = isUpgradeHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(generatorUpgrade, isUpgradeHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 3) * 2 + 6, screenHeight * 0.05 + 26,
                         screenWidth / 3 - 18, blockHeight, neonGreen);
      DrawText("Generator", (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 30, 22, neonGreen);
      char genItemsText[20];
      snprintf(genItemsText, sizeof(genItemsText), "Bought: %dx", genItems);
      DrawText(genItemsText,
               (screenWidth / 3) * 2 +
                   ((screenWidth / 3) - MeasureText(genItemsText, 20) - 18),
               screenHeight * 0.05 + 50, 20, neonGreen);
      char genPriceText[30];
      snprintf(genPriceText, sizeof(genPriceText), "Price: %0.2f", genPrice);
      DrawText(genPriceText, (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 50, 20, neonGreen);

      if ((isClicked || IsKeyPressed(KEY_ONE)) && money >= genPrice) {
        money -= genPrice;
        moneyPerSecond += 0.1;
        genItems += 1;
        genPrice *= 1.6;
      }

#pragma endregion
#pragma region Enhanced click Upgrade
      Rectangle clickUpgrade = {(screenWidth / 3) * 2 + 6,
                                screenHeight * 0.05 + 32 + blockHeight,
                                (screenWidth / 3) - 18, blockHeight};
      DrawRectangleRec(clickUpgrade, BLACK);
      isUpgradeHovering = CheckCollisionPointRec(mousePoint, clickUpgrade);
      isClicked = isUpgradeHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(clickUpgrade, isUpgradeHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 3) * 2 + 6,
                         screenHeight * 0.05 + 32 + blockHeight,
                         screenWidth / 3 - 18, blockHeight, neonGreen);
      DrawText("Click Upgrade", (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 34 + blockHeight, 22, neonGreen);
      char clickItemsText[30];
      snprintf(clickItemsText, sizeof(clickItemsText), "Bought: %dx",
               clickUpgItems);
      DrawText(clickItemsText,
               (screenWidth / 3) * 2 +
                   ((screenWidth / 3) - MeasureText(clickItemsText, 20) - 18),
               screenHeight * 0.05 + 56 + blockHeight, 20, neonGreen);
      char clickPriceText[30];
      snprintf(clickPriceText, sizeof(clickPriceText), "Price: %0.2f",
               clickUpgradePrice);
      DrawText(clickPriceText, (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 56 + blockHeight, 20, neonGreen);

      if ((isClicked || IsKeyPressed(KEY_TWO)) && money >= clickUpgradePrice) {
        money -= clickUpgradePrice;
        moneyPerClick *= 1.2;
        clickUpgItems += 1;
        clickUpgradePrice *= 1.6;
      }

#pragma endregion
    } else if (currentScreen == SCREEN_SHORTCUTS) {
      char titleSh[] = "Shortcuts:";
      int textWidth = MeasureText(titleSh, 24);
      int textX = screenWidth / 2 - textWidth / 2;
      DrawText("Shortcuts:", textX, screenHeight * 0.04, 24, neonGreen);
      DrawText("G: Generate",
               screenWidth / 6 - MeasureText("G: Generate", 24) / 2,
               screenHeight / 6, 24, neonGreen);
      DrawText("1: First Upgrade",
               screenWidth / 6 - MeasureText("1: First Upgrade", 24) / 2,
               screenHeight / 6 + 30, 24, neonGreen);
      DrawText("2: Second Upgrade",
               screenWidth / 6 - MeasureText("2: Second Upgrade", 24) / 2,
               screenHeight / 6 + 60, 24, neonGreen);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}