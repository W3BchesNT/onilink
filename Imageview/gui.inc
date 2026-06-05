macro create_gui_elements {
        init_fluent_ui
        
        CreateAppWindow login_window, "Picasso Mini Viewer", 800, 600, 0x1E1E1E
                SetIcon [hwnd_login_window], "ico\main.ico"
                DynamicWindowsToolbar [hwnd_login_window], 1 
                
                ; Используем ваш родной синтаксис для динамического просмотра картинок!
                fluent_dynamic_imageview 0, 0, 800, 600, 501, hwnd_imageview
                
        EndWindow

        call    ProcessCommandLine
        invoke  ShowWindow, [hwnd_login_window], 1
}
