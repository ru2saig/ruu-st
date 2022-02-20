/* See LICENSE file for copyright and license details. */

/*
 * appearance
 *
 * font: see http://freedesktop.org/software/fontconfig/fontconfig-user.html
 */
static char *font = "Fira Code Medium:pixelsize=15:antialias=true:autohint=true";
static int borderpx = 5;

/*
 * What program is execed by st depends of these precedence rules:
 * 1: program passed with -e
 * 2: scroll and/or utmp
 * 3: SHELL environment variable
 * 4: value of shell in /etc/passwd
 * 5: value of shell in config.h
 */
static char *shell = "/bin/sh";
char *utmp = NULL;
/* scroll program: to enable use a string like "scroll" */
char *scroll = NULL;
char *stty_args = "stty raw pass8 nl -echo -iexten -cstopb 38400";

/* identification sequence returned in DA and DECID */
char *vtiden = "\033[?6c";

/* Kerning / character bounding-box multipliers */
static float cwscale = 1.0;
static float chscale = 1.0;

/*
 * word delimiter string
 *
 * More advanced example: L" `'\"()[]{}"
 */
wchar_t *worddelimiters = L" ";

/* selection timeouts (in milliseconds) */
static unsigned int doubleclicktimeout = 300;
static unsigned int tripleclicktimeout = 600;

/* alt screens */
int allowaltscreen = 1;

/* allow certain non-interactive (insecure) window operations such as:
   setting the clipboard text */
int allowwindowops = 0;

/*
 * draw latency range in ms - from new content/keypress/etc until drawing.
 * within this range, st draws when content stops arriving (idle). mostly it's
 * near minlatency, but it waits longer for slow updates to avoid partial draw.
 * low minlatency will tear/flicker more, as it can "detect" idle too early.
 */
static double minlatency = 8;
static double maxlatency = 33;

/*
 * blinking timeout (set to 0 to disable blinking) for the terminal blinking
 * attribute.
 */
static unsigned int blinktimeout = 800;

/*
 * thickness of underline and bar cursors
 */
static unsigned int cursorthickness = 2;

/*
 * bell volume. It must be a value between -100 and 100. Use 0 for disabling
 * it
 */
static int bellvolume = 0;

/* default TERM value */
char *termname = "st-256color";

/*
 * spaces per tab
 *
 * When you are changing this value, don't forget to adapt the »it« value in
 * the st.info and appropriately install the st.info in the environment where
 * you use this st version.
 *
 *	it#$tabspaces,
 *
 * Secondly make sure your kernel is not expanding tabs. When running `stty
 * -a` »tab0« should appear. You can tell the terminal to not expand tabs by
 *  running following command:
 *
 *	stty tabs
 */
unsigned int tabspaces = 8;

/* bg opacity */
float alpha = 0.9;

/* Terminal colors (16 first used in escape sequence) */
static const char *colorname[] = {
	/* 8 normal colors */
	"black",
	"red3",
	"green3",
	"yellow3",
	"blue2",
	"magenta3",
	"cyan3",
	"gray90",

	/* 8 bright colors */
	"gray50",
	"red",
	"green",
	"yellow",
	"#5c5cff",
	"magenta",
	"cyan",
	"white",

	[255] = 0,

	/* more colors can be added after 255 to use with DefaultXX */
	"#cccccc",
	"#555555",
	"gray90", /* default foreground colour */
	"black", /* default background colour */
};


/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 258;
unsigned int defaultbg = 259;
unsigned int defaultcs = 256;
unsigned int defaultrcs = 257;

/*
 * https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h4-Functions-using-CSI-_-ordered-by-the-final-character-lparen-s-rparen:CSI-Ps-SP-q.1D81
 * Default style of cursor
 * 0: blinking block
 * 1: blinking block (default)
 * 2: steady block ("█")
 * 3: blinking underline
 * 4: steady underline ("_")
 * 5: blinking bar
 * 6: steady bar ("|")
 * 7: blinking st cursor
 * 8: steady st cursor
 */

static unsigned int cursorstyle = 1;
static Rune stcursor = 0x2603; /* snowman ("☃") */


/*
 * Default columns and rows numbers
 */

static unsigned int cols = 80;
static unsigned int rows = 24;

/*
 * Default colour and shape of the mouse cursor
 */
static unsigned int mouseshape = XC_xterm;
static unsigned int mousefg = 7;
static unsigned int mousebg = 0;

/*
 * Color used to display font attributes when fontconfig selected a font which
 * doesn't match the ones requested.
 */
static unsigned int defaultattr = 11;

/*
 * Force mouse select/shortcuts while mask is active (when MODE_MOUSE is set).
 * Note that if you want to use ShiftMask with selmasks, set this to an other
 * modifier, set to 0 to not use it.
 */
static uint forcemousemod = ShiftMask;

/*
 * Internal mouse shortcuts.
 * Beware that overloading Button1 will disable the selection.
 */
static MouseShortcut mshortcuts[] = {
    /* mask                 button   function        argument       release */
    {ShiftMask, Button4, kscrollup, {.i = 1}},
    {ShiftMask, Button5, kscrolldown, {.i = 1}},
    {XK_ANY_MOD, Button4, kscrollup, {.i = 1}},
    {XK_ANY_MOD, Button5, kscrolldown, {.i = 1}},
    {XK_ANY_MOD, Button2, selpaste, {.i = 0}, 1},
    {XK_ANY_MOD, Button4, ttysend, {.s = "\031"}},
    {XK_ANY_MOD, Button5, ttysend, {.s = "\005"}},
};

/* Internal keyboard shortcuts. */
#define MODKEY Mod1Mask
#define TERMMOD (ControlMask|ShiftMask)

static Shortcut shortcuts[] = {
	/* mask                 keysym          function        argument */
	{ XK_ANY_MOD,           XK_Break,       sendbreak,      {.i =  0} },
	{ ControlMask,          XK_Print,       toggleprinter,  {.i =  0} },
	{ ShiftMask,            XK_Print,       printscreen,    {.i =  0} },
	{ XK_ANY_MOD,           XK_Print,       printsel,       {.i =  0} },
	{ TERMMOD,              XK_Prior,       zoom,           {.f = +1} },
	{ TERMMOD,              XK_Next,        zoom,           {.f = -1} },
	{ TERMMOD,              XK_Home,        zoomreset,      {.f =  0} },
	{ TERMMOD,              XK_C,           clipcopy,       {.i =  0} },
	{ TERMMOD,              XK_V,           clippaste,      {.i =  0} },
	{ TERMMOD,              XK_Y,           selpaste,       {.i =  0} },
	{ ShiftMask,            XK_Insert,      selpaste,       {.i =  0} },
	{ TERMMOD,              XK_Num_Lock,    numlock,        {.i =  0} },
	{ Mod1Mask,            XK_k,     kscrollup,      {.i = -1} },
	{ Mod1Mask,            XK_j,   kscrolldown,    {.i = -1} },
};

/*
 * Special keys (change & recompile st.info accordingly)
 *
 * Mask value:
 * * Use XK_ANY_MOD to match the key no matter modifiers state
 * * Use XK_NO_MOD to match the key alone (no modifiers)
 * appkey value:
 * * 0: no value
 * * > 0: keypad application mode enabled
 * *   = 2: term.numlock = 1
 * * < 0: keypad application mode disabled
 * appcursor value:
 * * 0: no value
 * * > 0: cursor application mode enabled
 * * < 0: cursor application mode disabled
 *
 * Be careful with the order of the definitions because st searches in
 * this table sequentially, so any XK_ANY_MOD must be in the last
 * position for a key.
 */

/*
 * If you want keys other than the X11 function keys (0xFD00 - 0xFFFF)
 * to be mapped below, add them to this array.
 */
static KeySym mappedkeys[] = { -1 };

/*
 * State bits to ignore when matching key or button events.  By default,
 * numlock (Mod2Mask) and keyboard layout (XK_SWITCH_MOD) are ignored.
 */
static uint ignoremod = Mod2Mask|XK_SWITCH_MOD;

/*
 * This is the huge key array which defines all compatibility to the Linux
 * world. Please decide about changes wisely.
 */
static Key key[] = {
	/* keysym           mask            string      appkey appcursor */
	{ XK_KP_Home,       ShiftMask,      "\033[2J",       0,   -1},
	{ XK_KP_Home,       ShiftMask,      "\033[1;2H",     0,   +1},
	{ XK_KP_Home,       XK_ANY_MOD,     "\033[H",        0,   -1},
	{ XK_KP_Home,       XK_ANY_MOD,     "\033[1~",       0,   +1},
	{ XK_KP_Up,         XK_ANY_MOD,     "\033Ox",       +1,    0},
	{ XK_KP_Up,         XK_ANY_MOD,     "\033[A",        0,   -1},
	{ XK_KP_Up,         XK_ANY_MOD,     "\033OA",        0,   +1},
	{ XK_KP_Down,       XK_ANY_MOD,     "\033Or",       +1,    0},
	{ XK_KP_Down,       XK_ANY_MOD,     "\033[B",        0,   -1},
	{ XK_KP_Down,       XK_ANY_MOD,     "\033OB",        0,   +1},
	{ XK_KP_Left,       XK_ANY_MOD,     "\033Ot",       +1,    0},
	{ XK_KP_Left,       XK_ANY_MOD,     "\033[D",        0,   -1},
	{ XK_KP_Left,       XK_ANY_MOD,     "\033OD",        0,   +1},
	{ XK_KP_Right,      XK_ANY_MOD,     "\033Ov",       +1,    0},
	{ XK_KP_Right,      XK_ANY_MOD,     "\033[C",        0,   -1},
	{ XK_KP_Right,      XK_ANY_MOD,     "\033OC",        0,   +1},
	{ XK_KP_Prior,      ShiftMask,      "\033[5;2~",     0,    0},
	{ XK_KP_Prior,      XK_ANY_MOD,     "\033[5~",       0,    0},
	{ XK_KP_Begin,      XK_ANY_MOD,     "\033[E",        0,    0},
	{ XK_KP_End,        ControlMask,    "\033[J",       -1,    0},
	{ XK_KP_End,        ControlMask,    "\033[1;5F",    +1,    0},
	{ XK_KP_End,        ShiftMask,      "\033[K",       -1,    0},
	{ XK_KP_End,        ShiftMask,      "\033[1;2F",    +1,    0},
	{ XK_KP_End,        XK_ANY_MOD,     "\033[4~",       0,    0},
	{ XK_KP_Next,       ShiftMask,      "\033[6;2~",     0,    0},
	{ XK_KP_Next,       XK_ANY_MOD,     "\033[6~",       0,    0},
	{ XK_KP_Insert,     ShiftMask,      "\033[2;2~",    +1,    0},
	{ XK_KP_Insert,     ShiftMask,      "\033[4l",      -1,    0},
	{ XK_KP_Insert,     ControlMask,    "\033[L",       -1,    0},
	{ XK_KP_Insert,     ControlMask,    "\033[2;5~",    +1,    0},
	{ XK_KP_Insert,     XK_ANY_MOD,     "\033[4h",      -1,    0},
	{ XK_KP_Insert,     XK_ANY_MOD,     "\033[2~",      +1,    0},
	{ XK_KP_Delete,     ControlMask,    "\033[M",       -1,    0},
	{ XK_KP_Delete,     ControlMask,    "\033[3;5~",    +1,    0},
	{ XK_KP_Delete,     ShiftMask,      "\033[2K",      -1,    0},
	{ XK_KP_Delete,     ShiftMask,      "\033[3;2~",    +1,    0},
	{ XK_KP_Delete,     XK_ANY_MOD,     "\033[3~",       -1,    0},
	{ XK_KP_Delete,     XK_ANY_MOD,     "\033[3~",      +1,    0},
	{ XK_KP_Multiply,   XK_ANY_MOD,     "\033Oj",       +2,    0},
	{ XK_KP_Add,        XK_ANY_MOD,     "\033Ok",       +2,    0},
	{ XK_KP_Enter,      XK_ANY_MOD,     "\033OM",       +2,    0},
	{ XK_KP_Enter,      XK_ANY_MOD,     "\r",           -1,    0},
	{ XK_KP_Subtract,   XK_ANY_MOD,     "\033Om",       +2,    0},
	{ XK_KP_Decimal,    XK_ANY_MOD,     "\033On",       +2,    0},
	{ XK_KP_Divide,     XK_ANY_MOD,     "\033Oo",       +2,    0},
	{ XK_KP_0,          XK_ANY_MOD,     "\033Op",       +2,    0},
	{ XK_KP_1,          XK_ANY_MOD,     "\033Oq",       +2,    0},
	{ XK_KP_2,          XK_ANY_MOD,     "\033Or",       +2,    0},
	{ XK_KP_3,          XK_ANY_MOD,     "\033Os",       +2,    0},
	{ XK_KP_4,          XK_ANY_MOD,     "\033Ot",       +2,    0},
	{ XK_KP_5,          XK_ANY_MOD,     "\033Ou",       +2,    0},
	{ XK_KP_6,          XK_ANY_MOD,     "\033Ov",       +2,    0},
	{ XK_KP_7,          XK_ANY_MOD,     "\033Ow",       +2,    0},
	{ XK_KP_8,          XK_ANY_MOD,     "\033Ox",       +2,    0},
	{ XK_KP_9,          XK_ANY_MOD,     "\033Oy",       +2,    0},
	{ XK_Up,            ShiftMask,      "\033[1;2A",     0,    0},
	{ XK_Up,            Mod1Mask,       "\033[1;3A",     0,    0},
	{ XK_Up,         ShiftMask|Mod1Mask,"\033[1;4A",     0,    0},
	{ XK_Up,            ControlMask,    "\033[1;5A",     0,    0},
	{ XK_Up,      ShiftMask|ControlMask,"\033[1;6A",     0,    0},
	{ XK_Up,       ControlMask|Mod1Mask,"\033[1;7A",     0,    0},
	{ XK_Up,ShiftMask|ControlMask|Mod1Mask,"\033[1;8A",  0,    0},
	{ XK_Up,            XK_ANY_MOD,     "\033[A",        0,   -1},
	{ XK_Up,            XK_ANY_MOD,     "\033OA",        0,   +1},
	{ XK_Down,          ShiftMask,      "\033[1;2B",     0,    0},
	{ XK_Down,          Mod1Mask,       "\033[1;3B",     0,    0},
	{ XK_Down,       ShiftMask|Mod1Mask,"\033[1;4B",     0,    0},
	{ XK_Down,          ControlMask,    "\033[1;5B",     0,    0},
	{ XK_Down,    ShiftMask|ControlMask,"\033[1;6B",     0,    0},
	{ XK_Down,     ControlMask|Mod1Mask,"\033[1;7B",     0,    0},
	{ XK_Down,ShiftMask|ControlMask|Mod1Mask,"\033[1;8B",0,    0},
	{ XK_Down,          XK_ANY_MOD,     "\033[B",        0,   -1},
	{ XK_Down,          XK_ANY_MOD,     "\033OB",        0,   +1},
	{ XK_Left,          ShiftMask,      "\033[1;2D",     0,    0},
	{ XK_Left,          Mod1Mask,       "\033[1;3D",     0,    0},
	{ XK_Left,       ShiftMask|Mod1Mask,"\033[1;4D",     0,    0},
	{ XK_Left,          ControlMask,    "\033[1;5D",     0,    0},
	{ XK_Left,    ShiftMask|ControlMask,"\033[1;6D",     0,    0},
	{ XK_Left,     ControlMask|Mod1Mask,"\033[1;7D",     0,    0},
	{ XK_Left,ShiftMask|ControlMask|Mod1Mask,"\033[1;8D",0,    0},
	{ XK_Left,          XK_ANY_MOD,     "\033[D",        0,   -1},
	{ XK_Left,          XK_ANY_MOD,     "\033OD",        0,   +1},
	{ XK_Right,         ShiftMask,      "\033[1;2C",     0,    0},
	{ XK_Right,         Mod1Mask,       "\033[1;3C",     0,    0},
	{ XK_Right,      ShiftMask|Mod1Mask,"\033[1;4C",     0,    0},
	{ XK_Right,         ControlMask,    "\033[1;5C",     0,    0},
	{ XK_Right,   ShiftMask|ControlMask,"\033[1;6C",     0,    0},
	{ XK_Right,    ControlMask|Mod1Mask,"\033[1;7C",     0,    0},
	{ XK_Right,ShiftMask|ControlMask|Mod1Mask,"\033[1;8C",0,   0},
	{ XK_Right,         XK_ANY_MOD,     "\033[C",        0,   -1},
	{ XK_Right,         XK_ANY_MOD,     "\033OC",        0,   +1},
	{ XK_ISO_Left_Tab,  ShiftMask,      "\033[Z",        0,    0},
	{ XK_Return,        Mod1Mask,       "\033\r",        0,    0},
	{ XK_Return,        XK_ANY_MOD,     "\r",            0,    0},
	{ XK_Insert,        ShiftMask,      "\033[4l",      -1,    0},
	{ XK_Insert,        ShiftMask,      "\033[2;2~",    +1,    0},
	{ XK_Insert,        ControlMask,    "\033[L",       -1,    0},
	{ XK_Insert,        ControlMask,    "\033[2;5~",    +1,    0},
	{ XK_Insert,        XK_ANY_MOD,     "\033[4h",      -1,    0},
	{ XK_Insert,        XK_ANY_MOD,     "\033[2~",      +1,    0},
	{ XK_Delete,        ControlMask,    "\033[M",       -1,    0},
	{ XK_Delete,        ControlMask,    "\033[3;5~",    +1,    0},
	{ XK_Delete,        ShiftMask,      "\033[2K",      -1,    0},
	{ XK_Delete,        ShiftMask,      "\033[3;2~",    +1,    0},
	{ XK_Delete,        XK_ANY_MOD,     "\033[3~",       -1,    0},
	{ XK_Delete,        XK_ANY_MOD,     "\033[3~",      +1,    0},
	{ XK_BackSpace,     XK_NO_MOD,      "\177",          0,    0},
	{ XK_BackSpace,     Mod1Mask,       "\033\177",      0,    0},
	{ XK_Home,          ShiftMask,      "\033[2J",       0,   -1},
	{ XK_Home,          ShiftMask,      "\033[1;2H",     0,   +1},
	{ XK_Home,          XK_ANY_MOD,     "\033[H",        0,   -1},
	{ XK_Home,          XK_ANY_MOD,     "\033[1~",       0,   +1},
	{ XK_End,           ControlMask,    "\033[J",       -1,    0},
	{ XK_End,           ControlMask,    "\033[1;5F",    +1,    0},
	{ XK_End,           ShiftMask,      "\033[K",       -1,    0},
	{ XK_End,           ShiftMask,      "\033[1;2F",    +1,    0},
	{ XK_End,           XK_ANY_MOD,     "\033[4~",       0,    0},
	{ XK_Prior,         ControlMask,    "\033[5;5~",     0,    0},
	{ XK_Prior,         ShiftMask,      "\033[5;2~",     0,    0},
	{ XK_Prior,         XK_ANY_MOD,     "\033[5~",       0,    0},
	{ XK_Next,          ControlMask,    "\033[6;5~",     0,    0},
	{ XK_Next,          ShiftMask,      "\033[6;2~",     0,    0},
	{ XK_Next,          XK_ANY_MOD,     "\033[6~",       0,    0},
	{ XK_F1,            XK_NO_MOD,      "\033OP" ,       0,    0},
	{ XK_F1, /* F13 */  ShiftMask,      "\033[1;2P",     0,    0},
	{ XK_F1, /* F25 */  ControlMask,    "\033[1;5P",     0,    0},
	{ XK_F1, /* F37 */  Mod4Mask,       "\033[1;6P",     0,    0},
	{ XK_F1, /* F49 */  Mod1Mask,       "\033[1;3P",     0,    0},
	{ XK_F1, /* F61 */  Mod3Mask,       "\033[1;4P",     0,    0},
	{ XK_F2,            XK_NO_MOD,      "\033OQ" ,       0,    0},
	{ XK_F2, /* F14 */  ShiftMask,      "\033[1;2Q",     0,    0},
	{ XK_F2, /* F26 */  ControlMask,    "\033[1;5Q",     0,    0},
	{ XK_F2, /* F38 */  Mod4Mask,       "\033[1;6Q",     0,    0},
	{ XK_F2, /* F50 */  Mod1Mask,       "\033[1;3Q",     0,    0},
	{ XK_F2, /* F62 */  Mod3Mask,       "\033[1;4Q",     0,    0},
	{ XK_F3,            XK_NO_MOD,      "\033OR" ,       0,    0},
	{ XK_F3, /* F15 */  ShiftMask,      "\033[1;2R",     0,    0},
	{ XK_F3, /* F27 */  ControlMask,    "\033[1;5R",     0,    0},
	{ XK_F3, /* F39 */  Mod4Mask,       "\033[1;6R",     0,    0},
	{ XK_F3, /* F51 */  Mod1Mask,       "\033[1;3R",     0,    0},
	{ XK_F3, /* F63 */  Mod3Mask,       "\033[1;4R",     0,    0},
	{ XK_F4,            XK_NO_MOD,      "\033OS" ,       0,    0},
	{ XK_F4, /* F16 */  ShiftMask,      "\033[1;2S",     0,    0},
	{ XK_F4, /* F28 */  ControlMask,    "\033[1;5S",     0,    0},
	{ XK_F4, /* F40 */  Mod4Mask,       "\033[1;6S",     0,    0},
	{ XK_F4, /* F52 */  Mod1Mask,       "\033[1;3S",     0,    0},
	{ XK_F5,            XK_NO_MOD,      "\033[15~",      0,    0},
	{ XK_F5, /* F17 */  ShiftMask,      "\033[15;2~",    0,    0},
	{ XK_F5, /* F29 */  ControlMask,    "\033[15;5~",    0,    0},
	{ XK_F5, /* F41 */  Mod4Mask,       "\033[15;6~",    0,    0},
	{ XK_F5, /* F53 */  Mod1Mask,       "\033[15;3~",    0,    0},
	{ XK_F6,            XK_NO_MOD,      "\033[17~",      0,    0},
	{ XK_F6, /* F18 */  ShiftMask,      "\033[17;2~",    0,    0},
	{ XK_F6, /* F30 */  ControlMask,    "\033[17;5~",    0,    0},
	{ XK_F6, /* F42 */  Mod4Mask,       "\033[17;6~",    0,    0},
	{ XK_F6, /* F54 */  Mod1Mask,       "\033[17;3~",    0,    0},
	{ XK_F7,            XK_NO_MOD,      "\033[18~",      0,    0},
	{ XK_F7, /* F19 */  ShiftMask,      "\033[18;2~",    0,    0},
	{ XK_F7, /* F31 */  ControlMask,    "\033[18;5~",    0,    0},
	{ XK_F7, /* F43 */  Mod4Mask,       "\033[18;6~",    0,    0},
	{ XK_F7, /* F55 */  Mod1Mask,       "\033[18;3~",    0,    0},
	{ XK_F8,            XK_NO_MOD,      "\033[19~",      0,    0},
	{ XK_F8, /* F20 */  ShiftMask,      "\033[19;2~",    0,    0},
	{ XK_F8, /* F32 */  ControlMask,    "\033[19;5~",    0,    0},
	{ XK_F8, /* F44 */  Mod4Mask,       "\033[19;6~",    0,    0},
	{ XK_F8, /* F56 */  Mod1Mask,       "\033[19;3~",    0,    0},
	{ XK_F9,            XK_NO_MOD,      "\033[20~",      0,    0},
	{ XK_F9, /* F21 */  ShiftMask,      "\033[20;2~",    0,    0},
	{ XK_F9, /* F33 */  ControlMask,    "\033[20;5~",    0,    0},
	{ XK_F9, /* F45 */  Mod4Mask,       "\033[20;6~",    0,    0},
	{ XK_F9, /* F57 */  Mod1Mask,       "\033[20;3~",    0,    0},
	{ XK_F10,           XK_NO_MOD,      "\033[21~",      0,    0},
	{ XK_F10, /* F22 */ ShiftMask,      "\033[21;2~",    0,    0},
	{ XK_F10, /* F34 */ ControlMask,    "\033[21;5~",    0,    0},
	{ XK_F10, /* F46 */ Mod4Mask,       "\033[21;6~",    0,    0},
	{ XK_F10, /* F58 */ Mod1Mask,       "\033[21;3~",    0,    0},
	{ XK_F11,           XK_NO_MOD,      "\033[23~",      0,    0},
	{ XK_F11, /* F23 */ ShiftMask,      "\033[23;2~",    0,    0},
	{ XK_F11, /* F35 */ ControlMask,    "\033[23;5~",    0,    0},
	{ XK_F11, /* F47 */ Mod4Mask,       "\033[23;6~",    0,    0},
	{ XK_F11, /* F59 */ Mod1Mask,       "\033[23;3~",    0,    0},
	{ XK_F12,           XK_NO_MOD,      "\033[24~",      0,    0},
	{ XK_F12, /* F24 */ ShiftMask,      "\033[24;2~",    0,    0},
	{ XK_F12, /* F36 */ ControlMask,    "\033[24;5~",    0,    0},
	{ XK_F12, /* F48 */ Mod4Mask,       "\033[24;6~",    0,    0},
	{ XK_F12, /* F60 */ Mod1Mask,       "\033[24;3~",    0,    0},
	{ XK_F13,           XK_NO_MOD,      "\033[1;2P",     0,    0},
	{ XK_F14,           XK_NO_MOD,      "\033[1;2Q",     0,    0},
	{ XK_F15,           XK_NO_MOD,      "\033[1;2R",     0,    0},
	{ XK_F16,           XK_NO_MOD,      "\033[1;2S",     0,    0},
	{ XK_F17,           XK_NO_MOD,      "\033[15;2~",    0,    0},
	{ XK_F18,           XK_NO_MOD,      "\033[17;2~",    0,    0},
	{ XK_F19,           XK_NO_MOD,      "\033[18;2~",    0,    0},
	{ XK_F20,           XK_NO_MOD,      "\033[19;2~",    0,    0},
	{ XK_F21,           XK_NO_MOD,      "\033[20;2~",    0,    0},
	{ XK_F22,           XK_NO_MOD,      "\033[21;2~",    0,    0},
	{ XK_F23,           XK_NO_MOD,      "\033[23;2~",    0,    0},
	{ XK_F24,           XK_NO_MOD,      "\033[24;2~",    0,    0},
	{ XK_F25,           XK_NO_MOD,      "\033[1;5P",     0,    0},
	{ XK_F26,           XK_NO_MOD,      "\033[1;5Q",     0,    0},
	{ XK_F27,           XK_NO_MOD,      "\033[1;5R",     0,    0},
	{ XK_F28,           XK_NO_MOD,      "\033[1;5S",     0,    0},
	{ XK_F29,           XK_NO_MOD,      "\033[15;5~",    0,    0},
	{ XK_F30,           XK_NO_MOD,      "\033[17;5~",    0,    0},
	{ XK_F31,           XK_NO_MOD,      "\033[18;5~",    0,    0},
	{ XK_F32,           XK_NO_MOD,      "\033[19;5~",    0,    0},
	{ XK_F33,           XK_NO_MOD,      "\033[20;5~",    0,    0},
	{ XK_F34,           XK_NO_MOD,      "\033[21;5~",    0,    0},
	{ XK_F35,           XK_NO_MOD,      "\033[23;5~",    0,    0},
};

/*
 * Selection types' masks.
 * Use the same masks as usual.
 * Button1Mask is always unset, to make masks match between ButtonPress.
 * ButtonRelease and MotionNotify.
 * If no match is found, regular selection is used.
 */
static uint selmasks[] = {
	[SEL_RECTANGULAR] = Mod1Mask,
};

/*
 * Printable characters in ASCII, used to estimate the advance width
 * of single wide characters.
 */
static char ascii_printable[] =
	" !\"#$%&'()*+,-./0123456789:;<=>?"
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
	"`abcdefghijklmnopqrstuvwxyz{|}~";
/*
 * The ARGB buffer of the icon , used to set as the _NET_WM_ICON(CARDINAL).
 * It is 32x32.
 */
static int icon_buffer_length = 32*32 + 2;
static unsigned long icon_buffer[] = {
  32, 32,
  0, 1285661857, 4003636128, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 4288783007, 3970081953, 1252238240, 0, 0, 3970081953, 4291019714, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 4291019715, 3970147489, 0, 0, 4288783007, 4292796126, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4293388263, 4292796126, 4288783007, 0, 0, 4288783007, 4290953922, 4291085508, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291151301, 4291085508, 4290953922, 4288783007, 0, 0, 4288783007, 4290888129, 4291151301, 4281546294, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281546294, 4291151301, 4290888129, 4288783007, 0, 0, 4288783007, 4290822336, 4291151301, 4281085743, 4282666568, 4282666568, 4282666568, 4282666568, 4282600775, 4282534726, 4282468933, 4282403140, 4282337091, 4282271298, 4282205505, 4282139712, 4282073919, 4282008126, 4281942076, 4281876283, 4281810490, 4281809977, 4281744184, 4281678391, 4281612342, 4281546549, 4281085743, 4291151301, 4290822336, 4288783007, 0, 0, 4288783007, 4290756543, 4291085508, 4281085743, 4282337860, 4282337860, 4282929740, 4282272067, 4282206274, 4282140225, 4282074432, 4282008639, 4282008126, 4281942332, 4281876539, 4281810490, 4281744697, 4281678904, 4281612855, 4281547062, 4281481268, 4281415219, 4281349426, 4281283633, 4281217584, 4281151791, 4281085743, 4291085508, 4290756543, 4288783007, 0, 0, 4288783007, 4290690750, 4291085508, 4281085743, 4282666568, 4282666568, 4289573551, 4288784035, 4282534725, 4282468676, 4282402884, 4282337091, 4282271042, 4282205249, 4282139712, 4282073662, 4282007869, 4281942076, 4281876027, 4281810234, 4281744441, 4281678392, 4281612598, 4281546805, 4281480756, 4281414963, 4281085743, 4291085508, 4290690750, 4288783007, 0, 0, 4288651421, 4290624957, 4291019715, 4281085743, 4282337860, 4282337860, 4282337860, 4290165176, 4287994776, 4282074431, 4282008382, 4281942589, 4281876796, 4281810747, 4281744954, 4281679161, 4281613112, 4281547318, 4281481525, 4281481012, 4281415219, 4281349426, 4281283377, 4281217584, 4281151790, 4281085741, 4281085743, 4291019715, 4290624957, 4288651421, 0, 0, 4288585628, 4290624957, 4291085508, 4281085743, 4282666568, 4282600775, 4289375915, 4289112743, 4282403140, 4282402884, 4282336835, 4282271041, 4282205248, 4282139455, 4282073662, 4282007869, 4281941820, 4281876027, 4281810234, 4281744184, 4281678391, 4281612598, 4281546549, 4281480756, 4281414963, 4281348914, 4281085743, 4291085508, 4290624957, 4288585628, 0, 0, 4288454042, 4290559164, 4291085508, 4281085743, 4282337860, 4282271810, 4283126606, 4282140224, 4282074175, 4282008382, 4281942589, 4281876540, 4281810747, 4281744953, 4281678904, 4281613111, 4281547318, 4281481269, 4281415476, 4281349427, 4281283634, 4281217840, 4281151791, 4281085998, 4281020205, 4281019692, 4281085743, 4291085508, 4290559164, 4288454042, 0, 0, 4288322713, 4290493371, 4291085508, 4281085743, 4282600775, 4282534726, 4282468933, 4282403140, 4282337091, 4282271298, 4282205505, 4282139712, 4282073919, 4282008126, 4281942077, 4281876283, 4281876026, 4281809977, 4281744184, 4281678391, 4281612342, 4281546549, 4281480756, 4281414706, 4281348913, 4281283120, 4281085743, 4291085508, 4290493371, 4288322713, 0, 0, 4288256663, 4290427578, 4291019715, 4281085743, 4282206274, 4282140225, 4282074432, 4282074175, 4282008126, 4291217863, 4291217607, 4291217606, 4287204490, 4281678904, 4281612855, 4281547062, 4281481269, 4281415219, 4281349426, 4281283633, 4281217584, 4281151791, 4281085998, 4281019949, 4280954156, 4280888362, 4281085743, 4291019715, 4290427578, 4288256663, 0, 0, 4288125077, 4290361785, 4291019715, 4281085743, 4282534725, 4282468932, 4282402884, 4282337091, 4282271298, 4282205249, 4282139712, 4282073918, 4282007869, 4281942076, 4281876283, 4281810234, 4281744441, 4281678648, 4281612599, 4281546805, 4281481012, 4281414963, 4281414706, 4281348913, 4281282864, 4281217071, 4281085743, 4291019715, 4290361785, 4288125077, 0, 0, 4287993748, 4290361785, 4291019715, 4281085743, 4282140225, 4282074432, 4282008382, 4281942589, 4281876796, 4281810747, 4281744954, 4281679161, 4281613112, 4281612854, 4281547061, 4281481012, 4281415219, 4281349426, 4281283377, 4281217584, 4281151791, 4281085741, 4281019948, 4280954155, 4280888106, 4280822313, 4281085743, 4291019715, 4290361785, 4287993748, 0, 0, 4287927698, 4290295992, 4291019715, 4281085743, 4282468676, 4282402884, 4282336835, 4282271042, 4282205248, 4282139455, 4282073662, 4282007869, 4281941820, 4281876027, 4281810234, 4281744184, 4281678391, 4281612598, 4281546549, 4281480756, 4281414963, 4281348914, 4281283121, 4281217327, 4281151278, 4281085485, 4281085743, 4291019715, 4290295992, 4287927698, 0, 0, 4287796369, 4290230199, 4291019715, 4281085743, 4282074175, 4282008382, 4281942589, 4281876540, 4281810747, 4281744953, 4281678904, 4281613111, 4281547318, 4281481269, 4281415476, 4281349683, 4281283634, 4281217840, 4281152047, 4281085998, 4281085741, 4281019948, 4280953899, 4280888106, 4280822313, 4280756263, 4281085743, 4291019715, 4290230199, 4287796369, 0, 0, 4287664783, 4290164406, 4291019715, 4281085743, 4282337091, 4282271298, 4282205505, 4282139712, 4282073919, 4282008126, 4281942077, 4281941819, 4281876026, 4281809977, 4281744184, 4281678391, 4281612342, 4281546549, 4281480756, 4281414706, 4281348913, 4281283120, 4281217071, 4281151278, 4281085485, 4281019436, 4281085743, 4291019715, 4290164406, 4287664783, 0, 0, 4287598733, 4290098613, 4291019715, 4281085743, 4282008382, 4281942333, 4281876539, 4281810746, 4281744697, 4281678904, 4281612855, 4281547062, 4281481269, 4281415219, 4281349426, 4281283633, 4281217584, 4281151791, 4281085998, 4281019949, 4280954156, 4280888362, 4280822313, 4280756520, 4280690727, 4280624678, 4281085743, 4291019715, 4290098613, 4287598733, 0, 0, 4287467404, 4290098613, 4291217094, 4281085743, 4282271298, 4282205249, 4282139712, 4282073919, 4282007869, 4281942076, 4281876283, 4281810234, 4281744441, 4281678648, 4281612599, 4281546805, 4281481012, 4281480499, 4281414706, 4281348913, 4281282864, 4281217071, 4281151278, 4281085228, 4281019435, 4280953642, 4281085743, 4291217094, 4290098613, 4287467404, 0, 0, 4287335818, 4290032820, 4291282887, 4281085743, 4281876796, 4281810747, 4281744954, 4281679161, 4281678648, 4281612855, 4281547061, 4281481012, 4281415219, 4281349426, 4281283377, 4281217584, 4281151791, 4281085741, 4281019948, 4280954155, 4280888106, 4280822313, 4280756520, 4280690471, 4280624678, 4280558884, 4281085743, 4291282887, 4290032820, 4287335818, 0, 0, 4287270025, 4289967027, 4291414473, 4281085743, 4282205248, 4282139711, 4282073662, 4282007869, 4281942076, 4281876027, 4281810234, 4281744441, 4281678391, 4281612598, 4281546805, 4281480756, 4281414963, 4281349170, 4281283121, 4281217327, 4281151278, 4281085485, 4281019692, 4281019179, 4280953386, 4280887593, 4281085743, 4291414473, 4289967027, 4287270025, 0, 0, 4287138439, 4289901234, 4291546059, 4281085743, 4281810747, 4281744954, 4281678904, 4281613111, 4281547318, 4281481269, 4281415476, 4281349683, 4281283634, 4281217841, 4281217583, 4281151534, 4281085741, 4281019948, 4280953899, 4280888106, 4280822313, 4280756263, 4280690470, 4280624677, 4280558628, 4280492835, 4281085743, 4291546059, 4289901234, 4287138439, 0, 0, 4287072389, 4289835441, 4291677645, 4281085743, 4282073919, 4282073662, 4282007613, 4281941820, 4281876026, 4281809977, 4281744184, 4281678391, 4281612342, 4281546549, 4281480756, 4281414707, 4281348913, 4281283120, 4281217071, 4281151278, 4281085485, 4281019436, 4280953643, 4280887849, 4280821800, 4280821800, 4281085743, 4291677645, 4289835441, 4287072389, 0, 0, 4286941060, 4289769648, 4291743438, 4281480501, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281085743, 4281480501, 4291677645, 4289769648, 4286941060, 0, 33554432, 4286809474, 4289769648, 4290888129, 4291809231, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291875024, 4291809231, 4290822336, 4289769648, 4286809474, 33554432, 100663296, 4035150977, 4289045925, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4289703855, 4288980132, 4001530752, 100663296, 134217728, 2085507150, 4068442494, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4286612095, 4068376958, 2085506636, 134217728, 100663296, 704643072, 973078528, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 1006632960, 973078528, 704643072, 100663296, 33554432, 167772160, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 167772160, 33554432, 0, 0, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 33554432, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
