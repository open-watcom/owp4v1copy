.*
.*
.helppref NetWare:
:set symbol="unique_lbl" value="net".
:set symbol="opsys" value="NetWare".
:set symbol="sysprompt" value="".
:set symbol="exeformat" value="nov".
:set symbol="exefmtup" value="NOV".
:set symbol="form_drctv" value="form nov 'test'".
:set symbol="syst_drctv" value="form netware 'test'".
:set symbol="exeextn" value="nlm".
:set symbol="libvarname" value="lib".
:set symbol="tmpvarname" value="tmp".
:set symbol="currdir" value="default".
.*
.im dosfiles
.*
.chap *refid=novchap The NetWare O/S Executable File Format
.*
.im wlintro
.im wlsyntax
.np
where
.bi directive
is any of the following:
.begnote $compact
.note ALIAS alias_name=symbol_name{,alias_name=symbol_name}
.note AUTOUNLOAD
.note DEBUG dbtype [dblist] | DEBUG [dblist]
.note DISABLE msg_num{,msg_num}
.note ENDLINK
.note EXPORT entry_name {,entry_name}
.note FILE obj_spec{,obj_spec}
.note FORMAT NOVELL [NLM | LAN | DSK | NAM | 0-9] 'description'
.note IMPORT external_name {,external_name}
.note LANGUAGE lang
.note LIBFILE obj_file{,obj_file}
.cw !
.note LIBPATH path_name{&ps.path_name}
.cw ;
.note LIBRARY library_file{,library_file}
:cmt. .note MODFILE obj_file{,obj_file}
.note MODTRACE obj_module{,obj_module}
.note MODULE module_name {,module_name}
.note NAME exe_file
.note OPTION option{,option}
:cmt. where option is one of
.begnote $compact
.note ARTIFICIAL
.note [NO]CACHE
.note [NO]CASEEXACT
.note CHECK=symbol_name
.note COPYRIGHT 'string'
.note CUSTOM=file_name
.note CVPACK
.note DOSSEG
.note ELIMINATE
.note EXIT=symbol_name
.note HELP=help_file
.note IMPFILE[=imp_file]
.note IMPLIB[=imp_lib]
:cmt. crashes linker .note INCREMENTAL
.note MANGLEDNAMES
.note MAP[=map_file]
.note MAXERRORS=n
.note MESSAGES=msg_file
.note MULTILOAD
.note NAMELEN=n
.note NLMFLAGS=some_value
.note NODEFAULTLIBS
.note OSDOMAIN
.note OSNAME='string'
.note PSEUDOPREEMPTION
.note QUIET
.note REDEFSOK
.note SHOWDEAD
.note REENTRANT
.note SCREENNAME 'name'
.note SHARELIB=shared_nlm
.note STACK=n
.note START=symbol_name
.note STATICS
.note SYMFILE[=symbol_file]
.note SYNCHRONIZE
.note THREADNAME 'thread_name'
.note [NO]UNDEFSOK
.note VERBOSE
.note VERSION=major[.minor[.revision]]
.note VFREMOVAL
.note XDCDATA=rpc_file
.endnote
.note OPTLIB library_file{,library_file}
.cw !
.note PATH path_name{&ps.path_name}
.cw ;
.note REFERENCE symbol_name{,symbol_name}
.note SORT [GLOBAL] [ALPHABETICAL]
.note STARTLINK
.note SYMTRACE symbol_name{,symbol_name}
.note SYSTEM BEGIN system_name {directive} END
.note SYSTEM system_name
.note # comment
.note @ directive_file
.endnote
:CMT. .synote
:CMT. .mnote alias_name
:CMT. is an alias_name.
:CMT. .mnote comment
:CMT. is any sequence of characters.
:CMT. .mnote description
:CMT. is any sequence of characters.
:CMT. .mnote directive_file
:CMT. is a file specification for the name of a linker directive file.
:CMT. If no file extension is specified, a file extension of "lnk" is assumed.
:CMT. .mnote exe_file
:CMT. is a file specification for the name of the executable file.
:CMT. If no file extension is specified, a file extension of "nlm", "dsk",
:CMT. "lan", "nam", "cdm", "msl" or "ham" is assumed depending on the 
:CMT. executable file format selected.
:CMT. .mnote help_file
:CMT. is a file specification for the name of an internationalized help
:CMT. file.
:CMT. .im libfile
:CMT. .mnote map_file
:CMT. is a file specification for the name of the map file.
:CMT. If no file extension is specified, a file extension of "map" is assumed.
:CMT. .mnote msg_file
:CMT. is a file specification for the name of an internationalized message
:CMT. file.
:CMT. .mnote msg_num
:CMT. is a message number.
:CMT. .im lnkvalue
:CMT. .mnote name
:CMT. is any sequence of characters.
:CMT. .im objfile
:CMT. .im objmod
:CMT. .mnote path_name
:CMT. is a path name.
:CMT. .mnote rpc_file
:CMT. is a file specification for the name of a file containing RPC descriptions.
:CMT. file.
:CMT. .mnote shared_nlm
:CMT. is a file specification for the name of an NLM to be loaded as a
:CMT. shared NLM.
:CMT. .mnote symbol_file
:CMT. is a file specification for the name of the symbol file.
:CMT. If no file extension is specified, a file extension of "sym" is assumed.
:CMT. .mnote symbol_name
:CMT. is a symbol name.
:CMT. .mnote system_name
:CMT. is the name of a system.
:CMT. .esynote
.*
.im wlprompt
.*
.section NetWare Loadable Modules
.*
.np
NetWare Loadable Modules (NLMs) are executable files that run in
file server memory under the NetWare operating system.
NLMs can be loaded and unloaded from file server memory while the
server is running.
When running they actually become part of the operating system thus
acting as building blocks for a server environment tailored to your needs.
.np
There are multiple types of NLMs, each identified by the file extension 
of the executable file and the internal module type number.
.begbull
.bull
Utility and server applications (executable files with extension "nlm").
.bull
LAN drivers (executable files with extension "lan").
.bull
Disk drivers (executable files with extension "dsk").
.bull
Modules that define file system name spaces
(executable files with extension "nam").
.bull
Custom Device modules (executable images with extension "cdm").
.bull
Host Adapter modules (executable images with extension "ham").
.bull
Mirrored server link modules (executable images with extension "msl").
.endbull
.pc
The &lnkname can generate all four types of NLMs.
.*
.im wlmemlay
.im wlmem
.*
.helppref
