#include "ScreenRecording.h"
#include <iostream>
void ScreenRecording::screenDump(std::string filename)
{
    int width, height;
    getWidthHeight(width, height);
    const int numberOfPixels = width * height * 3;
    unsigned char pixels[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    FILE *outputFile = fopen(filename.c_str(), "w");
    short header[] = {0, 2, 0, 0, 0, 0, (short) width, (short) height, 24};
    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(pixels, numberOfPixels, 1, outputFile);
    fclose(outputFile);
}

void ScreenRecording::saveAndCreateVideo()
{ //TODO: find method to not have to delete frames
    int width, height;
    getWidthHeight(width, height); //TODO: changed for katie talk
    width = roundToMultiple(width, 4);
    height = roundToMultiple(height, 4);

    system("rm movie/0.tga; rm movie/1.tga");

    std::string cmdStr = std::string("cd movie; find . -type f -name '*.tga' | sort -V > list; mencoder mf://@list -mf w=" + std::to_string(width) + ":h=" + std::to_string(height) + ":type=tga -ovc lavc \
                           -lavcopts vcodec=mjpeg:mbd=2:trell -oac copy  -o springAgentRecording" + std::to_string(movieRunNumber) + ".avi");
    system(cmdStr.c_str());
    system("rm movie/list; sh removeUnnecessaryScreenshots.sh");
    movieRunNumber++;
}

void ScreenRecording::getWidthHeight(int &width, int &height)
{
    int x, y, w, h;
    GLUI_Master.get_viewport_area(&x, &y, &w, &h);
    width = w;
    height = h;
}

int ScreenRecording::roundToMultiple(int toRound, int multiple)
{
    const auto ratio = static_cast<double>(toRound) / multiple;
    const auto iratio = lround(ratio);
    return iratio * multiple;
}
