/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdint.h>
#include <windows.h>
#include <xinput.h>
#include <dsound.h>

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

typedef int32 bool32;

// NOTE:
// instead of linking the xinput lib when building the application
// we define it within our application
// we do this because if when running our application and the system is not
// using an the hardware (here, the xbox controller) or does not have the
// lib (here, xinput) we want to prevent the application from crashing

// NOTE:
// xinput.h::XInputGetstate
// we create a macro for the library function, by using its signature/prototype
// a param 'name' is used so that we can make as many different functions using
// that signature/prototype
#define X_INPUT_GET_STATE(name)                                                \
  DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
// typedef to the function type to be used, creates a function pointer
// with the signature/prototype of the macro defined
typedef X_INPUT_GET_STATE(x_input_get_state);
// creates the function stub for when the functions do not exist
// or the lib could not be linked to, so that the application
// does not crash
X_INPUT_GET_STATE(XInputGetStateStub) { return (ERROR_DEVICE_NOT_CONNECTED); }
// defines the function type
global_variable x_input_get_state *_XInputGetState = XInputGetStateStub;
// creates a macro so that we can call the function using the same name that
// is used within the library
#define XInputGetState _XInputGetState

// NOTE:
// xinput.h::XInputSetstate
#define X_INPUT_SET_STATE(name)                                                \
  DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub) { return (ERROR_DEVICE_NOT_CONNECTED); }
global_variable x_input_set_state *_XInputSetState = XInputSetStateStub;
#define XInputSetState _XInputSetState

// NOTE:
// dsound.h::DirectSoundCreate
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

// NOTE:
// Pixels are alwats 32-bits wide
// Memory Order BB GG RR xx
struct OffscreenBuffer {
  BITMAPINFO Info;
  void *Memory;
  int Width;
  int Height;
  int Pitch;
};

struct WindowDimension {
  int Width;
  int Height;
};

global_variable bool Running;
global_variable OffscreenBuffer GlobalBackbuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;

internal WindowDimension Win32GetWindowDimension(HWND Window) {
  WindowDimension Result;

  RECT ClientRect;
  GetClientRect(Window, &ClientRect);
  Result.Width = ClientRect.right - ClientRect.left;
  Result.Height = ClientRect.bottom - ClientRect.top;

  return (Result);
}

internal void Win32LoadXInput(void) {
  HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");

  if (!XInputLibrary) {
    // TODO:
    // logging diagnostic
    XInputLibrary = LoadLibraryA("xinput1_3.dll");
  }

  if (XInputLibrary) {
    XInputGetState =
        (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
    if (!XInputGetState) {
      XInputGetState = XInputGetStateStub;
    }

    XInputSetState =
        (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
    if (!XInputSetState) {
      XInputSetState = XInputSetStateStub;
    }
  } else {
    // TODO:
    // logging diagnostic
  }
}

internal void Win32InitSound(HWND Window, int32_t SamplesPerSecond,
                             int32_t BufferSize) {
  // NOTE:
  // load sound
  HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");

  if (DSoundLibrary) {
    // NOTE:
    // get a direct sound object
    direct_sound_create *DirectSoundCreate =
        (direct_sound_create *)GetProcAddress(DSoundLibrary,
                                              "DirectSoundCreate");

    // TODO:
    // double check this works on XP - DirectSound8 or 7
    LPDIRECTSOUND DirectSound;
    if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0))) {
      WAVEFORMATEX WaveFormat = {};
      WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
      WaveFormat.nChannels = 2;
      WaveFormat.nSamplesPerSec = SamplesPerSecond;
      WaveFormat.wBitsPerSample = 16;
      WaveFormat.nBlockAlign =
          (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
      WaveFormat.nAvgBytesPerSec =
          WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
      WaveFormat.cbSize = 0;

      if (SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY))) {
        // NOTE:
        // create a primary buffer
        DSBUFFERDESC BufferDescription = {};
        BufferDescription.dwSize = sizeof(BufferDescription);
        BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

        LPDIRECTSOUNDBUFFER PrimaryBuffer;
        if (SUCCEEDED(
                DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0))) {

          if (SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat))) {
              OutputDebugStringA("primary buffer is set");
          } else {
            // TODO:
            // logging diagnostic
          }
        } else {
          // TODO:
          // logging diagnostic
        }
      } else {
        // TODO:
        // logging diagnostic
      }

      // NOTE:
      // create a secondary buffer

      DSBUFFERDESC BufferDescription = {};
      BufferDescription.dwSize = sizeof(BufferDescription);
      BufferDescription.dwFlags = 0;
      BufferDescription.dwBufferBytes = BufferSize;
      BufferDescription.lpwfxFormat = &WaveFormat;

      if (SUCCEEDED(
              DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0))) {
              OutputDebugStringA("secondary buffer is set");
      } else {
        // TODO:
        // logging diagnostic
      }
    } else {
      // TODO:
      // logging diagnostic
    }
  }
}

internal void RenderGradient(OffscreenBuffer *Buffer, int BlueOffset,
                             int GreenOffset) {
  uint8 *Row = (uint8 *)Buffer->Memory;
  for (int y = 0; y < Buffer->Height; ++y) {
    uint32 *Pixel = (uint32 *)Row;
    for (int x = 0; x < Buffer->Width; ++x) {
      uint8 Blue = (x + BlueOffset);
      uint8 Green = (y + GreenOffset);

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

  int BytesPerPixel = 4;

  // NOTE:
  // when the biHeight field is negative, this is the clue to
  // Windows to treat this bitmap as top-down, not bottom-up;
  // meaning that the first three bytes of the image are the
  // color for the top left pixel in the bitmap, not the bottom left
  Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
  Buffer->Info.bmiHeader.biWidth = Buffer->Width;
  Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
  Buffer->Info.bmiHeader.biPlanes = 1;
  Buffer->Info.bmiHeader.biBitCount = 32;
  Buffer->Info.bmiHeader.biCompression = BI_RGB;

  // NOTE:
  // 4 bytes => 4 * 8 bits
  // extra 8 bits; padding for alignment
  int BitmapMemorySize = (Buffer->Width * Buffer->Height) * BytesPerPixel;
  Buffer->Memory =
      VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  Buffer->Pitch = Buffer->Width * BytesPerPixel;
}

internal void Win32DisplayBufferIntoWindow(OffscreenBuffer *Buffer,
                                           HDC DeviceContext, int WindowWidth,
                                           int WindowHeight) {
  StretchDIBits(DeviceContext, 0, 0, WindowWidth, WindowHeight, 0, 0,
                Buffer->Width, Buffer->Height, Buffer->Memory, &Buffer->Info,
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

  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_KEYDOWN:
  case WM_KEYUP: {
      uint32_t VKeyCode = WParam; 
      bool WasDown = ((LParam & (1 << 30)) != 0);
      bool IsDown = ((LParam & (1 << 31)) == 0);

      // NOTE:
      // omit key repeats, pressing down on a key
      if (WasDown != IsDown) {
        if (VKeyCode == 'W') {
        } else if (VKeyCode == 'A') {
        } else if (VKeyCode == 'S') {
        } else if (VKeyCode == 'D') {
        } else if (VKeyCode == 'Q') {
        } else if (VKeyCode == 'E') {
        } else if (VKeyCode == VK_UP) {
        } else if (VKeyCode == VK_DOWN) {
        } else if (VKeyCode == VK_LEFT) {
        } else if (VKeyCode == VK_RIGHT) {
        } else if (VKeyCode == VK_SPACE) {
        } else if (VKeyCode == VK_ESCAPE) {
          Running = false;
        }
      }

      bool32 AltKeyDown = (LParam & (1 << 29));
      if(VKeyCode == VK_F4 && AltKeyDown) {
          Running = false;
      }
  } break;

  case WM_PAINT: {
    PAINTSTRUCT Paint;
    HDC DeviceContext = BeginPaint(Window, &Paint);
    WindowDimension Dimension = Win32GetWindowDimension(Window);
    Win32DisplayBufferIntoWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
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

  Win32LoadXInput();

  // Register the window class.
  WNDCLASSA wc = {};

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
      HDC DeviceContext = GetDC(Window);

      // Run the message loop.
      Running = true;

      // Test Rendering
      int XOffset = 0;
      int YOffset = 0;

      // Test DSound
      int SamplesPerSecond = 48000;
      int ToneHz = 256;
      int16_t ToneVolume = 3000;
      uint32_t RunningSampleIndex = 0;
      int SquareWavePeriod = SamplesPerSecond / ToneHz;
      int HalfSquareWavePeriod = SquareWavePeriod / 2;
      int BytesPerSample = sizeof(int16_t) * 2;
      int SecondaryBufferSize = SamplesPerSecond * BytesPerSample;

      Win32InitSound(Window, SamplesPerSecond, SecondaryBufferSize);
      bool32 SoundIsPlaying = false;

      while (Running) {
        MSG Message = {};

        while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
          if (Message.message == WM_QUIT) {
            Running = false;
          }

          TranslateMessage(&Message);
          DispatchMessage(&Message);
        }

        for (DWORD ControllerIndex = 0; ControllerIndex < XUSER_MAX_COUNT;
             ++ControllerIndex) {
          XINPUT_STATE ControllerState;
          if (XInputGetState(ControllerIndex, &ControllerState) ==
              ERROR_SUCCESS) {
            // NOTE:
            // the controller is plugged in
            XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

            bool Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
            bool Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
            bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
            bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
            bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);
            bool LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool RightShoulder =
                (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);

            int16_t LeftStickX = Pad->sThumbLX;
            int16_t LeftStickY = Pad->sThumbLY;

            // changing the offset to animate the GlobalBackbuffer
            XOffset += LeftStickX >> 12;
            YOffset += LeftStickY >> 12;
          } else {
            // NOTE:
            // the controller is not plugged in
          }
        }

        // XINPUT_VIBRATION Vibration;
        // Vibration.wLeftMotorSpeed = 60000;
        // Vibration.wRightMotorSpeed = 60000;
        // XInputSetState(0, &Vibration);

        // Test Graphics: Load color data into buffer
        RenderGradient(&GlobalBackbuffer, XOffset, YOffset);

        // NOTE:
        // Test DirectSound
        DWORD PlayCursor;
        DWORD WriteCursor;

        if (SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(
                &PlayCursor, &WriteCursor))) {
          DWORD ByteToLock =
              RunningSampleIndex * BytesPerSample % SecondaryBufferSize;
          DWORD BytesToWrite;
          if (ByteToLock == PlayCursor) {
            BytesToWrite = SecondaryBufferSize;
          } else if (ByteToLock > PlayCursor) {
            BytesToWrite = (SecondaryBufferSize - ByteToLock);
            BytesToWrite += PlayCursor;
          } else {
            BytesToWrite = PlayCursor - ByteToLock;
          }

          VOID *Region1;
          DWORD Region1Size;
          VOID *Region2;
          DWORD Region2Size;

          if (SUCCEEDED(GlobalSecondaryBuffer->Lock(
                  ByteToLock, BytesToWrite, &Region1, &Region1Size, &Region2,
                  &Region2Size, 0))) {

            DWORD Region1SampleCount = Region1Size / BytesPerSample;
            int16 *SampleOut = (int16 *)Region1;
            for (DWORD SampleIndex = 0; SampleIndex < Region1SampleCount;
                 ++SampleIndex) {
              int16 SampleValue =
                  ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2)
                      ? ToneVolume
                      : -ToneVolume;
              *SampleOut++ = SampleValue;
              *SampleOut++ = SampleValue;
            }

            DWORD Region2SampleCount = Region2Size / BytesPerSample;
            SampleOut = (int16 *)Region2;
            for (DWORD SampleIndex = 0; SampleIndex < Region2SampleCount;
                 ++SampleIndex) {
              int16 SampleValue =
                  ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2)
                      ? ToneVolume
                      : -ToneVolume;
              *SampleOut++ = SampleValue;
              *SampleOut++ = SampleValue;
            }
            GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2,
                                          Region2Size);
          }
        }

        if (!SoundIsPlaying) {
          GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
          SoundIsPlaying = true;
        }

        WindowDimension Dimension = Win32GetWindowDimension(Window);
        Win32DisplayBufferIntoWindow(&GlobalBackbuffer, DeviceContext,
                                     Dimension.Width, Dimension.Height);
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
