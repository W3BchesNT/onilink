; ==============================================================================
; GUI.INC — ОПИСАНИЕ И ИНИЦИАЛИЗАЦИЯ ЭЛЕМЕНТОВ ИНТЕРФЕЙСА (ИСПРАВЛЕН)
; ==============================================================================

macro create_gui_elements {
        init_fluent_ui
        
        CreateAppWindow login_window, "Picasso Mini Viewer", 800, 600, 0x1E1E1E
                SetIcon [hwnd_login_window], "ico\main.ico"
                DynamicWindowsToolbar [hwnd_login_window], 1 
                
                ; Создаем ImageView элемент
                invoke  CreateWindowExA, 0, _vs_img_class, 0, 0x50000000, 0, 0, 800, 600, [hwnd_login_window], 501, 0, 0
                mov     [hwnd_imageview], eax
                
                ; КРИТИЧЕСКИЙ ФИКС ДЛЯ КАЧЕСТВА:
                ; Получаем контекст устройства (HDC) созданного виджета
                invoke  GetDC, [hwnd_imageview]
                push    eax
                
                ; Включаем режим HALFTONE (4) — это активирует аппаратное сглаживание пикселей
                ; на уровне Windows GDI. Картинка при ресайзе перестанет пикселиться!
                invoke  SetStretchBltMode, eax, 4
                
                pop     eax
                invoke  ReleaseDC, [hwnd_imageview], eax
                
        EndWindow

        call    ProcessCommandLine
        invoke  ShowWindow, [hwnd_login_window], 1
}
