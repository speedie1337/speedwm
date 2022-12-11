/* Color/Alpha settings
 * You probably don't need to change these unless you want special opacity settings.
 *
 * Once you're done with your edits, run 'make clean install'.
 */

/* Misc color options */
static char *colors[][3]                      = {
	[SchemeBar]                               = { col_textnorm,         col_background,       NULL				   },
	[SchemeTagsNorm]                          = { col_textnorm,         col_background,       NULL				   },
	[SchemeTagsSel]                           = { col_textsel,			col_titlesel,		  NULL				   },
	[SchemeTagsUrg]                           = { col_tagurgent,        col_tagurgent,		  NULL				   },
	[SchemeBorderNorm]                        = { col_textnorm,         col_windowbordernorm, col_windowbordernorm },
	[SchemeBorderSel]                         = { col_textsel,          col_windowbordersel,  col_windowbordersel  }, 
	[SchemeBorderUrg]                         = { col_textnorm,         col_windowbordersel,  col_windowborderurg  },
	[SchemeStatus]                            = { col_textnorm,			col_background,       NULL                 },
	[SchemeTitleNorm]                         = { col_textnorm,         col_titlenorm,        NULL				   },
	[SchemeTitleSel]                          = { col_textsel,          col_titlesel,         NULL                 },
	[SchemeTitleHidden]                       = { col_titlesel,         col_background,       NULL				   },
	[SchemeLayout]                            = { col_layouttext,       col_layout,           NULL				   },
	[SchemeSystray]                           = { col_systray,          col_systray,          NULL				   },
/*                                                text                  background            window border        */
};

/* Colors for the status if powerline status is used */
static char *powerlinecolors[][2]             = {
	[SchemeBar]                               = { col_powerline0_text,  col_powerline0  },
	[SchemeTagsNorm]                          = { col_powerline1_text,  col_powerline1  },
	[SchemeTagsSel]                           = { col_powerline2_text,	col_powerline2	},
	[SchemeTagsUrg]                           = { col_powerline3_text,  col_powerline3	},
	[SchemeBorderNorm]                        = { col_powerline4_text,  col_powerline4  },
	[SchemeBorderSel]                         = { col_powerline5_text,  col_powerline5  }, 
	[SchemeBorderUrg]                         = { col_powerline6_text,  col_powerline6  },
	[SchemeStatus]                            = { col_powerline7_text,	col_powerline7  },
};

/* Alpha to use for the powerlines */
static unsigned int powerlinealphas[][3]      = {
       /*                                         fg             bg                 unused           */
       [SchemeBar]                            = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeTagsNorm]                       = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeTagsSel]                        = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeTagsUrg]                        = { opaque,        powerlineopacity,  powerlineopacity },
       [SchemeStatus]                         = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeBorderNorm]                     = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeBorderSel]                      = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeBorderUrg]                      = { opaque,        powerlineopacity,  powerlineopacity },
	   [SchemeStatus]                         = { opaque,        powerlineopacity,  powerlineopacity },
};

/* Colors for the status bar (.Xresources) */
static char *colstatus[]                      = {
       col_status0,
       col_status1,
       col_status2,
       col_status3,
       col_status4,
       col_status5,
       col_status6,
       col_status7,
       col_status8,
       col_status9,
       col_status10,
       col_status11,
       col_status12,
       col_status13,
       col_status14,
       col_status15,
};

/* Alpha to use for opacity */
static unsigned int alphas[][3]         = {
       /*                                         fg             bg                 border/unused    */
       [SchemeBar]                            = { opaque,        baropacity,        baropacity       },
	   [SchemeTitleNorm]                      = { opaque,        normtitleopacity,  normtitleopacity },
	   [SchemeTitleSel]                       = { opaque,        seltitleopacity,   seltitleopacity  },
       [SchemeTitleHidden]                    = { opaque,        hiddenopacity,     hiddenopacity    },
       [SchemeLayout]                         = { opaque,        layoutopacity,     layoutopacity    },
       [SchemeStatus]                         = { opaque,        statusopacity,     statusopacity    },
	   [SchemeBorderNorm]                     = { opaque,        normborderopacity, selborderopacity },
	   [SchemeBorderSel]                      = { opaque,        normborderopacity, selborderopacity },
	   [SchemeBorderUrg]                      = { opaque,        normborderopacity, selborderopacity },
	   [SchemeTagsNorm]                       = { tagselopacity, tagselopacity,     tagnormopacity   },
	   [SchemeTagsSel]                        = { tagselopacity, tagselopacity,     tagnormopacity   },
	   [SchemeTagsUrg]                        = { tagselopacity, tagselopacity,     tagnormopacity   },
	   [SchemeSystray]                        = { opaque,        systrayopacity,    systrayopacity   },
};


/* Colors to use for tags */
static char *tagsel[][2]                      = {
	{ col_tag1_text, col_tag1 }, 
	{ col_tag2_text, col_tag2 },
	{ col_tag3_text, col_tag3 },
	{ col_tag4_text, col_tag4 },
	{ col_tag5_text, col_tag5 },
	{ col_tag6_text, col_tag6 },
	{ col_tag7_text, col_tag7 },
	{ col_tag8_text, col_tag8 },
	{ col_tag9_text, col_tag9 },
   /* foreground     background */
};

/* Alpha for tags */
static const unsigned int tagalpha[]          = {
		tagselopacity, 
		tagnormopacity,
};
