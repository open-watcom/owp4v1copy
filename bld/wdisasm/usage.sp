:segment ENGLISH
Usage:   wdisasm [options] obj_file [options]
:segment qnx
Options:
:elsesegment
Options:			( /option is also supported )
:endsegment
	-l[=list_file]      	generate listing file
        -s[=source_file]	include source lines
        -c=code_name        	additional code segment name
        -i=char             	initial character of internal labels
        -a[u]               	generate Intel/Unix .ASM format
        -b                  	alternate form of based modes
        -e                  	generate list of externs
        -p                  	generate list of publics
        -r                  	registers in upper case
        -u                  	instruction names in upper case
	-m			leave C++ names mangled
:segment HIDDEN
	-w		        decode Weitek macros if present
:endsegment
:elsesegment JAPANESE
使用方法: wdisasm [options] obj_file [options]
:segment qnx
オプション:
:elsesegment
オプション:			( /ｵﾌﾟｼｮﾝ でも指定可能)
:endsegment
	-l[=list_file]		リストファイルの生成
	-s[=source_file]	ソース行の挿入
	-c=code_name		コードセグメント名の追加
	-i=char			内部ラベル名で使用するイニシャル文字の指定
	-a[u]			Intel/Unix の.ASM フォーマットでの生成
	-b			ベース・モードの代替フォームでの生成
	-e			エクスターナル・リストの生成
	-p			パブリック・リストの生成
	-r			レジスタ名に大文字を使用
	-u			ニモニックに大文字を使用
	-m			プリプロセス後のC++ 名の保持
:segment HIDDEN
	-w			Weitekマクロのデコード
:endsegment
:endsegment
