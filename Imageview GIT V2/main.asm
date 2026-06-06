format PE GUI 4.0
entry start

include 'win32a.inc'         
include 'macro/proc32.inc'   
include 'imports.inc'
include 'macro/resource.inc'

include 'lib/uix.inc'  
include 'lib/wincore.inc'     
include 'lib/onilink.inc'     
include 'lib/fluent.inc'     

include 'lib/picaso.inc'

include 'gui.inc'            
include 'logic.inc'          

ImageViewProc = CustomImageViewProc

section '.data' data readable writeable
  _vs_b_class   db 'W10_BUTTON',0 
  _font_name    db 'Segoe UI',0

  window_class login_window, WindowProc, "LOGIN_CLASS"
  window_class chat_window, SubWindowProc, "CHAT_CLASS"

  ; Глобальные переменные для просмотрщика картинок
  hwnd_imageview    dd ?                  
  cmd_file_buffer   db 512 dup(0)         

  ; Конвейер адаптивного рендеринга (Картинка видна + 60 FPS + Защита от вылетов)
  fast_hdcMem       dd 0
  fast_hbmMem       dd 0
  fast_hbmOld       dd 0
  fast_bits         dd 0
  fast_pGraph       dd 0
  fast_is_resizing  dd 0
  fast_last_w       dd 100
  fast_last_h       dd 100
  fast_bmih         BITMAPINFOHEADER

  define_framework_data

section '.code' code readable executable

start:
        or      dword [wc_img+4], 8

        register_dynamic_window login_window
        register_dynamic_window chat_window

        create_gui_elements 

msg_loop:
        invoke  GetMessageA, msg, 0, 0, 0
        or      eax, eax
        jz      end_loop
        invoke  TranslateMessage, msg
        invoke  DispatchMessageA, msg
        jmp     msg_loop

end_loop:
        clean_fluent_ui
        invoke  ExitProcess, [msg.wParam]

; ==============================================================================
; СЕКЦИЯ РЕСУРСОВ
; ==============================================================================
section '.rsrc' resource data readable

  directory RT_ICON, icons,\
            RT_GROUP_ICON, group_icons

  resource icons,\
           1, LANG_NEUTRAL, icon_data

  resource group_icons,\
           1, LANG_NEUTRAL, main_icon

  icon main_icon, icon_data, 'ico/main.ico'
