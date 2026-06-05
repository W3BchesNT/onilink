; ==============================================================================
; PICASO.INC — УЛЬТИМАТИВНЫЙ ГРАФИЧЕСКИЙ ДВИЖОК ДЛЯ UIX FRAMEWORK
; ==============================================================================

proc UIX_ImageLoadFromFile lp_hwnd_img_var, lpFileName
        push    ebx esi edi
        
        ; 1. Достаем хэндл окна напрямую по его адресу в памяти
        mov     eax, [lp_hwnd_img_var]
        mov     eax, [eax]
        test    eax, eax
        jz      .exit
        mov     edi, eax        ; edi = хэндл нашего ImageView
        
        ; 2. Конвертируем путь из ANSI/CP1251 в UTF-16 для GDI+ (с поддержкой кириллицы)
        invoke  MultiByteToWideChar, 0, 0, [lpFileName], -1, out_buffer, 512
        
        ; 3. Загружаем изображение из файла на диске
        sub     esp, 4
        mov     ebx, esp
        invoke  GdipLoadImageFromFile, out_buffer, ebx
        pop     eax             ; eax = хэндл новой GDI+ картинки
        
        test    eax, eax
        jz      .exit           ; Если файл не картинка или отсутствует — выходим
        mov     esi, eax        ; esi = новая картинка
        
        ; 4. Безопасно вычищаем из памяти старую графику элемента ImageView
        invoke  GetWindowLongA, edi, 0
        test    eax, eax
        jz      .save_new
        invoke  GdipDisposeImage, eax
        
    .save_new:
        ; 5. Записываем новый хэндл картинки в память ImageView и принудительно перерисовываем
        invoke  SetWindowLongA, edi, 0, esi
        invoke  InvalidateRect, edi, 0, 1
        
    .exit:
        pop     edi esi ebx
        ret
endp

proc UIX_ParseCommandLineToBuffer lpTargetBuffer
        push    ebx esi edi
        
        invoke  GetCommandLineA
        mov     esi, eax
        
        mov     al, [esi]
        cmp     al, '"'
        je      .skip_exe_quoted
        
    .skip_exe_loop:
        mov     al, [esi]
        cmp     al, 0
        jz      .no_args
        cmp     al, ' '
        je      .find_arg_start
        inc     esi
        jmp     .skip_exe_loop
        
    .skip_exe_quoted:
        inc     esi
    .skip_exe_quoted_loop:
        mov     al, [esi]
        cmp     al, 0
        jz      .no_args
        cmp     al, '"'
        je      .end_exe_quote
        inc     esi
        jmp     .skip_exe_quoted_loop
    .end_exe_quote:
        inc     esi
        
    .find_arg_start:
        mov     al, [esi]
        cmp     al, 0
        jz      .no_args
        cmp     al, ' '
        jne     .copy_arg
        inc     esi
        jmp     .find_arg_start
        
    .copy_arg:
        mov     edi, [lpTargetBuffer]
        
        mov     al, [esi]
        cmp     al, '"'
        je      .copy_quoted
        
    .copy_loop:
        mov     al, [esi]
        cmp     al, 0
        jz      .done_copy
        mov     [edi], al
        inc     esi
        inc     edi
        jmp     .copy_loop
        
    .copy_quoted:
        inc     esi             
    .copy_quoted_loop:
        mov     al, [esi]
        cmp     al, 0
        jz      .done_copy
        cmp     al, '"'         
        je      .done_copy
        mov     [edi], al
        inc     esi
        inc     edi
        jmp     .copy_quoted_loop
        
    .done_copy:
        mov     byte [edi], 0   
        jmp     .exit
        
    .no_args:
        mov     edi, [lpTargetBuffer]
        mov     byte [edi], 0  
        
    .exit:
        pop     edi esi ebx
        ret
endp
