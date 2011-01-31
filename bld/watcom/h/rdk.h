/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2009-2010 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
* Description:  RDOS device-driver in-line assembly code definition. This file
*               is generated automatically, do not edit it.
*
****************************************************************************/

#define OsGate_register_osgate 0x9a 0 0 0 0 3 0
#define OsGate_is_valid_osgate 0x9a 1 0 0 0 3 0
#define OsGate_register_usergate 0x9a 2 0 0 0 3 0
#define OsGate_register_bimodal_usergate 0x9a 3 0 0 0 3 0
#define OsGate_register_usergate16 0x9a 4 0 0 0 3 0
#define OsGate_register_usergate32 0x9a 5 0 0 0 3 0
#define OsGate_register_usergate_v86 0x9a 6 0 0 0 3 0
#define OsGate_init_device 0x9a 7 0 0 0 3 0
#define OsGate_alias_code32 0x9a 8 0 0 0 3 0
#define OsGate_emulate_opcode 0x9a 9 0 0 0 3 0

#define OsGate_allocate_gdt 0x9a 10 0 0 0 3 0
#define OsGate_free_gdt 0x9a 11 0 0 0 3 0
#define OsGate_allocate_ldt 0x9a 12 0 0 0 3 0
#define OsGate_free_ldt 0x9a 13 0 0 0 3 0
#define OsGate_allocate_multiple_ldt 0x9a 14 0 0 0 3 0
#define OsGate_get_selector_base_size 0x9a 15 0 0 0 3 0
#define OsGate_create_data_sel16 0x9a 16 0 0 0 3 0
#define OsGate_create_data_sel32 0x9a 17 0 0 0 3 0
#define OsGate_create_code_sel16 0x9a 18 0 0 0 3 0
#define OsGate_create_code_sel32 0x9a 19 0 0 0 3 0
#define OsGate_create_conform_sel16 0x9a 20 0 0 0 3 0
#define OsGate_create_conform_sel32 0x9a 21 0 0 0 3 0
#define OsGate_create_ldt_sel 0x9a 22 0 0 0 3 0
#define OsGate_create_tss_sel 0x9a 23 0 0 0 3 0
#define OsGate_create_call_gate_sel16 0x9a 24 0 0 0 3 0
#define OsGate_create_call_gate_sel32 0x9a 25 0 0 0 3 0
#define OsGate_create_task_gate_sel 0x9a 26 0 0 0 3 0
#define OsGate_create_int_gate_sel 0x9a 27 0 0 0 3 0
#define OsGate_create_trap_gate_sel 0x9a 28 0 0 0 3 0
#define OsGate_selector_to_segment 0x9a 29 0 0 0 3 0
#define OsGate_segment_to_selector 0x9a 30 0 0 0 3 0
#define OsGate_free_selector 0x9a 31 0 0 0 3 0
#define OsGate_segment_not_present 0x9a 32 0 0 0 3 0

#define OsGate_get_physical_page 0x9a 33 0 0 0 3 0
#define OsGate_set_physical_page 0x9a 34 0 0 0 3 0
#define OsGate_get_thread_physical_page 0x9a 35 0 0 0 3 0
#define OsGate_set_thread_physical_page 0x9a 36 0 0 0 3 0
#define OsGate_read_thread_selector 0x9a 37 0 0 0 3 0
#define OsGate_write_thread_selector 0x9a 38 0 0 0 3 0
#define OsGate_read_thread_segment 0x9a 39 0 0 0 3 0
#define OsGate_write_thread_segment 0x9a 40 0 0 0 3 0
#define OsGate_allocate_page 0x9a 41 0 0 0 3 0
#define OsGate_free_page 0x9a 42 0 0 0 3 0
#define OsGate_set_page_emulate 0x9a 43 0 0 0 3 0
#define OsGate_set_page_kernel 0x9a 44 0 0 0 3 0
#define OsGate_hook_page 0x9a 45 0 0 0 3 0
#define OsGate_unhook_page 0x9a 46 0 0 0 3 0

#define OsGate_allocate_global_mem 0x9a 47 0 0 0 3 0
#define OsGate_allocate_small_global_mem 0x9a 48 0 0 0 3 0
#define OsGate_allocate_small_mem 0x9a 49 0 0 0 3 0
#define OsGate_allocate_big_mem 0x9a 50 0 0 0 3 0
#define OsGate_allocate_big_linear 0x9a 51 0 0 0 3 0
#define OsGate_allocate_small_linear 0x9a 52 0 0 0 3 0
#define OsGate_allocate_local_linear 0x9a 53 0 0 0 3 0
#define OsGate_reserve_local_linear 0x9a 54 0 0 0 3 0
#define OsGate_allocate_vm_linear 0x9a 55 0 0 0 3 0
#define OsGate_free_linear 0x9a 56 0 0 0 3 0
#define OsGate_resize_linear 0x9a 57 0 0 0 3 0
#define OsGate_used_big_linear 0x9a 60 0 0 0 3 0
#define OsGate_used_small_linear 0x9a 61 0 0 0 3 0
#define OsGate_used_local_linear_thread 0x9a 62 0 0 0 3 0
#define OsGate_allocate_fixed_process_mem 0x9a 64 0 0 0 3 0
#define OsGate_allocate_fixed_system_mem 0x9a 65 0 0 0 3 0
#define OsGate_allocate_process_linear 0x9a 67 0 0 0 3 0
#define OsGate_allocate_system_linear 0x9a 68 0 0 0 3 0
#define OsGate_allocate_fixed_vm_linear 0x9a 69 0 0 0 3 0

#define OsGate_allocate_physical 0x9a 70 0 0 0 3 0
#define OsGate_allocate_dma_physical 0x9a 71 0 0 0 3 0
#define OsGate_free_physical 0x9a 72 0 0 0 3 0

#define OsGate_create_process 0x9a 74 0 0 0 3 0
#define OsGate_wake_thread 0x9a 79 0 0 0 3 0
#define OsGate_sleep_thread 0x9a 80 0 0 0 3 0
#define OsGate_clear_signal 0x9a 83 0 0 0 3 0
#define OsGate_signal 0x9a 84 0 0 0 3 0
#define OsGate_wait_for_signal 0x9a 85 0 0 0 3 0
#define OsGate_enter_section 0x9a 86 0 0 0 3 0
#define OsGate_leave_section 0x9a 87 0 0 0 3 0
#define OsGate_start_timer 0x9a 89 0 0 0 3 0
#define OsGate_stop_timer 0x9a 90 0 0 0 3 0
#define OsGate_debug_break 0x9a 92 0 0 0 3 0
#define OsGate_hook_create_thread 0x9a 93 0 0 0 3 0
#define OsGate_hook_create_process 0x9a 94 0 0 0 3 0
#define OsGate_hook_terminate_thread 0x9a 95 0 0 0 3 0
#define OsGate_hook_terminate_process 0x9a 96 0 0 0 3 0
#define OsGate_hook_init_tasking 0x9a 97 0 0 0 3 0
#define OsGate_hook_state 0x9a 98 0 0 0 3 0

#define OsGate_sim_sti 0x9a 99 0 0 0 3 0
#define OsGate_sim_cli 0x9a 100 0 0 0 3 0
#define OsGate_sim_set_flags 0x9a 101 0 0 0 3 0
#define OsGate_sim_get_flags 0x9a 102 0 0 0 3 0
#define OsGate_save_context 0x9a 103 0 0 0 3 0
#define OsGate_restore_context 0x9a 104 0 0 0 3 0
#define OsGate_call_vm 0x9a 105 0 0 0 3 0
#define OsGate_call_pm16 0x9a 106 0 0 0 3 0
#define OsGate_call_pm32 0x9a 107 0 0 0 3 0
#define OsGate_hook_vm_int 0x9a 108 0 0 0 3 0
#define OsGate_hook_pm16_int 0x9a 109 0 0 0 3 0
#define OsGate_hook_pm32_int 0x9a 110 0 0 0 3 0
#define OsGate_hook_exception 0x9a 111 0 0 0 3 0
#define OsGate_hook_get_vm_int 0x9a 112 0 0 0 3 0
#define OsGate_hook_set_vm_int 0x9a 113 0 0 0 3 0
#define OsGate_hook_get_pm16_int 0x9a 114 0 0 0 3 0
#define OsGate_hook_set_pm16_int 0x9a 115 0 0 0 3 0
#define OsGate_hook_get_pm32_int 0x9a 116 0 0 0 3 0
#define OsGate_hook_set_pm32_int 0x9a 117 0 0 0 3 0
#define OsGate_reflect_exception 0x9a 118 0 0 0 3 0
#define OsGate_reflect_pm_to_vm 0x9a 119 0 0 0 3 0
#define OsGate_v86_bios_int 0x9a 120 0 0 0 3 0
#define OsGate_set_bitness 0x9a 121 0 0 0 3 0

#define OsGate_hook_in 0x9a 122 0 0 0 3 0
#define OsGate_hook_out 0x9a 123 0 0 0 3 0

#define OsGate_request_private_irq_handler 0x9a 124 0 0 0 3 0
#define OsGate_release_private_irq_handler 0x9a 125 0 0 0 3 0

#define OsGate_set_system_time 0x9a 128 0 0 0 3 0

#define OsGate_register_handle 0x9a 129 0 0 0 3 0
#define OsGate_allocate_handle 0x9a 130 0 0 0 3 0
#define OsGate_free_handle 0x9a 131 0 0 0 3 0
#define OsGate_deref_handle 0x9a 132 0 0 0 3 0

#define OsGate_register_net_class 0x9a 133 0 0 0 3 0
#define OsGate_register_net_protocol 0x9a 134 0 0 0 3 0
#define OsGate_register_net_driver 0x9a 135 0 0 0 3 0
#define OsGate_register_ppp_driver 0x9a 136 0 0 0 3 0
#define OsGate_send_net 0x9a 137 0 0 0 3 0
#define OsGate_send_ppp 0x9a 138 0 0 0 3 0
#define OsGate_net_received 0x9a 139 0 0 0 3 0
#define OsGate_hook_ip 0x9a 140 0 0 0 3 0
#define OsGate_create_ip_header 0x9a 141 0 0 0 3 0
#define OsGate_send_ip_data 0x9a 142 0 0 0 3 0
#define OsGate_hook_ip_cache 0x9a 143 0 0 0 3 0
#define OsGate_allocate_ip_cache_mem 0x9a 144 0 0 0 3 0
#define OsGate_lookup_ip_cache 0x9a 145 0 0 0 3 0
#define OsGate_get_host_timeout 0x9a 146 0 0 0 3 0
#define OsGate_update_round_trip_time 0x9a 147 0 0 0 3 0
#define OsGate_get_ppp_ip 0x9a 148 0 0 0 3 0
#define OsGate_open_ppp 0x9a 149 0 0 0 3 0
#define OsGate_close_ppp 0x9a 150 0 0 0 3 0
#define OsGate_query_udp 0x9a 151 0 0 0 3 0
#define OsGate_listen_udp_port 0x9a 152 0 0 0 3 0

#define OsGate_allocate_dos_linear 0x9a 153 0 0 0 3 0
#define OsGate_free_dos_linear 0x9a 154 0 0 0 3 0
#define OsGate_resize_dos_linear 0x9a 155 0 0 0 3 0
#define OsGate_available_dos_linear 0x9a 156 0 0 0 3 0
#define OsGate_query_xms 0x9a 157 0 0 0 3 0
#define OsGate_query_dpmi 0x9a 158 0 0 0 3 0
#define OsGate_query_dpmi16 0x9a 159 0 0 0 3 0
#define OsGate_get_bios_data 0x9a 160 0 0 0 3 0
#define OsGate_set_bios_data 0x9a 161 0 0 0 3 0
#define OsGate_hook_get_bios_data 0x9a 162 0 0 0 3 0
#define OsGate_hook_set_bios_data 0x9a 163 0 0 0 3 0
#define OsGate_hook_load_dos_exe 0x9a 164 0 0 0 3 0
#define OsGate_read_keyboard_serial 0x9a 165 0 0 0 3 0
#define OsGate_poll_keyboard_serial 0x9a 166 0 0 0 3 0
#define OsGate_write_dos_string 0x9a 167 0 0 0 3 0
#define OsGate_get_psp 0x9a 168 0 0 0 3 0
#define OsGate_set_psp 0x9a 169 0 0 0 3 0
#define OsGate_enter_dpmi 0x9a 170 0 0 0 3 0
#define OsGate_enter_dos16 0x9a 171 0 0 0 3 0
#define OsGate_enter_dos32 0x9a 172 0 0 0 3 0
#define OsGate_register_device 0x9a 173 0 0 0 3 0
#define OsGate_check_device 0x9a 174 0 0 0 3 0

#define OsGate_hook_open_app 0x9a 177 0 0 0 3 0
#define OsGate_hook_close_app 0x9a 178 0 0 0 3 0
#define OsGate_hook_load_exe 0x9a 179 0 0 0 3 0
#define OsGate_open_app 0x9a 180 0 0 0 3 0
#define OsGate_close_app 0x9a 181 0 0 0 3 0

#define OsGate_hook_enable_focus 0x9a 182 0 0 0 3 0
#define OsGate_hook_lost_focus 0x9a 183 0 0 0 3 0
#define OsGate_hook_got_focus 0x9a 184 0 0 0 3 0
#define OsGate_get_focus_thread 0x9a 185 0 0 0 3 0
#define OsGate_get_thread_focus_key 0x9a 186 0 0 0 3 0
#define OsGate_allocate_focus_linear 0x9a 187 0 0 0 3 0
#define OsGate_allocate_fixed_focus_mem 0x9a 188 0 0 0 3 0

#define OsGate_bios_pci_int 0x9a 189 0 0 0 3 0
#define OsGate_read_pci_byte 0x9a 190 0 0 0 3 0
#define OsGate_read_pci_word 0x9a 191 0 0 0 3 0
#define OsGate_read_pci_dword 0x9a 192 0 0 0 3 0
#define OsGate_write_pci_byte 0x9a 193 0 0 0 3 0
#define OsGate_write_pci_word 0x9a 194 0 0 0 3 0
#define OsGate_write_pci_dword 0x9a 195 0 0 0 3 0
#define OsGate_find_pci_class 0x9a 196 0 0 0 3 0
#define OsGate_find_pci_device 0x9a 197 0 0 0 3 0

#define OsGate_hook_init_disc 0x9a 198 0 0 0 3 0
#define OsGate_install_disc 0x9a 199 0 0 0 3 0
#define OsGate_register_disc_change 0x9a 200 0 0 0 3 0
#define OsGate_start_disc 0x9a 201 0 0 0 3 0
#define OsGate_stop_disc 0x9a 202 0 0 0 3 0
#define OsGate_set_disc_param 0x9a 203 0 0 0 3 0
#define OsGate_wait_for_disc_request 0x9a 204 0 0 0 3 0
#define OsGate_get_disc_request 0x9a 205 0 0 0 3 0
#define OsGate_poll_disc_request 0x9a 206 0 0 0 3 0
#define OsGate_disc_request_completed 0x9a 207 0 0 0 3 0
#define OsGate_new_disc_request 0x9a 208 0 0 0 3 0
#define OsGate_get_disc_request_array 0x9a 209 0 0 0 3 0
#define OsGate_open_drive 0x9a 214 0 0 0 3 0
#define OsGate_close_drive 0x9a 215 0 0 0 3 0
#define OsGate_flush_drive 0x9a 216 0 0 0 3 0
#define OsGate_get_drive_param 0x9a 217 0 0 0 3 0
#define OsGate_lock_sector 0x9a 218 0 0 0 3 0
#define OsGate_unlock_sector 0x9a 219 0 0 0 3 0
#define OsGate_modify_sector 0x9a 220 0 0 0 3 0
#define OsGate_new_sector 0x9a 221 0 0 0 3 0
#define OsGate_create_disc_seq 0x9a 222 0 0 0 3 0
#define OsGate_modify_seq_sector 0x9a 223 0 0 0 3 0
#define OsGate_perform_disc_seq 0x9a 224 0 0 0 3 0
#define OsGate_req_sector 0x9a 225 0 0 0 3 0
#define OsGate_define_sector 0x9a 226 0 0 0 3 0
#define OsGate_wait_for_sector 0x9a 227 0 0 0 3 0
#define OsGate_reset_drive 0x9a 228 0 0 0 3 0

#define OsGate_hook_init_file_system 0x9a 229 0 0 0 3 0
#define OsGate_register_file_system 0x9a 230 0 0 0 3 0
#define OsGate_define_media_check 0x9a 231 0 0 0 3 0
#define OsGate_demand_load_file_system 0x9a 232 0 0 0 3 0
#define OsGate_is_file_system_available 0x9a 233 0 0 0 3 0
#define OsGate_install_file_system 0x9a 234 0 0 0 3 0
#define OsGate_format_file_system 0x9a 235 0 0 0 3 0
#define OsGate_start_file_system 0x9a 236 0 0 0 3 0
#define OsGate_stop_file_system 0x9a 237 0 0 0 3 0
#define OsGate_get_file_list_entry 0x9a 238 0 0 0 3 0
#define OsGate_free_file_list_entry 0x9a 239 0 0 0 3 0
#define OsGate_cache_dir 0x9a 240 0 0 0 3 0
#define OsGate_insert_dir_entry 0x9a 241 0 0 0 3 0
#define OsGate_insert_file_entry 0x9a 242 0 0 0 3 0

#define OsGate_get_file_info 0x9a 247 0 0 0 3 0
#define OsGate_dupl_file_info 0x9a 248 0 0 0 3 0

#define OsGate_get_char_mask 0x9a 250 0 0 0 3 0
#define OsGate_register_video_mode 0x9a 251 0 0 0 3 0

#define OsGate_init_mouse 0x9a 252 0 0 0 3 0
#define OsGate_update_mouse 0x9a 253 0 0 0 3 0

#define OsGate_init_video_bitmap 0x9a 254 0 0 0 3 0

#define OsGate_allocate_small_kernel_mem 0x9a 255 0 0 0 3 0

#define OsGate_hide_sprite_line 0x9a 0 1 0 0 3 0
#define OsGate_show_sprite_line 0x9a 1 1 0 0 3 0

#define OsGate_start_com_port 0x9a 2 1 0 0 3 0
#define OsGate_stop_com_port 0x9a 3 1 0 0 3 0

#define OsGate_start_keyboard 0x9a 4 1 0 0 3 0
#define OsGate_stop_keyboard 0x9a 5 1 0 0 3 0

#define OsGate_start_floppy 0x9a 6 1 0 0 3 0
#define OsGate_stop_floppy 0x9a 7 1 0 0 3 0

#define OsGate_set_keyboard_state 0x9a 8 1 0 0 3 0

#define OsGate_add_wait 0x9a 10 1 0 0 3 0
#define OsGate_signal_wait 0x9a 11 1 0 0 3 0

#define OsGate_invert_mouse 0x9a 12 1 0 0 3 0

#define OsGate_load_dos_exe 0x9a 13 1 0 0 3 0

#define OsGate_get_exception_stack16 0x9a 14 1 0 0 3 0
#define OsGate_get_exception_stack32 0x9a 15 1 0 0 3 0

#define OsGate_lock_sys_env 0x9a 16 1 0 0 3 0
#define OsGate_unlock_sys_env 0x9a 17 1 0 0 3 0

#define OsGate_lock_proc_env 0x9a 18 1 0 0 3 0
#define OsGate_unlock_proc_env 0x9a 19 1 0 0 3 0

#define OsGate_erase_sectors 0x9a 20 1 0 0 3 0
#define OsGate_flush_sector 0x9a 21 1 0 0 3 0

#define OsGate_lock_disc_request 0x9a 22 1 0 0 3 0
#define OsGate_modify_disc_request 0x9a 23 1 0 0 3 0
#define OsGate_unlock_disc_request 0x9a 24 1 0 0 3 0

#define OsGate_get_net_buffer 0x9a 25 1 0 0 3 0
#define OsGate_get_ppp_buffer 0x9a 26 1 0 0 3 0

#define OsGate_allocate_multiple_physical 0x9a 27 1 0 0 3 0

#define OsGate_create_alias_sel16 0x9a 28 1 0 0 3 0
#define OsGate_create_alias_sel32 0x9a 29 1 0 0 3 0

#define OsGate_add_net_source_address 0x9a 30 1 0 0 3 0

#define OsGate_get_broadcast_buffer 0x9a 31 1 0 0 3 0
#define OsGate_send_broadcast 0x9a 32 1 0 0 3 0

#define OsGate_create_broadcast_ip 0x9a 33 1 0 0 3 0
#define OsGate_send_broadcast_ip 0x9a 34 1 0 0 3 0

#define OsGate_net_broadcast 0x9a 35 1 0 0 3 0
#define OsGate_add_dhcp_option 0x9a 36 1 0 0 3 0

#define OsGate_define_protocol_addr 0x9a 37 1 0 0 3 0

#define OsGate_register_swap_proc 0x9a 38 1 0 0 3 0

#define OsGate_is_irq_free 0x9a 39 1 0 0 3 0

#define OsGate_setup_irq_detect 0x9a 40 1 0 0 3 0
#define OsGate_poll_irq_detect 0x9a 41 1 0 0 3 0

#define OsGate_find_pci_cap 0x9a 42 1 0 0 3 0

#define OsGate_wait_for_signal_timeout 0x9a 43 1 0 0 3 0

#define OsGate_notify_ethernet_packet 0x9a 44 1 0 0 3 0

#define OsGate_init_usb_device 0x9a 45 1 0 0 3 0
#define OsGate_notify_usb_attach 0x9a 46 1 0 0 3 0

#define OsGate_request_shared_irq_handler 0x9a 47 1 0 0 3 0

#define OsGate_notify_usb_detach 0x9a 48 1 0 0 3 0

#define OsGate_add_com_port 0x9a 49 1 0 0 3 0

#define OsGate_hook_usb_attach 0x9a 50 1 0 0 3 0
#define OsGate_hook_usb_detach 0x9a 51 1 0 0 3 0

#define OsGate_set_mouse 0x9a 52 1 0 0 3 0
#define OsGate_set_mouse_limit 0x9a 53 1 0 0 3 0

#define OsGate_get_net_driver 0x9a 54 1 0 0 3 0

#define OsGate_create_usb_req 0x9a 55 1 0 0 3 0
#define OsGate_add_write_usb_control_req 0x9a 56 1 0 0 3 0
#define OsGate_add_write_usb_data_req 0x9a 57 1 0 0 3 0
#define OsGate_add_read_usb_data_req 0x9a 58 1 0 0 3 0
#define OsGate_add_usb_status_in_req 0x9a 59 1 0 0 3 0
#define OsGate_add_usb_status_out_req 0x9a 60 1 0 0 3 0
#define OsGate_start_usb_req 0x9a 61 1 0 0 3 0
#define OsGate_is_usb_req_started 0x9a 62 1 0 0 3 0
#define OsGate_is_usb_req_ready 0x9a 63 1 0 0 3 0
#define OsGate_get_usb_req_data 0x9a 64 1 0 0 3 0
#define OsGate_usb_req_done 0x9a 65 1 0 0 3 0
#define OsGate_close_usb_req 0x9a 66 1 0 0 3 0

#define OsGate_notify_ir_data 0x9a 67 1 0 0 3 0

#define OsGate_read_codec 0x9a 68 1 0 0 3 0
#define OsGate_write_codec 0x9a 69 1 0 0 3 0

#define OsGate_get_audio_dac_rate 0x9a 70 1 0 0 3 0
#define OsGate_set_audio_dac_rate 0x9a 71 1 0 0 3 0
#define OsGate_get_audio_adc_rate 0x9a 72 1 0 0 3 0
#define OsGate_set_audio_adc_rate 0x9a 73 1 0 0 3 0

#define OsGate_open_audio_out 0x9a 74 1 0 0 3 0
#define OsGate_close_audio_out 0x9a 75 1 0 0 3 0
#define OsGate_send_audio_out 0x9a 76 1 0 0 3 0

#define OsGate_get_audio_out_buf 0x9a 77 1 0 0 3 0
#define OsGate_post_audio_out_buf 0x9a 78 1 0 0 3 0
#define OsGate_is_audio_out_completed 0x9a 79 1 0 0 3 0

#define OsGate_set_module 0x9a 80 1 0 0 3 0
#define OsGate_create_module 0x9a 81 1 0 0 3 0
#define OsGate_free_module 0x9a 82 1 0 0 3 0
#define OsGate_reset_module 0x9a 83 1 0 0 3 0
#define OsGate_deref_module_handle 0x9a 84 1 0 0 3 0
#define OsGate_alias_module_handle 0x9a 85 1 0 0 3 0

#define OsGate_thread_to_sel 0x9a 86 1 0 0 3 0

#define OsGate_hook_shift_keys 0x9a 87 1 0 0 3 0

#define OsGate_stop_usb_req 0x9a 88 1 0 0 3 0

#define OsGate_get_usb_info 0x9a 89 1 0 0 3 0

#define OsGate_reserve_com_line 0x9a 90 1 0 0 3 0

#define OsGate_device_set_dtr 0x9a 91 1 0 0 3 0
#define OsGate_device_reset_dtr 0x9a 92 1 0 0 3 0

#define OsGate_wait_for_line_state 0x9a 93 1 0 0 3 0
#define OsGate_get_line_state 0x9a 94 1 0 0 3 0

#define OsGate_create_proc_handle 0x9a 95 1 0 0 3 0
#define OsGate_deref_proc_handle 0x9a 96 1 0 0 3 0

#define OsGate_notify_time_drift 0x9a 97 1 0 0 3 0
#define OsGate_set_options 0x9a 98 1 0 0 3 0

#define OsGate_clone_handle_mem 0x9a 99 1 0 0 3 0
#define OsGate_clone_app 0x9a 100 1 0 0 3 0

#define OsGate_get_apic_id 0x9a 101 1 0 0 3 0

#define OsGate_enter_int 0x9a 102 1 0 0 3 0
#define OsGate_leave_int 0x9a 103 1 0 0 3 0

#define OsGate_debug_exception 0x9a 104 1 0 0 3 0

#define OsGate_create_processor 0x9a 106 1 0 0 3 0

#define OsGate_shutdown 0x9a 107 1 0 0 3 0

#define OsGate_locked_debug_exception 0x9a 108 1 0 0 3 0

#define OsGate_lock_task 0x9a 109 1 0 0 3 0
#define OsGate_unlock_task 0x9a 110 1 0 0 3 0

#define OsGate_get_processor 0x9a 112 1 0 0 3 0

#define OsGate_resume_processor 0x9a 113 1 0 0 3 0
#define OsGate_start_processor 0x9a 114 1 0 0 3 0

#define OsGate_preempt_processor 0x9a 115 1 0 0 3 0
#define OsGate_do_preempt_processor 0x9a 116 1 0 0 3 0

#define OsGate_reload_sys_timer 0x9a 117 1 0 0 3 0
#define OsGate_start_sys_timer 0x9a 118 1 0 0 3 0

#define OsGate_is_net_address_valid 0x9a 119 1 0 0 3 0

#define OsGate_get_acpi_table 0x9a 120 1 0 0 3 0

#define OsGate_allocate_debug_local_linear 0x9a 121 1 0 0 3 0

#define OsGate_process_key_scan 0x9a 122 1 0 0 3 0

#define OsGate_get_debug_thread_sel 0x9a 123 1 0 0 3 0

#define OsGate_find_pci_class_all 0x9a 124 1 0 0 3 0

#define OsGate_show_proc_debug 0x9a 125 1 0 0 3 0

#define OsGate_get_pci_irq 0x9a 126 1 0 0 3 0

#define OsGate_start_smp_debug 0x9a 127 1 0 0 3 0
#define OsGate_send_eoi 0x9a 128 1 0 0 3 0

#define OsGate_add_debug_core 0x9a 129 1 0 0 3 0

#define OsGate_send_nmi 0x9a 130 1 0 0 3 0

#define OsGate_add_printer 0x9a 131 1 0 0 3 0
