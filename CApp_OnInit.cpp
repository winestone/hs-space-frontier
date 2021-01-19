#include "CApp.h"
 
bool CApp::OnInit() {
    debug = fopen("debug.txt", "w");
    settings = fopen("settings.txt", "r");
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    
    //Spam make ships V1.0, implement something to keep track of keydowns for improved version
    //SDL_EnableKeyRepeat(1,1);
    
    if((Surf_Display = SDL_SetVideoMode(WIDTH,  HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }
    
    //Title
    SDL_WM_SetCaption("Space Frontier", 0);
    
    char dummy[500];
    
    #define load(a,b) if((a = CSurface::OnLoad(b)) == NULL) {a = NULL;fprintf(debug, "Failed loading: %s.\n", b);}
    //Homescreen
    load(Surf_HomeBack, "gfx\\Homescreen\\Background.bmp ")
    load(Surf_HomeMenu, "gfx\\Homescreen\\Menu.bmp");
    load(Surf_HomeLogo, "gfx\\Homescreen\\Space Frontier Banner.bmp");
    
    //Game
    load(Surf_Background, "gfx\\Background.bmp");
    load(Surf_Ribbon, "gfx\\Ribbon.bmp");
    
    load(Surf_PauseMenu, "gfx\\PauseMenu\\Menu.bmp");
    
    load(Surf_MHover, "gfx\\StandardMenu\\ButtonHover.bmp");
    load(Surf_MSelected, "gfx\\StandardMenu\\ButtonSelected.bmp");
    load(Surf_MClicked, "gfx\\StandardMenu\\ButtonClicked.bmp");
    
    load(Surf_HealthBar, "gfx\\HealthBar.bmp");
    load(Surf_Win, "gfx\\Win.bmp");
    load(Surf_Lose, "gfx\\Lose.bmp");
    
    for (int i = 0;i < 256;i++) {
        sprintf(dummy, "gfx\\Characters\\%d.bmp", i);
        load(Surf_Character[i], dummy);
    }
    
    load(Surf_Arrow[0], "gfx\\Arrows\\Right.bmp");
    load(Surf_ArrowH[0], "gfx\\Arrows\\RightH.bmp");
    load(Surf_Arrow[1], "gfx\\Arrows\\Left.bmp");
    load(Surf_ArrowH[1], "gfx\\Arrows\\LeftH.bmp");
    
    load(Surf_Bullet, "gfx\\Bullet.bmp");
    
    for (int i = 0;i < SHIPS;i++) {
        for (int j = 0;j < 2;j++) {
            sprintf(dummy, "gfx\\Ship\\%d-%d.bmp", i, j);
            load(Surf_Ship[i][j], dummy);
        }
    }
    #undef load
    
    #define trans(a,r,g,b) CSurface::Transparent(a, r, g, b);
    trans(Surf_MHover, 255, 255, 255);
    trans(Surf_MSelected, 255, 255, 255);
    trans(Surf_MClicked, 255, 255, 255);
    
    trans(Surf_Win, 255, 255, 255);
    
    for (int i = 0;i < 2;i++) {
        trans(Surf_Arrow[i], 255, 255, 255);
        trans(Surf_ArrowH[i], 255, 255, 255);
    }
    
    for (int i = 0;i < 256;i++) {
        if (Surf_Character[i] != NULL) {
            trans(Surf_Character[i], 255, 255, 255);
        }
    }
    
    for (int i = 0;i < SHIPS;i++) {
        for (int j = 0;j < 2;j++) {
            trans(Surf_Ship[i][j], 255, 255, 255);
        }
    }
    #undef trans
    
    currWindow = 0;
    
    fps = 0;
    prev_fps = 0;
    
    //Get high resolution timer performance frequency
    PCFREQ_FPS_CAP = 400;
    HPETsupported = true;
    LARGE_INTEGER liFreq; liFreq.QuadPart = 0;
    if (QueryPerformanceFrequency(&liFreq) == 0) {
        int nError = GetLastError();
        fprintf(debug, "QueryPerformanceFrequency Error: %d\n", nError);
        HPETsupported = false;
    }
    PCFreq = (liFreq.QuadPart / PCFREQ_FPS_CAP);//specified fps/processing cap
    fprintf(debug, "PCFreq: %f\n", PCFreq);
    //Check whether user wants this
    
    
    GameReset();
    
    //set buttons in home menu
    #define set(a,b) homeMenu.a = b
    //Clear
    for (int i = 0;i < MAXBUTTONS;i++) {
        set(bP[i].x, -1);
        set(bP[i].y, -1);
        set(bL[i].x, -1);
        set(bL[i].y, -1);
    }
    set(hover, -1);
    set(selected, -1);
    set(clicked, -1);
    set(currPos.x, -1);
    set(currPos.y, -1);
    //Set
    for (int i = 0;i < HOMEMENUBUTTONS;i++) {
        set(bL[i].x, 120);
        set(bL[i].y, 22);
    }
    //Menu
    set(mP.x, 400); set(mP.y, 150);
    set(mL.x, 200); set(mL.y, 300);
    //Resume
    set(bP[0].x, 40); set(bP[0].y, 20);
    //Options
    set(bP[1].x, 40); set(bP[1].y, 230);
    //Exit
    set(bP[HOMEMENUBUTTONS - 1].x, 40); set(bP[HOMEMENUBUTTONS - 1].y, 260);
    #undef set
    
    //set buttons in ribbon
    #define set(a,b) ribbon.a = b
    //Clear
    for (int i = 0;i < MAXBUTTONS;i++) {
        set(bP[i].x, -1);
        set(bP[i].y, -1);
        set(bL[i].x, -1);
        set(bL[i].y, -1);
    }
    //Set
    set(mP.x, 0); set(mP.y, 400);
    set(mL.x, 1000); set(mL.y, 100);
    //MENU button
    set(bP[0].x, 901); set(bP[0].y, 0);
    set(bL[0].x, 58); set(bL[0].y, 22);
    //Select None Button
    set(bP[1].x, 198); set(bP[1].y, 59);
    set(bL[1].x, 101); set(bL[1].y, 23);
    //QU Ship
    set(bP[2].x, 299); set(bP[2].y, 1);
    set(bL[2].x, 64); set(bL[2].y, 19);
    //WI Ship
    set(bP[3].x, 299); set(bP[3].y, 20);
    set(bL[3].x, 64); set(bL[3].y, 19);
    //EO Ship
    set(bP[4].x, 299); set(bP[4].y, 39);
    set(bL[4].x, 64); set(bL[4].y, 19);
    //RP Ship
    set(bP[5].x, 299); set(bP[5].y, 60);
    set(bL[5].x, 64); set(bL[5].y, 19);
    //AJ Ship
    set(bP[6].x, 363); set(bP[6].y, 1);
    set(bL[6].x, 103); set(bL[6].y, 19);
    //SK Ship
    set(bP[7].x, 363); set(bP[7].y, 20);
    set(bL[7].x, 103); set(bL[7].y, 17);
    //DL Ship
    set(bP[8].x, 363); set(bP[8].y, 37);
    set(bL[8].x, 103); set(bL[8].y, 19);
    //F; Ship
    set(bP[9].x, 363); set(bP[9].y, 56);
    set(bL[9].x, 126); set(bL[9].y, 43);
    #undef set
    
    //set buttons in game pause menu
    #define set(a,b) pauseMenu.a = b
    //Clear
    for (int i = 0;i < MAXBUTTONS;i++) {
        set(bP[i].x, -1);
        set(bP[i].y, -1);
        set(bL[i].x, -1);
        set(bL[i].y, -1);
    }
    set(selected, -1);
    set(currPos.x, -1);
    set(currPos.y, -1);
    //Set
    for (int i = 0;i < PAUSEMENUBUTTONS;i++) {
        set(bL[i].x, 120);
        set(bL[i].y, 22);
    }
    pauseMenuMove = false;
    //Menu
    set(mP.x, 400); set(mP.y, 80);
    set(mL.x, 200); set(mL.y, 300);
    //Resume
    set(bP[0].x, 40); set(bP[0].y, 20);
    //Exit
    set(bP[PAUSEMENUBUTTONS - 1].x, 40); set(bP[PAUSEMENUBUTTONS - 1].y, 260);
    #undef set
    
    //sizeX and sizeY doesn't matter if you set calcSize to true
    //SetShipStats(num, hp, RoF, dmg, speed, range, price, calcSize, sizeX, sizeY, guns, gunX, gunY, ...);
    SetShipStats(  0,   1,  500, 1,   0.5,   50,    500,       true,    40,    20,    1,    33, 12);//Basic
    SetShipStats(  1,   2,  500, 1,   0.4,   50,    900,       true,    40,    20,    1,    33, 12);//Hardened
    SetShipStats(  2,   1,  490, 2,   0.4,   55,    900,       true,    40,    20,    1,    34, 17);//Attack
    SetShipStats(  3,   2,  480, 2,   0.3,   65,    1600,      true,    40,    20,    2,    29,  2, 34, 17);//Ultimate
    SetShipStats(  4,   1,  400, 1,   0.1,   400,   2000,      true,    80,    20,    1,    73, 12);//Ranged
    SetShipStats(  5,   1,  500, 1,     1,   50,    800,       true,    40,    20,    1,    20, 10);//Invis
    SetShipStats(  6,   1,   10, 1,   0.1,   65,    1800,      true,    50,    20,    1,    44,  9);//Kevin
    SetShipStats(  7,   5,  450, 2,   0.4,   80,    4000,      true,   100,    40,    1,    93, 27);//BigPassenger
    //Apply speed multiplier
    
    //make it automatically add ship size to ship range
    //debug stats
    #define print(a) fprintf(debug, "ShipStats %d %s: %d\n", i, #a, ShipStats[i].a)
    for (int i = 0;i < SHIPS;i++) {
        print(hp);
        print(RoF);
        print(dmg);
        fprintf(debug, "ShipStats %d %s: %f\n", i, "speed", ShipStats[i].speed);
        print(range);
        print(price);
        print(sizeX);
        print(sizeY);
        print(guns);
        for (int j = 0;j < ShipStats[i].guns;j++) {
            fprintf(debug, "ShipStats %d %s: %d\n", i, "Gunstat.x", ShipStats[i].gunstats[j].x);
            fprintf(debug, "ShipStats %d %s: %d\n", i, "Gunstat.y", ShipStats[i].gunstats[j].y);
        }
    }
    #undef print
    return true;
}

void CApp::SetShipStats (int num, int hp, int RoF, int dmg, double speed, int range, int price, bool calcSize, int sizeX, int sizeY, int guns,...) {
    va_list gunPos;//x and y's for guns
    #define set(a,b) ShipStats[num].a = b;fprintf(debug, "Set ShipStats[%d].%s to %d\n", num, #a, ShipStats[num].a)
    set(hp, hp);
    set(RoF, RoF);
    set(dmg, dmg);
    set(speed, speed);
    set(range, range);
    set(guns, guns);
    set(price, price);
    switch (calcSize) {
        case false:
            set(sizeX, sizeX);
            set(sizeY, sizeY);
            break;
        case true:
            set(sizeX, Surf_Ship[num][0]->w);
            set(sizeY, Surf_Ship[num][0]->h);
            break;
    }
    va_start(gunPos, guns);
    for (int i = 0;i < guns;i++) {
        set(gunstats[i].x, va_arg(gunPos, int));
        set(gunstats[i].y, va_arg(gunPos, int));
    }
    va_end(gunPos);
    #undef set
    return;
}

void CApp::screenPrint(int x, int y, char * string) {
    int length = 0;
    for (int i = 0;i < strlen(string);i++) {
        CSurface::OnDraw(Surf_Display, Surf_Character[string[i]], (x + length), y);
        if (Surf_Character[string[i]] != NULL) {
            length = length + Surf_Character[string[i]]->w;
        }
        length = length + 2;
    }
    return;
}

void CApp::GameReset() {
    win = -1;
    for (int i = 0;i < 2;i++) {
        lane[i] = 1;
        
        upgrade[i] = 1;
        
        money[i] = 10000;
        
        health[i] = 100;
        
        for (int j = 0;j < SHIPSTRUCT;j++) {
            Ship[i][j].life = 0;
            Ship[i][j].engaged = 0;
        }
        for (int j = 0;j < BULLETSTRUCT;j++) {
            Bullets[i][j].life = 0;
        }
    }
    
    //set buttons in ribbon
    #define set(a,b) ribbon.a = b
    //Clear
    set(hover, -1);
    set(selected, -1);
    set(clicked, -1);
    #undef set
    
    return;
}

void CApp::scanLine (char* szString) {
    int currPos = 0;
    char tempChar = '\0';
    
    while (tempChar != '\n') {
        scanf("%c", &tempChar);
        szString[currPos] = tempChar;
        currPos++;
    }
    szString[currPos - 1] = '\0';
    
    return;
}

void CApp::getLine (FILE * pFile, char * szString) {
    int currPos = 0;
    char tempChar = '\0';
    
    while ((!feof(pFile)) && (tempChar != '\n')) {
        fscanf(pFile, "%c", &tempChar);
        szString[currPos] = tempChar;
        currPos++;
    }
    if (szString[currPos - 1] = '\n') {
        szString[currPos - 1] = '\0';
    } else {
        szString[currPos] = '\0';
    }
    
    return;
}
