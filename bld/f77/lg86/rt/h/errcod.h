/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#define    NO_CARROT  0
#define    OPR_CARROT 1
#define    OPN_CARROT 2
#define    AR_ARG_CHAR_MISMATCH 2
#define    AR_COD_MISMATCH 3
#define    AR_CODI_MISMATCH 4
#define    AR_FUN_EXPECT 5
#define    AR_PARM_COUNT 6
#define    AR_SUB_EXPECT 7
#define    AR_TYP_MISMATCH 8
#define    AR_STRUCT_MISMATCH 9
#define    AR_PASS_BY_MISMATCH 11
#define    CP_TERMINATE 1536
#define    CV_OVERLAPING_ASSGT 1794
#define    DA_NOT_ENOUGH 2056
#define    DA_TOO_MUCH 2058
#define    DA_TYPE_MISMATCH 2060
#define    DA_DATA_TWICE 2061
#define    DM_RANGE_ERR_ON_DIMN 2305
#define    DO_0INC 2567
#define    EX_Z_2_NOT_POS 3840
#define    EX_NOT_INT_ARG 3841
#define    EX_CZ_2_NOT_REAL 3842
#define    FM_ASSUME_COMMA 4352
#define    FM_CONST 4353
#define    FM_D_EXT 4354
#define    FM_DECIMAL 4355
#define    FM_DELIM 4356
#define    FM_FMTCHAR 4357
#define    FM_INV_REP 4358
#define    FM_M_EXT 4359
#define    FM_MODIFIER 4360
#define    FM_NO_REP 4362
#define    FM_QUOTE 4364
#define    FM_SPEC_256 4365
#define    FM_WIDTH 4366
#define    FM_Z_EXT 4367
#define    FM_TOO_LARGE 4368
#define    FM_NOT_INP 4369
#define    FM_REP 4370
#define    FM_ASSUME_CONST 4371
#define    FM_Q_EXT 4372
#define    FM_Q_FORMAT 4373
#define    IO_ABACK 5888
#define    IO_ACTIVE_ALREADY 5889
#define    IO_AEND 5890
#define    IO_AF1 5891
#define    IO_AF2 5892
#define    IO_AREW 5893
#define    IO_BAD_CHAR 5894
#define    IO_BLNK_FMT 5895
#define    IO_FILE_PROBLEM 5896
#define    IO_FMT_MISMATCH 5897
#define    IO_FREE_MISMATCH 5898
#define    IO_IFULL 5899
#define    IO_IRECL 5900
#define    IO_ISTAT 5901
#define    IO_NOT_CONNECTED 5902
#define    IO_PAST_EOF 5903
#define    IO_RACCM 5904
#define    IO_REC1_ACCM 5905
#define    IO_REC2_ACCM 5906
#define    IO_SUBSEQUENT_OPEN 5907
#define    IO_SFILE 5908
#define    IO_SNAME 5909
#define    IO_UNFMT_RECL 5910
#define    IO_UNIT_EXIST 5911
#define    IO_IREC 5912
#define    IO_IUNIT 5913
#define    IO_BUFF_LEN 5914
#define    IO_BAD_SPEC 5915
#define    IO_CC_FORM 5916
#define    IO_BAD_ACTION 5917
#define    IO_NML_NO_SUCH_NAME 5918
#define    IO_NML_BAD_SYNTAX 5919
#define    IO_NML_BAD_SUBSCRIPT 5920
#define    IO_NML_BAD_SUBSTRING 5921
#define    IO_IBLOCKSIZE 5922
#define    IO_NO_RECORDS 5923
#define    IO_IOVERFLOW 5924
#define    IO_FRANGE_EXCEEDED 5925
#define    KO_FDIV_ZERO 6144
#define    KO_FOVERFLOW 6145
#define    KO_FUNDERFLOW 6146
#define    KO_IDIV_ZERO 6147
#define    KO_INTERRUPT 6148
#define    KO_IOVERFLOW 6149
#define    KO_PAGES_OUT 6150
#define    KO_STMT_CNT 6151
#define    KO_TIME_OUT 6152
#define    LI_ARG_GT_ZERO 6400
#define    LI_ARG_LE_ONE 6401
#define    LI_ARG_NEG 6402
#define    LI_ARG_ZERO 6403
#define    LI_CHAR_BOUND 6404
#define    LI_ICHAR_ONE 6408
#define    MO_DYNAMIC_OUT 6913
#define    MO_STORAGE_NOT_ALLOCATED 6915
#define    MO_STORAGE_ALLOCATED 6916
#define    PC_NO_OPENPAREN 7169
#define    PC_UNMATCH_PAREN 7171
#define    SR_ALREADY_ACTIVE 8972
#define    SR_ARG_USED_NOT_PASSED 8973
#define    SR_BAD_TYPE 8974
#define    SR_CHAR_SIZE_MISMATCH 8975
#define    SR_SUB_AS_FUNC 8977
#define    SR_BLKDAT_CALLED 8978
#define    SR_STRUCT_MISMATCH 8979
#define    SS_SSCR_RANGE 9218
#define    SS_SSTR_RANGE 9219
#define    ST_FORMAT_LABEL 9493
#define    ST_NUMBER 9494
#define    ST_BRANCH_INTO 9495
#define    SV_BAD_SSCR 9730
#define    SV_DIMENSION_LIMIT 9736
#define    UV_CH_ASGN_UNDEFINED 10496
#define    UV_CH_FN_UNDEFINED 10497
#define    UV_CH_REL_UNDEFINED 10498
#define    UV_IF_CHAR_ARG_UNDEFINED 10499
#define    UV_UNDEFD 10500
#define    UV_ARG_UNDEFINED 10501
#define    MS_EXT_PROC 11118
#define    MS_TRACE_INFO 11136
#define    MS_CALLED_FROM 11137
#define    MS_ERR_WRT_LST 11138
#define    MS_EXEC_IN 11139
#define    MS_CALLED_FROM_LG 11140
#define    MS_TB_LIBRARY 11141
#define    MS_TB_FILE 11142
#define    MS_INTERRUPT_PROMPT 11143
#define    MS_NO_8087 11144
#define    MS_DB_BAD_ISN 11145
#define    MS_DB_ENTERED 11146
#define    MS_DB_BREAK 11147
#define    MS_DB_RET_FROM 11148
#define    MS_DB_TRACE_INSTR 11149
#define    MS_DB_VAR_NOT_FOUND 11150
#define    MS_DB_ARR_NOT_DIMMED 11151
#define    MS_DB_SYNTAX_ERR 11152
#define    MS_DB_SSTR_BAD 11153
#define    MS_DB_SSCR_BAD 11154
#define    MS_DB_BAD_UNIT 11155
#define    MS_DB_NO_LISTING_FILE 11156
#define    MS_DB_HELP_1 11157
#define    MS_DB_HELP_2 11158
#define    MS_DB_HELP_3 11159
#define    MS_DB_HELP_4 11160
#define    MS_DB_HELP_5 11161
#define    MS_DB_HELP_6 11162
#define    MS_DB_HELP_7 11163
#define    MS_DB_HELP_8 11164
#define    MS_DB_HELP_9 11165
#define    MS_DB_HELP_10 11166
#define    MS_DB_HELP_11 11167
#define    MS_DB_HELP_12 11168
