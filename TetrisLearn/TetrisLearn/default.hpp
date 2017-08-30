#pragma once

// DEFAULT VALUES
// OVERRIDE POSSIBLE


// Window
// Window Config. 
#define DEFAULT_WIN_WIDTH  500
#define DEFAULT_WIN_HEIGHT 600
#define DEFAULT_WIN_NAME   "Tetris Learn"

// Boards Grid
// Number and Layout of Boards
#define DEFAULT_NB_BOARDS_HOR 1
#define DEFAULT_NB_BOARD_VER  1

// Text
#define DEFAULT_TEXT_COLOR     sf::Color::White
#define DEFAULT_TEXT_POS       sf::Vector2f(0,0)
#define DEFAULT_TEXT_STRING    ""
#define DEFAULT_TEXT_SIZE      10

//Counters / GameOver
#define DEFAULT_SCORE_TEXT     "Score: "
#define DEFAULT_LEVEL_TEXT     "Level: "
#define DEFAULT_SCORE_COUNTER  "999999999999"
#define DEFAULT_LEVEL_COUNTER  "999"

#define DEFAULT_GAMEOVER_TEXT  "GAME OVER !"
#define DEFAULT_GAMEOVER_COLOR sf::Color::Red

//Thumbnails
#define DEFAULT_THUMBNAIL_COLOR_HOLD     sf::Color::Red
#define DEFAULT_THUMBNAIL_COLOR_PREVIEW  sf::Color::Blue
#define DEFAULT_THUMBNAIL_FONT_SIZE      15
#define DEFAULT_HOLD_TEXT                "Hold"
#define DEFAULT_PREVIEW_TEXT             "Previews"

// Font.
#define DEFAULT_FONT_SIZE      25
#define DEFAULT_FONT_FILE      "arial.ttf"

//Rules.
#define DEFAULT_LOCK_TIME_BASE        1000
#define DEFAULT_AUTOSCROLL_TIME_BASE  500
#define DEFAULT_FLASH_TIME            1000
#define DEFAULT_DAS_TIMER             100
#define DEFAULT_STEP_TO_ENABLE_DAS    3

// Visual effects.
#define DEFAULT_FLASH_ANIM_DELAY         100
#define DEFAULT_SPRITE_COLOR_ACTIVE      sf::Color(255, 255, 255, 255)
#define DEFAULT_SPRITE_COLOR_LOCKED      sf::Color(150, 150, 150, 255)
#define DEFAULT_SPRITE_COLOR_FLASHING_1  sf::Color(255, 0, 0, 255)
#define DEFAULT_SPRITE_COLOR_FLASHING_2  sf::Color(0, 255, 0, 255)
#define DEFAULT_SPRITE_COLOR_GHOST       sf::Color(255, 255, 255, 50)

//Block Management
#define DEFAULT_BLOCKPOOL_SIZE_BASE     100
#define DEFAULT_BLOCK_TEXTURE           "BlockTexture.png"


// Scoring / Level
#define DEFAULT_LINE_SCORE             1.0f
#define DEFAULT_LINE_MULTIPLIER        2.0f
#define DEFAULT_LEVEL_SCORE_MULTIPLIER 1.0f
#define DEFAULT_LEVEL_SPEED_MULTIPLIER 0.1f
#define DEFAULT_LINES_TO_LEVELUP       20.0f


// InputData saving
#define DEFAULT_VAL_SEPARATOR ','
#define DEFAULT_SAVE_PATH     "./inputData_save.raw"

//Board specific
#define DEFAULT_BOARD_NAME     "Default Board"
#define DEFAULT_BOARD_CONTROL  InputMethod::Player
#define DEFAULT_RECORDING      true

#define DEFAULT_KEY_UP         sf::Keyboard::Key::Z
#define DEFAULT_KEY_LEFT       sf::Keyboard::Key::Q
#define DEFAULT_KEY_RIGHT      sf::Keyboard::Key::D
#define DEFAULT_KEY_DOWN       sf::Keyboard::Key::S
#define DEFAULT_KEY_LFLIP      sf::Keyboard::Key::A
#define DEFAULT_KEY_RFLIP      sf::Keyboard::Key::E
#define DEFAULT_KEY_HOLD       sf::Keyboard::Key::F

#define DEFAULT_SCRIPT_PATH    "./PythonSrc/DummyClassifier.py"
