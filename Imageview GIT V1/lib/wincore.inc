macro OnMessage message, label {
        cmp     [wmsg], message
        je      label
}

macro OnCommandID btn_id, label {
        cmp     eax, btn_id
        je      label
}

macro window_class name, proc_name, class_str {
        local ..class_str
        jmp     @f
          ..class_str db class_str, 0
        @@:
        name WNDCLASSEX 48, 3, proc_name, 0, 32, ?, 0, 0, 6, 0, ..class_str, 0
        hwnd_#name dd ?
}

macro register_dynamic_window struct_name {
        invoke  GetModuleHandle, 0
        mov     [struct_name+16], eax
        invoke  LoadCursorA, 0, 32512
        push    struct_name
        call    [RegisterClassExA]
}

macro define_framework_data {
  _sys_static       db 'STATIC',0
  _sys_button       db 'BUTTON',0
  _sys_edit         db 'EDIT',0
  _vs_e_class       db 'W10_EDITLINE',0 
  _vs_img_class     db 'W10_IMAGEVIEW',0
  _vs_txt_class     db 'W10_TEXTBOX',0
  _prop_bg          db 'CSS_BG',0
  _prop_bc          db 'CSS_BC',0

  wc_btn            WNDCLASSEX 48, 3, ButtonProc, 0, 32, ?, 0, 0, 0, 0, _vs_b_class, 0
  wc_edit           WNDCLASSEX 48, 3, EditLineProc, 0, 32, ?, 0, 0, 0, 0, _vs_e_class, 0
  wc_img            WNDCLASSEX 48, 11, ImageViewProc, 0, 32, ?, 0, 0, 0, 0, _vs_img_class, 0
  wc_txtbox         WNDCLASSEX 48, 3, TextBoxProc, 0, 32, ?, 0, 0, 0, 0, _vs_txt_class, 0

  gdip_input        dd 1, 0, 0, 0
  gdip_token        dd ?
  hwnd_btn          dd ?
  hwnd_panel        dd ?
  hfont             dd ?
  h_hand_cursor     dd ?
  w10_bg_brush      dd ? 
  w10_toolbar_brush dd ? 
  w10_ctrl_brush    dd ? 
  w10_hover_brush   dd ? 
  w10_press_brush   dd ? 
  w10_border_pen    dd ? 
  w10_edit_bg       dd ?
  w10_edit_border   dd ?
  w10_edit_focus    dd ?
  _lbl_color        dd 0
  _lbl_font         dd ?
  btn_state         dd 0
  edit_focus_state  dd 0 
  rect_struct       dd 4 dup(0)
  hwnd_edit         dd ?
  hwnd_textbox      dd ?
  buffer            db 512 dup(0)
  out_buffer        db 1024 dup(0)
  
  ; ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ХАРДВЕРНОГО КЭША
  hdc_mem           dd ?
  hbmp_mem          dd ?
  img_w             dd ?
  img_h             dd ?
  
  align 4
  msg               MSG
}

macro CreateAppWindow name, title, width, height, bg_color, parent_hwnd {
        local ..title, ..skip, ..final_color
        jmp     ..skip
          ..title db title, 0
        ..skip:
        ..final_color = ((bg_color and 0x0000FF) shl 16) or (bg_color and 0x00FF00) or ((bg_color and 0xFF0000) shr 16)
        invoke  CreateSolidBrush, ..final_color
        mov     [w10_bg_brush], eax

        if parent_hwnd eq
                invoke  CreateWindowExA, 0, [name#.lpszClassName], ..title, 0x00CF0000, 100, 100, width, height, 0, 0, 0, 0
        else
                invoke  CreateWindowExA, 0, [name#.lpszClassName], ..title, 0x00CF0000, 200, 200, width, height, parent_hwnd, 0, 0, 0
        end if
        mov     [hwnd_#name], eax
        current_parent_hwnd equ hwnd_#name
}

macro EndWindow { restore current_parent_hwnd }

macro fluent_editline x, y, w, h, id, hwnd_out {
        invoke  CreateWindowExA, 0, _vs_e_class, 0, 0x50000000, x, y, w, h, [current_parent_hwnd], id, 0, 0
        if ~ hwnd_out eq
                mov     [hwnd_out], eax
        end if
        push    eax
        invoke  SetPropA, eax, _prop_bg, 0xFFFFFF
        invoke  SetPropA, eax, _prop_bc, 0x7A7A7A
        pop     eax
}

macro with_style target {
        if target eq eax
                push    eax
        else
                push    dword [target]
        end if
        define current_style_target [esp]
}

macro end_style {
        add     esp, 4
        restore current_style_target
}
