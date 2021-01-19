#ifndef _CAPP_H_
    #define _CAPP_H_
 
#include <SDL\SDL.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <windows.h>

#include "CEvent.h"
#include "CSurface.h"

const int WIDTH = 1000;
const int HEIGHT = 500;

const int SHIPS = 8;
const int SHIPSTRUCT = 1000;
const int BULLETSTRUCT = 1000;

const int MAXBUTTONS = 10;
const int HOMEMENUBUTTONS = 2;
const int RIBBONBUTTONS = 10;
const int PAUSEMENUBUTTONS = 2;

struct point_struct {
    int x, y;
};

struct button_struct {
    point_struct P;//Position
    point_struct L;//Length
};

struct buttonMenu_struct {
    //buttonPosition and buttonLength
    point_struct bP[MAXBUTTONS];
    point_struct bL[MAXBUTTONS];
    //menuPosition and menuLength
    point_struct mP, mL;
    int hover, selected, clicked;
    point_struct currPos;
};

struct gun_struct {
    int x, y;
    int dmg;//unused
};

struct shipStat_struct {
    int hp;
    //move most of these to gunstats
    int RoF;//Rate of Fire
    int dmg;
    double speed;
    int range;
    int guns;//amount of guns
    gun_struct gunstats[10];
    int price;
    int sizeX;
    int sizeY;
    //char name for name of ship
};

struct ship_struct {
    double x;
    int y;
    int life;
    int type;
    int moving;
    int lane;
    int engaged;
    int reload[10];//for ships with multiple guns
};

struct bullet_struct {
    int life;//1 = in the air, 0 = can be used
    int x;
    int y;
    int dmg;
    int shiptype;
    int guntype;
    int speed;//2 pixels per cycle
    int lane;
};

class CApp : public CEvent {
    private:
        bool            Running;
        
        SDL_Surface*    Surf_Display;
    
    private:
        SDL_Surface*    Surf_HomeBack;
        SDL_Surface*    Surf_HomeMenu;
        SDL_Surface*    Surf_HomeLogo;
        
        SDL_Surface*    Surf_Background;
        SDL_Surface*    Surf_Ribbon;
        
        SDL_Surface*    Surf_PauseMenu;
        
        SDL_Surface*    Surf_MHover;
        SDL_Surface*    Surf_MSelected;
        SDL_Surface*    Surf_MClicked;
        
        SDL_Surface*    Surf_HealthBar;
        SDL_Surface*    Surf_Win;
        SDL_Surface*    Surf_Lose;
        
        SDL_Surface*    Surf_Character[256];
        
        SDL_Surface*    Surf_Arrow[2];
        SDL_Surface*    Surf_ArrowH[2];
        
        SDL_Surface*    Surf_Bullet;
        
        SDL_Surface*    Surf_Ship[SHIPS][2];
        //0 - Basic Ship
        //1 - Hardened Basic Ship
        //2 - Attack Basic Ship
        //3 - Ultimate Basic Ship
        //4 - Basic Ranged Ship
        //5 - Invisible Basic Ship
        //6 - Kevin Basic Ship
    
    private:
        FILE * debug;
        FILE * settings;
        
        int currWindow;
        
        //For calculating the fps
        int fps, prev_fps;
        time_t prevTime;
        
        //More timing...
        int PCFREQ_FPS_CAP; //fps/processing cap
        bool HPETsupported; //HighPrecisionEventTimersupported
        double PCFreq;
        
        //Game
        int lane[2];
        int win;
        
        int upgrade[2];
        
        int money[2];
        
        int health[2];
        
        ship_struct Ship[2][SHIPSTRUCT];
        
        bullet_struct Bullets[2][BULLETSTRUCT];
        
        shipStat_struct ShipStats[SHIPS];
        
        //mouse location
        point_struct currm;
        //Homescreen buttons
        buttonMenu_struct homeMenu;
        //Ribbon buttons
        buttonMenu_struct ribbon;
        //Game pause menu
        buttonMenu_struct pauseMenu;
        bool pauseMenuMove;
    
    public:
        CApp();
        
        int OnExecute();
 
    public:
 
        bool OnInit();
        
            void GameReset();
            void SetShipStats(int num, int hp, int RoF, int dmg, double speed, int range, int price, bool calcSize, int sizeX, int sizeY, int guns,...);
            void screenPrint(int x, int y, char * string);
            void scanLine(char * szString);
            void getLine(FILE * pFile, char * szString);
        
        void OnEvent(SDL_Event* Event);
            
            void OnLButtonUp(int mX, int mY);
            void OnLButtonDown(int mX, int mY);
            void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
            void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
            void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
            void MakeShip(int side, int type, int currlane);
            void Upgrade(int side);
            void OnExit();
        
        void OnLoop();
        
            void Shoot(int side, int damage, int type, int gun, int currlane, int x, int y);
            int roundDown (double num);
        
        void OnRender();
        
            void RenderGame();
            void RenderMoney(int x, int y, int amount);
        
        void OnCleanup();
};
 
#endif
