/* messages for WGML
 *  id, english text, japanese??? text
 *  id  in uppercase please
 *      for the enum definition these will be converted to lower case
 *      for the defines and WRC use used as is
 *
 *      japanese is provided as the templates use it
 */
pick( ERR_INTERN,               "Internal Error",
                                "Internal Error" )
pick( ERR_STR_NOT_FOUND,        "Resourcestring not found",
                                "Resourcestring not found" )
pick( ERR_FILE_IO,              "FileIO %s %s",
                                "FileIO %s %s" )
pick( ERR_NO_MEMORY,            "Out of memory!",
                                "Out of memory!" )
pick( ERR_NO_HANDLES,           "Out of file handles!",
                                "Out of file handles!" )
pick( ERR_LOGIC_MAC,            "Logic error: get_macroline for file",
                                "Logic error: get_macroline for file" )
pick( ERR_INF_REOPEN,           "Calling reopen_inc_fp",
                                "Calling reopen_inc_fp" )
pick( ERR_INF_LINE_FILE,        "\tIncluded from line %s of file '%s'",
                                "\tIncluded from line %s of file '%s'" )
pick( ERR_INF_TAG,              "\tCalled from GML tag '%s' in",
                                "\tCalled from GML tag '%s' in" )
pick( ERR_INF_MAC_DEF,          "\tLine %s of macro '%s' defined at line %s of file '%s'",
                                "\tLine %s of macro '%s' defined at line %s of file '%s'" )
pick( ERR_INC_UNKNOWN,          "\tERR Included from unknown ( master document? )",
                                "\tERR Included from unknown ( master document? )" )
pick( WNG_FILEATTR_IGNORED,     "File attribute ignored (%s) %s",
                                "File attribute ignored (%s) %s" )
pick( ERR_MAX_INPUT_NESTING,    "Max input nesting exceeded %s",
                                "Max input nesting exceeded %s" )
pick( ERR_INPUT_FILE_NOT_FOUND, "Input file not found %s",
                                "Input file not found %s" )
pick( INF_CURR_INPUT,           "Current %s is '%s'",
                                "Current %s is '%s'" )
pick( INF_CURR_LINE,            "Current file is '%s'(%s)",
                                "Current file is '%s'(%s)" )
pick( ERR_GOTO,                 "Goto label not found %s Line %s of macro '%s'",
                                "Goto label not found %s Line %s of macro '%s'" )
pick( ERR_MISSING_MAINFILENAME, "Missing main file name",
                                "Missing main file name" )
pick( ERR_IF_LEVEL,             "If nesting level bot zero at EOF %s",
                                "If nesting level bot zero at EOF %s" )
pick( INF_INDENT_REM,           "%s<< Indent removed",
                                "%s<< Indent removed" )
pick( INF_INCLUDED,             "\tIncluded from %s",
                                "\tIncluded from %s" )
pick( INF_STAT_1,               "Statistics:",
                                "Statistics:" )
pick( INF_STAT_2,               "Max inc level: %6s",
                                "Max inc level: %6s" )
pick( INF_STAT_3,               "  Error count: %6s",
                                "  Error count: %6s" )
pick( INF_STAT_4,               "Warning count: %6s",
                                "Warning count: %6s" )
pick( INF_STAT_5,               "   Returncode: %6s",
                                "   Returncode: %6s" )
pick( INF_CMDLINE,              "cmdline=%s",
                                "cmdline=%s" )
pick( INF_CMDLINE_TOK_CNT,      "Token count cmdline=%s",
                                "Token count cmdline=%s" )
pick( INF_PASS_1,               "Starting pass %s of %s ( %s mode )",
                                "Starting pass %s of %s ( %s mode )" )
pick( INF_PASS_2,               "  End of pass %s of %s ( %s mode )",
                                "  End of pass %s of %s ( %s mode )" )
pick( WNG_IGN_OPTION,           "Option ignored: %s",
                                "Option ignored: %s" )
pick( ERR_PU_NUM,               "Workfile number not 1 - 9 Line %s of %s '%s'",
                                "Workfile number not 1 - 9 Line %s of %s '%s'" )

pick( ERR_SUB_OUT_OF_RANGE,     "Subscript out of range (-1000000 - +1000000) %s",
                                "Subscript out of range (-1000000 - +1000000) %s" )

pick( ERR_LOGIC_ERR,            "Logic error in %s",
                                "Logic error in %s"  )
pick( INF_TRMEM_CLOSE,          "trmem close() unfreed chunks: %s",
                                "trmem close() unfreed chunks: %s" )
pick( ERR_NOMEM_AVAIL,          "alloc() no memory available",
                                "alloc() no memory available" )
pick( ERR_TAG_NAME,             "Tag name too long '%s' Line %s of %s '%s'",
                                "Tag name too long '%s' Line %s of %s '%s'" )
pick( ERR_TAG_MACRO,            "The macro '%s' for the gml tag '%s' is not defined Line %s of %s '%s'",
                                "The macro '%s' for the gml tag '%s' is not defined Line %s of %s '%s'" )
pick( ERR_IF_INTERN,            "If internal logic error Line %s of %s '%s'",
                                "If internal logic error Line %s of %s '%s'" )
pick( INF_SUBST_LINE,           "> >%s< <",
                                "> >%s< <" )
pick( INF_TEXT_LINE,            ") )%s( (",
                                ") )%s( (" )
pick( INF_SKIP_LINE,            "Skip line",
                                "Skip line" )
pick( ERR_MISS_INV_OPT_VALUE,   "Invalid / missing option value for %s: %s",
                                "Invalid / missing option value for %s: %s" )
pick( ERR_MISSING_OPT_VALUE,    "Missing option value for %s",
                                "Missing option value for %s" )
pick( ERR_MISSING_DEVICE_NAME,  "Missing device name %s",
                                "Missing device name %s" )
pick( ERR_MISSING_FONT_NUMBER,  "Missing font number %s",
                                "Missing font number %s" )
pick( ERR_INVALID_FONT_NUMBER,  "Invalid font number %s",
                                "Invalid font number %s" )
pick( ERR_MISSING_FONT_NAME,    "Missing font name %s",
                                "Missing font name %s" )
pick( ERR_INVALID_FONT_SPACE,   "Invalid font space %s",
                                "Invalid font space %s" )
pick( ERR_INVALID_FONT_HEIGHT,  "Invalid font height %s",
                                "Invalid font height %s" )
pick( ERR_INVALID_OPTION,       "Invalid option %s",
                                "Invalid option %s" )
pick( ERR_DOC_DUPLICATE,        "Master document specified more than once %s",
                                "Master document specified more than once %s" )
pick( INF_RECOGNIZED_XXX,       "Recognized %s %s",
                                "Recognized %s %s" )
pick( ERR_PASSES_VALUE,         "Passes value out of range (1 - %s) %s",
                                "Passes value out of range (1 - %s) %s" )
pick( ERR_GT_NULL,              "The %s option value must be greater than_zero",
                                "The %s option value must be greater than_zero" )
pick( ERR_MISSING_NAME,         "Missing name %s",
                                "Missing name %s" )
pick( ERR_MISSING_VALUE,        "Missing value for %s",
                                "Missing value for %s" )
pick( ERR_RECURSIVE_OPTION,     "The option file '%s' is recursively included",
                                "The option file '%s' is recursively included" )
pick( ERR_FILE_NOT_FOUND,       "File not found '%s'",
                                "File not found '%s'" )
pick( ERR_MAX_NESTING_OPT,      "Max nesting exceeded for option files",
                                "Max nesting exceeded for option files" )
pick( ERR_INV_PAGE_RANGE,       "Page range invalid %s - %s",
                                "Page range invalid %s - %s" )
pick( ERR_INV_CW_SEP,           "Invalid controlword separator",
                                "Invalid controlword separator" )
pick( INF_MAC_LINE,             "\t\t\tLine %s of macro '%s'",
                                "\t\t\tLine %s of macro '%s'" )
pick( INF_FILE_LINE,            "\t\t\tLine %s of file '%s'",
                                "\t\t\tLine %s of file '%s'" )
pick( ERR_ATT_MISSING,          "Required attribute not found",
                                "Required attribute not found" )
pick( ERR_SYM_LONG,             "Symbol name too long '%s'",
                                "Symbol name too long '%s'" )
pick( INF_SYM_10,               "The length of a symbol cannot exceed 10 characters\n",
                                "The length of a symbol cannot exceed 10 characters\n" )
pick( ERR_SUB_INVALID,          "Subscript invalid %s",
                                "Subscript invalid %s" )
pick( WNG_MISS_INV_VALUE,       "Invalid / missing value for %s: %s",
                                "Invalid / missing value for %s: %s" )
