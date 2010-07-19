/*
 * Internal video miniport interfaces.
 */

/* Video mode description structure */
typedef struct {
    ULONG                   ulModeId;           /* Internal mode ID */
    BOOLEAN                 bValid;             /* Flag to filter out modes */
    VIDEO_MODE_INFORMATION  modeInformation;    /* NT style video mode data */
} VIDEOMP_MODE, *PVIDEOMP_MODE;


/* The device extension - private miniport data */
typedef struct {
    PVOID               IoPorts;                /* I/O ports mapping */
    PVOID               FrameAddress;           /* Framebuffer mapping */
    PHYSICAL_ADDRESS    PhysicalFrameAddress;   /* Physical FB address */
    ULONG               FramebufLen;            /* Framebuffer length */
    ULONG               CurrentModeNumber;      /* Current mode index */
    ULONG               NumValidModes;          /* Number of valid modes */
    ULONG               NumMonitors;            /* Number of attached monitors */
    PUCHAR              IOAddrVGA;              /* VGA I/O ports mapping */
} HW_DEV_EXT, *PHW_DEV_EXT;

/* Variables defined in modedata.c */
extern VIDEOMP_MODE     VideoModes[];
extern ULONG            ulAllModes;
