; ==============================================================================
; FLUENT.INC — МАКРОСЫ И ЛОГИКА ГРАФИЧЕСКОГО ДВИЖОКА В СТИЛЕ FLUENT
; ==============================================================================
macro init_fluent_ui {
        invoke  GetModuleHandle, 0
        mov     [wc_btn+16], eax
        push    wc_btn
        call    [RegisterClassExA]
        
        mov     [wc_edit+16], eax
        push    wc_edit
        call    [RegisterClassExA]

        mov     [wc_img+16], eax
        push    wc_img
        call    [RegisterClassExA]

        mov     [wc_txtbox+16], eax
        push    wc_txtbox
        call    [RegisterClassExA]
        
        invoke  LoadCursorA, 0, 32649
        mov     [h_hand_cursor], eax
        
        invoke  CreateSolidBrush, 0xF2F2F2 
        mov     [w10_edit_bg], eax
        
        invoke  CreatePen, 0, 1, 0x7A7A7A  
        mov     [w10_edit_border], eax
        invoke  CreatePen, 0, 1, 0x0078D7  
        mov     [w10_edit_focus], eax

        invoke  CreateFontA, 15, 0, 0, 0, 400, 0, 0, 0, 1, 0, 0, 5, 0, _font_name
        mov     [hfont], eax

        invoke  GdiplusStartup, gdip_token, gdip_input, 0
}

macro fluent_window title, width, height, hwnd_output, bg_hex_color {
        local ..title, ..skip, ..final_color
        jmp     ..skip
          ..title db title, 0
        ..skip:
        ..final_color = ((bg_hex_color and 0x0000FF) shl 16) or (bg_hex_color and 0x00FF00) or ((bg_hex_color and 0xFF0000) shr 16)
        invoke  CreateSolidBrush, ..final_color
        mov     [w10_bg_brush], eax

        fix_utf8 ..title
        invoke  CreateWindowExA, 0, _my_class, eax, 0x00CF0000, 100, 100, width, height, 0, 0, 0, 0
        mov     [hwnd_output], eax
}

macro fluent_button text, x, y, width, height, id {
        local ..text, ..skip
        jmp     ..skip
          ..text db text, 0
        ..skip:
        fix_utf8 ..text
        invoke  CreateWindowExA, 0, _vs_b_class, eax, 0x50000000, x, y, width, height, [current_parent_hwnd], id, 0, 0
}

macro fluent_editline x, y, w, h, id, hwnd_out {
        invoke  CreateWindowExA, 0, _vs_e_class, 0, 0x50000000, x, y, w, h, [current_parent_hwnd], id, 0, 0
        if ~ hwnd_out eq
                mov     [hwnd_out], eax
        end if
}

; Улучшение: чистый динамический ImageView для работы с внешними файлами картинок
macro fluent_dynamic_imageview x, y, width, height, id, hwnd_out {
        invoke  CreateWindowExA, 0, _vs_img_class, 0, 0x50000000, x, y, width, height, [current_parent_hwnd], id, 0, 0
        if ~ hwnd_out eq
                mov     [hwnd_out], eax
        end if
}

macro fluent_imageview path, x, y, width, height, id {
        local ..img_data, ..img_size, ..skip_data
        jmp     ..skip_data
          align 4
          ..img_data: file path
          ..img_size = $ - ..img_data
        ..skip_data:

        invoke  CreateWindowExA, 0, _vs_img_class, 0, 0x50000000, x, y, width, height, [current_parent_hwnd], id, 0, 0
        push    eax 
        invoke  GlobalAlloc, 0x0002, ..img_size
        push    eax 
        invoke  GlobalLock, eax
        
        push    esi edi ecx
        mov     esi, ..img_data
        mov     edi, eax
        mov     ecx, ..img_size
        rep     movsb
        pop     ecx edi esi
        
        mov     edx, [esp] 
        sub     esp, 4
        mov     ebx, esp
        invoke  CreateStreamOnHGlobal, edx, 1, ebx
        pop     eax 
        
        test    eax, eax
        jz      .stream_error
        sub     esp, 4
        mov     ebx, esp
        invoke  GdipCreateBitmapFromStream, eax, ebx
        pop     eax 
        jmp     .store_img
    .stream_error:
        xor     eax, eax 
    .store_img:
        add     esp, 4 
        pop     edx    
        push    eax
        invoke  SetWindowLongA, edx, 0, eax
        pop     eax
}

macro fluent_textbox x, y, w, h, id, hwnd_out {
        invoke  CreateWindowExA, 0, _vs_txt_class, 0, 0x50000000, x, y, w, h, [current_parent_hwnd], id, 0, 0
        if ~ hwnd_out eq
                mov     [hwnd_out], eax
        end if
        
        push    eax
        invoke  CreateWindowExA, 0, _sys_edit, 0, 0x50010044, 7, 5, w-14, h-10, eax, 998, 0, 0
        pop     eax

        push    eax
        current_style_target equ [esp]
                set_background   0x2D2D2D   
                set_border       0x3F3F46   
        add     esp, 4
        restore current_style_target
}

macro fluent_layout x, y, width, height, parent_hwnd, hwnd_output, bg_hex_color {
        local ..final_color
        ..final_color = ((bg_hex_color and 0x0000FF) shl 16) or (bg_hex_color and 0x00FF00) or ((bg_hex_color and 0xFF0000) shr 16)
        invoke  CreateSolidBrush, ..final_color
        mov     [w10_toolbar_brush], eax
        invoke  CreateWindowExA, 0, _sys_static, 0, 0x50000000, x, y, width, height, parent_hwnd, 0, 0, 0
        mov     [hwnd_output], eax
        invoke  SendMessageA, eax, 0x0030, [hfont], 1
}

macro fluent_label text, x, y, width, height, id, text_color {
        local ..text, ..skip
        jmp     ..skip
          ..text db text, 0
        ..skip:
        mov     [_lbl_color], text_color
        fix_utf8 ..text
        invoke  CreateWindowExA, 0, _sys_static, eax, 0x50000000, x, y, width, height, [current_parent_hwnd], id, 0, 0
        invoke  SendMessageA, eax, 0x0030, [hfont], 1
}

include 'enc/fluent_core.inc'
include 'lib/enc/fluent_proc.inc'