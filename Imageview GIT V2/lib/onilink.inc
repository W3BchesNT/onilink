macro DefaultWindowCore {
        cmp     [wmsg], WM_CTLCOLORSTATIC
        je      .handle_ctrl_color
        cmp     [wmsg], 0x0135
        je      .handle_ctrl_color
        jmp     @f

    .handle_ctrl_color:
        invoke  GetWindowLongA, [lparam], -12 
        cmp     eax, 301                      
        je      .paint_fluent_label
        cmp     eax, 105                      
        je      .paint_fluent_button
        cmp     eax, 108                      
        je      .paint_fluent_button
        StaticPaint 0x000000, [w10_bg_brush]

    .paint_fluent_label:
        StaticPaintFluent [_lbl_color]

    .paint_fluent_button:
        invoke  SetBkMode, [wparam], 1
        invoke  GetStockObject, 5 
        pop     edi esi ebx
        ret
        
    @@:
        cmp     [wmsg], WM_ERASEBKGND
        jne     @f
        EraseBackground [w10_bg_brush]
    @@:
        cmp     [wmsg], 0x0020
        jne     @f
        ClientCursorOnly .def_proc
        SetStandardCursor 32512 
    @@:
        cmp     [wmsg], WM_DESTROY
        jne     @f
        invoke  PostQuitMessage, 0
        ReturnEvent 0
    @@:
}

macro WindowHandler name {
        proc name hwnd, wmsg, wparam, lparam
        push    ebx esi edi
}

macro ControlHandler name {
        proc name hwnd, wmsg, wparam, lparam
        push    ebx esi edi
}

macro EndHandler {
    .def_proc:
        pop     edi esi ebx
        invoke  DefWindowProcA, [hwnd], [wmsg], [wparam], [lparam]
        ret
        endp
}

macro ReturnEvent value {
        if ~ value eq
                mov     eax, value
        else
                xor     eax, eax
        end if
        pop     edi esi ebx
        ret
}

macro SetStandardCursor type {
        invoke  LoadCursorA, 0, type
        invoke  SetCursor, eax
        ReturnEvent 1
}

macro ClientCursorOnly label {
        mov     eax, [lparam]
        and     eax, 0xFFFF
        cmp     eax, 1
        jne     label
}

macro SetControlColors bg_color_src, text_color_hex {
        invoke  GetWindowLongA, [hwnd], bg_color_src
        push    eax
        invoke  SetBkColor, [wparam], eax
        invoke  SetTextColor, [wparam], text_color_hex
        pop     eax
        invoke  CreateSolidBrush, eax
        pop     edi esi ebx
        ret
}

macro PaintControl {
        sub     esp, 64
        mov     ebx, esp
        invoke  BeginPaint, [hwnd], ebx
        mov     esi, eax
        invoke  GetClientRect, [hwnd], rect_struct
}

macro EndPaintControl {
        mov     ebx, esp
        invoke  EndPaint, [hwnd], ebx
        add     esp, 64
        ReturnEvent 0
}

macro TrackMouse {
        sub     esp, 16
        mov     dword [esp], 16
        mov     dword [esp+4], 2 
        mov     eax, [hwnd]
        mov     [esp+8], eax
        mov     dword [esp+12], 0
        invoke  TrackMouseEvent, esp
        add     esp, 16
}

macro StaticPaint color, brush {
        invoke  SetTextColor, [wparam], color
        invoke  SetBkMode, [wparam], 1
        mov     eax, brush
        pop     edi esi ebx
        ret
}

macro StaticPaintFluent color {
        invoke  SetTextColor, [wparam], color
        invoke  SetBkMode, [wparam], 1          
        invoke  GetStockObject, 5 
        pop     edi esi ebx
        ret
}

macro EraseBackground brush {
        invoke  GetClientRect, [hwnd], rect_struct
        invoke  FillRect, [wparam], rect_struct, brush
        mov     eax, 1
        pop     edi esi ebx
        ret
}

macro fix_utf8 lpString {
        push    ebx esi edi
        invoke  MultiByteToWideChar, 65001, 0, lpString, -1, out_buffer, 512
        invoke  WideCharToMultiByte, 0, 0, out_buffer, -1, buffer, 512, 0, 0
        mov     eax, buffer 
        pop     edi esi ebx
}

macro set_background hex_color {
        mov     edx, current_style_target
        invoke  SetPropA, edx, _prop_bg, hex_color
}

macro set_border hex_color {
        mov     edx, current_style_target
        invoke  SetPropA, edx, _prop_bc, hex_color
}

macro set_text_color hex_color {
        mov     edx, current_style_target
        local ..prop_txt
        jmp     @f
          ..prop_txt db 'CSS_TXT',0
        @@:
        invoke  SetPropA, edx, ..prop_txt, hex_color      
}

macro set_border_round radius {
        mov     edx, current_style_target
        local ..prop_rnd
        jmp     @f
          ..prop_rnd db 'CSS_RND',0
        @@:
        invoke  SetPropA, edx, ..prop_rnd, radius         
}

macro set_border_radius radius {
        mov     edx, current_style_target
        local ..prop_rnd
        jmp     @f
          ..prop_rnd db 'CSS_RND',0
        @@:
        invoke  SetPropA, edx, ..prop_rnd, radius         
}

macro set_scrollbar state {
        mov     edx, current_style_target
        local ..prop_scr
        jmp     @f
          ..prop_scr db 'CSS_SCR',0
        @@:
        invoke  SetPropA, edx, ..prop_scr, state    
}

macro set_text_size size {
        mov     edx, current_style_target
        local ..prop_sz
        jmp     @f
          ..prop_sz db 'CSS_SIZE',0
        @@:
        invoke  SetPropA, edx, ..prop_sz, size    
}

macro DynamicWindowsToolbar hwnd, state {
        if state eq 1
                push    4
                sub     esp, 4
                mov     dword [esp], 1  
                mov     eax, esp
                invoke  DwmSetWindowAttribute, hwnd, 20, eax, [esp+4]
                add     esp, 8

                push    4
                sub     esp, 4
                mov     dword [esp], 0xFFFFFFFE
                mov     eax, esp
                invoke  DwmSetWindowAttribute, hwnd, 35, eax, [esp+4]
                add     esp, 8
        end if
}
