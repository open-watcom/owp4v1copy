.*
.section  Predefined Spy Message Classes
.*
.np
The sections that follow list the messages contained in each message class
that &spy defines for you.
.np
Messages followed by (*) are undocumented messages.
.br
Messages followed by (**) are available under Win32 only.
.*
.beglevel
.*
.section Clipboard Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_ASKCBFORMATNAME $WM_CHANGECBCHAIN
WM_CLEAR           $WM_COPY
WM_CUT             $WM_DESTROYCLIPBOARD
WM_DRAWCLIPBOARD   $WM_HSCROLLCLIPBOARD
WM_PAINTCLIPBOARD  $WM_PASTE
WM_RENDERALLFORMATS$WM_RENDERFORMAT
WM_SIZECLIPBOARD   $WM_UNDO
WM_VSCROLLCLIPBOARD
.millust end
.*
.section DDE Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_DDE_ACK         $WM_DDE_ADVISE
WM_DDE_DATA        $WM_DDE_EXECUTE
WM_DDE_INITIATE    $WM_DDE_POKE
WM_DDE_REQUEST     $WM_DDE_TERMINATE
WM_DDE_UNADVISE
.millust end
.*
.section Init Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_INITDIALOG      $WM_INITMENU
WM_INITMENUPOPUP
.millust end
.*
.section Input Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_BEGINDRAG (*)      $WM_CHAR
WM_CHARTOITEM         $WM_COMMAND
WM_COPYDATA (**)      $WM_DEADCHAR
WM_DRAGLOOP (*)       $WM_DRAGMOVE (*)
WM_DRAGSELECT (*)     $WM_DROPOBJECT (*)
WM_EXITSIZEMEOVE (*)  $WM_HSCROLL
WM_KEYDOWN            $WM_KEYUP
WM_LBTRACKPOINT (*)   $WM_MENUCHAR
WM_MENUSELECT         $WM_NEXTMENU (*)
WM_QUERYDROPOBJECT (*)$WM_VSCROLL
.millust end
.*
.section MDI Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_ISACTIVEICON (*)$WM_MDIACTIVATE
WM_MDICASCADE      $WM_MDICREATE
WM_MDIDESTROY      $WM_MDIGETACTIVE
WM_MDIICONARRANGE  $WM_MDIMAXIMIZE
WM_MDINEXT         $WM_MDIRESTORE
WM_MDISETMENU      $WM_MDITILE
.millust end
.*
.section Mouse Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_LBUTTONDBLCLK   $WM_LBUTTONDOWN
WM_LBUTTONUP       $WM_MBUTTONDBLCLK
WM_MBUTTONDOWN     $WM_MBUTTONUP
WM_MOUSEACTIVATE   $WM_MOUSEMOVE
WM_RBUTTONDBLCLK   $WM_RBUTTONDOWN
WM_RBUTTONUP       $WM_SETCURSOR
.millust end
.*
.section NC Misc Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_NCACTIVATE      $WM_NCCALCSIZE
WM_NCCREATE        $WM_NCDESTROY
WM_NCPAINT
.millust end
.*
.section NC Mouse Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_NCHITTEST       $WM_NCLBUTTONDBLCLK
WM_NCLBUTTONDOWN   $WM_NCLBUTTONUP
WM_NCMBUTTONDBLCLK $WM_NCMBUTTONDOWN
WM_NCMBUTTONUP     $WM_NCMOUSEMOVE
WM_NCRBUTTONDBLCLK $WM_NCRBUTTONDOWN
WM_NCRBUTTONUP
.millust end
.*
.section Other Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_COALESCE_FIRST   $WM_COALESCE_LAST
WM_COMMNOTIFY       $WM_CONVERTREQUEST (*)
WM_CONVERTRESULT (*)$WM_DROPFILES
WM_INTERIM (*)      $WM_PENWINFIRST
WM_PENWINLAST       $WM_QUERYSAVESTATE (*)
WM_QUEUESYNC        $WM_SIZEWAIT (*)
WM_SYNCTASK (*)     $WM_TESTING (*)
WM_YOMICHAR (*)
.millust end
.*
.section System Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_CANCELJOURNAL (**)$WM_COMPACTING
WM_DEVMODECHANGE     $WM_ENDSESSION
WM_ENTERIDLE         $WM_ENTERMENULOOP
WM_EXITMENULOOP      $WM_FILESYSCHANGE (*)
WM_NULL              $WM_POWER
WM_SPOOLERSTATUS     $WM_SYSCHAR
WM_SYSCOLORCHANGE    $WM_SYSCOMMAND
WM_SYSDEADCHAR       $WM_SYSKEYDOWN
WM_SYSKEYUP          $WM_SYSTEMERROR (*)
WM_SYSTIMER (*)      $WM_TIMECHANGE
WM_TIMER             $WM_WININICHANGE
.millust end
.*
.section User Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_USER
.millust end
.*
.section Unknown Messages
.*
.millust begin
.tb set $
.tb &syscl./2
None
.millust end
.*
.section Window Messages
.*
.millust begin
.tb set $
.tb &syscl./2
WM_ACTIVATE           $WM_ACTIVATEAPP
WM_ACTIVATESHELLWINDOW (*)
$WM_ALTTABACTIVE (*)
WM_CANCELMODE         $WM_CREATE
WM_CHILDACTIVATE      $WM_CLOSE
WM_COMPAREITEM        $WM_CTLCOLOR
WM_CTLCOLORBTN (**)   $WM_CTLCOLORDLG (**)
WM_CTLCOLOREDIT (**)  $WM_CTLCOLORLISTBOX (**)
WM_CTLCOLORMSGBOX (**)$WM_CTLCOLORSCROLLBAR (**)
WM_CTLCOLORSTATIC (**)$WM_DELETEITEM
WM_DESTROY            $WM_DRAWITEM
WM_ENABLE             $WM_ENTERSIZEMOVE (*)
WM_ERASEBKGND         $WM_FONTCHANGE
WM_GETDLGCODE         $WM_GETFONT
WM_GETHOTKEY          $WM_GETMINMAXINFO
WM_GETTEXT            $WM_GETTEXTLENGTH
WM_ICONERASEBKGND     $WM_KILLFOCUS
WM_MEASUREITEM        $WM_MOVE
WM_NEXTDLGCTL         $WM_OTHERWINDOWCREATED
WM_OTHERWINDOWDESTROYED
$WM_PAINT
WM_PAINTICON          $WM_PALETTECHANGED
WM_PALETTEISCHANGING  $WM_PARENTNOTIFY
WM_QUERYDRAGICON      $WM_QUERYENDSESSION
WM_QUERYNEWPALETTE    $WM_QUERYOPEN
WM_QUERYPARKICON (*)  $WM_QUIT
WM_SETFOCUS           $WM_SETFONT
WM_SETHOTKEY          $WM_SETREDRAW
WM_SETTEXT            $WM_SETVISIBLE (*)
WM_SHOWWINDOW         $WM_SIZE
WM_SYNCPAINT (*)      $WM_VKEYTOITEM
WM_WINDOWPOSCHANGED   $WM_WINDOWPOSCHANGING
.millust end
.*
.endlevel
