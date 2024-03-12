#include <X11/XF86keysym.h>

static int showsystray                   = 1;         /* 是否显示托盘栏 */
static const int newclientathead         = 0;         /* 定义新窗口在栈顶还是栈底 */
static const unsigned int borderpx       = 2;         /* 窗口边框大小 */
static const unsigned int systraypinning = 1;         /* 托盘跟随的显示器 0代表不指定显示器 */
static const unsigned int systrayspacing = 1;         /* 托盘间距 */
static const unsigned int systrayspadding = 5;        /* 托盘和状态栏的间隙 */
static int gappi                         = 10;        /* 窗口与窗口 缝隙大小 */
static int gappo                         = 10;        /* 窗口与边缘 缝隙大小 */
static const int _gappo                  = 12;        /* 窗口与窗口 缝隙大小 不可变 用于恢复时的默认值 */
static const int _gappi                  = 12;        /* 窗口与边缘 缝隙大小 不可变 用于恢复时的默认值 */
static const int vertpad                 = 5;         /* vertical padding of bar */
static const int sidepad                 = 5;         /* horizontal padding of bar */
static const int overviewgappi           = 24;        /* overview时 窗口与边缘 缝隙大小 */
static const int overviewgappo           = 60;        /* overview时 窗口与窗口 缝隙大小 */
static const int showbar                 = 1;         /* 是否显示状态栏 */
static const int topbar                  = 1;         /* 指定状态栏位置 0底部 1顶部 */
static const float mfact                 = 0.6;       /* 主工作区 大小比例 */
static const int   nmaster               = 1;         /* 主工作区 窗口数量 */
static const unsigned int snap           = 10;        /* 边缘依附宽度 */
static const unsigned int baralpha       = 0x88;      /* 状态栏透明度 */
static const unsigned int borderalpha    = 0xdd;      /* 边框透明度 */
static const char *fonts[]               = { "FiraCode Nerd Font:style=medium:size=14", "WenQuanYi Micro Hei Mono:style=medium:size=14" };
static const char *colors[][3]           = {          /* 颜色设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { "#ffffff", "#111111", "#8e7cc3" },
    [SchemeSel] = { "#ffffff", "#111111", "#8e7cc3" },
    [SchemeSelGlobal] = { "#ffffff", "#111111", "#8e7cc3" },
    [SchemeHid] = { "#555555", NULL, NULL },
    [SchemeSystray] = { "#ffffff", "#111111", "#8e7cc3" },
    [SchemeUnderline] = { "#8e7cc3", NULL, NULL }, 
    [SchemeNormTag] = { "#ffffff", "#111111", "#8e7cc3" },
    [SchemeSelTag] = { "#ffffff", "#111111", "#8e7cc3" },
    [SchemeBarEmpty] = { "#ffffff", "#111111", "#8e7cc3" },
};
static const unsigned int alphas[][3]    = {          /* 透明度设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSel] = { OPAQUE, baralpha, borderalpha },
    [SchemeSelGlobal] = { OPAQUE, baralpha, borderalpha },
    [SchemeSystray] = { NULL, baralpha, borderalpha },
    [SchemeNormTag] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSelTag] = { OPAQUE, baralpha, borderalpha },
    [SchemeBarEmpty] = { NULL, baralpha, borderalpha },
    [SchemeStatusText] = { OPAQUE, 0x88, NULL },
};

/* 自定义脚本位置 */
static const char *autostartscript = "$DWM/autostart.sh";
static const char *statusbarscript = "$DWM/statusbar/statusbar.sh";

/* 自定义 scratchpad instance */
static const char scratchpadname[] = "scratchpad";

/* 自定义tag名称 */
/* 自定义特定实例的显示状态 */
//            ﮸  ﭮ 切
static const char *tags[] = { 
    "", // tag:0  key:1  desc:terminal1
    "", // tag:1  key:2  desc:terminal2
    "", // tag:2  key:3  desc:terminal3
    "", // tag:3  key:4  desc:terminal4
    "󰨞", // tag:4  key:r  desc:vscode
    "", // tag:5  key:5  desc:idea
    "󰟓", // tag:6  key:6  desc:goland
    "", // tag:7  key:7  desc:clion
    "󰕧", // tag:8  key:9  desc:obs
    "", // tag:9  key:c  desc:chrome
    "", // tag:10 key:m  desc:yesmusic
    "ﬄ", // tag:11 key:0  desc:qq
    "﬐", // tag:12 key:w  desc:wechat
    "", // tag:13 key:l  desc:wxwork
    "", // tag:14 key:   desc:firefox
    "ﴱ", // tag:15 key:   desc:poi
};

/* 自定义窗口显示规则 */
/* class instance title 主要用于定位窗口适合哪个规则 */
/* tags mask 定义符合该规则的窗口的tag 0 表示当前tag */
/* isfloating 定义符合该规则的窗口是否浮动 */
/* isglobal 定义符合该规则的窗口是否全局浮动 */
/* isnoborder 定义符合该规则的窗口是否无边框 */
/* monitor 定义符合该规则的窗口显示在哪个显示器上 -1 为当前屏幕 */
/* floatposition 定义符合该规则的窗口显示的位置 0 中间，1到9分别为9宫格位置，例如1左上，9右下，3右上 */
static const Rule rules[] = {
    /* class                 instance              title             tags mask     isfloating  isglobal    isnoborder monitor floatposition */
    /** 优先级高 越在上面优先度越高 */
    { NULL,                  NULL,                "图片查看器",      0,            1,          0,          0,        -1,      0}, // qq图片查看器        浮动
    { NULL,                  NULL,                "图片查看",        0,            1,          0,          0,        -1,      0}, // 微信图片查看器      浮动
    { NULL,                  "TIM.exe",           NULL,              0,            1,          0,          0,        -1,      0}, // TIM                 浮动
    { NULL,                  "QQ.exe",            NULL,              0,            1,          0,          0         -1,      0},
    { NULL,                  "QQExternal.exe",    NULL,              0,            1,          0,          0         -1,      0},
	/** 普通优先度 */
    {"vscode",              "code",                NULL,             1 << 4,       0,          0,          0,        -1,      0}, // vscode     tag -> 󰨞
    {"idea",                "idea",                NULL,             1 << 5,       0,          0,          0,        -1,      0}, // idea       tag -> 
    {"goland",              "goland",              NULL,             1 << 6,       0,          0,          0,        -1,      0}, // goland     tag -> 󰟓
    {"clion",               "clion",               NULL,             1 << 7,       0,          0,          0,        -1,      0}, // clion      tag -> 
    {"obs",                  NULL,                 NULL,             1 << 8,       0,          0,          0,        -1,      0}, // obs        tag -> 󰕧
    {"chrome",               NULL,                 NULL,             1 << 9,       0,          0,          0,        -1,      0}, // chrome     tag -> 
    //{"Chromium",             NULL,                 NULL,             1 << 10,      0,          0,          0,        -1,      0}, // Chromium   tag -> 
    {"yesplaymusic",        "yesplaymusic",        NULL,             1 << 10,      0,          0,          0,        -1,      0}, // yesmusic   tag ->  浮动、无边框
    { NULL,                 "qq",                  NULL,             1 << 11,      0,          0,          1,        -1,      0}, // qq         tag -> ﬄ 无边框
    { NULL,                 "wechat.exe",          NULL,             1 << 12,      0,          0,          1,        -1,      0}, // wechat     tag -> ﬐ 无边框
    { NULL,                 "wxwork.exe",          NULL,             1 << 13,      0,          0,          1,        -1,      0}, // workwechat tag ->  无边框
    {"firefox",              NULL,                 NULL,             1 << 14,      0,          0,          0,        -1,      0}, // firefox    tag -> 
    {"poi",                 "poi",                 NULL,             1 << 15,      0,          0,          0,        -1,      0}, // poi        tag -> ﴱ
    {"Vncviewer",            NULL,                 NULL,             0,            1,          0,          1,        -1,      2}, // Vncviewer           浮动、无边框 屏幕顶部
    {"flameshot",            NULL,                 NULL,             0,            1,          0,          0,        -1,      0}, // 火焰截图            浮动
    {"scratchpad",          "scratchpad",         "scratchpad",      TAGMASK,      1,          1,          1,        -1,      2}, // scratchpad          浮动、全局、无边框 屏幕顶部
    {"Pcmanfm",              NULL,                 NULL,             0,            1,          0,          1,        -1,      3}, // pcmanfm             浮动、无边框 右上角
    {"wemeetapp",            NULL,                 NULL,             TAGMASK,      1,          1,          0,        -1,      0}, // !!!腾讯会议在切换tag时有诡异bug导致退出 变成global来规避该问题

    /** 部分特殊class的规则 */
    {"float",                NULL,                 NULL,             0,            1,          0,          0,        -1,      0}, // class = float       浮动
    {"global",               NULL,                 NULL,             TAGMASK,      0,          1,          0,        -1,      0}, // class = gloabl      全局
    {"noborder",             NULL,                 NULL,             0,            0,          0,          1,        -1,      0}, // class = noborder    无边框
    {"FGN",                  NULL,                 NULL,             TAGMASK,      1,          1,          1,        -1,      0}, // class = FGN         浮动、全局、无边框
    {"FG",                   NULL,                 NULL,             TAGMASK,      1,          1,          0,        -1,      0}, // class = FG          浮动、全局
    {"FN",                   NULL,                 NULL,             0,            1,          0,          1,        -1,      0}, // class = FN          浮动、无边框
    {"GN",                   NULL,                 NULL,             TAGMASK,      0,          1,          1,        -1,      0}, // CLASS = GN          全局、无边框

    /** 优先度低 越在上面优先度越低 */
    { NULL,                  NULL,                "crx_",            0,            1,          0,          0,        -1,      0}, // 错误载入时 会有crx_ 浮动
    { NULL,                  NULL,                "broken",          0,            1,          0,          0,        -1,      0}, // 错误载入时 会有broken 浮动
};
static const char *overviewtag = "OVERVIEW";
static const Layout overviewlayout = { "舘",  overview };

/* 自定义布局 */
static const Layout layouts[] = {
    { "﬿",  tile },         /* 主次栈 */
    { "﩯",  magicgrid },    /* 网格 */
};

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG, cmd) \
    { MODKEY,              KEY, view,       {.ui = 1 << TAG, .v = cmd} }, \
    { MODKEY|ShiftMask,    KEY, tag,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,  KEY, toggleview, {.ui = 1 << TAG} }, \

static Key keys[] = {
    /* modifier            key              function          argument */
    { MODKEY,              XK_equal,        togglesystray,    {0} },                     /* super +            |  切换 托盘栏显示状态 */

    { MODKEY,              XK_Tab,          focusstack,       {.i = +1} },               /* super tab          |  本tag内切换聚焦窗口 */
    { MODKEY|ShiftMask,    XK_Tab,          focusstack,       {.i = -1} },               /* super shift tab    |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_Up,           focusstack,       {.i = -1} },               /* super up           |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_Down,         focusstack,       {.i = +1} },               /* super down         |  本tag内切换聚焦窗口 */

    { MODKEY,              XK_Left,         viewtoleft,       {0} },                     /* super left         |  聚焦到左边的tag */
    { MODKEY,              XK_Right,        viewtoright,      {0} },                     /* super right        |  聚焦到右边的tag */
    { MODKEY|ShiftMask,    XK_Left,         tagtoleft,        {0} },                     /* super shift left   |  将本窗口移动到左边tag */
    { MODKEY|ShiftMask,    XK_Right,        tagtoright,       {0} },                     /* super shift right  |  将本窗口移动到右边tag */

    { MODKEY,              XK_a,            toggleoverview,   {0} },                     /* super a            |  显示所有tag 或 跳转到聚焦窗口的tag */

    { MODKEY,              XK_comma,        setmfact,         {.f = -0.05} },            /* super ,            |  缩小主工作区 */
    { MODKEY,              XK_period,       setmfact,         {.f = +0.05} },            /* super .            |  放大主工作区 */

    { MODKEY,              XK_h,            hidewin,          {0} },                     /* super h            |  隐藏 窗口 */
    { MODKEY|ShiftMask,    XK_h,            restorewin,       {0} },                     /* super shift h      |  取消隐藏 窗口 */

    { MODKEY|ShiftMask,    XK_Return,       zoom,             {0} },                     /* super shift enter  |  将当前聚焦窗口置为主窗口 */

    { MODKEY,              XK_t,            togglefloating,   {0} },                     /* super t            |  开启/关闭 聚焦目标的float模式 */
    { MODKEY|ShiftMask,    XK_t,            toggleallfloating,{0} },                     /* super shift t      |  开启/关闭 全部目标的float模式 */
    { MODKEY|ShiftMask,    XK_f,            fullscreen,       {0} },                     /* super f            |  开启/关闭 全屏 */
    //{ MODKEY|ShiftMask,    XK_f,            togglebar,        {0} },                     /* super shift f      |  开启/关闭 状态栏 */
    //{ MODKEY,              XK_g,            toggleglobal,     {0} },                     /* super g            |  开启/关闭 全局 */
    { MODKEY,              XK_u,            toggleborder,     {0} },                     /* super u            |  开启/关闭 边框 */
    { MODKEY,              XK_e,            incnmaster,       {.i = +1} },               /* super e            |  改变主工作区窗口数量 (1 2中切换) */

    { MODKEY,              XK_b,            focusmon,         {.i = +1} },               /* super b            |  光标移动到另一个显示器 */
    { MODKEY|ShiftMask,    XK_b,            tagmon,           {.i = +1} },               /* super shift b      |  将聚焦窗口移动到另一个显示器 */

    { MODKEY,              XK_q,            killclient,       {0} },                     /* super q            |  关闭窗口 */
    { MODKEY|ControlMask,  XK_q,            forcekillclient,  {0} },                     /* super ctrl q       |  强制关闭窗口(处理某些情况下无法销毁的窗口) */
    { MODKEY|ControlMask,  XK_F12,          quit,             {0} },                     /* super ctrl f12     |  退出dwm */

    { MODKEY|ShiftMask,    XK_space,        selectlayout,     {.v = &layouts[1]} },      /* super shift space  |  切换到网格布局 */
    { MODKEY,              XK_o,            showonlyorall,    {0} },                     /* super o            |  切换 只显示一个窗口 / 全部显示 */

    { MODKEY|ControlMask,  XK_equal,        setgap,           {.i = -6} },               /* super ctrl +       |  窗口增大 */
    { MODKEY|ControlMask,  XK_minus,        setgap,           {.i = +6} },               /* super ctrl -       |  窗口减小 */
    { MODKEY|ControlMask,  XK_space,        setgap,           {.i = 0} },                /* super ctrl space   |  窗口重置 */

    { MODKEY|ControlMask,  XK_Up,           movewin,          {.ui = UP} },              /* super ctrl up      |  移动窗口 */
    { MODKEY|ControlMask,  XK_Down,         movewin,          {.ui = DOWN} },            /* super ctrl down    |  移动窗口 */
    { MODKEY|ControlMask,  XK_Left,         movewin,          {.ui = LEFT} },            /* super ctrl left    |  移动窗口 */
    { MODKEY|ControlMask,  XK_Right,        movewin,          {.ui = RIGHT} },           /* super ctrl right   |  移动窗口 */

    { MODKEY|Mod1Mask,     XK_Up,           resizewin,        {.ui = V_REDUCE} },        /* super alt up       |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_Down,         resizewin,        {.ui = V_EXPAND} },        /* super alt down     |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_Left,         resizewin,        {.ui = H_REDUCE} },        /* super alt left     |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_Right,        resizewin,        {.ui = H_EXPAND} },        /* super alt right    |  调整窗口 */

    /* spawn + SHCMD 执行对应命令(已下部分建议完全自己重新定义) */
    { MODKEY,              XK_s,      togglescratch, SHCMD("st -t scratchpad -c float") },                      /* super s          | 打开scratch终端        */
    { MODKEY,              XK_Return, spawn, SHCMD("kitty") },                                                     /* super enter      | 打开st终端             */
    { MODKEY,              XK_minus,  spawn, SHCMD("st -c FG") },                                               /* super +          | 打开全局st终端         */
    { MODKEY,              XK_space,  spawn, SHCMD("st -c float") },                                            /* super space      | 打开浮动st终端         */
    { MODKEY,              XK_F1,     spawn, SHCMD("dolphin") },                                                /* super F1         | 打开dolphin            */
    { MODKEY|ShiftMask,    XK_d,      spawn, SHCMD("~/scripts/call_rofi.sh launcher") },                        /* super shift d    | rofi: 执行launcher     */
    { MODKEY|ShiftMask,    XK_Up,     spawn, SHCMD("~/scripts/set_vol.sh up") },                                /* super shift up   | 音量加                 */
    { MODKEY|ShiftMask,    XK_Down,   spawn, SHCMD("~/scripts/set_vol.sh down") },                              /* super shift down | 音量减                 */
    { MODKEY|ShiftMask,    XK_a,      spawn, SHCMD("flameshot gui") },                                          /* super shift a    | 截图                   */
    { MODKEY|ShiftMask,    XK_k,      spawn, SHCMD("~/scripts/screenkey.sh") },                                 /* super shift k    | 打开键盘输入显示       */
    { MODKEY|ShiftMask,    XK_q,      spawn, SHCMD("kill -9 $(xprop | grep _NET_WM_PID | awk '{print $3}')") }, /* super shift q    | 选中某个窗口并强制kill */
    { ShiftMask|ControlMask, XK_c,    spawn, SHCMD("xclip -o | xclip -selection c") },                          /* super shift c    | 进阶复制               */

    /* super key : 跳转到对应tag (可附加一条命令 若目标目录无窗口，则执行该命令) */
    /* super shift key : 将聚焦窗口移动到对应tag */
    /* key tag cmd */
    TAGKEYS(XK_1, 0,  0)
    TAGKEYS(XK_2, 1,  0)
    TAGKEYS(XK_3, 2,  0)
    TAGKEYS(XK_4, 3,  0)
    TAGKEYS(XK_r, 4,  "code")
    TAGKEYS(XK_j, 5,  "idea")
    TAGKEYS(XK_g, 6,  "goland")
    TAGKEYS(XK_7, 7,  "clion")
    TAGKEYS(XK_9, 8,  "obs")
    TAGKEYS(XK_c, 9,  "google-chrome-stable")
    TAGKEYS(XK_m, 10, "/opt/YesPlayMusic/yesplaymusic")
    TAGKEYS(XK_0, 11, "linuxqq")
    TAGKEYS(XK_w, 12, "/opt/apps/com.qq.weixin.deepin/files/run.sh")
	// TAGKEYS(XK_l, 13, "/opt/apps/com.qq.weixin.work.deepin/files/run.sh")
    TAGKEYS(XK_x, 14, "firefox")
};
static Button buttons[] = {
    /* click               event mask       button            function       argument  */
    /* 点击窗口标题栏操作 */
    { ClkWinTitle,         0,               Button1,          hideotherwins, {0} },                                   // 左键        |  点击标题     |  隐藏其他窗口仅保留该窗口
    { ClkWinTitle,         0,               Button3,          togglewin,     {0} },                                   // 右键        |  点击标题     |  切换窗口显示状态
    /* 点击窗口操作 */
    { ClkClientWin,        MODKEY,          Button1,          movemouse,     {0} },                                   // super+左键  |  拖拽窗口     |  拖拽窗口
    { ClkClientWin,        MODKEY,          Button3,          resizemouse,   {0} },                                   // super+右键  |  拖拽窗口     |  改变窗口大小
    /* 点击tag操作 */
    { ClkTagBar,           0,               Button1,          view,          {0} },                                   // 左键        |  点击tag      |  切换tag
    { ClkTagBar,           0,               Button3,          toggleview,    {0} },                                   // 右键        |  点击tag      |  切换是否显示tag
    { ClkTagBar,           MODKEY,          Button1,          tag,           {0} },                                   // super+左键  |  点击tag      |  将窗口移动到对应tag
    { ClkTagBar,           0,               Button4,          viewtoleft,    {0} },                                   // 鼠标滚轮上  |  tag          |  向前切换tag
    { ClkTagBar,           0,               Button5,          viewtoright,   {0} },                                   // 鼠标滚轮下  |  tag          |  向后切换tag
    /* 点击状态栏操作 */
    { ClkStatusText,       0,               Button1,          clickstatusbar,{0} },                                   // 左键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal L
    { ClkStatusText,       0,               Button2,          clickstatusbar,{0} },                                   // 中键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal M
    { ClkStatusText,       0,               Button3,          clickstatusbar,{0} },                                   // 右键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal R
    { ClkStatusText,       0,               Button4,          clickstatusbar,{0} },                                   // 鼠标滚轮上  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal U
    { ClkStatusText,       0,               Button5,          clickstatusbar,{0} },                                   // 鼠标滚轮下  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal D
                                                                                                                      //
    /* 点击bar空白处 */
    { ClkBarEmpty,         0,               Button1,          spawn, SHCMD("~/scripts/call_rofi.sh launcher") },      // 左键        |  bar空白处    |  rofi 执行 launcher
    { ClkBarEmpty,         0,               Button3,          spawn, SHCMD("~/scripts/call_rofi.sh drun") },          // 右键        |  bar空白处    |  rofi 执行 drun
};