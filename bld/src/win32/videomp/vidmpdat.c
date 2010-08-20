/*
 * Video miniport static data.
 */

#include <miniport.h>
#include <ntddvdeo.h>
#include <video.h>
#include "videomp.h"

#if defined(ALLOC_PRAGMA)
#pragma data_seg("PAGE")
#endif

/*
 * Legacy resources claimed by the device.
 *
 *               RangeStart        RangeLength
 *               |                 |      RangeInIoSpace
 *               |                 |      |  RangeVisible
 *         +-----+-----+           |      |  |  RangeShareable
 *        low         high         |      |  |  |  RangePassive
 *         v           v           v      v  v  v  v
 */
VIDEO_ACCESS_RANGE  LegacyRanges[] = {
    { 0x000001CC, 0x00000000, 0x00000002, 1, 1, 1, 0 }, /* 0x1CC-0x1CD */
    { 0x000003B0, 0x00000000, 0x0000000C, 1, 1, 1, 0 }, /* 0x3B0-0x3BB */
    { 0x000003C0, 0x00000000, 0x00000020, 1, 1, 1, 0 }, /* 0x3C0-0x3DF */
    { 0x000A0000, 0x00000000, 0x00020000, 0, 0, 1, 0},  /* 0xA0000-0xBFFFF */
};

ULONG   ulNumLegacyRanges = sizeof( LegacyRanges ) / sizeof( VIDEO_ACCESS_RANGE );

/* Mode table */
VIDEOMP_MODE    VideoModes[] = {
{
  0,//v1280_1K_8,                       /* Mode number */
  FALSE,                                /* Mode valid flag */
    {
      sizeof( VIDEO_MODE_INFORMATION ), /* Size of the mode info struct */
      0,                                /* Mode index (filled in later */
      1152,                             /* Horizontal resolution in pixels */
      864,                              /* Vertical resolution in pixels */
      1152,                             /* Framebuffer stride in bytes */
      1,                                /* Number of video memory planes */
      8,                                /* Bits per plane */
      60,                               /* Refresh frequency in Hz */
      320,                              /* Horizontal screen size in mm */
      240,                              /* Vertical screen size in mm */
      8,                                /* Red pixels in DAC */
      8,                                /* Green pixels in DAC */
      8,                                /* Blue pixels in DAC */
      0x00000000,                       /* Red pixel mask */
      0x00000000,                       /* Green pixel mask */
      0x00000000,                       /* Blue pixel mask */
      VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
          VIDEO_MODE_MANAGED_PALETTE    /* Mode flags */
   }
},
{
  0x117,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      2048,
      1,
      16,
      60,
      320,
      240,
      8,
      8,
      8,
      0x0000f800,
      0x000007e0,
      0x0000001f,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS
    }
},
{
  0x105,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      1024,
      1,
      8,
      60,
      320,
      240,
      6,
      6,
      6,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
          VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  0,//v1152_900_16,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1152,
      900,
      2304,
      1,
      16,
      60,
      320,
      240,
      8,
      8,
      8,
      0x0000fc00,
      0x000003f0,
      0x0000000f,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS
    }
},
{
  0x114,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      800,
      600,
      1600,
      1,
      16,
      60,
      320,
      240,
      8,
      8,
      8,
      0x0000f800,
      0x000007e0,
      0x0000001f,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS
    }
},
{
  0x103,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      800,
      600,
      800,
      1,
      8,
      60,
      320,
      240,
      6,
      6,
      6,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
          VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  0x111,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      480,
      1280,
      1,
      16,
      60,
      320,
      240,
      8,
      8,
      8,
      0x0000f800,
      0x000007e0,
      0x0000001f,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS
    }
},
{
  0x101,
  FALSE,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      480,
      640,
      1,
      8,
      60,
      320,
      240,
      6,
      6,
      6,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
          VIDEO_MODE_MANAGED_PALETTE
    }
}
};

ULONG   ulAllModes = sizeof( VideoModes ) / sizeof( VIDEOMP_MODE );


#if defined( ALLOC_PRAGMA )
#pragma data_seg()
#endif
