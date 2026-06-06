include 'lib/uix.inc'

ControlHandler CustomImageViewProc
        OnMessage 0x000F,         .img_paint
        OnMessage 0x0014,         .img_erasebkgnd
        OnMessage 0x0002,         .img_destroy
        OnMessage 0x0020,         .img_setcursor
        OnMessage 0x0206,         .img_rbutton_dblclk
        jmp     .def_proc

.img_paint:
        push    ebx esi edi
        
        invoke  GetWindowLongA, [hwnd], 0
        mov     edi, eax                                
        test    edi, edi
        jz      .draw_empty_only

        push    ebp
        mov     ebp, esp
        sub     esp, 24

        sub     esp, 64
        mov     ebx, esp
        invoke  BeginPaint, [hwnd], ebx
        mov     esi, eax                                
        
        invoke  GetClientRect, [hwnd], rect_struct

        lea     eax, [ebp-4]
        invoke  GdipGetImageWidth, edi, eax
        lea     eax, [ebp-8]
        invoke  GdipGetImageHeight, edi, eax

        cmp     dword [ebp-4], 0
        jnz     .check_h
        mov     dword [ebp-4], 4896
    .check_h:
        cmp     dword [ebp-8], 0
        jnz     .math_start
        mov     dword [ebp-8], 3264

    .math_start:
        cmp     dword [ebp-4], 2000
        jle     .skip_downscale
        shr     dword [ebp-4], 2                        
        shr     dword [ebp-8], 2                        
    .skip_downscale:

        mov     eax, [rect_struct+8]                    
        imul    eax, [ebp-8]                            
        mov     edx, [rect_struct+12]                   
        imul    edx, [ebp-4]                            

        cmp     eax, edx
        jge     .fit_to_height

    .fit_to_width:
        mov     eax, [rect_struct+8]
        mov     [ebp-12], eax                           
        mov     eax, [rect_struct+8]
        mul     dword [ebp-8]
        div     dword [ebp-4]
        mov     [ebp-16], eax                           
        mov     dword [ebp-20], 0                       
        mov     eax, [rect_struct+12]
        sub     eax, [ebp-16]
        sar     eax, 1
        mov     [ebp-24], eax                           
        jmp     .prepare_graphics

    .fit_to_height:
        mov     eax, [rect_struct+12]
        mov     [ebp-16], eax                           
        mov     eax, [rect_struct+12]
        mul     dword [ebp-4]
        div     dword [ebp-8]
        mov     [ebp-12], eax                           
        mov     dword [ebp-24], 0                       
        mov     eax, [rect_struct+8]
        sub     eax, [ebp-12]
        sar     eax, 1
        mov     [ebp-20], eax                           

.prepare_graphics:
        sub     esp, 4
        mov     edx, esp
        invoke  GdipCreateFromHDC, esi, edx
        pop     eax                                     
        mov     ebx, eax

        invoke  CreateSolidBrush, 0x1E1E1E
        push    eax
        invoke  FillRect, esi, rect_struct, eax
        pop     eax
        invoke  DeleteObject, eax                       

        invoke  GdipDrawImageRectI, ebx, edi, [ebp-20], [ebp-24], [ebp-12], [ebp-16]
        invoke  GdipDeleteGraphics, ebx                 

.no_image_cleanup:
        mov     ebx, esp
        invoke  EndPaint, [hwnd], ebx
        add     esp, 64

        mov     esp, ebp
        pop     ebp
        pop     edi esi ebx
        ReturnEvent 0

    .draw_empty_only:
        sub     esp, 64
        mov     ebx, esp
        invoke  BeginPaint, [hwnd], ebx
        mov     esi, eax
        invoke  GetClientRect, [hwnd], rect_struct
        invoke  CreateSolidBrush, 0x1E1E1E
        push    eax
        invoke  FillRect, esi, rect_struct, eax
        pop     eax
        invoke  DeleteObject, eax
        mov     ebx, esp
        invoke  EndPaint, [hwnd], ebx
        add     esp, 64
        pop     edi esi ebx
        ReturnEvent 0

.img_erasebkgnd:
        ReturnEvent 1

.img_destroy:
        invoke  GetWindowLongA, [hwnd], 0
        test    eax, eax
        jz      .skip_dispose
        invoke  GdipDisposeImage, eax
    .skip_dispose:
        ReturnEvent 0

.img_setcursor:
        SetStandardCursor 32512 

.img_rbutton_dblclk:
        invoke  ReleaseCapture                  
        invoke  GetParent, [hwnd]
        invoke  SendMessageA, eax, 0x0010, 0, 0 
        ReturnEvent 0

EndHandler

; ==============================================================================
; ХЭНДЛЕР ГЛАВНОГО ОКНА
; ==============================================================================
WindowHandler WindowProc
        DefaultWindowCore
        OnMessage 0x0005,         .on_size
        jmp     .def_proc

.on_size:
        Control_DockFill hwnd_imageview, [lparam]
        ReturnEvent 0

EndHandler

WindowHandler SubWindowProc
        OnMessage WM_CLOSE, .on_sub_close
        jmp     .def_proc
.on_sub_close:
        invoke  ShowWindow, [hwnd], 0
        ReturnEvent 0
EndHandler

proc ProcessCommandLine
        push    ebx esi edi
        App_GetLaunchArgument cmd_file_buffer
        mov     al, [cmd_file_buffer]
        test    al, al
        jz      .exit
        Image_LoadFromFile hwnd_imageview, cmd_file_buffer
    .exit:
        pop     edi esi ebx
        ret
endp
