#ifndef __ASMPKT_H
#define __ASMPKT_H

#if (DOSX == 0)                                  /* for real-mode targets   */
 #if __WATCOMC__ > 1280
  extern void far cdecl pkt_receiver_rm (void);        /* in asmpkt.asm/asmpkt2.s */
 #else
  extern void far pkt_receiver_rm (void);        /* in asmpkt.asm/asmpkt2.s */
 #endif

#elif defined(__WATCOMC__) && (DOSX & DOS4GW) || /* in asmpkt4.asm */ \
      defined(__BORLAND386__) && (DOSX == WDOSX)

  extern WORD             asmpkt_size_chk;
  extern struct pkt_info *asmpkt_inf;

  extern void pkt_receiver4_start();
  extern void pkt_receiver4_rm();
  extern void pkt_receiver4_end();

#endif

#endif

