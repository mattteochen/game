#ifndef CONFIG_H
#define CONFIG_H

#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <utility>
#include <memory>
#include <map>
#include <queue>
#include <iostream>
#include <time.h>
#include <math.h>
#include "../SFML-2.5.1-macos-clang/include/SFML/Graphics.hpp"
#include "../SFML-2.5.1-macos-clang/include/SFML/Audio.hpp"
#include "../SFML-2.5.1-macos-clang/include/SFML/Window.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

#define PRINT           std::cout

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

/* alpha scaling */
#define ALPHA_SCALING   100


#endif
