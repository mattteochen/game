#ifndef CONFIG_H
#define CONFIG_H

#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>
#include <iostream>
#include "../SFML-2.5.1-macos-clang/include/SFML/Graphics.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

/* window param */
#define WINDOW_HEIGH    480
#define WINDOW_LENGTH   640

/* font param */
#define FONT_DIR        "../../fonts/monaco.ttf"
#define FONT_SIZE       10

/* Circle param */
#define CIRCLE_SIZE     50
#define CIRCLE_POS      (float)100
#define CIRCLE_SPEED    (float)0.1

/* RGB std::vector len */
#define RGB_COLOUR_LEN  3

#endif
