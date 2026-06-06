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

; УНИВЕРСАЛЬНЫЙ СУПЕР-МАКРОС SETTEXT (С ЛОКАЛЬНЫМИ МЕТКАМИ)
macro SetText target_hwnd, text_param {
        local ..txt, ..skip, ..end_m, ..found_edit
        push    ebx
        
        ; Автоматически проверяем структуру контейнера
        invoke  GetDlgItem, target_hwnd, 998
        mov     ebx, eax
        test    ebx, ebx
        jnz     @f
        invoke  GetDlgItem, target_hwnd, 999
        mov     ebx, eax
    @@:
        test    ebx, ebx
        jnz     ..found_edit
        mov     ebx, target_hwnd
        
    ..found_edit:
        if text_param shl 0 eq text_param
                ; Если передан готовый адрес (регистр или переменная буфера)
                invoke  SendMessageA, ebx, 0x000C, 0, text_param
        else
                ; Если передана обычная текстовая строка в кавычках
                jmp     ..skip
                  ..txt db text_param, 0
                ..skip:
                push    edi esi ecx
                invoke  MultiByteToWideChar, 65001, 0, ..txt, -1, out_buffer, 512
                invoke  WideCharToMultiByte, 0, 0, out_buffer, -1, buffer, 512, 0, 0
                pop     ecx edi esi
                invoke  SendMessageA, ebx, 0x000C, 0, buffer
        end if
        pop     ebx
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

macro EnableWindowDrag main_hwnd_var {
        invoke  ReleaseCapture
        invoke  SendMessageA, main_hwnd_var, 0x00A1, 2, 0 
}

macro CloseApplication main_hwnd {
        invoke  SendMessageA, main_hwnd, 0x0010, 0, 0
}

macro RemoveWindowToolbar hwnd_var, state {
        if state eq 1
                invoke  GetWindowLongA, hwnd_var, -16
                and     eax, not (0x00C00000 or 0x00080000)
                invoke  SetWindowLongA, hwnd_var, -16, eax
                invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
        else
                invoke  GetWindowLongA, hwnd_var, -16
                or      eax, (0x00C00000 or 0x00080000)
                invoke  SetWindowLongA, hwnd_var, -16, eax
                invoke  SetWindowPos, hwnd_var, 0, 0, 0, 0, 0, 0x0027
        end if
}

macro RedrawWindow hwnd_var {
        invoke  InvalidateRect, hwnd_var, 0, 1
}

; === МАКРОС SetFontSize ===
macro SetFontSize target_hwnd, size_px {
        local ..found_edit, ..end_m
        push    ebx
        
        invoke  GetDlgItem, target_hwnd, 998
        mov     ebx, eax
        test    ebx, ebx
        jnz     ..found_edit
        invoke  GetDlgItem, target_hwnd, 999
        mov     ebx, eax
    @@:
        test    ebx, ebx
        jnz     ..found_edit
        mov     ebx, target_hwnd
        
    ..found_edit:
        invoke  CreateFontA, size_px, 0, 0, 0, 400, 0, 0, 0, 1, 0, 0, 5, 0, _font_name
        invoke  SendMessageA, ebx, 0x0030, eax, 1
        pop     ebx
}

; ЖЕЛЕЗОБЕТОННЫЙ МАКРОС SetIcon ПО ЧИСЛОВОМУ ИДЕНТИФИКАТОРУ РЕСУРСА
macro SetIcon hwnd_var, icon_path {
        local ..no_icon
        push    ebx
        
        ; Загружаем иконку из ресурсов EXE по жесткому числовому ID = 1 (как прописано в таблицах)
        invoke  GetModuleHandle, 0
        invoke  LoadImageA, eax, 1, 1, 32, 32, 0
        mov     ebx, eax
        test    ebx, ebx
        jnz     ..set_it
        
        ; Подстраховка на диске
        local ..file_path, ..skip_file
        jmp     ..skip_file
          ..file_path db icon_path, 0
        ..skip_file:
        invoke  LoadImageA, 0, ..file_path, 1, 32, 32, 0x0010
        mov     ebx, eax
        test    ebx, ebx
        jz      ..no_icon
        
    ..set_it:
        invoke  SendMessageA, hwnd_var, 0x0080, 1, ebx
        invoke  SendMessageA, hwnd_var, 0x0080, 0, ebx
        
    ..no_icon:
        pop     ebx
}

macro Control_DockFill target_ctrl, lParam_value {
        push    ebx
        mov     eax, lParam_value
        mov     edx, eax
        and     eax, 0xFFFF       
        shr     edx, 16           
        invoke  SetWindowPos, [target_ctrl], 0, 0, 0, eax, edx, 2 or 4
        pop     ebx
}

macro App_GetLaunchArgument target_buffer {
        stdcall UIX_ParseCommandLineToBuffer, target_buffer
}

macro Image_LoadFromFile target_view_var, lpFilePath {
        stdcall UIX_ImageLoadFromFile, target_view_var, lpFilePath
}
