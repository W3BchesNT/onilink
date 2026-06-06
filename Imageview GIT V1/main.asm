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
  hwnd_imageview    dd ?                  ; Хэндл нашего ImageView
  cmd_file_buffer   db 512 dup(0)         ; Буфер для пути к открываемой картинке

  define_framework_data

section '.code' code readable executable

start:
        ; Включаем поддержку двойных кликов (CS_DBLCLKS = 8) для класса картинок в памяти
        or      dword [wc_img+4], 8

        ; Регистрируем основные оконные классы для окон
        register_dynamic_window login_window
        register_dynamic_window chat_window

        ; Инициализирует Fluent UI фреймворк и создает элементы на экране (окно и ImageView)
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

  ; Используем стандартный макрос библиотеки resource.inc для построения дерева ресурсов
  directory RT_ICON, icons,\
            RT_GROUP_ICON, group_icons

  resource icons,\
           1, LANG_NEUTRAL, icon_data

  resource group_icons,\
           1, LANG_NEUTRAL, main_icon

  ; Макрос автоматически распарсит 256x256 .ico файл
  icon main_icon, icon_data, 'ico/main.ico'
