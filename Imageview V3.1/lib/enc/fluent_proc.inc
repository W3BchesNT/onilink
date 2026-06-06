; ==============================================================================
; FLUENT_PROC.INC — СИСТЕМНЫЕ ОКОННЫЕ ПРОЦЕДУРЫ КОМПОНЕНТОВ (РЕДКО ИЗМЕНЯЕМЫЕ)
; ==============================================================================

ControlHandler ButtonProc
        OnMessage 0x000F,         .btn_paint
        OnMessage 0x0201,         .btn_lbuttondown
        OnMessage 0x0202,         .btn_lbuttonup
        OnMessage 0x0020,         .btn_setcursor
        jmp     .def_proc

.btn_paint:
        RenderFluentButton 
        ReturnEvent 0

.btn_lbuttondown:
        invoke  SetCapture, [hwnd]
        ReturnEvent 0

.btn_lbuttonup:
        invoke  ReleaseCapture
        invoke  GetWindowLongA, [hwnd], -12 
        push    eax 
        invoke  GetParent, [hwnd]           
        mov     ebx, eax
        pop     eax      
        invoke  SendMessageA, ebx, 0x0111, eax, [hwnd] 
        ReturnEvent 0

.btn_setcursor:
        invoke  SetCursor, [h_hand_cursor]
        ReturnEvent 1

EndHandler

ControlHandler EditLineProc
        OnMessage 0x0001,         .on_edit_create
        OnMessage 0x000F,         .on_edit_paint
        OnMessage 0x0133,         .on_edit_color
        OnMessage 0x0111,         .on_edit_command
        OnMessage 0x0007,         .on_edit_focus
        OnMessage 0x0020,         .on_edit_setcursor
        jmp     .def_proc

.on_edit_create:
        invoke  GetClientRect, [hwnd], rect_struct
        mov     eax, [rect_struct+8]
        sub     eax, 14           
        invoke  CreateWindowExA, 0, _sys_edit, 0, 0x50010080, 7, 5, eax, 22, [hwnd], 999, 0, 0
        ReturnEvent 0

.on_edit_paint:
        invoke  GetDlgItem, [hwnd], 999
        invoke  SendMessageA, eax, 0x0030, [hfont], 0
        RenderFluentEditLine
        ReturnEvent 0

.on_edit_color:
        invoke  GetParent, [hwnd]
        invoke  GetPropA, eax, _prop_bg
        test    eax, eax
        jnz     @f
        mov     eax, 0x2D2D2D
    @@:
        invoke  SetBkColor, [wparam], eax
        invoke  SetTextColor, [wparam], 0xFFFFFF
        invoke  CreateSolidBrush, eax
        ReturnEvent eax

.on_edit_command:
        invoke  GetParent, [hwnd]
        invoke  SendMessageA, eax, [wmsg], [wparam], [lparam]
        ReturnEvent 0

.on_edit_focus:
        mov     [edit_focus_state], 1
        invoke  InvalidateRect, [hwnd], 0, 1
        ReturnEvent 0

.on_edit_setcursor:
        ClientCursorOnly .def_proc
        SetStandardCursor 32513 

EndHandler

ControlHandler TextBoxProc
        OnMessage 0x000F,         .on_txt_paint
        OnMessage 0x0133,         .on_txt_color
        OnMessage 0x0111,         .on_txt_command
        OnMessage 0x0020,         .on_txt_setcursor
        jmp     .def_proc

.on_txt_paint:
        invoke  GetDlgItem, [hwnd], 998
        invoke  SendMessageA, eax, 0x0030, [hfont], 1
        RenderFluentEditLine
        ReturnEvent 0

.on_txt_color:
        jmp     @f
          .txt_prop_box db 'CSS_TXT',0
        @@:
        invoke  GetPropA, [hwnd], .txt_prop_box
        mov     edi, eax
        test    edi, edi
        jnz     @f
        mov     edi, 0xFFFFFF
    @@:
        invoke  GetPropA, [hwnd], _prop_bg
        mov     ebx, eax
        test    ebx, ebx
        jnz     @f
        mov     ebx, 0x2D2D2D
    @@:
        invoke  SetBkColor, [wparam], ebx
        invoke  SetTextColor, [wparam], edi
        invoke  CreateSolidBrush, ebx
        ReturnEvent eax

.on_txt_command:
        invoke  GetParent, [hwnd]
        invoke  SendMessageA, eax, [wmsg], [wparam], [lparam]
        ReturnEvent 0

.on_txt_setcursor:
        ClientCursorOnly .def_proc
        SetStandardCursor 32513 

EndHandler
