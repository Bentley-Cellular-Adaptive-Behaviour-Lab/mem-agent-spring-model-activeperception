#ifndef SCREENRECORDING_H
#define SCREENRECORDING_H

#include "string"
#include "math.h"

#ifdef __APPLE__

#include <GLUT/glut.h>         // The GL Utility Toolkit (GLUT) Header

#include <OpenGL/glu.h>

#include <OpenGL/gl.h>

#include <GL/glui.h>

#elif __linux__

#include <GL/glut.h>

#include <GL/glu.h>

#include <GL/gl.h>

#include <glui.h>

#endif
enum Recording
{
    NotRecording,
    Recording,
    SaveRecording
};

static int movieRunNumber = 0;
static int frameNumber = 0;
static enum Recording screenRecordState;

class ScreenRecording
{
public:
    static void screenDump(std::string filename);
    static void saveAndCreateVideo();
private:
    ScreenRecording();
    static void getWidthHeight(int &width, int &height);
    static int roundToMultiple(int toRound, int multiple);
};
#endif // SCREENRECORDING_H
