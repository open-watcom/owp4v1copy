/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2009-2010 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
* Description:  RDOS API Call in-line assembly code definition
*
****************************************************************************/


#define CallGate_allocate_dos_mem 0x9a 0 0 0 0 2 0
#define CallGate_allocate_local_mem 0x9a 1 0 0 0 2 0
#define CallGate_free_mem 0x9a 2 0 0 0 2 0
#define CallGate_available_local_linear 0x9a 3 0 0 0 2 0
#define CallGate_used_local_linear 0x9a 4 0 0 0 2 0
#define CallGate_available_vm_linear 0x9a 5 0 0 0 2 0
#define CallGate_used_vm_linear 0x9a 6 0 0 0 2 0

#define CallGate_reserve_pe_mem 0x9a 7 0 0 0 2 0
#define CallGate_set_flat_linear_valid 0x9a 8 0 0 0 2 0
#define CallGate_set_flat_linear_invalid 0x9a 9 0 0 0 2 0
#define CallGate_set_flat_linear_read 0x9a 10 0 0 0 2 0
#define CallGate_set_flat_linear_readwrite 0x9a 11 0 0 0 2 0

#define CallGate_get_raw_switch_ads 0x9a 12 0 0 0 2 0
#define CallGate_raw_switch 0x9a 13 0 0 0 2 0
#define CallGate_get_exception 0x9a 14 0 0 0 2 0
#define CallGate_set_exception 0x9a 15 0 0 0 2 0
#define CallGate_get_pm_int 0x9a 16 0 0 0 2 0
#define CallGate_set_pm_int 0x9a 17 0 0 0 2 0
#define CallGate_get_vm_int 0x9a 18 0 0 0 2 0
#define CallGate_set_vm_int 0x9a 19 0 0 0 2 0
#define CallGate_dpmi_int 0x9a 20 0 0 0 2 0
#define CallGate_dpmi_call_int 0x9a 21 0 0 0 2 0
#define CallGate_dpmi_call 0x9a 22 0 0 0 2 0
#define CallGate_allocate_vm_callback 0x9a 23 0 0 0 2 0
#define CallGate_free_vm_callback 0x9a 24 0 0 0 2 0

#define CallGate_wait_milli 0x9a 25 0 0 0 2 0
#define CallGate_wait_micro 0x9a 26 0 0 0 2 0
#define CallGate_wait_until 0x9a 27 0 0 0 2 0
#define CallGate_create_thread 0x9a 28 0 0 0 2 0
#define CallGate_terminate_thread 0x9a 29 0 0 0 2 0
#define CallGate_get_thread 0x9a 30 0 0 0 2 0
#define CallGate_swap 0x9a 31 0 0 0 2 0

#define CallGate_create_user_section 0x9a 32 0 0 0 2 0
#define CallGate_create_blocked_user_section 0x9a 33 0 0 0 2 0
#define CallGate_delete_user_section 0x9a 34 0 0 0 2 0
#define CallGate_enter_user_section 0x9a 35 0 0 0 2 0
#define CallGate_leave_user_section 0x9a 36 0 0 0 2 0

#define CallGate_get_local_mailslot 0x9a 37 0 0 0 2 0
#define CallGate_get_remote_mailslot 0x9a 38 0 0 0 2 0
#define CallGate_free_mailslot 0x9a 39 0 0 0 2 0
#define CallGate_send_mailslot 0x9a 40 0 0 0 2 0
#define CallGate_define_mailslot 0x9a 41 0 0 0 2 0
#define CallGate_receive_mailslot 0x9a 42 0 0 0 2 0
#define CallGate_reply_mailslot 0x9a 43 0 0 0 2 0

#define CallGate_load_exe 0x9a 44 0 0 0 2 0
#define CallGate_spawn_exe 0x9a 45 0 0 0 2 0
#define CallGate_unload_exe 0x9a 46 0 0 0 2 0
#define CallGate_get_exit_code 0x9a 47 0 0 0 2 0
#define CallGate_get_exe_name 0x9a 48 0 0 0 2 0
#define CallGate_get_cmd_line 0x9a 49 0 0 0 2 0
#define CallGate_get_env 0x9a 50 0 0 0 2 0
#define CallGate_load_dll 0x9a 51 0 0 0 2 0
#define CallGate_free_dll 0x9a 52 0 0 0 2 0
#define CallGate_get_module_proc 0x9a 53 0 0 0 2 0
#define CallGate_get_module_resource 0x9a 54 0 0 0 2 0
#define CallGate_get_module_name 0x9a 55 0 0 0 2 0
#define CallGate_get_module 0x9a 56 0 0 0 2 0
#define CallGate_allocate_app_mem 0x9a 57 0 0 0 2 0
#define CallGate_free_app_mem 0x9a 58 0 0 0 2 0
#define CallGate_get_psp_sel 0x9a 59 0 0 0 2 0

#define CallGate_get_debug_tss 0x9a 60 0 0 0 2 0
#define CallGate_get_debug_thread 0x9a 61 0 0 0 2 0
#define CallGate_debug_trace 0x9a 62 0 0 0 2 0
#define CallGate_debug_pace 0x9a 63 0 0 0 2 0
#define CallGate_debug_go 0x9a 64 0 0 0 2 0
#define CallGate_debug_next 0x9a 65 0 0 0 2 0

#define CallGate_add_wait_for_debug_event 0x9a 66 0 0 0 2 0
#define CallGate_get_debug_event_data 0x9a 67 0 0 0 2 0
#define CallGate_continue_debug_event 0x9a 68 0 0 0 2 0
#define CallGate_notify_pe_exception 0x9a 70 0 0 0 2 0
#define CallGate_read_thread_mem 0x9a 71 0 0 0 2 0
#define CallGate_write_thread_mem 0x9a 72 0 0 0 2 0
#define CallGate_get_thread_tss 0x9a 73 0 0 0 2 0
#define CallGate_set_thread_tss 0x9a 74 0 0 0 2 0

#define CallGate_get_cpu_time 0x9a 75 0 0 0 2 0
#define CallGate_get_system_time 0x9a 76 0 0 0 2 0
#define CallGate_get_time 0x9a 77 0 0 0 2 0
#define CallGate_time_to_system_time 0x9a 78 0 0 0 2 0
#define CallGate_system_time_to_time 0x9a 79 0 0 0 2 0
#define CallGate_days_in_month 0x9a 80 0 0 0 2 0
#define CallGate_adjust_time 0x9a 81 0 0 0 2 0
#define CallGate_passed_days 0x9a 82 0 0 0 2 0
#define CallGate_time_to_binary 0x9a 83 0 0 0 2 0
#define CallGate_binary_to_time 0x9a 84 0 0 0 2 0
#define CallGate_sync_time 0x9a 85 0 0 0 2 0

#define CallGate_set_focus 0x9a 86 0 0 0 2 0
#define CallGate_enable_focus 0x9a 87 0 0 0 2 0

#define CallGate_get_dns 0x9a 88 0 0 0 2 0
#define CallGate_get_ppp_dns 0x9a 89 0 0 0 2 0
#define CallGate_open_tcp_connection 0x9a 90 0 0 0 2 0
#define CallGate_wait_for_tcp_connection 0x9a 92 0 0 0 2 0
#define CallGate_close_tcp_connection 0x9a 93 0 0 0 2 0
#define CallGate_delete_tcp_connection 0x9a 94 0 0 0 2 0
#define CallGate_is_tcp_connection_closed 0x9a 95 0 0 0 2 0
#define CallGate_abort_tcp_connection 0x9a 96 0 0 0 2 0
#define CallGate_read_tcp_connection 0x9a 97 0 0 0 2 0
#define CallGate_write_tcp_connection 0x9a 98 0 0 0 2 0
#define CallGate_push_tcp_connection 0x9a 99 0 0 0 2 0
#define CallGate_get_ip_address 0x9a 100 0 0 0 2 0
#define CallGate_name_to_ip 0x9a 101 0 0 0 2 0
#define CallGate_ip_to_name 0x9a 102 0 0 0 2 0
#define CallGate_ping 0x9a 103 0 0 0 2 0

#define CallGate_get_disc_info 0x9a 104 0 0 0 2 0
#define CallGate_format_drive 0x9a 105 0 0 0 2 0
#define CallGate_get_rdfs_info 0x9a 106 0 0 0 2 0
#define CallGate_read_disc 0x9a 107 0 0 0 2 0
#define CallGate_write_disc 0x9a 108 0 0 0 2 0
#define CallGate_get_drive_info 0x9a 109 0 0 0 2 0

#define CallGate_set_cur_drive 0x9a 110 0 0 0 2 0
#define CallGate_get_cur_drive 0x9a 111 0 0 0 2 0
#define CallGate_set_cur_dir 0x9a 112 0 0 0 2 0
#define CallGate_get_cur_dir 0x9a 113 0 0 0 2 0
#define CallGate_make_dir 0x9a 114 0 0 0 2 0
#define CallGate_remove_dir 0x9a 115 0 0 0 2 0
#define CallGate_rename_file 0x9a 116 0 0 0 2 0
#define CallGate_delete_file 0x9a 117 0 0 0 2 0
#define CallGate_get_file_attribute 0x9a 118 0 0 0 2 0
#define CallGate_set_file_attribute 0x9a 119 0 0 0 2 0
#define CallGate_open_dir 0x9a 120 0 0 0 2 0
#define CallGate_close_dir 0x9a 121 0 0 0 2 0
#define CallGate_read_dir 0x9a 122 0 0 0 2 0

#define CallGate_open_file 0x9a 123 0 0 0 2 0
#define CallGate_create_file 0x9a 124 0 0 0 2 0
#define CallGate_close_file 0x9a 125 0 0 0 2 0
#define CallGate_dupl_file 0x9a 126 0 0 0 2 0
#define CallGate_get_ioctl_data 0x9a 127 0 0 0 2 0
#define CallGate_get_file_size 0x9a 128 0 0 0 2 0
#define CallGate_set_file_size 0x9a 129 0 0 0 2 0
#define CallGate_get_file_pos 0x9a 130 0 0 0 2 0
#define CallGate_set_file_pos 0x9a 131 0 0 0 2 0
#define CallGate_get_file_time 0x9a 132 0 0 0 2 0
#define CallGate_set_file_time 0x9a 133 0 0 0 2 0
#define CallGate_read_file 0x9a 134 0 0 0 2 0
#define CallGate_write_file 0x9a 135 0 0 0 2 0
#define CallGate_read_con 0x9a 136 0 0 0 2 0

#define CallGate_create_mapping 0x9a 137 0 0 0 2 0
#define CallGate_create_named_mapping 0x9a 138 0 0 0 2 0
#define CallGate_create_file_mapping 0x9a 139 0 0 0 2 0
#define CallGate_create_named_file_mapping 0x9a 140 0 0 0 2 0
#define CallGate_open_named_mapping 0x9a 141 0 0 0 2 0
#define CallGate_sync_mapping 0x9a 142 0 0 0 2 0
#define CallGate_close_mapping 0x9a 143 0 0 0 2 0
#define CallGate_map_view 0x9a 144 0 0 0 2 0
#define CallGate_unmap_view 0x9a 145 0 0 0 2 0

#define CallGate_read_keyboard 0x9a 146 0 0 0 2 0
#define CallGate_poll_keyboard 0x9a 147 0 0 0 2 0
#define CallGate_flush_keyboard 0x9a 148 0 0 0 2 0
#define CallGate_get_keyboard_state 0x9a 151 0 0 0 2 0

#define CallGate_show_mouse 0x9a 152 0 0 0 2 0
#define CallGate_hide_mouse 0x9a 153 0 0 0 2 0
#define CallGate_get_mouse_position 0x9a 154 0 0 0 2 0
#define CallGate_set_mouse_position 0x9a 155 0 0 0 2 0
#define CallGate_set_mouse_window 0x9a 156 0 0 0 2 0
#define CallGate_set_mouse_mickey 0x9a 157 0 0 0 2 0
#define CallGate_get_left_button 0x9a 158 0 0 0 2 0
#define CallGate_get_right_button 0x9a 159 0 0 0 2 0
#define CallGate_get_left_button_press_position 0x9a 160 0 0 0 2 0
#define CallGate_get_right_button_press_position 0x9a 161 0 0 0 2 0
#define CallGate_get_left_button_release_position 0x9a 162 0 0 0 2 0
#define CallGate_get_right_button_release_position 0x9a 163 0 0 0 2 0
#define CallGate_hook_mouse 0x9a 164 0 0 0 2 0
#define CallGate_unhook_mouse 0x9a 165 0 0 0 2 0

#define CallGate_lock_cd 0x9a 166 0 0 0 2 0
#define CallGate_unlock_cd 0x9a 167 0 0 0 2 0
#define CallGate_eject_cd 0x9a 168 0 0 0 2 0
#define CallGate_insert_cd 0x9a 169 0 0 0 2 0

#define CallGate_open_com 0x9a 170 0 0 0 2 0
#define CallGate_close_com 0x9a 171 0 0 0 2 0
#define CallGate_flush_com 0x9a 172 0 0 0 2 0
#define CallGate_add_wait_for_com 0x9a 174 0 0 0 2 0
#define CallGate_read_com 0x9a 175 0 0 0 2 0
#define CallGate_write_com 0x9a 176 0 0 0 2 0
#define CallGate_set_dtr 0x9a 177 0 0 0 2 0
#define CallGate_reset_dtr 0x9a 178 0 0 0 2 0

#define CallGate_init_printer 0x9a 179 0 0 0 2 0
#define CallGate_check_printer 0x9a 180 0 0 0 2 0
#define CallGate_write_printer 0x9a 181 0 0 0 2 0

#define CallGate_set_cursor_position 0x9a 182 0 0 0 2 0
#define CallGate_get_cursor_position 0x9a 183 0 0 0 2 0
#define CallGate_write_char 0x9a 184 0 0 0 2 0
#define CallGate_write_asciiz 0x9a 185 0 0 0 2 0
#define CallGate_write_size_string 0x9a 186 0 0 0 2 0

#define CallGate_set_video_mode 0x9a 187 0 0 0 2 0
#define CallGate_set_vga_mode 0x9a 188 0 0 0 2 0
#define CallGate_set_forecolor 0x9a 189 0 0 0 2 0
#define CallGate_set_backcolor 0x9a 190 0 0 0 2 0
#define CallGate_get_string_metrics 0x9a 193 0 0 0 2 0
#define CallGate_set_font 0x9a 194 0 0 0 2 0
#define CallGate_get_video_mode 0x9a 195 0 0 0 2 0
#define CallGate_draw_string 0x9a 197 0 0 0 2 0

#define CallGate_xms_handler 0x9a 198 0 0 0 2 0
#define CallGate_ems_handler 0x9a 199 0 0 0 2 0

#define CallGate_test 0x9a 200 0 0 0 2 0

#define CallGate_resize_flat_linear 0x9a 201 0 0 0 2 0

#define CallGate_set_drawcolor 0x9a 202 0 0 0 2 0
#define CallGate_set_lgop 0x9a 203 0 0 0 2 0
#define CallGate_set_hollow_style 0x9a 204 0 0 0 2 0
#define CallGate_set_filled_style 0x9a 205 0 0 0 2 0
#define CallGate_get_pixel 0x9a 206 0 0 0 2 0
#define CallGate_set_pixel 0x9a 207 0 0 0 2 0
#define CallGate_draw_line 0x9a 208 0 0 0 2 0
#define CallGate_draw_rect 0x9a 209 0 0 0 2 0
#define CallGate_draw_ellipse 0x9a 210 0 0 0 2 0
#define CallGate_create_bitmap 0x9a 211 0 0 0 2 0
#define CallGate_close_bitmap 0x9a 212 0 0 0 2 0
#define CallGate_blit 0x9a 213 0 0 0 2 0
#define CallGate_open_font 0x9a 215 0 0 0 2 0
#define CallGate_close_font 0x9a 216 0 0 0 2 0
#define CallGate_create_string_bitmap 0x9a 217 0 0 0 2 0
#define CallGate_get_bitmap_info 0x9a 218 0 0 0 2 0
#define CallGate_draw_mask 0x9a 219 0 0 0 2 0

#define CallGate_create_sprite 0x9a 220 0 0 0 2 0
#define CallGate_close_sprite 0x9a 221 0 0 0 2 0
#define CallGate_show_sprite 0x9a 222 0 0 0 2 0
#define CallGate_hide_sprite 0x9a 223 0 0 0 2 0
#define CallGate_move_sprite 0x9a 224 0 0 0 2 0

#define CallGate_dup_bitmap_handle 0x9a 225 0 0 0 2 0
#define CallGate_set_clip_rect 0x9a 226 0 0 0 2 0
#define CallGate_clear_clip_rect 0x9a 227 0 0 0 2 0

#define CallGate_set_rts 0x9a 228 0 0 0 2 0
#define CallGate_reset_rts 0x9a 229 0 0 0 2 0
#define CallGate_get_com_receive_space 0x9a 230 0 0 0 2 0
#define CallGate_get_com_send_space 0x9a 231 0 0 0 2 0

#define CallGate_get_char_attrib 0x9a 232 0 0 0 2 0

#define CallGate_create_wait 0x9a 233 0 0 0 2 0
#define CallGate_close_wait 0x9a 234 0 0 0 2 0
#define CallGate_is_wait_idle 0x9a 235 0 0 0 2 0
#define CallGate_wait_no_timeout 0x9a 236 0 0 0 2 0
#define CallGate_wait_timeout 0x9a 237 0 0 0 2 0
#define CallGate_stop_wait 0x9a 238 0 0 0 2 0
#define CallGate_add_wait_for_keyboard 0x9a 239 0 0 0 2 0

#define CallGate_peek_key_event 0x9a 240 0 0 0 2 0
#define CallGate_read_key_event 0x9a 241 0 0 0 2 0

#define CallGate_add_wait_for_mouse 0x9a 242 0 0 0 2 0
#define CallGate_remove_wait 0x9a 243 0 0 0 2 0

#define CallGate_add_wait_for_adc 0x9a 244 0 0 0 2 0
#define CallGate_open_adc 0x9a 245 0 0 0 2 0
#define CallGate_close_adc 0x9a 246 0 0 0 2 0
#define CallGate_define_adc_time 0x9a 247 0 0 0 2 0
#define CallGate_read_adc 0x9a 248 0 0 0 2 0

#define CallGate_free_v86 0x9a 249 0 0 0 2 0

#define CallGate_read_serial_lines 0x9a 250 0 0 0 2 0
#define CallGate_toggle_serial_line 0x9a 251 0 0 0 2 0

#define CallGate_read_serial_val 0x9a 252 0 0 0 2 0
#define CallGate_write_serial_val 0x9a 253 0 0 0 2 0

#define CallGate_create_file_drive 0x9a 254 0 0 0 2 0
#define CallGate_open_file_drive 0x9a 255 0 0 0 2 0

#define CallGate_dos_ext_exec 0x9a 0 1 0 0 2 0

#define CallGate_open_sys_env 0x9a 1 1 0 0 2 0
#define CallGate_open_proc_env 0x9a 2 1 0 0 2 0
#define CallGate_close_env 0x9a 3 1 0 0 2 0
#define CallGate_add_env_var 0x9a 4 1 0 0 2 0
#define CallGate_delete_env_var 0x9a 5 1 0 0 2 0
#define CallGate_find_env_var 0x9a 6 1 0 0 2 0
#define CallGate_get_env_data 0x9a 7 1 0 0 2 0
#define CallGate_set_env_data 0x9a 8 1 0 0 2 0

#define CallGate_open_sys_ini 0x9a 9 1 0 0 2 0
#define CallGate_close_ini 0x9a 10 1 0 0 2 0
#define CallGate_goto_ini_section 0x9a 11 1 0 0 2 0
#define CallGate_remove_ini_section 0x9a 12 1 0 0 2 0
#define CallGate_read_ini 0x9a 13 1 0 0 2 0
#define CallGate_write_ini 0x9a 14 1 0 0 2 0
#define CallGate_delete_ini 0x9a 15 1 0 0 2 0

#define CallGate_app_debug 0x9a 16 1 0 0 2 0
#define CallGate_get_version 0x9a 17 1 0 0 2 0

#define CallGate_enable_status_led 0x9a 18 1 0 0 2 0
#define CallGate_disable_status_led 0x9a 19 1 0 0 2 0

#define CallGate_start_watchdog 0x9a 20 1 0 0 2 0
#define CallGate_kick_watchdog 0x9a 21 1 0 0 2 0

#define CallGate_erase_disc_sectors 0x9a 22 1 0 0 2 0

#define CallGate_cpu_reset 0x9a 23 1 0 0 2 0

#define CallGate_enable_cts 0x9a 24 1 0 0 2 0
#define CallGate_disable_cts 0x9a 25 1 0 0 2 0

#define CallGate_wait_for_send_completed_com 0x9a 26 1 0 0 2 0

#define CallGate_add_wait_for_tcp_connection 0x9a 27 1 0 0 2 0

#define CallGate_get_remote_tcp_connection_ip 0x9a 28 1 0 0 2 0
#define CallGate_get_remote_tcp_connection_port 0x9a 29 1 0 0 2 0
#define CallGate_get_local_tcp_connection_port 0x9a 30 1 0 0 2 0

#define CallGate_enable_auto_rts 0x9a 31 1 0 0 2 0
#define CallGate_disable_auto_rts 0x9a 32 1 0 0 2 0

#define CallGate_put_keyboard_code 0x9a 33 1 0 0 2 0

#define CallGate_poll_tcp_connection 0x9a 34 1 0 0 2 0

#define CallGate_create_signal 0x9a 35 1 0 0 2 0
#define CallGate_free_signal 0x9a 36 1 0 0 2 0
#define CallGate_add_wait_for_signal 0x9a 37 1 0 0 2 0
#define CallGate_set_signal 0x9a 38 1 0 0 2 0
#define CallGate_reset_signal 0x9a 39 1 0 0 2 0
#define CallGate_is_signalled 0x9a 40 1 0 0 2 0

#define CallGate_get_drive_disc_param 0x9a 41 1 0 0 2 0

#define CallGate_get_ide_disc 0x9a 42 1 0 0 2 0
#define CallGate_get_floppy_disc 0x9a 43 1 0 0 2 0
#define CallGate_demand_load_drive 0x9a 44 1 0 0 2 0
#define CallGate_set_disc_info 0x9a 45 1 0 0 2 0

#define CallGate_get_thread_state 0x9a 46 1 0 0 2 0
#define CallGate_suspend_thread 0x9a 47 1 0 0 2 0
#define CallGate_resume_thread 0x9a 48 1 0 0 2 0

#define CallGate_update_time 0x9a 49 1 0 0 2 0

#define CallGate_allocate_static_drive 0x9a 50 1 0 0 2 0
#define CallGate_allocate_fixed_drive 0x9a 51 1 0 0 2 0
#define CallGate_allocate_dynamic_drive 0x9a 52 1 0 0 2 0

#define CallGate_get_focus 0x9a 53 1 0 0 2 0

#define CallGate_add_wait_for_tcp_listen 0x9a 54 1 0 0 2 0
#define CallGate_create_tcp_listen 0x9a 55 1 0 0 2 0
#define CallGate_get_tcp_listen 0x9a 56 1 0 0 2 0
#define CallGate_close_tcp_listen 0x9a 57 1 0 0 2 0

#define CallGate_get_random 0x9a 58 1 0 0 2 0

#define CallGate_suspend_and_signal_thread 0x9a 59 1 0 0 2 0

#define CallGate_start_net_capture 0x9a 60 1 0 0 2 0
#define CallGate_stop_net_capture 0x9a 61 1 0 0 2 0

#define CallGate_create_crc 0x9a 62 1 0 0 2 0
#define CallGate_close_crc 0x9a 63 1 0 0 2 0
#define CallGate_calc_crc 0x9a 64 1 0 0 2 0

#define CallGate_get_usb_device 0x9a 65 1 0 0 2 0
#define CallGate_get_usb_config 0x9a 66 1 0 0 2 0

#define CallGate_open_usb_pipe 0x9a 67 1 0 0 2 0
#define CallGate_close_usb_pipe 0x9a 68 1 0 0 2 0
#define CallGate_add_wait_for_usb_pipe 0x9a 69 1 0 0 2 0
#define CallGate_req_usb_data 0x9a 70 1 0 0 2 0
#define CallGate_get_usb_data_size 0x9a 71 1 0 0 2 0
#define CallGate_write_usb_data 0x9a 72 1 0 0 2 0
#define CallGate_req_usb_status 0x9a 73 1 0 0 2 0
#define CallGate_write_usb_status 0x9a 74 1 0 0 2 0
#define CallGate_write_usb_control 0x9a 75 1 0 0 2 0
#define CallGate_lock_usb_pipe 0x9a 77 1 0 0 2 0
#define CallGate_unlock_usb_pipe 0x9a 78 1 0 0 2 0

#define CallGate_get_max_com_port 0x9a 79 1 0 0 2 0

#define CallGate_config_usb_device 0x9a 80 1 0 0 2 0

#define CallGate_start_usb_transaction 0x9a 81 1 0 0 2 0

#define CallGate_open_ini 0x9a 82 1 0 0 2 0

#define CallGate_open_icsp 0x9a 83 1 0 0 2 0
#define CallGate_close_icsp 0x9a 84 1 0 0 2 0
#define CallGate_write_icsp_cmd 0x9a 85 1 0 0 2 0
#define CallGate_write_icsp_data 0x9a 86 1 0 0 2 0
#define CallGate_read_icsp_data 0x9a 87 1 0 0 2 0

#define CallGate_get_gateway 0x9a 88 1 0 0 2 0

#define CallGate_get_free_physical 0x9a 89 1 0 0 2 0
#define CallGate_get_free_gdt 0x9a 90 1 0 0 2 0
#define CallGate_available_big_linear 0x9a 91 1 0 0 2 0
#define CallGate_available_small_linear 0x9a 92 1 0 0 2 0

#define CallGate_get_ip_mask 0x9a 93 1 0 0 2 0

#define CallGate_get_master_volume 0x9a 94 1 0 0 2 0
#define CallGate_set_master_volume 0x9a 95 1 0 0 2 0

#define CallGate_get_line_out_volume 0x9a 96 1 0 0 2 0
#define CallGate_set_line_out_volume 0x9a 97 1 0 0 2 0

#define CallGate_create_audio_out_channel 0x9a 98 1 0 0 2 0
#define CallGate_close_audio_out_channel 0x9a 99 1 0 0 2 0
#define CallGate_write_audio 0x9a 100 1 0 0 2 0

#define CallGate_stop_watchdog 0x9a 101 1 0 0 2 0

#define CallGate_is_valid_usergate 0x9a 102 1 0 0 2 0

#define CallGate_open_fm 0x9a 103 1 0 0 2 0
#define CallGate_close_fm 0x9a 104 1 0 0 2 0
#define CallGate_fm_wait 0x9a 105 1 0 0 2 0
#define CallGate_create_fm_instrument 0x9a 106 1 0 0 2 0
#define CallGate_free_fm_instrument 0x9a 107 1 0 0 2 0
#define CallGate_set_fm_attack 0x9a 108 1 0 0 2 0
#define CallGate_set_fm_sustain 0x9a 109 1 0 0 2 0
#define CallGate_set_fm_release 0x9a 110 1 0 0 2 0
#define CallGate_play_fm_note 0x9a 111 1 0 0 2 0

#define CallGate_get_thread_linear 0x9a 112 1 0 0 2 0
#define CallGate_get_current_module 0x9a 113 1 0 0 2 0

#define CallGate_get_module_focus_key 0x9a 114 1 0 0 2 0

#define CallGate_get_debug_event 0x9a 115 1 0 0 2 0
#define CallGate_clear_debug_event 0x9a 116 1 0 0 2 0

#define CallGate_get_free_handles 0x9a 117 1 0 0 2 0

#define CallGate_get_env_size 0x9a 118 1 0 0 2 0

#define CallGate_show_exception_text 0x9a 119 1 0 0 2 0

#define CallGate_get_watchdog_tics 0x9a 120 1 0 0 2 0

#define CallGate_is_usb_trans_done 0x9a 121 1 0 0 2 0
#define CallGate_was_usb_trans_ok 0x9a 122 1 0 0 2 0

#define CallGate_is_tcp_connection_idle 0x9a 123 1 0 0 2 0

#define CallGate_set_codec_gpio0 0x9a 124 1 0 0 2 0

