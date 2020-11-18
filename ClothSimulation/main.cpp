#pragma once
#include "stdafx.h"
#include "ClothSimulationApplication.h"

#pragma comment(lib, "d3d11.lib")

// Main code
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    ClothSimulationApplication* application = new ClothSimulationApplication("Cloth Simulation COMP 477", 1680, 1050);
    if (!application->Initialize())
    {
        return -1;
        delete application;
    }

    application->Run();
    delete application;
    return 0;
}
