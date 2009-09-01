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

pick( ERR_GOTO,                 "Goto label '%s' not found",
                                "Goto label '%s' not found" )

pick( ERR_MISSING_MAINFILENAME, "Missing main file name",
                                "Missing main file name" )

pick( ERR_IF_LEVEL,             "If nesting level not zero at EOF %s",
                                "If nesting level not zero at EOF %s" )

pick( INF_INDENT_REM,           "%s<< Indent removed",
                                "%s<< Indent removed" )

pick( INF_INCLUDED,             "\tIncluded from %s",
                                "\tIncluded from %s" )

pick( INF_STAT_1,               "Statistics:",
                                "Statistics:" )

pick( INF_STAT_2,               "  Max inc level:   %6s",
                                "  Max inc level:   %6s" )

pick( INF_STAT_3,               "    Error count:   %6s",
                                "    Error count:   %6s" )

pick( INF_STAT_4,               "  Warning count:   %6s",
                                "  Warning count:   %6s" )

pick( INF_STAT_5,               "     Returncode:   %6s",
                                "     Returncode:   %6s" )

pick( INF_STAT_6,               "Time to process: %s",
                                "Time to process: %s" )

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

pick( ERR_OUT_RANGE,            "The %s option value is out of range",
                                "The %s option value is out of range" )

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

pick( INF_MAC_LINE,             "\tLine %s of macro '%s'",
                                "\tLine %s of macro '%s'" )

pick( INF_FILE_LINE,            "\tLine %s of file '%s'",
                                "\tLine %s of file '%s'" )

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

pick( ERR_FUNC_PARM,            "Function parm %s invalid",
                                "Function parm %s invalid" )

pick( ERR_FUNC_PARM_MISS,       "Function parms missing",
                                "Function parms missing" )

pick( ERR_FUNC_PARM_END,        "Function parm end missing",
                                "Function parm end missing" )

pick( ERR_FUNC_NAME,            "Unknown function '%s'",
                                "Unknown function '%s'" )

pick( WNG_FUNC_UNSUPPORT,       "Function unsupported &%s",
                                "Function unsupported &%s" )

pick( WNG_FUNC_UNIMPL,          "Function not yet implemented &%s",
                                "Function not yet implemented &%s" )

pick( ERR_TAG_NAME_INV,         "Tag name missing / invalid",
                                "Tag name missing / invalid" )

pick( ERR_ATT_NAME_INV,         "Attribute name missing / invalid",
                                "Attribute name missing / invalid" )

pick( ERR_ATT_VAL_INV,          "Attribute value missing / invalid",
                                "Attribute value missing / invalid" )

pick( ERR_ATT_RANGE_INV,        "Attribute range missing / invalid",
                                "Attribute range missing / invalid" )

pick( ERR_ATT_DEFAULT,          "Attribute default outside range",
                                "Attribute default outside range" )

pick( ERR_USER_TAG,             "User tag '%s' has not been defined",
                                "User tag '%s' has not been defined" )

pick( ERR_TAG_TOOMANY,          "Too many attributes",
                                "Too many attributes" )

pick( ERR_LABEL_LINE,           "Label lineno not as expected %s",
                                "Label lineno not as expected %s" )

pick( WNG_LABEL_NUM,            "Numeric Label not implemented",
                                "Numeric Label not implemented" )

pick( ERR_LABEL_DUP,            "Label name duplicate at different line",
                                "Label name duplicate at different line" )

pick( ERR_LABEL_ZERO,           "Label lineno not greater 0",
                                "Label lineno not greater 0" )

pick( ERR_TAG_MAC_NAME,         "Tag macro name missing / invalid",
                                "Tag macro name missing / invalid" )

pick( ERR_TAG_FUNC_INV,         "Tag function missing / invalid (not ADD, CHANGE, OFF, ON, DELETE, PRINT",
                                "Tag function missing / invalid (not ADD, CHANGE, OFF, ON, DELETE, PRINT" )

pick( ERR_TAG_OPT_INV,          "Tag option invalid",
                                "Tag option invalid" )

pick( ERR_IF_TERM,              "If condition missing term",
                                "If condition missing term" )

pick( ERR_IF_RELOP,             "If condition invalid relational operator",
                                "If condition invalid relational operator" )

pick( ERR_IF_NESTING,           "If condition nesting too deep",
                                "If condition nesting too deep" )

pick( ERR_IF_THEN,              ".th then can only follow .if",
                                ".th then can only follow .if" )

pick( ERR_IF_ELSE,              ".el else not after .if",
                                ".el else not after .if" )

pick( ERR_IF_DO,                ".do not after .if or .el",
                                ".do not after .if or .el" )

pick( ERR_IF_DO_END,            ".do END does not match",
                                ".do END does not match" )

pick( ERR_IF_DO_FUN,            ".do invalid operand",
                                ".do invalid operand" )

pick( ERR_MAC_DEF_FUN,          ".dm '%s' macro define expecting BEGIN END /macro/lines/",
                                ".dm '%s' macro define expecting BEGIN END /macro/lines/" )

pick( ERR_MAC_DEF_NEST,         "Nested macro define '%s' expecting END",
                                "Nested macro define '%s' expecting END" )

pick( ERR_MAC_DEF_END,          "Macro define END without BEGIN",
                                "Macro define END without BEGIN" )

pick( ERR_MAC_DEF_NOT,          "Macro '%s' is not being defined",
                                "Macro '%s' is not being defined" )

pick( ERR_MAC_DEF_MISS,         "Macro control word parameter missing",
                                "Macro control word parameter missing" )

pick( ERR_MAC_DEF_INV,          "Macro control word parameter '%s' is invalid",
                                "Macro control word parameter '%s' is invalid" )

pick( ERR_MAC_DEF_EOF,          "End of file reached, macro '%s' is still being defined",
                                "End of file reached, macro '%s' is still being defined" )

pick( INF_MAC_DEFINED,          "Macro '%s' defined with %s lines",
                                "Macro '%s' defined with %s lines" )

pick( ERR_MAC_DEF_LOGIC,        "Macro '%s' define logic error",
                                "Macro '%s' define logic error" )

pick( ERR_MAC_NAME_INV,         "Macro name missing / invalid",
                                "Macro name missing / invalid" )

pick( ERR_TAG_EXIST,            "User tag '%s' already exists",
                                "User tag '%s' already exists" )

pick( ERR_AUTO_ATT,             "Cannot specify the automatic attribute '%s'",
                                "Cannot specify the automatic attribute '%s'" )

pick( ERR_ATT_LEN_INV,          "Length value too large",
                                "Length value too large" )

pick( ERR_ATT_VAL,              "Value '%s' for the '%s' attribute is not defined",
                                "Value '%s' for the '%s' attribute is not defined" )

pick( ERR_ATT_TEXT,             "Tag text may not be specified with the '%s' tag",
                                "Tag text may not be specified with the '%s' tag" )

pick( ERR_ATT_TEXT_REQ,         "Tag text must be specified with the '%s' tag",
                                "Tag text must be specified with the '%s' tag" )

pick( WNG_ATT_NAME,             "'%s' is not a valid attribute name",
                                "'%s' is not a valid attribute name" )

pick( ERR_ATT_REQ,              "For the tag '%s', the required attribute(s) %s have not been specified",
                                "For the tag '%s', the required attribute(s) %s have not been specified" )

pick( ERR_CW_UNRECOGNIZED,      "Unrecognized control word .%s",
                                "Unrecognized control word .%s" )

pick( ERR_DC_OPT,               "Invalid .dc option %s",
                                "Invalid .dc option %s" )

pick( ERR_XX_OPT,               "Invalid %s option %s",
                                "Invalid %s option %s" )

pick( ERR_NESTED_PE,            "Nested .pe not supported",
                                "Nested .pe not supported" )

pick( INF_ERROR_STOP,           "  Remaining pass(es) skipped due to error count",
                                "  Remaining pass(es) skipped due to error count" )

pick( ERR_PARM_MISSING,         "A control word parameter is required for %s",
                                "A control word parameter is required for %s" )

pick( ERR_EXTRA_IGNORED,        "Extra operand(s) for '%s' ignored: '%s'",
                                "Extra operand(s) for '%s' ignored: '%s'" )

pick( WNG_UNSUPP_CW,            "Unsupported SCRIPT control word: '%s'",
                                "Unsupported SCRIPT control word: '%s'" )

pick( WNG_UNSUPP_CW_OPT,        "Unsupported SCRIPT control word option: '%s %s'",
                                "Unsupported SCRIPT control word option: '%s %s'" )

pick( WNG_UNSUPP_TAG,           "Unsupported GML tag: ':%s'",
                                "Unsupported GML tag: ':%s'" )

pick( WNG_UNSUPP_LAY,           "Unsupported GML layout tag: ':%s'",
                                "Unsupported GML layout tag: ':%s'" )

pick( WNG_UNSUPP_LAY_ATT,       "Unsupported GML layout tag attribute: ':%s %s'",
                                "Unsupported GML layout tag attribute: ':%s %s'" )

pick( ERR_NESTED_LAY,           "Nested %s not supported",
                                "Nested %s not supported" )

pick( ERR_NO_LAY,               "No %s active for '%s'",
                                "No %s active for '%s'" )

pick( ERR_GML_IN_LAY,           "GML tag '%s' not allowed in LAYOUT",
                                "GML tag '%s' not allowed in LAYOUT" )

pick( ERR_LAY_IN_GML,           "LAYOUT tag '%s' not allowed outside LAYOUT",
                                "LAYOUT tag '%s' not allowed outside LAYOUT" )

pick( ERR_LAY_TOO_LATE,         "'LAYOUT' tag is specified too late (must come before text)",
                                "'LAYOUT' tag is specified too late (must come before text)" )

