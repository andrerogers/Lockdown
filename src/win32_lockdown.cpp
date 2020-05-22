/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdint.h>
#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

struct OffscreenBuffer {
  BITMAPINFO Info;
  void *Memory;
  int Width;
  int Height;
  int Pitch;
  int BytesPerPixel;
};

struct WindowDimension {
  int Width;
  int Height;
};

WindowDimension Win32GetWindowDimension(HWND Window) {
  WindowDimension Result;

  RECT ClientRect;
  GetClientRect(Window, &ClientRect);
  Result.Width = ClientRect.right - ClientRect.left;
  Result.Height = ClientRect.bottom - ClientRect.top;

  return (Result);
}

global_variable bool Running;
global_variable OffscreenBuffer GlobalBackbuffer;

internal void RenderGradient(OffscreenBuffer *Buffer, int XOffset,
                             int YOffset) {
  uint8 *Row = (uint8 *)Buffer->Memory;
  for (int y = 0; y < Buffer->Height; ++y) {
    uint32 *Pixel = (uint32 *)Row;
    for (int x = 0; x < Buffer->Width; ++x) {
      uint8 Blue = (x + XOffset);
      uint8 Green = (y + YOffset);

      /*
        Memory:   BB GG RR xx
        Register: xx RR GG BB
       */

      *Pixel++ = ((Green << 8) | Blue);
    }
    Row += Buffer->Pitch;
  }
}

internal void Win32ResizeDIBSection(OffscreenBuffer *Buffer, int Width,
                                    int Height) {
  if (Buffer->Memory) {
    VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
  }

  Buffer->Width = Width;
  Buffer->Height = Height;
  Buffer->BytesPerPixel = 4;

  Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;

  Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
  Buffer->Info.bmiHeader.biWidth = Buffer->Width;
  Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
  Buffer->Info.bmiHeader.biPlanes = 1;
  Buffer->Info.bmiHeader.biBitCount = 32;
  Buffer->Info.bmiHeader.biCompression = BI_RGB;

  // NOTE
  // 4 bytes => 4 * 8 bits
  // extra 8 bits; padding for alignment
  int BitmapMemorySize =
      (Buffer->Width * Buffer->Height) * Buffer->BytesPerPixel;
  Buffer->Memory =
      VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

internal void Win32CopyBufferIntoWindow(HDC DeviceContext, int WindowWidth,
                                        int WindowHeight,
                                        OffscreenBuffer Buffer, int Width,
                                        int Height) {
  StretchDIBits(DeviceContext, 0, 0, WindowWidth, WindowHeight, 0, 0,
                Buffer.Width, Buffer.Height, Buffer.Memory, &Buffer.Info,
                DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Win32WindowProc(HWND Window, UINT Message, WPARAM WParam,
                                 LPARAM LParam) {
  LRESULT Result = 0;

  switch (Message) {
  case WM_SIZE: {
  } break;

  case WM_CLOSE: {
    Running = false;
  } break;

  case WM_DESTROY: {
    Running = false;
  } break;

  case WM_ACTIVATEAPP: {
    OutputDebugStringA("WM_ACTIVATEAPP\n");
  } break;

  case WM_PAINT: {
    PAINTSTRUCT Paint;
    HDC DeviceContext = BeginPaint(Window, &Paint);
    int X = Paint.rcPaint.left;
    int Y = Paint.rcPaint.top;
    int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
    int Width = Paint.rcPaint.right - Paint.rcPaint.left;

    WindowDimension Dimension = Win32GetWindowDimension(Window);
    Win32CopyBufferIntoWindow(DeviceContext, Dimension.Width, Dimension.Height,
                              GlobalBackbuffer, Width, Height);
    EndPaint(Window, &Paint);
  } break;

  default: {
    OutputDebugStringA("default\n");
    Result = DefWindowProc(Window, Message, WParam, LParam);
  } break;
  }

  return (Result);
}

int WINAPI wWinMain(HINSTANCE Instance, HINSTANCE PrevInstance,
                    PWSTR CommandLine, int ShowCode) {
  // Register the window class.
  WNDCLASS wc = {};

  Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = Win32WindowProc;
  wc.hInstance = Instance;
  wc.lpszClassName = "LockdownWindowClass";

  if (RegisterClass(&wc)) {
    // Create the window.
    HWND Window = CreateWindowEx(
        0,                                // Optional window styles.
        wc.lpszClassName,                 // Window class
        "Lockdown Engine",                // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style

        // Size and position
        // x, y, width, height
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        0,        // Parent window
        0,        // Menu
        Instance, // Instance handle
        0         // Additional application data
    );

    if (Window) {
      // Run the message loop.
      Running = true;

      int XOffset = 0;
      int YOffset = 0;

      while (Running) {
        MSG Message = {};

        while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
          if (Message.message == WM_QUIT) {
            Running = false;
          }

          TranslateMessage(&Message);
          DispatchMessage(&Message);
        }

        RenderGradient(&GlobalBackbuffer, XOffset, YOffset);

        HDC DeviceContext = GetDC(Window);

        WindowDimension Dimension = Win32GetWindowDimension(Window);
        Win32CopyBufferIntoWindow(DeviceContext, Dimension.Width,
                                  Dimension.Height, GlobalBackbuffer,
                                  Dimension.Width, Dimension.Height);
        ReleaseDC(Window, DeviceContext);
        ++XOffset;
        ++YOffset;
      }
    } else {
      // TODO: Logging
    }
  } else {
    // TODO: Logging
  }

  // ShowWindow(hwnd, ShowCode);

  return 0;
}
