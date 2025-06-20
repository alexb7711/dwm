#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 3; /* border pixel of windows */
static const unsigned int gappx = 5;    /* gaps between windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int swallowfloating = 0;   /* 1 means swallow floating windows by default */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 0;  /* 0 means bottom bar */

static char *fonts[] = {"SpaceMono Nerd Font Mono:size=11"};
static char dmenufont[] = "SpaceMono Nerd Font Mono:size=11";

static char normbg[] = "#282828";
static char normborder[] = "#282828";
static char normfg[] = "#eeeeee";
static char selfg[] = "#eeeeee";
static char selbg[] = "#458588";
static char selborder[] = "#689d6a";
static char *colors[][3] = {
    /*               fg     , bg     , border       , */
    [SchemeNorm] = {normfg, normbg, normborder},
    [SchemeSel] = {selfg, selbg, selborder},
};

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3] = {
    /*               fg      bg        border     */
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class , instance , title , tags mask , isfloating , isterminal ,
       noswallow , monitor , scratch key */
    // Misc. Applications
    {"Brave", NULL, NULL, 1 << 0, 0, 0, 1, 2, 0},
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1, 0},
    {"Nextcloud", NULL, NULL, 1 << 8, 0, 1, 0, 1, 0},

    // Gaming
    {"steam", NULL, NULL, 1 << 3, 0, 1, 0, 0, 0},
    {"discord", NULL, NULL, 1 << 7, 0, 1, 0, 1, 0},

    // Scratch pads
    {NULL, NULL, "ncspot", 0, 1, 0, 1, -1, 'n'},
    {NULL, NULL, "pulsemixer", 0, 1, 0, 1, -1, 'p'},
    {NULL, NULL, "scratchpad", 0, 1, 0, 1, -1, 's'},

    // Suckless
    {"St", NULL, NULL, 0, 0, 1, 1, -1, 0},
    {"obs", NULL, NULL, 1 << 8, 0, 1, 0, 2, 0},
    {"tabbed", NULL, "st", 0, 0, 1, 0, -1, 0},

    // CAD and Slicers
    {"PrusaSlicer", NULL, NULL, 1 << 2, 0, 1, 0, -1, 0},
    {"FreeCAD", NULL, NULL, 1 << 2, 0, 1, 0, -1, 0},

    // Document Tools
    {"Zotero", NULL, NULL, 1 << 1, 0, 1, 0, -1, 0},
    {"calibre", NULL, NULL, 1 << 1, 0, 1, 0, -1, 0},

    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1, 0}, /* xev */
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[=]", tile}, /* first entry is default */
    {"[~]", NULL}, /* [> no layout function means floating behavior <] */
    // { "[M]",      monocle },
    {"[M]", centeredmaster},
    // { ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",   dmenumon, "-fn", dmenufont, "-nb", normbg,
    "-nf",       normfg, "-sb",    selbg, "-sf",     selfg, NULL};
static const char *clpmnucmd[] = {"clipmenu", NULL};
static const char *termcmd[] = {"tabbed", "-r", "2",  "-c", "st", "-w", "''", NULL};
static const char *browsercmd[] = {"brave", NULL};
static const char *editorcmd[] = {"tabbed", "-r", "2", "st", "-w", "''", "-e", "vim"};

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "tabbed", "-r", "2", "-c", "st", "-t", "scratchpad", NULL};
static const char *ncspotify[] = {"n",  "st",     "-t", "ncspot",
                                  "-e", "ncspot", NULL};
static const char *pulsemixer[] = {"p",  "st",         "-t", "pulsemixer",
                                   "-e", "pulsemixer", NULL};

// Backlight Control
static const char *brupcmd[] = {"sudo", "xbacklight", "-inc", "10", NULL};
static const char *brdowncmd[] = {"sudo", "xbacklight", "-dec", "10", NULL};

// Media Player Controls
static const char *playpause[] = {"playerctl", "play-pause"};
static const char *previous[] = {"playerctl", "previous"};
static const char *next[] = {"playerctl", "next"};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
   { "color0",      STRING,  &normborder },
   { "background",  STRING,  &normbg },
   { "foreground",  STRING,  &normfg },
   { "color12",     STRING,  &selbg },
   { "color2",      STRING,  &selborder },
};

#include "movestack.c"
static Key keys[] = {
    /* modifier                          , key                      , function
       , argument */
    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("audioSwapper up")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("audioSwapper down")},
    {0, XF86XK_AudioMute, spawn, SHCMD("audioSwapper mute")},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd}},
    {0, XF86XK_AudioPlay, spawn, {.v = playpause}},
    {0, XF86XK_AudioPause, spawn, {.v = playpause}},
    {0, XF86XK_AudioNext, spawn, {.v = next}},
    {0, XF86XK_AudioPrev, spawn, {.v = previous}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY | ShiftMask, XK_b, spawn, SHCMD("dm-bluetooth")},
    {MODKEY, XK_c, setlayout, {.v = &layouts[2]}},
    {MODKEY | ShiftMask, XK_c, spawn, {.v = clpmnucmd}},
    {MODKEY | ShiftMask, XK_d, spawn, SHCMD("dm-kill")},
    {MODKEY | ShiftMask, XK_e, spawn, {.v = editorcmd}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_i, spawn, {.v = browsercmd}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_l, spawn, SHCMD("dm-logout")},
    {MODKEY | ShiftMask, XK_j, movestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, movestack, {.i = -1}},
    {MODKEY, XK_f, togglefullscr, {0}},
    {MODKEY | ShiftMask, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_m, spawn, SHCMD("dm-man")},
    {MODKEY | ShiftMask, XK_m, spawn, SHCMD("monitorSwapper")},
    {MODKEY, XK_o, spawn, SHCMD("~/code/scripts/dmenu/dm-books")},
    // { MODKEY                          , XK_m                     , setlayout
    // , {.v = &layouts[2]} }                        ,
    {MODKEY | ShiftMask, XK_s, spawn, SHCMD("screenshot")},
    {MODKEY | ShiftMask, XK_v, spawn, SHCMD("~/code/scripts/ezw")},
    {MODKEY | ShiftMask, XK_w, spawn, SHCMD("setWallpaper")},
    {MODKEY, XK_y, spawn, SHCMD("dm-youtube")},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_space, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY, XK_minus, setgaps, {.i = -1}},
    {MODKEY, XK_equal, setgaps, {.i = +1}},
    {MODKEY | ShiftMask, XK_equal, setgaps, {.i = 0}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_F1, togglescratch, {.v = scratchpadcmd}},
    {MODKEY, XK_F2, togglescratch, {.v = ncspotify}},
    {MODKEY, XK_F3, togglescratch, {.v = pulsemixer}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY, XK_q, killclient, {0}},
    {MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
