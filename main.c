#include "raylib.h"
#include <math.h>
#include <stdio.h>

typedef struct GameData {
  // Default Variables
  double money;
  double moneyPerSecond;
  double moneyPerClick;

  // Battery & Stuff
  double batteryCapacity;
  double batteryPercent;
  double batteryDrainPerSecond;

  // Items Variables
  double genPrice;
  int genItems;

  double clickPrice;
  int clickItems;
} GameData;

void saveGame(GameData *data);
GameData loadGame();

int main(void) {
  // Base Variables
  const int screenWidth = 1280;
  const int screenHeight = 720;
  char title[] = "Clicker";

  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  // Game Screens
  typedef enum {
    SCREEN_MAIN,
    SCREEN_SHORTCUTS,
    SCREEN_MAINMENU,
    SCREEN_GAMEOVER
  } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  bool isMainMenuOpened = false;

  // Defaulting Variables
  bool justSaved = false;
  bool justLoaded = false;

  GameData game = {0};
  game.moneyPerClick = 0.1;
  game.moneyPerSecond = 0.0;
  game.genPrice = 1.0;
  game.clickPrice = 1.0;
  game.batteryCapacity = 100.0;
  game.batteryPercent = game.batteryCapacity;
  game.batteryDrainPerSecond = 1.0;

  // Timer Variables
  static float timer = 0.0f;
  static float saveTimer = 0.0f;
  static float secondsTillSave = 30.0f;

#pragma endregion

#pragma region APP
  while (!WindowShouldClose()) {
    // Colors
    Color neonGreen = (Color){57, 255, 20, 255};

    // Mouse XY Axis
    Vector2 mousePoint = GetMousePosition();

    // Openning Shortcut Screen
    if (IsKeyPressed(KEY_TAB)) {
      currentScreen =
          (currentScreen == SCREEN_MAIN) ? SCREEN_SHORTCUTS : SCREEN_MAIN;
    } else if (IsKeyPressed(KEY_ESCAPE)) {
      isMainMenuOpened = isMainMenuOpened ? false : true;
      currentScreen = isMainMenuOpened ? SCREEN_MAINMENU : SCREEN_MAIN;
    }

    // Drawing APP
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleLines(6, 6, screenWidth - 12, screenHeight - 12, neonGreen);

    // Screen Selector
    if (currentScreen == SCREEN_MAIN) {
#pragma region Base Render
      // Generate Button
      Rectangle button = {screenWidth / 6 - 100, screenHeight / 2 - 50, 200,
                          50};
      bool isHovering = CheckCollisionPointRec(mousePoint, button);
      bool isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      if (isClicked || IsKeyPressed(KEY_G)) {
        game.money += game.moneyPerClick;
      }

      // Generating Money Per Second
      if (game.moneyPerSecond > 0) {
        game.money += game.moneyPerSecond / 60;
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
      DrawText("ESC: Main Menu", 10, 10, 16, neonGreen);
      DrawText("TAB: Shortcut Menu", 10, 26, 16, neonGreen);
      DrawLine(screenWidth / 3, screenHeight * 0.05 + 20, screenWidth - 6,
               screenHeight * 0.05 + 20, neonGreen);

#pragma endregion

#pragma region Battery Function
      // Battery Render
      Rectangle batteryRecharge = {(screenWidth / 6) - 100, screenHeight * 0.8,
                                   200, 75};
      isHovering = CheckCollisionPointRec(mousePoint, batteryRecharge);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(batteryRecharge, isHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 6 - 100), screenHeight * 0.8, 200, 75,
                         neonGreen);

      char batteryLife[30];
      snprintf(batteryLife, sizeof(batteryLife), "Battery: %0.1f",
               game.batteryPercent);
      DrawText(batteryLife,
               (screenWidth / 6) - MeasureText(batteryLife, 20) / 2,
               screenHeight * 0.8 - 20, 20, neonGreen);
      DrawText("Recharge", screenWidth / 6 - MeasureText("Recharge", 22) / 2,
               screenHeight * 0.8 + 75 / 2 - 11, 22, neonGreen);

      // Battery function
      if (game.moneyPerSecond > 0.0) {
        game.batteryPercent -= GetFrameTime();
        if (game.batteryPercent < 0.1) {
          currentScreen = SCREEN_GAMEOVER;
        }
      }
#pragma endregion

#pragma region AutoSave Func
      secondsTillSave -= GetFrameTime();
      if (secondsTillSave < 0.2f && secondsTillSave > 0.1f) {
        saveGame(&game);
        justSaved = true;
      } else if (secondsTillSave < 0.1f) {
        secondsTillSave = 30.0f;
      }

      if (justSaved) {
        timer += GetFrameTime();
        if (timer < 1.5f) {
          DrawText("Game Saved!",
                   screenWidth / 2 - MeasureText("Game Saved!", 26) / 2,
                   (screenHeight / 10) * 9 - 26, 26, neonGreen);
        } else {
          justSaved = false;

          timer = 0.0f;
        }
      }

      char autosaveTimer[20];
      snprintf(autosaveTimer, sizeof(autosaveTimer), "Autosave in: %0.1fs",
               secondsTillSave);
      DrawText(autosaveTimer,
               screenWidth / 2 - MeasureText(autosaveTimer, 20) / 2,
               (screenHeight / 10) * 9, 20, neonGreen);
#pragma endregion
#pragma region Draw Stats
      // Drawing Statistics
      char batteryDrainText[30];
      snprintf(batteryDrainText, sizeof(batteryDrainText),
               "Battery Drain: %0.1fs", game.batteryDrainPerSecond);
      DrawText(batteryDrainText, screenWidth / 3 + 6, screenHeight * 0.05 + 30,
               22, neonGreen);
      char moneyPerClickText[30];
      snprintf(moneyPerClickText, sizeof(moneyPerClickText),
               "Generating per click: %0.2f", game.moneyPerClick);
      DrawText(moneyPerClickText, screenWidth / 3 + 6, screenHeight * 0.05 + 60,
               22, neonGreen);
      char generatorPerSecond[30];
      snprintf(generatorPerSecond, sizeof(generatorPerSecond),
               "Generators generating: %0.2f/s", game.genItems * 0.1);
      DrawText(generatorPerSecond, screenWidth / 3 + 6,
               screenHeight * 0.05 + 90, 22, neonGreen);
#pragma endregion
#pragma region Money
      // Format money into Text
      if (game.money < 0) {
        game.money = 0;
      }
      char moneyText[20];
      char moneyPerSecondText[20];
      snprintf(moneyText, sizeof(moneyText), "Money: %0.2f", game.money);
      snprintf(moneyPerSecondText, sizeof(moneyPerSecondText), "%0.2f/s",
               game.moneyPerSecond);
      // Drawing Money
      int textWidth = MeasureText(moneyText, 32);
      int textX = screenWidth / 6 - textWidth / 2;
      DrawText(moneyText, textX, screenHeight / 3, 32, neonGreen);

      // Drawing Money per Second
      textWidth = MeasureText(moneyPerSecondText, 22);
      textX = screenWidth / 6 - textWidth / 2;
      DrawText(moneyPerSecondText, textX, screenHeight / 3 + 32, 22, neonGreen);
#pragma endregion

      // Drawing Upgrades
      bool isUpgradeHovering = false;
      const int blockHeight = 75;
#pragma region Click Upgrade
      Rectangle clickUpgrade = {(screenWidth / 3) * 2 + 6,
                                screenHeight * 0.05 + 26,
                                (screenWidth / 3) - 18, blockHeight};
      DrawRectangleRec(clickUpgrade, BLACK);
      isUpgradeHovering = CheckCollisionPointRec(mousePoint, clickUpgrade);
      isClicked = isUpgradeHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(clickUpgrade, isUpgradeHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 3) * 2 + 6, screenHeight * 0.05 + 26,
                         screenWidth / 3 - 18, blockHeight, neonGreen);
      DrawText("Click Upgrade", (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 30, 22, neonGreen);
      char clickItemsText[30];
      snprintf(clickItemsText, sizeof(clickItemsText), "Bought: %dx",
               game.clickItems);
      DrawText(clickItemsText,
               (screenWidth / 3) * 2 +
                   ((screenWidth / 3) - MeasureText(clickItemsText, 20) - 18),
               screenHeight * 0.05 + 50, 20, neonGreen);
      char clickPriceText[30];
      snprintf(clickPriceText, sizeof(clickPriceText), "Price: %0.2f",
               game.clickPrice);
      DrawText(clickPriceText, (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 50, 20, neonGreen);

      if ((isClicked || IsKeyPressed(KEY_ONE)) &&
          game.money >= game.clickPrice - 0.1) {
        game.money -= game.clickPrice;
        game.moneyPerClick *= 1.2;
        game.clickItems += 1;
        game.clickPrice *= 1.6;
      }
#pragma endregion
#pragma region Generator Upgrade
      Rectangle generatorUpgrade = {(screenWidth / 3) * 2 + 6,
                                    screenHeight * 0.05 + 32 + blockHeight,
                                    (screenWidth / 3) - 18, blockHeight};
      DrawRectangleRec(generatorUpgrade, BLACK);
      isUpgradeHovering = CheckCollisionPointRec(mousePoint, generatorUpgrade);
      isClicked = isUpgradeHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(generatorUpgrade, isUpgradeHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 3) * 2 + 6,
                         screenHeight * 0.05 + 32 + blockHeight,
                         screenWidth / 3 - 18, blockHeight, neonGreen);
      DrawText("Generator", (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 34 + blockHeight, 22, neonGreen);
      char genItemsText[20];
      snprintf(genItemsText, sizeof(genItemsText), "Bought: %dx",
               game.genItems);
      DrawText(genItemsText,
               (screenWidth / 3) * 2 +
                   ((screenWidth / 3) - MeasureText(genItemsText, 20) - 18),
               screenHeight * 0.05 + 56 + blockHeight, 20, neonGreen);
      char genPriceText[30];
      snprintf(genPriceText, sizeof(genPriceText), "Price: %0.2f",
               game.genPrice);
      DrawText(genPriceText, (screenWidth / 3) * 2 + 10,
               screenHeight * 0.05 + 56 + blockHeight, 20, neonGreen);
      if ((isClicked || IsKeyPressed(KEY_TWO)) &&
          game.money >= game.genPrice - 0.1) {
        game.money -= game.genPrice;
        game.moneyPerSecond += 0.1;
        game.genItems += 1;
        game.genPrice *= 1.6;
      }
#pragma endregion

    } else if (currentScreen == SCREEN_SHORTCUTS) {
#pragma region SHORTCUTS
      char titleSh[] = "Shortcuts:";
      int textWidth = MeasureText(titleSh, 24);
      int textX = screenWidth / 2 - textWidth / 2;

      // FIRST COLUMN
      DrawText("Shortcuts:", textX, screenHeight * 0.04, 24, neonGreen);
      DrawText("G: Acts like a click", screenWidth / 6, screenHeight / 6, 24,
               neonGreen);
      DrawText("1: Buys First Upgrade", screenWidth / 6, screenHeight / 6 + 30,
               24, neonGreen);
      DrawText("2: Buys Second Upgrade", screenWidth / 6, screenHeight / 6 + 60,
               24, neonGreen);

      // SECOND COLUMN
      DrawText("ESC: Opens Main Menu",
               (screenWidth / 6) * 4 -
                   MeasureText("ESC: Opens Main Menu", 24) / 2,
               screenHeight / 6, 24, neonGreen);
#pragma endregion

#pragma region Battery Cap Upg

#pragma endregion

#pragma region MAINMENU
    } else if (currentScreen == SCREEN_MAINMENU) {
      int blockHeight = 65;
      const int spaceBetween = 6;
      bool isButtonHovering = false;
      bool isClicked = false;

      // Main Menu Base Render
      ClearBackground(BLACK);
      DrawRectangleLines(screenWidth / 3, screenHeight / 5, screenWidth / 3,
                         (screenHeight / 5) * 3, neonGreen);
      DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 32) / 2,
               screenHeight / 6 - 6, 32, neonGreen);

      // Back Button
      Rectangle backButton = {(screenWidth / 3) + 6, screenHeight / 4,
                              screenWidth / 3 - 12, blockHeight};

      isButtonHovering = CheckCollisionPointRec(mousePoint, backButton);
      isClicked = isButtonHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(backButton, isButtonHovering ? GRAY : BLACK);
      DrawText("BACK", (screenWidth / 2) - MeasureText("BACK", 26) / 2,
               screenHeight / 4 + blockHeight / 2 - 13, 26, neonGreen);
      if (isClicked) {
        currentScreen = SCREEN_MAIN;
        isMainMenuOpened = false;
      }

      // New Game Button
      Rectangle newGameButton = {
          (screenWidth / 3) + 6, screenHeight / 4 + blockHeight + spaceBetween,
          screenWidth / 3 - 2 * spaceBetween, blockHeight};
      isButtonHovering = CheckCollisionPointRec(mousePoint, newGameButton);
      isClicked = isButtonHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(newGameButton, isButtonHovering ? GRAY : BLACK);
      DrawText("New Game", (screenWidth / 2) - MeasureText("New Game", 26) / 2,
               screenHeight / 4 + 2 * blockHeight - blockHeight / 2 - 9, 26,
               neonGreen);
      if (isClicked) {
        game = (GameData){0};
        currentScreen = SCREEN_MAIN;
        isMainMenuOpened = false;
      }

      // Save Game Button
      Rectangle saveGameButton = {
          (screenWidth / 3) + 6,
          screenHeight / 4 + (2 * blockHeight) + (spaceBetween * 2),
          screenWidth / 3 - 2 * spaceBetween, blockHeight};
      isButtonHovering = CheckCollisionPointRec(mousePoint, saveGameButton);
      isClicked = isButtonHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(saveGameButton, isButtonHovering ? GRAY : BLACK);
      DrawText(
          "Save Game", (screenWidth / 2) - MeasureText("Save Game", 26) / 2,
          screenHeight / 4 + 3 * blockHeight - blockHeight / 2, 26, neonGreen);
      if (isClicked) {
        saveGame(&game);
        justSaved = true;
      }
      if (justSaved) {
        timer += GetFrameTime();

        if (timer < 1.5f) {
          DrawText("Game Saved!",
                   screenWidth / 2 - MeasureText("Game Saved!", 26) / 2,
                   (screenHeight / 10) * 9, 26, neonGreen);
        } else {
          justSaved = false;
          timer = 0.0f;
        }
      }

      // Load Game Button
      Rectangle loadGameButton = {
          (screenWidth / 3) + 6,
          screenHeight / 4 + (3 * blockHeight) + (spaceBetween * 3),
          screenWidth / 3 - 2 * spaceBetween, blockHeight};
      isButtonHovering = CheckCollisionPointRec(mousePoint, loadGameButton);
      isClicked = isButtonHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(loadGameButton, isButtonHovering ? GRAY : BLACK);
      DrawText("Load Game",
               (screenWidth / 2) - MeasureText("Load Game", 26) / 2,
               screenHeight / 4 + 4 * blockHeight - blockHeight / 2 + 6, 26,
               neonGreen);
      if (isClicked) {
        game = loadGame();
        justLoaded = true;
      }

      if (justLoaded) {
        timer += GetFrameTime();

        if (timer < 1.5f) {
          DrawText("Game Loaded!",
                   screenWidth / 2 - MeasureText("Game Loaded!", 26) / 2,
                   (screenHeight / 10) * 9, 26, neonGreen);
        } else {
          justLoaded = false;
          timer = 0.0f;
        }
      }

      // Exit Game Button
      Rectangle exitButton = {(screenWidth / 3) + 6,
                              screenHeight / 4 + (4 * blockHeight) +
                                  (spaceBetween * 4),
                              screenWidth / 3 - 2 * spaceBetween, blockHeight};
      isButtonHovering = CheckCollisionPointRec(mousePoint, exitButton);
      isClicked = isButtonHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(exitButton, isButtonHovering ? GRAY : BLACK);
      DrawText("Exit", (screenWidth / 2) - MeasureText("Exit", 26) / 2,
               screenHeight / 4 + 5 * blockHeight - blockHeight / 2 + 12, 26,
               neonGreen);
      if (isClicked) {
        saveGame(&game);
        EndDrawing();
        CloseWindow();
        return 0;
      }

#pragma endregion
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

#pragma region Save Game Func
void saveGame(GameData *data) {
  FILE *file = fopen("savefile.txt", "wb");
  if (file != KEY_NULL) {
    fwrite(data, sizeof(GameData), 1, file);
    fclose(file);
    TraceLog(LOG_INFO, "Game Saved!");
  } else {
    TraceLog(LOG_ERROR, "Could not save game!");
  }
  return;
}
#pragma endregion

#pragma region Load Game Func
GameData loadGame() {
  GameData data = {0};
  data.moneyPerClick = 0.1;
  data.moneyPerSecond = 0.0;
  data.genPrice = 1.0;
  data.clickPrice = 1.0;
  data.batteryCapacity = 100.0;
  data.batteryPercent = data.batteryCapacity;
  data.batteryDrainPerSecond = 1.0;
  FILE *file = fopen("savefile.txt", "rb");
  if (file != NULL) {
    fread(&data, sizeof(GameData), 1, file);
    fclose(file);
    TraceLog(LOG_INFO, "Game Loaded!");
  } else {
    TraceLog(LOG_ERROR, "No save file loaded, using defaults.");
  }
  return data;
}
#pragma endregion