; ==============================================================================
; FLUENT_CORE.INC — ЧИСТЫЕ МАКРОСЫ РЕНДЕРИНГА И СТИЛЕЙ (ТОЛЬКО ХУДОЖНИК)
; ==============================================================================

macro clean_fluent_ui {
        invoke  DeleteObject, [hfont]
        invoke  DeleteObject, [w10_bg_brush]
        invoke  DeleteObject, [w10_toolbar_brush]
        invoke  GdiplusShutdown, [gdip_token]
}

macro RenderFluentButton {
        PaintControl 
        invoke  GetPropA, [hwnd], _prop_bg
        test    eax, eax
        jnz     @f
        mov     eax, 0x2D2D2D 
    @@:
        invoke  CreateSolidBrush, eax
        push    eax                         
        invoke  SelectObject, esi, eax

        invoke  GetPropA, [hwnd], _prop_bc
        test    eax, eax
        jnz     @f
        mov     eax, 0x3F3F46
    @@:
        invoke  CreatePen, 0, 1, eax
        push    eax
        invoke  SelectObject, esi, eax

        invoke  RoundRect, esi, [rect_struct], [rect_struct+4], [rect_struct+8], [rect_struct+12], 14, 14
        
        pop     eax
        invoke  DeleteObject, eax           
        pop     eax
        invoke  DeleteObject, eax           

        invoke  GetWindowTextA, [hwnd], out_buffer, 512
        invoke  SelectObject, esi, [hfont]
        
        jmp     @f
          .txt_prop_btn db 'CSS_TXT',0
        @@:
        invoke  GetPropA, [hwnd], .txt_prop_btn
        test    eax, eax
        jnz     @f
        mov     eax, 0xFFFFFF
    @@:
        invoke  SetTextColor, esi, eax
        invoke  SetBkMode, esi, 1
        invoke  DrawTextA, esi, out_buffer, -1, rect_struct, 0x25
        
        EndPaintControl 
}

macro RenderFluentEditLine {
        sub     esp, 64
        mov     ebx, esp
        invoke  BeginPaint, [hwnd], ebx
        mov     esi, eax
        invoke  GetClientRect, [hwnd], rect_struct
        
        invoke  GetPropA, [hwnd], _prop_bg
        test    eax, eax
        jnz     @f
        mov     eax, 0x2D2D2D
    @@:
        invoke  CreateSolidBrush, eax
        push    eax 
        invoke  SelectObject, esi, eax
        
        invoke  GetPropA, [hwnd], _prop_bc
        test    eax, eax
        jnz     @f
        mov     eax, 0x3F3F46
    @@:
        invoke  CreatePen, 0, 1, eax
        push    eax 
        invoke  SelectObject, esi, eax
        
        invoke  Rectangle, esi, 0, 0, [rect_struct+8], [rect_struct+12]
        
        pop     eax
        invoke  DeleteObject, eax
        pop     eax
        invoke  DeleteObject, eax
        
        mov     ebx, esp
        invoke  EndPaint, [hwnd], ebx
        add     esp, 64
}

macro RenderFluentImage {
        push    ebx esi edi
        sub     esp, 64
        mov     ebx, esp
        invoke  BeginPaint, [hwnd], ebx
        mov     esi, eax ; esi = HDC
        invoke  GetClientRect, [hwnd], rect_struct
        invoke  GetWindowLongA, [hwnd], 0
        mov     edi, eax ; edi = хэндл GDI+ картинки
        test    edi, edi
        jz      .no_image

        ; 1. Получаем реальные размеры картинки от GDI+
        invoke  GdipGetImageWidth, edi, img_real_w
        invoke  GdipGetImageHeight, edi, img_real_h

        ; 2. Считаем пропорции: (WinWidth * ImgHeight) vs (WinHeight * ImgWidth)
        mov     eax, [rect_struct+8]  ; eax = Ширина окна
        imul    eax, [img_real_h]     ; eax = WinWidth * ImgHeight
        mov     edx, [rect_struct+12] ; edx = Высота окна
        imul    edx, [img_real_w]     ; edx = WinHeight * ImgWidth

        cmp     eax, edx
        jge     .fit_to_height

    .fit_to_width:
        ; Картинка упирается в бока окна
        mov     eax, [rect_struct+8]
        mov     [img_target_w], eax   ; TargetWidth = WinWidth
        
        mov     eax, [rect_struct+8]
        mul     [img_real_h]
        div     [img_real_w]
        mov     [img_target_h], eax   ; TargetHeight = (WinWidth * ImgHeight) / ImgWidth
        
        mov     [img_target_x], 0     ; X = 0
        
        mov     eax, [rect_struct+12]
        sub     eax, [img_target_h]
        sar     eax, 1
        mov     [img_target_y], eax   ; Centered Y = (WinHeight - TargetHeight) / 2
        jmp     .draw_now

    .fit_to_height:
        ; Картинка упирается в верх и низ окна
        mov     eax, [rect_struct+12]
        mov     [img_target_h], eax   ; TargetHeight = WinHeight
        
        mov     eax, [rect_struct+12]
        mul     [img_real_w]
        div     [img_real_h]
        mov     [img_target_w], eax   ; TargetWidth = (WinHeight * ImgWidth) / ImgHeight
        
        mov     [img_target_y], 0     ; Y = 0
        
        mov     eax, [rect_struct+8]
        sub     eax, [img_target_w]
        sar     eax, 1
        mov     [img_target_x], eax   ; Centered X = (WinWidth - TargetWidth) / 2

    .draw_now:
        ; 3. Создаем графический контекст и рисуем с идеальными пропорциями
        sub     esp, 4
        mov     ebx, esp
        invoke  GdipCreateFromHDC, esi, ebx
        pop     eax ; eax = Graphics context
        
        invoke  GdipDrawImageRectI, eax, edi, [img_target_x], [img_target_y], [img_target_w], [img_target_h]
        invoke  GdipDeleteGraphics, eax

    .no_image:
        mov     ebx, esp
        invoke  EndPaint, [hwnd], ebx
        add     esp, 64
        pop     edi esi ebx
}
