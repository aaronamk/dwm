/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int user_bh            = 16;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#504945";
static const char col_gray3[]       = "#bdae93";
static const char col_gray4[]       = "#fbf1c7";
static const char col_cyan[]        = "#fabd2f";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray1, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance             title               tags mask  iscentered  isfloating  isterminal  noswallow  monitor */
	{ "st",      NULL,                NULL,               0,         1,          0,          1,          -1,        -1 },
	{ "Sakura",  NULL,                NULL,               0,         1,          0,          1,          -1,        -1 },
	{ NULL,      NULL,                "music_browser",    0,         1,          1,          0,           1,        -1 },
	{ NULL,      NULL,                "audio_mixer",      0,         1,          1,          0,           1,        -1 },
	{ NULL,      NULL,                "session_options",  0,         1,          1,          0,           1,        -1 },
	{ NULL,      NULL,                "name_screenshot",  0,         1,          1,          0,           1,        -1 },
	{ NULL,      NULL,                "clipboard",        0,         1,          1,          0,           1,        -1 },
	{ NULL,      NULL,                "Event Tester",     0,         0,          1,          0,           1,        -1 }, /* xev */
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ SUPER,                        XK_p,      spawn,                {.v = dmenucmd } },
	{ SUPER,                        XK_b,      togglebar,            {0} },
	{ SUPER,                        XK_j,      focusstack,           {.i = +1 } },
	{ SUPER,                        XK_k,      focusstack,           {.i = -1 } },
	{ SUPER,                        XK_i,      incnmaster,           {.i = +1 } },
	{ SUPER,                        XK_d,      incnmaster,           {.i = -1 } },
	{ SUPER,                        XK_minus,  setmfact,             {.f = -0.025} },
	{ SUPER,                        XK_equal,  setmfact,             {.f = +0.025} },
	{ SUPER|ShiftMask,              XK_minus,  setcfact,             {.f = -0.25} },
	{ SUPER|ShiftMask,              XK_equal,  setcfact,             {.f = +0.25} },
	{ SUPER,                        XK_space,  zoom,                 {0} },
	{ SUPER,                        XK_Tab,    view,                 {0} },
	{ SUPER,                        XK_q,      killclient,           {0} },
	{ SUPER,                        XK_t,      setlayout,            {.v = &layouts[0]} },
	{ SUPER|ShiftMask,              XK_t,      setlayout,            {.v = &layouts[3]} },
	{ SUPER,                        XK_f,      setlayout,            {2} },
	{ SUPER|ShiftMask,              XK_f,      togglefakefullscreen, {0} },
	{ SUPER|ShiftMask,              XK_space,  togglefloating,       {0} },
	{ SUPER,                        XK_0,      view,                 {.ui = ~0 } },
	{ SUPER|ShiftMask,              XK_0,      tag,                  {.ui = ~0 } },
	{ SUPER,                        XK_l,      focusmon,             {.i = +1 } },
	{ SUPER|ShiftMask,              XK_l,      tagmon,               {.i = +1 } },
	TAGKEYS(                        XK_1,                            0)
	TAGKEYS(                        XK_2,                            1)
	TAGKEYS(                        XK_3,                            2)
	TAGKEYS(                        XK_4,                            3)
	TAGKEYS(                        XK_5,                            4)
	TAGKEYS(                        XK_6,                            5)
	TAGKEYS(                        XK_7,                            6)
	TAGKEYS(                        XK_8,                            7)
	TAGKEYS(                        XK_9,                            8)
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
