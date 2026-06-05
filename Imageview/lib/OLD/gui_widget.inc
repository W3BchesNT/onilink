macro _set_widget_font hwnd_reg {
        local ..skip_font
        cmp     [hfont], 0
        je      ..skip_font
        invoke  SendMessageA, hwnd_reg, 0x0030, [hfont], 1
    ..skip_font:
}

macro create_font font_name, size {
        local ..name, ..skip
        jmp     ..skip
          ..name db font_name, 0
    ..skip:
        invoke  CreateFontA, size, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, ..name
        mov     [hfont], eax
}

macro create_window class_name, title, width, height, hwnd_output {
        local ..title, ..skip
        jmp     ..skip
          ..title db title, 0
    ..skip:
        push    ..title
        call    FixUtf8Text
        invoke  CreateWindowExA, 0, class_name, eax,\
                WS_OVERLAPPED or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_THICKFRAME,\
                CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, 0, 0
        mov     [hwnd_output], eax
}

macro create_label text, x, y, width, height, parent_hwnd {
        local ..txt, ..skip
        jmp     ..skip
          ..txt db text, 0
    ..skip:
        push    ..txt
        call    FixUtf8Text
        invoke  CreateWindowExA, 0, _class, eax, WS_VISIBLE or WS_CHILD, x, y, width, height, [parent_hwnd], 0, 0, 0
        _set_widget_font eax
}

macro create_edit text, x, y, width, height, id, parent_hwnd, hwnd_output {
        local ..txt, ..skip
        jmp     ..skip
          if text eqtype ""
            ..txt db text, 0
          else
            ..txt dd 0
          end if
    ..skip:
        if text eqtype ""
            push    ..txt
            call    FixUtf8Text
            invoke  CreateWindowExA, WS_EX_CLIENTEDGE, _edit_class, eax, WS_VISIBLE or WS_CHILD or ES_AUTOHSCROLL, x, y, width, height, [parent_hwnd], id, 0, 0
        else
            invoke  CreateWindowExA, WS_EX_CLIENTEDGE, _edit_class, 0, WS_VISIBLE or WS_CHILD or ES_AUTOHSCROLL, x, y, width, height, [parent_hwnd], id, 0, 0
        end if
        mov     [hwnd_output], eax
        _set_widget_font [hwnd_output]
}

macro create_button title, x, y, width, height, id, parent_hwnd, hwnd_output {
        local ..title, ..skip
        jmp     ..skip
          ..title db title, 0
    ..skip:
        push    ..title
        call    FixUtf8Text
        invoke  CreateWindowExA, 0, _button_class, eax, WS_VISIBLE or WS_CHILD or BS_PUSHBUTTON, x, y, width, height, [parent_hwnd], id, 0, 0
        if ~ hwnd_output eq
                mov [hwnd_output], eax
        end if
        _set_widget_font eax
}

macro create_layout x, y, width, height, parent_hwnd, hwnd_output {
        ; Исправлено: [parent_hwnd] в скобках, чтобы брался хэндл окна из памяти, а не текст
        invoke  CreateWindowExA, 0, _class, 0, WS_VISIBLE or WS_CHILD, x, y, width, height, [parent_hwnd], 0, 0, 0
        mov     [hwnd_output], eax
}
