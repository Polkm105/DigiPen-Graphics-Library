//-------------------------------------------------------------------------------------------------
// file:    main.c
// author:  
// brief:   
//
// Copyright © 2022 DigiPen, All rights reserved.
//-------------------------------------------------------------------------------------------------

#include "Resource.h"

#include "DGL.h"

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    int result;
    if (DGL_System_HandleWindowsMessage(message, wParam, lParam, &result))
        return result;

    switch (message)
    {
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

static DGL_Mesh* mesh;
static const DGL_VertexShader* vertex_shader;

static void TestingInit()
{
    typedef struct Vertex {
        DGL_Vec2 position;
        DGL_Color color;
        DGL_Vec2 texture_coordinates;
    } Vertex;

    Vertex vertices[] = {
        {{-0.5f,  0.5f}, {1, 0, 0, 1   }, {0, 0}},
        {{-0.5,  -0.5f}, {0, 1, 0, 0.5f}, {0, 1}},
        {{ 0.5f, -0.5f}, {0, 0, 1, 0.5f}, {1, 1}},
        {{ 0.5f,  0.5f}, {0, 1, 1, 0   }, {1, 0}}
    };

    unsigned indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    DGL_Graphics_StartMesh();

    for (int i = 0; i < 4; ++i)
    {
        DGL_Graphics_AddVertex(
            &vertices[i].position,
            &vertices[i].color,
            &vertices[i].texture_coordinates);
    }

    mesh = DGL_Graphics_EndMeshIndexed(indices, 6);
    vertex_shader = DGL_Graphics_LoadVertexShader("CustomVertexShader.hlsl");
    DGL_Graphics_SetBlendMode(DGL_BM_BLEND);

    DGL_Graphics_SetCustomVertexShader(vertex_shader);
}

static void TestingUpdate()
{
    DGL_Vec2 position = { -200, 0 };
    DGL_Vec2 scale = { 100, 100 };
    float rotation = 0;

    
    DGL_Graphics_SetVertexShaderMode(DGL_VSM_CUSTOM);
    DGL_Graphics_SetCB_TransformData(&position, &scale, rotation);
    DGL_Graphics_DrawMesh(mesh, DGL_DM_TRIANGLELIST);

    position.x += 400;
    DGL_Graphics_SetCB_TransformData(&position, &scale, rotation);
    DGL_Graphics_SetVertexShaderMode(DGL_VSM_DEFAULT);
    DGL_Graphics_DrawMesh(mesh, DGL_DM_TRIANGLELIST);
}

static void TestingShutdown()
{
    DGL_Graphics_FreeMesh(&mesh);
    DGL_Graphics_FreeVertexShader(&vertex_shader);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    lpCmdLine;
    hPrevInstance;
    DGL_SysInitInfo initInfo;
    initInfo.mAppInstance = hInstance;
    initInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
    initInfo.mMaxFrameRate = 60;
    initInfo.mShow = nCmdShow;
    initInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;
    initInfo.mWindowTitle = "Game Window";
    initInfo.mWindowHeight = 768;
    initInfo.mWindowWidth = 1024;
    initInfo.mCreateConsole = TRUE;
    initInfo.pWindowsCallback = WndProc;
    initInfo.mWindowIcon = IDI_DGLTEMPLATE;

    HWND windowHandle = DGL_System_Init(&initInfo);
    if (windowHandle == NULL)
        return 1;

    DGL_Color color = { 0.1f, 0.1f, 0.1f, 1.0f };
    DGL_Graphics_SetBackgroundColor(&color);

    TestingInit();

    int running = TRUE;
    while (running)
    {
        DGL_System_FrameControl();
        DGL_System_Update();

        DGL_Graphics_StartDrawing();

        TestingUpdate();

        DGL_Graphics_FinishDrawing();

        if (!DGL_System_DoesWindowExist())
            running = FALSE;
    }

    TestingShutdown();
    DGL_System_Exit();

    return 0;
}
