#include "string.h"
#include "raylib.h"

int main(void)
{
    int ScreenWidth;
    int ScreenHeight;
    const char* HelpText;
    int DroppedFilesCount;
    bool ProcessImage;
    bool DrawImage;
    bool ShowHelpText;
    bool ShowError;
    float CaptionTextPosX;
    float CaptionTextPosY;
    int Key;
    int FontSize;
    int CaptionIndex;
    char Caption[1024];
    char FileName[1024];
    Color CaptionColor;
    Image TheImage;
    Image TheImageCaptioned;
    Texture2D HelpTextImage;
    Texture2D TheTexture;
    
    goto Initialize;
    
    Initialize:
        ScreenWidth = 1000;
        ScreenHeight = 400;
        HelpText = "Drop an image into the window\nType in caption text\nMove caption text with arrow keys and change size with the mouse wheel\nSelect text color with function keys\nPress Enter to save to file\nHave fun!";
        DroppedFilesCount = 0;
        DrawImage = false;
        ShowHelpText = true;
        ShowError = false;
        CaptionTextPosX = 0;
        CaptionTextPosY = 0;
        Key = 0;
        FontSize = 25;
        CaptionIndex = 0;
        for (Key = 0; Key < 1024; Key++) {Caption[Key] = 0;}
        for (Key = 0; Key < 1024; Key++) {FileName[Key] = 0;}
        strcpy(FileName,GetWorkingDirectory());
        CaptionColor = WHITE;
        TheImage = (Image){0};
        TheImageCaptioned = (Image){0};
        TheTexture = (Texture2D){0};
        InitWindow(ScreenWidth,ScreenHeight,"Caption - made with Raylib");
        HelpTextImage = LoadTextureFromImage(ImageTextEx(GetFontDefault(),HelpText,180,2,WHITE));
        goto Main;
    
    Reset:
        ClearDroppedFiles();
        memset(Caption,0,1024);
        UnloadTexture(TheTexture);
        UnloadImage(TheImage);
        UnloadImage(TheImageCaptioned);
        CaptionTextPosX = 0;
        CaptionTextPosY = 0;
        CaptionColor = WHITE;
        Key = 0;
        FontSize = 25;
        CaptionIndex = 0;
        DrawImage = false;
        ShowHelpText = false;
        DroppedFilesCount = 0;
        ScreenWidth = 800;
        ScreenHeight = 400;
        SetWindowSize(ScreenWidth,ScreenHeight);
        ShowError = true;
        goto Main;
    
    LoadImage:
        ShowHelpText = false;
        ShowError = false;
        TheImage = LoadImage(GetDroppedFiles(&DroppedFilesCount)[0]);
        if ((int)TheImage.data == 0) goto Reset;
        TheTexture = LoadTextureFromImage(TheImage);
        CaptionTextPosX = 0;
        CaptionTextPosY = 0;
        memset(Caption,0,1024);
        CaptionIndex = 0;
        ScreenWidth = TheImage.width;
        ScreenHeight = TheImage.height;
        SetWindowSize(ScreenWidth,ScreenHeight);
        DrawImage = true;
        UnloadImage(TheImage);
        ClearDroppedFiles();
        goto Main;
     
    Main:
        if (WindowShouldClose()) goto Terminate;
        if (IsFileDropped()) goto LoadImage;
        
        ProcessInput:
            FontSize += GetMouseWheelMove();
            Key = GetKeyPressed();
            if (IsKeyPressed(KEY_ENTER))
            {
                strcpy(FileName,Caption);
                if (!IsFileExtension(FileName,".png"))   strcat(FileName,".png");
                TheImageCaptioned = GetTextureData(TheTexture);
                ImageDrawText(&TheImageCaptioned,(Vector2){CaptionTextPosX,CaptionTextPosY},Caption,FontSize,CaptionColor);
                ExportImage(TheImageCaptioned,FileName);
            }
            if (IsKeyPressed(KEY_F1))
                CaptionColor = WHITE;
            if (IsKeyPressed(KEY_F2))
                CaptionColor = BLACK;
            if (IsKeyPressed(KEY_F3))
                CaptionColor = MAGENTA;
            if (IsKeyPressed(KEY_F4))
                CaptionColor = GOLD;
            if (IsKeyPressed(KEY_F5))
                CaptionColor = ORANGE;
            if (IsKeyPressed(KEY_F6))
                CaptionColor = SKYBLUE;
            if (IsKeyPressed(KEY_F7))
                CaptionColor = GREEN;
            if (IsKeyPressed(KEY_F8))
                CaptionColor = VIOLET;
            if (IsKeyPressed(KEY_F9))
                CaptionColor = RED;
            if (IsKeyDown(KEY_UP))
                CaptionTextPosY -= 0.1f;
            if (IsKeyDown(KEY_DOWN))
                CaptionTextPosY += 0.1f;
            if (IsKeyDown(KEY_LEFT))
                CaptionTextPosX -= 0.1f;
            if (IsKeyDown(KEY_RIGHT))
                CaptionTextPosX += 0.1f;
            if (IsKeyReleased(KEY_BACKSPACE))
            {
                if (CaptionIndex == 0) goto Draw;
                Caption[--CaptionIndex] = '\0';     
            }
            if (Key >= 32 && Key <= 126 || Key == 260 )
            {
                if (CaptionIndex > 1023) goto Draw;
                Caption[CaptionIndex++] = (char)Key;
                Caption[CaptionIndex] = '\0';
            }
        
        Draw:
            BeginDrawing();
                ClearBackground(BLACK);
                if (DrawImage)
                {
                    DrawTexture(TheTexture,0,0,WHITE);
                    DrawTextEx(GetFontDefault(),Caption,(Vector2){CaptionTextPosX,CaptionTextPosY},FontSize,5,CaptionColor);
                }
                if (ShowHelpText)   DrawTexture(HelpTextImage,0,0,WHITE);
                if (ShowError) DrawText("ERROR: Image could not be loaded, load a different file type.",10,ScreenHeight/2,25,WHITE);
            EndDrawing();
        
        goto Main;
    
    Terminate:
        UnloadTexture(HelpTextImage);
        UnloadTexture(TheTexture);
        UnloadImage(TheImage);
        UnloadImage(TheImageCaptioned);
        CloseWindow();
        return 0;
}