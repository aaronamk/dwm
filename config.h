#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int user_bh            = 16;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char background[]      = "#000000";
static const char color0[]          = "#282828";
static const char color1[]          = "#928374";
static const char color2[]          = "#cc241d";
static const char color3[]          = "#fb4934";
static const char color4[]          = "#98971a";
static const char color5[]          = "#b8bb26";
static const char color6[]          = "#d79921";
static const char color7[]          = "#fabd2f";
static const char color8[]          = "#458588";
static const char color9[]          = "#83a598";
static const char color10[]         = "#b16286";
static const char color11[]         = "#d3869b";
static const char color12[]         = "#689d6a";
static const char color13[]         = "#8ec07c";
static const char color14[]         = "#a89984";
static const char color15[]         = "#ebdbb2";
static const char *colors[][3]      = {
	/*               fg          bg          border   */
	[SchemeNorm] = { color1,     background, color1 },
	[SchemeInv]  = { background, color1,     color1 },
	[SchemeSel]  = { background, color7,     color7 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance             title               tags mask  iscentered  isfloating  isterminal  noswallow  monitor  scratch key */
	{ "st",      NULL,                NULL,               0,         1,          0,          1,          -1,        -1,      0 },
	{ "Sakura",  NULL,                NULL,               0,         1,          0,          1,          -1,        -1,      0 },
	{ NULL,      NULL,                "scratch_term",     0,         1,          1,          1,          -1,        -1,     'h'},
	{ NULL,      NULL,                "music_browser",    0,         1,          1,          0,           1,        -1,     'm'},
	{ NULL,      NULL,                "audio_mixer",      0,         1,          1,          0,           1,        -1,      0 },
	{ NULL,      NULL,                "session_options",  0,         1,          1,          0,           1,        -1,      0 },
	{ NULL,      NULL,                "name_screenshot",  0,         1,          1,          0,           1,        -1,      0 },
	{ NULL,      NULL,                "clipboard",        0,         1,          1,          0,           1,        -1,      0 },
	{ NULL,      NULL,                "Event Tester",     0,         0,          1,          0,           1,        -1,      0 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|",        tile },    /* first entry is default */
	{ "~",        NULL },    /* no layout function means floating behavior */
	{ "[]",       monocle },
	{ "-",        bstackhoriz },
	{ "||",       centeredmaster },
};

/* key definitions */
#define SUPER Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ SUPER,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ SUPER|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ SUPER|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ SUPER|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", background, "-nf", color14, "-sb", color7, "-sf", color15, NULL };
static const char *termcmd[]  = { "st", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratch_term[] = {"h", "st", "-g", "130x50", "-t", "scratch_term", "-e", "vifm", NULL};
static const char *music_browser[] = {"m", "st", "-g", "130x50", "-t", "music_browser", "-e", "ncmpcpp", NULL};

static Key keys[] = {
	/* modifier                     key             function        argument */
	{ SUPER,                        XK_p,           spawn,                {.v = dmenucmd } },
	{ SUPER,                        XK_b,           togglebar,            {0} },
	{ SUPER,                        XK_j,           focusstack,           {.i = +1 } },
	{ SUPER,                        XK_k,           focusstack,           {.i = -1 } },
	{ SUPER|ShiftMask,              XK_j,           pushdown,             {0} },
	{ SUPER|ShiftMask,              XK_k,           pushup,               {0} },
	{ SUPER,                        XK_i,           incnmaster,           {.i = +1 } },
	{ SUPER,                        XK_d,           incnmaster,           {.i = -1 } },
	{ SUPER,                        XK_minus,       setmfact,             {.f = -0.025} },
	{ SUPER,                        XK_equal,       setmfact,             {.f = +0.025} },
	{ SUPER|ShiftMask,              XK_minus,       setcfact,             {.f = -0.25} },
	{ SUPER|ShiftMask,              XK_equal,       setcfact,             {.f = +0.25} },
	{ SUPER,                        XK_space,       zoom,                 {0} },
	{ SUPER,                        XK_Tab,         view,                 {0} },
	{ SUPER,                        XK_q,           killclient,           {0} },
	{ SUPER,                        XK_t,           setlayout,            {.v = &layouts[0]} },
	{ SUPER|ShiftMask,              XK_t,           setlayout,            {.v = &layouts[3]} },
	{ SUPER,                        XK_c,           setlayout,            {.v = &layouts[4]} },
	{ SUPER,                        XK_f,           setlayout,            {.v = &layouts[2]} },
	{ SUPER|ShiftMask,              XK_f,           togglefakefullscreen, {0} },
	{ SUPER|ShiftMask,              XK_space,       togglefloating,       {0} },
	{ SUPER,                        XK_h,           togglescratch,        {.v = scratch_term } },
	{ SUPER,                        XF86XK_AudioPlay,  togglescratch,        {.v = music_browser } },
	{ SUPER,                        XK_0,           view,                 {.ui = ~0 } },
	{ SUPER|ShiftMask,              XK_0,           tag,                  {.ui = ~0 } },
	{ SUPER,                        XK_l,           focusmon,             {.i = +1 } },
	{ SUPER|ShiftMask,              XK_l,           tagmon,               {.i = +1 } },
	{ SUPER,                        XK_period,      shiftview,            {.i = +1 } },
	{ SUPER,                        XK_comma,       shiftview,            {.i = -1 } },
	TAGKEYS(                        XK_1,                                 0)
	TAGKEYS(                        XK_2,                                 1)
	TAGKEYS(                        XK_3,                                 2)
	TAGKEYS(                        XK_4,                                 3)
	TAGKEYS(                        XK_5,                                 4)
	TAGKEYS(                        XK_6,                                 5)
	TAGKEYS(                        XK_7,                                 6)
	TAGKEYS(                        XK_8,                                 7)
	TAGKEYS(                        XK_9,                                 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         SUPER,          Button1,        movemouse,      {0} },
	{ ClkClientWin,         SUPER,          Button2,        togglefloating, {0} },
	{ ClkClientWin,         SUPER,          Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            SUPER,          Button1,        tag,            {0} },
	{ ClkTagBar,            SUPER,          Button3,        toggletag,      {0} },
};
