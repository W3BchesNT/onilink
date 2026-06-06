macro ToggleWindowVisibility hwnd_var, state {
        if state eq 1
                invoke  ShowWindow, [hwnd_var], 5
        else
                invoke  ShowWindow, [hwnd_var], 0
        end if
}

macro ToggleMaximizeButton hwnd_var, state {
        invoke  GetWindowLongA, hwnd_var, -16
        if state eq 1
                or      eax, 0x00010000
        else
                and     eax, not 0x00010000
        end if
        invoke  SetWindowLongA, hwnd_var, -16, eax
        invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
}

macro ToggleMinimizeButton hwnd_var, state {
        invoke  GetWindowLongA, hwnd_var, -16
        if state eq 1
                or      eax, 0x00020000
        else
                and     eax, not 0x00020000
        end if
        invoke  SetWindowLongA, hwnd_var, -16, eax
        invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
}

macro ToggleCloseButton hwnd_var, state {
        invoke  GetSystemMenu, hwnd_var, 0
        if state eq 1
                invoke  EnableMenuItem, eax, 0xF060, 0
        else
                invoke  EnableMenuItem, eax, 0xF060, 1
        end if
        invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
}

macro FreezeWindowSize hwnd_var {
        invoke  GetWindowLongA, hwnd_var, -16
        and     eax, not (0x00010000 or 0x00040000)
        invoke  SetWindowLongA, hwnd_var, -16, eax
        invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
}

macro UnfreezeWindowSize hwnd_var {
        invoke  GetWindowLongA, hwnd_var, -16
        or      eax, (0x00010000 or 0x00040000)
        invoke  SetWindowLongA, hwnd_var, -16, eax
        invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
}

macro SetTitle hwnd_var, text_str {
        local ..txt, ..skip
        jmp     ..skip
          ..txt db text_str, 0
        ..skip:
        push    ..txt
        call    FixUtf8Text
        invoke  SendMessageA, hwnd_var, 0x000C, 0, eax
}

macro SetSize hwnd_var, width, height {
        invoke  SetWindowPos, hwnd_var, 0, 0, 0, width, height, 2 or 4
}

macro SetText hwnd_var, text_str {
        local ..txt, ..skip
        jmp     ..skip
          ..txt db text_str, 0
        ..skip:
        push    ..txt
        call    FixUtf8Text
        invoke  SendMessageA, hwnd_var, 0x000C, 0, eax
}

macro ClearText hwnd_edit_var {
        local ..empty, ..skip
        jmp     ..skip
          ..empty db 0
        ..skip:
        invoke  SendMessageA, hwnd_edit_var, 0x000C, 0, ..empty
}

macro MessageBox parent_hwnd_arg, format_str, src_buffer, title_str {
        local ..title, ..skip, ..msg
        jmp     ..skip
          ..title db title_str, 0
          ..msg   db format_str, 0
        ..skip:
        push    ..title
        call    FixUtf8Text
        push    eax
        push    ..msg
        call    FixUtf8Text
        invoke  wsprintfA, out_buffer, eax, src_buffer
        pop     edx
        if parent_hwnd_arg eq hwnd
                invoke  MessageBoxA, [hwnd], out_buffer, edx, 0x00000040
        else
                invoke  MessageBoxA, parent_hwnd_arg, out_buffer, edx, 0x00000040
        end if
}

macro WM_BASIC_WINPROC msg_var {
        push    ebx esi edi
        cmp     [msg_var], 0x0111
        je      wmcommand
}

macro WM_BASIC_DESTROY msg_var {
        cmp     [msg_var], 0x0002
        je      wmdestroy
}

macro defwinproc hwnd_arg, msg_arg, wparam_arg, lparam_arg {
        pop     edi esi ebx
        invoke  DefWindowProcA, [hwnd], [wmsg], [wparam], [lparam]
        ret
}

macro check_id id_num, label_to_go {
        mov     edx, eax
        and     edx, 0FFFFh
        cmp     edx, id_num
        je      label_to_go
}

macro end_proc {
        pop     edi esi ebx
        ret
}

macro WM_QUIT {
        invoke  PostQuitMessage, 0
        xor     eax, eax
        ret
}

; --- КРУТАЯ МЕХАНИКА: ПЕРЕТАСКИВАНИЕ БЕЗРАМОЧНОГО ОКНА ЗА ЛЮБУЮ ЧАСТЬ ФОНА ---
macro EnableWindowDrag main_hwnd_var {
        invoke  ReleaseCapture
        invoke  SendMessageA, main_hwnd_var, 0x00A1, 2, 0 ; WM_NCLBUTTONDOWN = 0xA1, HTCAPTION = 2
}
