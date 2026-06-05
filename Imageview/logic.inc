include 'lib/uix.inc'

; ==============================================================================
; ХЭНДЛЕР ДЛЯ IMAGEVIEW
; ==============================================================================
ControlHandler CustomImageViewProc
        OnMessage 0x000F,         .img_paint
        OnMessage 0x0002,         .img_destroy
        OnMessage 0x0020,         .img_setcursor
        OnMessage 0x0206,         .img_rbutton_dblclk   ; 0x0206 = WM_RBUTTONDBLCLK
        jmp     .def_proc

.img_paint:
        RenderFluentImage
        ReturnEvent 0

.img_destroy:
        invoke  GetWindowLongA, [hwnd], 0
        test    eax, eax
        jz      .skip_dispose
        invoke  GdipDisposeImage, eax
    .skip_dispose:
        ReturnEvent 0

.img_setcursor:
        SetStandardCursor 32512 

.img_rbutton_dblclk:
        ; Просмотрщик: двойной правый клик по картинке красиво закрывает окно
        invoke  GetParent, [hwnd]
        invoke  SendMessageA, eax, 0x0010, 0, 0         ; Отправляем WM_CLOSE главному окну
        ReturnEvent 0

EndHandler

; ==============================================================================
; ХЭНДЛЕР ГЛАВНОГО ОКНА
; ==============================================================================
WindowHandler WindowProc
        DefaultWindowCore
        OnMessage 0x0005,         .on_size              ; 0x0005 = WM_SIZE
        jmp     .def_proc

.on_size:
        ; Адаптивность: элемент автоматически растягивается на всё окно
        Control_DockFill hwnd_imageview, [lparam]
        ReturnEvent 0

EndHandler

WindowHandler SubWindowProc
        OnMessage WM_CLOSE, .on_sub_close
        jmp     .def_proc
.on_sub_close:
        invoke  ShowWindow, [hwnd], 0
        ReturnEvent 0
EndHandler

; ==============================================================================
; ИНИЦИАЛИЗАЦИЯ И СТАРТ ПРИЛОЖЕНИЯ (ЧИСТЫЙ СРЕДНЕУРОВНЕВЫЙ СТИЛЬ)
; ==============================================================================
proc ProcessCommandLine
        push    ebx esi edi
        
        ; 1. Просим библиотеку Picaso считать аргумент запуска в наш буфер
        App_GetLaunchArgument cmd_file_buffer
        
        ; 2. Проверяем, передали ли файл
        mov     al, [cmd_file_buffer]
        test    al, al
        jz      .exit
        
        ; 3. Загружаем графику с диска ультимативной командой напрямую по адресу переменной!
        Image_LoadFromFile hwnd_imageview, cmd_file_buffer
        
    .exit:
        pop     edi esi ebx
        ret
endp
