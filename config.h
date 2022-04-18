/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int scalepreview       = 4;        /* tag preview scaling */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "FiraCode Nerd Font:size=10", "Noto Color Emoji:size=10" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";
static const char col_fg[]          = "#dddddd";
static const char col_bg[]          = "#282828";
static const char col_border[]      = "#282828";
static const char *colors[][3]      = {
	/*                fg       bg      border   */
	[SchemeNorm] = { col_fg, col_bg, col_bg },
	[SchemeSel]  = { col_fg, col_border, col_border },
	[SchemeStatus]  = { col_fg, col_bg,  col_border  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_fg, col_bg,  col_border }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { col_fg, col_bg,  col_border }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { col_fg, col_bg,  col_border }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { col_fg, col_bg,  col_border }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define script(name) "/home/ch/.config/i3/script/" name
#define xmodmap(name) "/home/ch/.config/i3/xmodmap/" name

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *chromecmd[] = { "google-chrome-stable", NULL };
static const char *firefoxcmd[] = { "firefox", NULL };
static const char *rangercmd[] = { "kitty", "ranger", NULL };
static const char *i3lockcmd[] = { script("i3lock.sh"), NULL };
static const char *gnomescreenshotcmd[] = { script("gnome-screenshot.sh"), NULL };
static const char *flameshotcmd[] = { "flameshot", "gui", NULL };
static const char *arandrcmd[] = { "arandr", NULL };
static const char *emojicmd[] = { script("dmenu-emoji.sh"), NULL };
static const char *neteasecmd[] = { "netease-cloud-music", NULL };
static const char *clipmenucmd[] = { "clipmenu", NULL };
static const char *wpscmd[] = { "wps", NULL };
static const char *qtcreatorcmd[] = { "qtcreator", NULL };
static const char *pavucontrolcmd[] = { "pavucontrol", NULL };
static const char *timcmd[] = { script("tim.sh"), NULL };
static const char *wechatcmd[] = { script("wechat.sh"), NULL };
static const char *qwertycmd[] = { "xmodmap", xmodmap(".Xmodmap-qwerty"), NULL };
static const char *colemakcmd[] = { "xmodmap", xmodmap(".Xmodmap-colemak"), NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */

	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_c,         spawn,          {.v = chromecmd } },
	{ MODKEY|ShiftMask,             XK_c,         spawn,          {.v = firefoxcmd } },
	{ MODKEY,                       XK_F4,        spawn,          {.v = i3lockcmd } },
	{ MODKEY,                       XK_semicolon, spawn,          {.v = rangercmd } },
	{ MODKEY|ShiftMask,             XK_a,         spawn,          {.v = flameshotcmd } },
	{ MODKEY,                       XK_a,         spawn,          {.v = gnomescreenshotcmd } },
	{ MODKEY|ControlMask,           XK_a,         spawn,          {.v = arandrcmd } },
	{ MODKEY|ControlMask,           XK_s,         spawn,          {.v = pavucontrolcmd } },
	{ MODKEY,                       XK_apostrophe,spawn,          {.v = emojicmd } },
	{ MODKEY,                       XK_n,         spawn,          {.v = neteasecmd } },
	{ MODKEY|ShiftMask,             XK_n,         spawn,          {.v = clipmenucmd } },
	{ MODKEY,                       XK_o,         spawn,          {.v = wpscmd } },
	{ MODKEY|ShiftMask,             XK_q,         spawn,          {.v = qtcreatorcmd } },
	{ MODKEY,                       XK_t,         spawn,          {.v = timcmd } },
	{ MODKEY,                       XK_w,         spawn,          {.v = wechatcmd } },
	{ Mod4Mask,                     XK_k,         spawn,          {.v = qwertycmd } },
	{ Mod4Mask|ShiftMask,           XK_k,         spawn,          {.v = colemakcmd } },
	{ MODKEY,                       XK_d,         spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,         togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,         togglebar,      {0} },
	{ MODKEY,                       XK_r,         rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,         incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,         incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_e,         zoom,           {0} },
	{ MODKEY|Mod4Mask,              XK_h,         incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_l,         incrgaps,       {.i = -1 } },
	{ MODKEY,                       XK_p,         incrogaps,      {.i = +1 } },
	{ MODKEY,                       XK_m,         incrogaps,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_p,         incrigaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_m,         incrigaps,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_f,         togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_d,         defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,       view,           {0} },
	{ MODKEY,                       XK_q,         killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_y,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_u,         setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_i,         setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_f,         fullscreen,     {0} },
	{ MODKEY|ShiftMask,             XK_space,     togglefloating, {0} },
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },
	  TAGKEYS(                      XK_1,                         0)
	  TAGKEYS(                      XK_2,                         1)
	  TAGKEYS(                      XK_3,                         2)
	  TAGKEYS(                      XK_4,                         3)
	  TAGKEYS(                      XK_5,                         4)
	  TAGKEYS(                      XK_6,                         5)
	  TAGKEYS(                      XK_7,                         6)
	  TAGKEYS(                      XK_8,                         7)
	  TAGKEYS(                      XK_9,                         8)
	{ MODKEY|ShiftMask,             XK_e,         quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
