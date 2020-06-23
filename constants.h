/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

const float PI = 3.141592653589793f;
const float MAX_DISTANCE = 10.0f;
const float CAMERA_KEY_SPEED = 1.0f;
const float CAMERA_MOUSE_SPEED = 0.75f;
const float BOARD_CORNER = 0.287;
const float FIELD_SIZE = BOARD_CORNER * 2.0 / 7.0 + 0.001;
const float PIECE_SPEED = 1000.0f;
const float CAPTURED_WHITE = -0.45f;
const float CAPTURED_BLACK = 0.45f;
const float HEIGHT = 0.15;
const float LIGHT_SPEED = 1000.0f;
const bool WHITE = true;
const bool BLACK = false;
const std::string INPUT_FILE= "game.txt";
const float MAX_CAMERA_DISTANCE = 1.5f;

#endif
