        include 'graphapi.fi'

        program grdemo1

*$ifdef _NEC_98
        include 'graph98.fi'
*$else
        include 'graph.fi'
*$endif

        integer TextColor, TextColor2, BorderColor
        record /videoconfig/ VC
        common TextColor, TextColor2, BorderColor, VC

        integer width, y

        integer initscreen

        if( initscreen() .eq. 0 )then
            print *, 'No graphics adapter present'
            stop
        endif

        ! draw a border around the screen

        call _setcolor( BorderColor )
        call _rectangle( _GBORDER, 0, 0, VC.numxpixels - 1,
     +                       VC.numypixels - 1 )

        ! sweep text in from top & bottom of screen, gradually increasing
        ! character size as the center of the screen is approached.

        call _setcolor( TextColor )
        width = 0
        y = 5
        do while( y .lt. VC.numypixels / 2 - 10 )
            call drawtext( width, y )
            width = width + 1
            y = y + 5
        enddo

        ! draw text over final positions using a different color index

        call _setcolor( TextColor2 )
        call drawtext( width - 1, y - 5 )

*$ifdef _NEC_98
        call fadecolors()
*$else
        if( VC.adapter .gt. _MCGA )then
            call fadecolors()
        endif
*$endif

        ! wait for keyboard input and then reset the screen

        call _settextposition( VC.numtextrows, VC.numtextcols - 18 )
        call _outtext( 'Press Enter key...'//char(0) )
        pause
        call _setvideomode( _DEFAULTMODE )
        end


        subroutine drawtext( width, y )

*$ifdef _NEC_98
        include 'graph98.fi'
*$else
        include 'graph.fi'
*$endif

        ! This routine displays the text strings.

        integer TextColor, TextColor2, BorderColor
        record /videoconfig/ VC
        common TextColor, TextColor2, BorderColor, VC

        integer width, y, xc

        xc = VC.numxpixels / 2
        call _setcharsize( width, width * 3 / 4 )
        call _settextalign( _CENTER, _BOTTOM )
        call _grtext( xc, y, 'WATCOM FORTRAN 77'//char(0) )
        call _setcharsize( width, width )
        call _settextalign( _CENTER, _TOP )
        call _grtext( xc, VC.numypixels - y, 'GRAPHICS'//char(0) )
        end


        integer function initscreen()

*$ifdef _NEC_98
        include 'graph98.fi'
*$else
        include 'graph.fi'
*$endif

        ! This routine selects the best video mode for a given adapter.

        integer TextColor, TextColor2, BorderColor
        record /videoconfig/ VC
        common TextColor, TextColor2, BorderColor, VC

*$ifdef _NEC_98
        mode = _MAXCOLORMODE
*$else
        call _getvideoconfig( VC )
        select( VC.adapter )
        case( _VGA, _SVGA )
            mode = _VRES16COLOR
        case( _MCGA )
            mode = _MRES256COLOR
        case( _EGA )
            if( VC.monitor .eq. _MONO )then
                mode = _ERESNOCOLOR
            else
                mode = _ERESCOLOR
            endif
        case( _CGA )
            mode = _MRES4COLOR
        case( _HERCULES )
            mode = _HERCMONO
        case default
            initscreen = 0        ! report insufficient hardware
            return
        endselect
*$endif

        if( _setvideomode( mode ) .eq. 0 )then
            initscreen = 0
            return
        endif
        call _getvideoconfig( VC )
        if( VC.numcolors .lt. 4 )then
            TextColor = 1
            TextColor2 = 1
            BorderColor = 1
        else
            TextColor = 1
            TextColor2 = 3
            BorderColor = 2
        endif
*$ifdef _NEC_98
        call newcolors()
*$else
        if( VC.adapter .ge. _MCGA )then
            call newcolors()
        endif
*$endif
        initscreen = 1
        end


        subroutine newcolors()

*$ifdef _NEC_98
        include 'graph98.fi'
*$else
        include 'graph.fi'
*$endif

        ! This routine sets the default colors for the program.

        integer TextColor, TextColor2, BorderColor
        record /videoconfig/ VC
        common TextColor, TextColor2, BorderColor, VC

*$ifdef _NEC_98
        call _remappalette( TextColor, _98BLUE )      ! light blue
        call _remappalette( TextColor2, _98BLUE )     ! light blue
        call _remappalette( BorderColor, _98BLACK )   ! black
*$else
        call _remappalette( TextColor, '003f0000'x )  ! light blue
        call _remappalette( TextColor2, '003f0000'x ) ! light blue
        call _remappalette( BorderColor, _BLACK  )    ! black
*$endif
        end


        subroutine fadecolors()

*$ifdef _NEC_98
c$ifdef __386__
*$pragma aux delay "*_" parm (value)
c$else
*$pragma aux delay "*_" parm (value*2)
c$endif
        include 'graph98.fi'
*$else
        include 'graph.fi'
*$endif

        ! This routine gradually fades the background text, brightening
        ! the foreground text and the border at the same time.

        integer TextColor, TextColor2, BorderColor
        record /videoconfig/ VC
        common TextColor, TextColor2, BorderColor, VC

        integer i, red, blue, green, maxc

*$ifdef _NEC_98
        maxc = 15   ! 4 color bits
*$else
        maxc = 63   ! 6 color bits
*$endif
        do i=1, maxc
*$ifdef _NEC_98
            call delay( 150 )
*$endif
            red = i
            green = ishl( i, 8 )
            blue = ishl( maxc - i, 16 )
            call _remappalette( TextColor, blue )
            call _remappalette( TextColor2, blue + green )
            call _remappalette( BorderColor, red )
        enddo
        end
