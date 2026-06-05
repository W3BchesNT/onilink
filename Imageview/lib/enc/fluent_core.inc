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
        mov     esi, eax 
        invoke  GetClientRect, [hwnd], rect_struct
        invoke  GetWindowLongA, [hwnd], 0
        mov     edi, eax 
        test    edi, edi
        jz      .no_image
        sub     esp, 4
        mov     ebx, esp
        invoke  GdipCreateFromHDC, esi, ebx
        pop     eax 
        invoke  GdipDrawImageRectI, eax, edi, 0, 0, [rect_struct+8], [rect_struct+12]
        invoke  GdipDeleteGraphics, eax
    .no_image:
        mov     ebx, esp
        invoke  EndPaint, [hwnd], ebx
        add     esp, 64
        pop     edi esi ebx
}
