pick( MSG_FATAL,		"fatal: " ,
				"致命的: " )
pick( MSG_WARNING,		"warning: " ,
				"警告: " )
pick( MSG_AT_RECORD,		" at record " ,
				" レコードで" )
pick( MSG_NOT_OPEN_SOURCE,	"Cannot open source file: " ,
				"ソースファイルをオープン出来ません: " )
pick( MSG_BANNER,		"WATCOM Disassembler" ,
				"WATCOM 逆アセンブラ" )
pick( MSG_ASTERISK_1,		"*** " ,
				"*** " )
pick( MSG_ASTERISK_2,		" ***" ,
				" ***" )
pick( MSG_MODULE,		"Module: " ,
				"モジュール: " )
pick( MSG_START_ADDR,		"Starting Address: " ,
				"先頭アドレス: " )
pick( MSG_GROUP,		"Group: " ,
				"グループ: " )
pick( MSG_SEGMENT,		"Segment: " ,
				"セグメント: " )
pick( MSG_BYTES,		"%s bytes  " ,
				"%s バイト " )
pick( MSG_LOCATION,		"Location:" ,
				"位置: " )
pick( MSG_ERROR,		"ERROR: " ,
				"エラー: " )
pick( MSG_NO_DISASM_ERR,	"No disassembly errors" ,
				"逆アセンブルエラーはありません" )
pick( MSG_DISASM_ERR,		"%s disassembly error(s)" ,
				"%s 個の逆アセンブルエラーがあります" )
pick( MSG_ABOVE_LN_REPEAT,	"       --- Above line repeats %s time(s) --- " ,
				"       --- 上の行が %s 回繰り返されています --- " )
pick( MSG_LCL_COMM_WONT_ASM,	"Local COMM symbols can not be assembled." ,
				"ローカル COMM シンボルがアセンブル出来ません." )
pick( MSG_ORG_MAY_NOT_ASSEM,	"Label '%s' emitted using ORG. File may not assemble." ,
				"ラベル '%s' にORGを指定しています. ファイルはアセンブルされません." )
pick( MSG_LIST_PUBLIC_SYM,	"List of public symbols" ,
				"PUBLICシンボル一覧表" )
pick( MSG_SYMBOL_PLIST,		"SYMBOL" ,
				"シンボル" )
pick( MSG_GROUP_PLIST,		"GROUP" ,
				"グループ" )
pick( MSG_SEGMENT_PLIST,	"SEGMENT" ,
				"セグメント" )
pick( MSG_ADDRESS_PLIST,	"ADDRESS" ,
				"アドレス" )
pick( MSG_LIST_EXTERN_SYM,	"List of external symbols" ,
				"EXTERNALシンボル一覧表" )
pick( MSG_SYMBOL_ELIST,	"Symbol" ,
				"シンボル" )
pick( MSG_COMDAT_WONT_ASM,	"COMDAT symbols can not be assembled." ,
				"COMDAT シンボルをアセンブルできません." )
pick( MSG_COMDAT, 		"Comdat: " ,
				"Comdat: " )
pick( ERR_INV_CMMD_OPTION,	"Invalid command line option" ,
				"無効なコマンドラインオプションです" )
pick( ERR_NO_OBJ_SPEC,		"No object file specified" ,
				"オブジェクトファイルが指定されていません" )
pick( ERR_NOT_OPEN_OBJ,		"Cannot open object file: " ,
				"オブジェクトファイルをオープン出来ません" )
pick( ERR_NOT_OPEN_LIST,	"Cannot open listing file: " ,
				"リストファイルをオープンできません: " )
pick( ERR_UNIMPLE_GROUP,	"Unimplemented group type in GRPDEF at record %s" ,
				"%sレコードのGRPDEFのグループ型が条件外です" )
pick( ERR_INV_FIXUP_LOC,	"Invalid fixup LOC at record %s" ,
				"%sレコードの LOC フィックスアップが無効です" )
pick( ERR_TOO_MUCH_LEDATA,	"Too much data in LEDATA at record %s" ,
				"%sレコードのLEDATAのデータが多すぎます" )
pick( ERR_TOO_MUCH_LIDATA,	"Too much data in LIDATA at record %s" ,
				"%sレコードのLIDATAのデータが多すぎます" )
pick( ERR_REC_TOO_LONG,		"Record too long at record %s" ,
				"%sレコードが長すぎます" )
pick( ERR_THEADR_NOT_1ST,	"THEADR not first record: Non-intel std. at record %s" ,
				"THEADR は最初のレコードではありません: %sレコードは非インテル標準です" )
pick( ERR_UNEXPECTED_EOF,	"Unexpected end of file on input at record %s" ,
				"入力ファイルの %s レコードが途中で終わっています" )
pick( ERR_UNKNOWN_REC,		"Unknown Record type %1 at record %2" ,
				"識別不可能なレコード型 %1 がレコード %2 にあります" )
pick( ERR_NAME_LONG,		"Name too long at record %s" ,
				"%sレコードの名前が長すぎます" )
pick( ERR_UNDEF_FORWARD,	"Undefined forward reference at record %s" ,
				"%sレコードに未定義の前方参照があります" )
pick( ERR_WRITE_FILE,		"Error writing file: " ,
				"ファイル書き込みエラー: " )
pick( ERR_OUT_OF_MEM,		"Out of Memory" ,
				"メモリ不足です" )
pick( ERR_EOF_ENCOUNTERED,	"EOF encountered in source file" ,
				"ソースファイルでEOFを検出しました" )
pick( ERR_ORL_INV_MACHINE_TYPE,	"Object file is not for Intel 386 machine type" ,
				"オブジェクトファイルがIntel 386用ではありません" )
pick( ERR_ORL_INV_BYTE_ORDER,	"Object file is not in little-endian byte format" ,
				"オブジェクトファイルがリトル・エンディアン・バイト形式ではありません" )
pick( ERR_ORL_FILE_SCAN,	"Error scanning object file" ,
				"オブジェクトファイルのスキャン・エラー" )
pick( ERR_ORL_SYM_SEC_SCAN,	"Error processing symbol table" ,
				"シンボルテーブル処理のエラー" )
pick( ERR_ORL_RELOC_SEC_SCAN,	"Error processing relocation entries" ,
				"リロケーション・エントリ処理のエラー" )
