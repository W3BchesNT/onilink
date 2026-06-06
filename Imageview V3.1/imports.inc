section '.idata' import data readable

  library kernel32,'KERNEL32.DLL',\
          user32,'USER32.DLL',\
          gdi32,'GDI32.DLL',\
          dwmapi,'DWMAPI.DLL',\
          gdiplus,'GDIPLUS.DLL',\
          ole32,'OLE32.DLL'

  import dwmapi,\
         DwmSetWindowAttribute,'DwmSetWindowAttribute'

  import gdiplus,\
         GdiplusStartup,'GdiplusStartup',\
         GdiplusShutdown,'GdiplusShutdown',\
         GdipDisposeImage,'GdipDisposeImage',\
         GdipCreateFromHDC,'GdipCreateFromHDC',\
         GdipDeleteGraphics,'GdipDeleteGraphics',\
         GdipDrawImageRectI,'GdipDrawImageRectI',\
         GdipCreateBitmapFromStream,'GdipCreateBitmapFromStream',\
         GdipGetImageWidth,'GdipGetImageWidth',\
         GdipGetImageHeight,'GdipGetImageHeight',\
         GdipGetImageThumbnail,'GdipGetImageThumbnail',\
         GdipLoadImageFromFile,'GdipLoadImageFromFile',\
         GdipSetInterpolationMode,'GdipSetInterpolationMode'

  import ole32,\
         CreateStreamOnHGlobal,'CreateStreamOnHGlobal'

  import kernel32,\
         GetModuleHandle,'GetModuleHandleA',\
         ExitProcess,'ExitProcess',\
         MultiByteToWideChar,'MultiByteToWideChar',\
         WideCharToMultiByte,'WideCharToMultiByte',\
         GlobalAlloc,'GlobalAlloc',\
         GlobalLock,'GlobalLock',\
         GetCommandLineA,'GetCommandLineA'

  import user32,\
         RegisterClassExA,'RegisterClassExA',\
         CreateWindowExA,'CreateWindowExA',\
         GetWindowLongA,'GetWindowLongA',\
         SetWindowLongA,'SetWindowLongA',\
         GetSystemMenu,'GetSystemMenu',\
         EnableMenuItem,'EnableMenuItem',\
         ShowWindow,'ShowWindow',\
         SetWindowPos,'SetWindowPos',\
         GetMessageA,'GetMessageA',\
         TranslateMessage,'TranslateMessage',\
         DispatchMessageA,'DispatchMessageA',\
         DefWindowProcA,'DefWindowProcA',\
         SendMessageA,'SendMessageA',\
         GetWindowTextA,'GetWindowTextA',\
         FillRect,'FillRect',\
         GetClientRect,'GetClientRect',\
         GetWindowRect,'GetWindowRect',\
         GetCursorPos,'GetCursorPos',\
         GetDlgItem,'GetDlgItem',\
         GetParent,'GetParent',\
         PostQuitMessage,'PostQuitMessage',\
         SetFocus,'SetFocus',\
         BeginPaint,'BeginPaint',\
         EndPaint,'EndPaint',\
         GetDC,'GetDC',\
         ReleaseDC,'ReleaseDC',\
         TrackMouseEvent,'TrackMouseEvent',\
         DrawTextA,'DrawTextA',\
         InvalidateRect,'InvalidateRect',\
         LoadCursorA,'LoadCursorA',\
         SetCursor,'SetCursor',\
         SetCapture,'SetCapture',\
         ReleaseCapture,'ReleaseCapture',\
         MessageBoxA,'MessageBoxA',\
         SetPropA,'SetPropA',\
         GetPropA,'GetPropA',\
         LoadImageA,'LoadImageA',\
         LoadIconA,'LoadIconA',\
         PostMessageA,'PostMessageA',\
         SetTimer,'SetTimer',\
         KillTimer,'KillTimer',\
         wsprintfA,'wsprintfA'

  import gdi32,\
         CreateFontA,'CreateFontA',\
         CreateSolidBrush,'CreateSolidBrush',\
         CreateCompatibleDC,'CreateCompatibleDC',\
         CreateDIBSection,'CreateDIBSection',\
         DeleteDC,'DeleteDC',\
         StretchBlt,'StretchBlt',\
         SetTextColor,'SetTextColor',\
         SetBkColor,'SetBkColor',\
         DeleteObject,'DeleteObject',\
         SelectObject,'SelectObject',\
         SetBkMode,'SetBkMode',\
         CreatePen,'CreatePen',\
         Rectangle,'Rectangle',\
         ExcludeClipRect,'ExcludeClipRect',\
         GetStockObject,'GetStockObject',\
         RoundRect,'RoundRect',\
         SetStretchBltMode,'SetStretchBltMode',\
         GetObjectA,'GetObjectA',\
         SetBrushOrgEx,'SetBrushOrgEx',\
         BitBlt,'BitBlt',\
         CreateCompatibleBitmap,'CreateCompatibleBitmap'
