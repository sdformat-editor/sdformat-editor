#include "model_viewer.h"

int main(int argc, char** argv)
{

    ModelViewerI* model_viewer = new ModelViewer();

    model_viewer->Initialize();

    return 0;
}