/* Header for text, do not edit it. */

/* Text for tags */
static char *tags[]                         = {  text_tag1_empty,   /* Tag 1 text (empty) */
												 text_tag2_empty,   /* Tag 2 text (empty) */
												 text_tag3_empty,   /* Tag 3 text (empty) */
												 text_tag4_empty,   /* Tag 4 text (empty) */
												 text_tag5_empty,   /* Tag 5 text (empty) */
												 text_tag6_empty,   /* Tag 6 text (empty) */
												 text_tag7_empty,   /* Tag 7 text (empty) */
										 		 text_tag8_empty,   /* Tag 8 text (empty) */
											 	 text_tag9_empty,   /* Tag 9 text (empty) */
};

/* Text for tags with windows open */
static char *usedtags[]                     = {  text_tag1_used,   /* Tag 1 text (used) */
												 text_tag2_used,   /* Tag 2 text (used) */
												 text_tag3_used,   /* Tag 3 text (used) */
												 text_tag4_used,   /* Tag 4 text (used) */
												 text_tag5_used,   /* Tag 5 text (used) */
												 text_tag6_used,   /* Tag 6 text (used) */
												 text_tag7_used,   /* Tag 7 text (used) */
												 text_tag8_used,   /* Tag 8 text (used) */
												 text_tag9_used,   /* Tag 9 text (used) */
};

/* Length of tags, text inside doesn't matter
 * If you're increasing the number of tags, also increase this.
 */
static char *ftags[]                        = {  "aaaaaaaaaaaaaaa",
												 "bbbbbbbbbbbbbbb",
												 "ccccccccccccccc",
												 "ddddddddddddddd",
												 "eeeeeeeeeeeeeee",
												 "fffffffffffffff",
												 "ggggggggggggggg",
										 		 "hhhhhhhhhhhhhhh",
											 	 "iiiiiiiiiiiiiii",
};

/* Same for layouts */
static char *flayouts[]                      = { "aaaaaaaaaaaaaaa",
                                                 "bbbbbbbbbbbbbbb",
                                                 "ccccccccccccccc",
                                                 "ddddddddddddddd",
                                                 "eeeeeeeeeeeeeee",
                                                 "fffffffffffffff",
                                                 "ggggggggggggggg",
                                                 "hhhhhhhhhhhhhhh",
                                                 "iiiiiiiiiiiiiii",
                                                 "jjjjjjjjjjjjjjj",
                                                 "kkkkkkkkkkkkkkk",
                                                 "mmmmmmmmmmmmmmm",
                                                 "nnnnnnnnnnnnnnn",
                                                 "ooooooooooooooo",
                                                 "ppppppppppppppp",
                                                 "qqqqqqqqqqqqqqq",
};

/* Text for layouts */
static Layout layouts[]                 = {
    { text_layout1,          tile,    { -1, -1, splitvertical, toptobottom, toptobottom, 0, NULL } }, /* Tiling */
	{ text_layout2,          NULL,    {0} }, /* Floating */
	{ text_layout3,          tile,    { -1, -1, nosplit, monocle, monocle, 0, NULL } }, /* Monocle */
	{ text_layout4,          ngrid,   {0} }, /* Grid */
	{ text_layout5,          tile,    { -1, -1, splitvertical, toptobottom, monocle, 0, NULL } }, /* Deck */
	{ text_layout6,          tile,    { -1, -1, splithorizontal, lefttoright, toptobottom, 0, NULL } }, /* Centered Master */
	{ text_layout7,          tile,    { -1, -1, splitvertical, lefttoright, tatami, 0, NULL } }, /* Tatami */
	{ text_layout8,			 tile,	  { -1, -1, nosplit, spiral, spiral, 0, NULL } }, /* Spiral */
	{ text_layout9,          tile,    { -1, -1, nosplit, dwindle, dwindle, 0, NULL } }, /* Dwindle */
	{ text_layout10,         tile,    { -1, -1, splithorizontal, lefttoright, lefttoright, 0, NULL } }, /* Bottom Stack Vertical */
	{ text_layout11,         tile,    { -1, -1, splithorizontal, lefttoright, toptobottom, 0, NULL } }, /* Bottom Stack Horizontal */
	{ text_layout12,         hgrid,   {0} }, /* Horizontal Grid */
	{ text_layout13,         tile,    { -1, -1, nosplit, dynamicgrid, dynamicgrid, 0, NULL } }, /* Dynamic Grid */
	{ text_layout14,         custom,  {0} }, /* Custom */
	{ text_layout15,         empty,   {0} }, /* Empty */
	{ NULL,		             NULL, {0} }, /* Reset to layout 1 */
};
