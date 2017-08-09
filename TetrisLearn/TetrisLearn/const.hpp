#pragma once

// CONSTANT PARAMETERS
// CANNOT BE CHANGED

//Confg file PATH
#define SETTING_PATH "settings.xml"

// Board Param.
#define BOARD_HEIGHT         25
#define BOARD_VISIBLE_HEIGHT 20
#define BOARD_WIDTH          10

//UI.
#define PANEL_HEIGHT 600.0f
#define PANEL_WIDTH  500.0f

#define PANEL_BIG_BOX_HEIGHT 600.0f
#define PANEL_BIG_BOX_WIDTH  500.0f

#define PANEL_SMALL_BOX_HEIGHT 600.0f
#define PANEL_SMALL_BOX_WIDTH  200.0f

#define PANEL_BOX_BORDER 10.0f
#define PANEL_BOX_COLOR  sf::Color::Magenta

#define BLOCK_SIZE 29.0f


#define SCORE_COUNTER_LENGTH   12
#define LEVEL_COUNTER_LENGTH   3


// UI Texts.
#define SCORE_TEXT_POS_X     20
#define SCORE_TEXT_POS_Y     20
#define SCORE_COUNTER_POS_X  20
#define SCORE_COUNTER_POS_Y  50

#define LEVEL_TEXT_POS_X     20
#define LEVEL_TEXT_POS_Y     95
#define LEVEL_COUNTER_POS_X  20
#define LEVEL_COUNTER_POS_Y  125

#define GAMEOVER_TEXT_POS_X  20
#define GAMEOVER_TEXT_POS_Y  175


// Log Texts.
//#define CLOSING_LOG_STR "Logger shutting off."

// Tetramino Positions
#define SPAWN_POINT sf::Vector2i(5, 20)

// Tetramino general
#define TETRAMINO_SIZE  4
#define TETRAMINO_TYPES 7

// SRS
// The number of different offset to try for a wall-kick
#define SRS_OFFSET_VARIANTS 5
#define TETRAMINO_PHASES    4

// Move
#define MOVE_DIRECTIONS 4

// Tetramino Bags.
#define BAG_SIZE 7

// Inputs
#define TOTAL_KEYS 7

// Thumbnail
#define THUMBNAIL_BORDER       5.0f
#define THUMBNAIL_GRID_HEIGHT  4
#define THUMBNAIL_GRID_WIDTH   5

#define HOLD_ANCHOR_X 105.0f
#define HOLD_ANCHOR_Y 240.0f

#define THUMBNAIL_PREVIEW_NUMBER 3
#define PREVIEW_ANCHOR_X         20.0f
#define PREVIEW_ANCHOR_Y         240.0f
#define PREVIEW_OFFSET           80.0f


// InputData
#define FLUSH_TIMER 100


//Python
#define PYTHON_MODULE       "Wrapper"
#define PYTHON_FUNC         "getWrapper"
#define PYTHON_FIT_METHOD   "fit"
#define PYTHON_PRED_METHOD  "predictFromStr"



/////////////////////////////////////////////////////////////////////////////////////////////