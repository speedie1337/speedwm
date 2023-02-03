/* Signal definitions
 * Signum must be greater than 0
 * Trigger signals using `speedwm-utils -exec <signum>"`
 *
 * Once you're done with your edits, run 'make clean install'. */
static Signal signals[] = {
	/* signum       function           argument  */
	{ 1,            setlayout,            {.v = &layouts[0]} }, /* Tiling layout */
	{ 2,            setlayout,            {.v = &layouts[1]} }, /* Floating layout */
	{ 3,            setlayout,            {.v = &layouts[2]} }, /* Monocle layout */
	{ 4,            setlayout,            {.v = &layouts[3]} }, /* Grid layout */
	{ 5,            setlayout,            {.v = &layouts[4]} }, /* Deck layout */
	{ 6,            setlayout,            {.v = &layouts[5]} }, /* Centered Master layout */
	{ 7,            setlayout,            {.v = &layouts[6]} }, /* Tatami layout */
	{ 8,            setlayout,            {.v = &layouts[7]} }, /* Fibonacci Spiral layout */
	{ 9,            setlayout,            {.v = &layouts[8]} }, /* Fibonacci Dwindle layout */
	{ 10,           setlayout,            {.v = &layouts[9]} }, /* Bottom Stack layout */
	{ 11,           setlayout,            {.v = &layouts[10]} }, /* Horizontal Bottom Stack layout */
	{ 12,           setlayout,            {.v = &layouts[11]} }, /* Horizontal Grid layout */
	{ 13,           setlayout,            {.v = &layouts[12]} }, /* Dynamic Grid layout */
	{ 14,           setlayout,            {.v = &layouts[13]} }, /* Custom layout */
	{ 15,           set_s_layout,         {.v = &layouts[13]} },
	{ 16,           cyclelayout,          {.i = +1 } },
	{ 17,           cyclelayout,          {.i = -1 } },
	{ 18,           setmfact,             {.f = +0.05} },
	{ 19,           setmfact,             {.f = -0.05} },
	{ 20,           togglesticky,         {0} },
	{ 21,           togglebar,            {0} },
	{ 22,           togglefullscr,        {0} },
	{ 23,           togglefloating,       {0} },
	{ 24,           zoom,                 {0} },
	{ 25,           reorganizetags,       {0} },
	{ 26,           quit,                 {0} },
	{ 27,           quit,                 {1} },
	{ 28,           show,                 {0} },
	{ 29,           hide,                 {0} },
	{ 30,           killclient,           {0} },
	{ 31,           inplacerotate,        {.i = +1 } },
	{ 32,           inplacerotate,        {.i = -1 } },
	{ 33,           incmastercount,       {.i = +1 } },
	{ 34,           incmastercount,       {.i = -1 } },
	{ 35,           focusmaster,          {0} },
	{ 36,           focusstackvis,        {.i = +1 } },
	{ 37,           focusstackvis,        {.i = -1 } },
	{ 38,           focusstackhid,        {.i = +1 } },
	{ 39,           focusstackhid,        {.i = -1 } },
	{ 40,           toggleopacity,        {0} },
	{ 41,           setcfact,             {.f = +0.05 } },
	{ 42,           setcfact,             {.f = -0.05 } },
	{ 43,           viewtoleft,           {0} },
	{ 44,           viewtoright,          {0} },
	{ 45,           defaultgaps,          {0} },
	{ 46,           togglegaps,           {0} },
	{ 47,           incrgaps,             {.i = +1 } },
	{ 48,           incrgaps,             {.i = -1 } },
	{ 49,           incrigaps,            {.i = +1 } },
	{ 50,           incrigaps,            {.i = -1 } },
	{ 51,           incrogaps,            {.i = +1 } },
	{ 52,           incrogaps,            {.i = -1 } },
	{ 53,           killunsel,            {0} },
	{ 54,           focusmon,             {.i = +1 } },
	{ 55,           focusmon,             {.i = -1 } },
    { 56,           scratchpad_show,      {0} },
    { 57,           scratchpad_hide,      {0} },
    { 58,           scratchpad_remove,    {0} },
	{ 59,           reset_layout,         {0} },
	{ 60,           resetmastercount,     {0} },
    #if USESYSTRAY
	{ 61,           togglesystray,        {0} },
    #endif
	{ 62,           hideall,              {0} },
	{ 63,           showall,              {0} },
	{ 64,           reset_mfact,          {0} },
    #if USEXRESOURCES
	{ 65,           reloadcolors,         {0} },
    #endif
	{ 66,           viewtoleft_vacant,    {0} },
	{ 67,           viewtoright_vacant,   {0} },
	{ 68,           togglebartags,        {0} },
	{ 69,           togglebaremptytags,   {0} },
    /*
	{ 70,           Unused,               {0} },
	{ 71,           Unused,               {0} },
	{ 72,           Unused,               {0} },
	{ 73,           Unused,               {0} },
	{ 74,           Unused,               {0} },
	{ 75,           Unused,               {0} },
	{ 76,           Unused,               {0} },
    */
	{ 77,           togglebartitle,       {0} },
	{ 78,           togglebarunseltitle,  {0} },
	{ 79,           togglebarlt,          {0} },
	{ 80,           togglebarstatus,      {0} },
	{ 81,           togglebarfloat,       {0} },
	{ 82,           togglebarsticky,      {0} },
	{ 83,           togglebaricon,        {0} },
    /*
	{ 84,           Unused,               {0} },
    { 85,           Unused,               {0} },
	{ 86,           Unused,               {0} },
    */
	{ 87,           setbarheight,         {.i = +1 } },
	{ 88,           setbarheight,         {.i = -1 } },
	{ 89,           resetbarheight,       {0} },
	{ 90,           setbarpaddingov,      {.i = +1 } },
	{ 91,           setbarpaddingov,      {.i = -1 } },
	{ 92,           setbarpaddingoh,      {.i = +1 } },
	{ 93,           setbarpaddingoh,      {.i = -1 } },
	{ 94,           setbarpadding,        {.i = +1 } },
	{ 95,           setbarpadding,        {.i = -1 } },
	{ 96,           togglebarpaddingov,   {0} },
	{ 97,           togglebarpaddingoh,   {0} },
	{ 98,           togglebarpadding,     {0} },
	{ 99,           resetbarpaddingov,    {0} },
	{ 100,          resetbarpaddingoh,    {0} },
	{ 101,          resetbarpadding,      {0} },
	{ 102,          incstackcount,        {.i = +1 } },
	{ 103,          incstackcount,        {.i = -1 } },
	{ 104,          rotatelayoutaxis,     {.i = +1 } },
	{ 105,          rotatelayoutaxis,     {.i = +2 } },
	{ 106,          rotatelayoutaxis,     {.i = +3 } },
	{ 107,          rotatelayoutaxis,     {.i = +4 } },
	{ 108,          rotatelayoutaxis,     {.i = -1 } },
	{ 109,          rotatelayoutaxis,     {.i = -2 } },
	{ 110,          rotatelayoutaxis,     {.i = -3 } },
	{ 111,          rotatelayoutaxis,     {.i = -4 } },
	{ 112,          mirrorlayout,         {0} },
	{ 113,          setlayout,            {.v = &layouts[14]} }, /* Empty layout */
    { 114,          setbpgaps,            {.i = +1 } },
    { 115,          setbpgaps,            {.i = -1 } },
    { 116,          togglemark,           {0} },
    { 117,          swapfocus,            {0} },
    { 118,          swapclient,           {0} },
    { 119,          centerwindow,         {0} },
    { 120,          toggleborder,         {0} },
    { 121,          resetbpgaps,          {0} },
};
