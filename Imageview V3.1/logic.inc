include 'lib/uix.inc'

if ~ defined GdipSetInterpolationMode
    macro GdipSetInterpolationMode graphics, interpolationMode {
        invoke GdipSetInterpolationMode, graphics, interpolationMode
    }
end if

ControlHandler CustomImageViewProc
        OnMessage 0x000F,         .img_paint
        OnMessage 0x0014,         .img_erasebkgnd
        OnMessage 0x0002,         .img_destroy
        OnMessage 0x0020,         .img_setcursor
        OnMessage 0x0206,         .img_rbutton_dblclk
        OnMessage 0x0113,         .img_timer          
        jmp     .def_proc

.img_erasebkgnd:
        ReturnEvent 1

.img_timer:
        invoke  KillTimer, [hwnd], 1
        mov     [fast_is_resizing], 0
        invoke  InvalidateRect, [hwnd], NULL, FALSE
        ReturnEvent 0

.img_paint:
        push    ebx esi edi
        
        invoke  GetWindowLongA, [hwnd], 0
        mov     edi, eax                                
        test    edi, edi
        jz      .draw_empty_only

        push    ebp
        mov     ebp, esp
        sub     esp, 32

        sub     esp, 64
        mov     ebx, esp
        invoke  BeginPaint, [hwnd], ebx
        mov     esi, eax                             
        
        invoke  GetClientRect, [hwnd], rect_struct

        mov     eax, [rect_struct+8]
        sub     eax, [rect_struct+0]
        mov     [ebp-28], eax

        mov     edx, [rect_struct+12]
        sub     edx, [rect_struct+4]
        mov     [ebp-32], edx

        lea     eax, [ebp-4]
        invoke  GdipGetImageWidth, edi, eax
        lea     eax, [ebp-8]
        invoke  GdipGetImageHeight, edi, eax

        cmp     dword [ebp-4], 0
        jnz     .math_prep
        mov     dword [ebp-4], 4608
        mov     dword [ebp-8], 3450

    .math_prep:
        mov     eax, [ebp-28]
        imul    eax, [ebp-8]                            
        mov     edx, [ebp-32]
        imul    edx, [ebp-4]                            

        cmp     eax, edx
        jge     .fit_to_height

    .fit_to_width:
        mov     eax, [ebp-28]
        mov     [ebp-12], eax
        mul     dword [ebp-8]
        div     dword [ebp-4]
        mov     [ebp-16], eax
        mov     dword [ebp-20], 0
        
        mov     eax, [ebp-32]
        sub     eax, [ebp-16]
        sar     eax, 1
        mov     [ebp-24], eax
        jmp     .picasa_pipeline

    .fit_to_height:
        mov     eax, [ebp-32]
        mov     [ebp-16], eax
        mul     dword [ebp-4]
        div     dword [ebp-8]
        mov     [ebp-12], eax
        mov     dword [ebp-24], 0
        
        mov     eax, [ebp-28]
        sub     eax, [ebp-12]
        sar     eax, 1
        mov     [ebp-20], eax

.picasa_pipeline:
        cmp     [fast_hdcMem], 0
        jnz     .render_from_cache

        invoke  CreateCompatibleDC, esi
        mov     [fast_hdcMem], eax

        mov     [fast_bmih.biSize], sizeof.BITMAPINFOHEADER
        mov     [fast_bmih.biWidth], 2560               
        mov     [fast_bmih.biHeight], 1600              
        mov     [fast_bmih.biPlanes], 1
        mov     [fast_bmih.biBitCount], 32
        mov     [fast_bmih.biCompression], 0

        lea     eax, [fast_bits]
        lea     ecx, [fast_bmih]
        invoke  CreateDIBSection, [fast_hdcMem], ecx, 0, eax, 0, 0
        mov     [fast_hbmMem], eax

        invoke  SelectObject, [fast_hdcMem], [fast_hbmMem]
        mov     [fast_hbmOld], eax

        lea     eax, [fast_pGraph]
        invoke  GdipCreateFromHDC, [fast_hdcMem], eax
        mov     ebx, [fast_pGraph]
        invoke  GdipSetInterpolationMode, ebx, 7
        invoke  GdipDrawImageRectI, ebx, edi, 0, 0, 2560, 1600
        invoke  GdipDeleteGraphics, ebx

        mov     [fast_last_w], 2560
        mov     [fast_last_h], 1600

    .render_from_cache:
        invoke  CreateCompatibleDC, esi
        mov     ebx, eax                                

        invoke  CreateCompatibleBitmap, esi, [ebp-28], [ebp-32]
        mov     edx, eax
        invoke  SelectObject, ebx, edx
        push    edx                                     

        invoke  CreateSolidBrush, 0x1E1E1E
        push    eax
        invoke  FillRect, ebx, rect_struct, eax
        pop     eax
        invoke  DeleteObject, eax

        mov     eax, [fast_is_resizing]
        test    eax, eax
        jz      .high_quality

        invoke  SetStretchBltMode, ebx, 3
        jmp     .blit_cache

    .high_quality:
        invoke  SetStretchBltMode, ebx, 4
        invoke  SetBrushOrgEx, ebx, 0, 0, NULL

    .blit_cache:
        invoke  StretchBlt, ebx, [ebp-20], [ebp-24], [ebp-12], [ebp-16], \
                            [fast_hdcMem], 0, 0, [fast_last_w], [fast_last_h], SRCCOPY

        invoke  BitBlt, esi, 0, 0, [ebp-28], [ebp-32], ebx, 0, 0, 0x00CC0020

        pop     edx
        invoke  DeleteObject, edx
        invoke  DeleteDC, ebx

.paint_done:
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

.img_destroy:
        invoke  KillTimer, [hwnd], 1
        cmp     [fast_hdcMem], 0
        jz      .skip_gdi_clean
        invoke  SelectObject, [fast_hdcMem], [fast_hbmOld]
        invoke  DeleteObject, [fast_hbmMem]
        invoke  DeleteDC, [fast_hdcMem]
        mov     [fast_hdcMem], 0
    .skip_gdi_clean:
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

WindowHandler WindowProc
        DefaultWindowCore
        OnMessage 0x0005,         .on_size
        jmp     .def_proc

.on_size:
        Control_DockFill hwnd_imageview, [lparam]
        mov     [fast_is_resizing], 1
        invoke  SetTimer, hwnd_imageview, 1, 100, NULL  
        invoke  InvalidateRect, [hwnd], NULL, FALSE
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
        
        cmp     [fast_hdcMem], 0
        jz      .load_file
        invoke  SelectObject, [fast_hdcMem], [fast_hbmOld]
        invoke  DeleteObject, [fast_hbmMem]
        invoke  DeleteDC, [fast_hdcMem]
        mov     [fast_hdcMem], 0

    .load_file:
        Image_LoadFromFile hwnd_imageview, cmd_file_buffer
    .exit:
        pop     edi esi ebx
        ret
endp
