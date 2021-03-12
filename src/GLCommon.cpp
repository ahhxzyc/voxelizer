#include "GLCommon.h"

#include <iostream>
using namespace std;

void checkError() {
    GLenum err = glGetError();
    if (err)
        cout << "GL ERROR " << err << ".\n";
}