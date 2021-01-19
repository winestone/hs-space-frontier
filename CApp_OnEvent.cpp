#include "CApp.h"

void CApp::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}

void CApp::OnLButtonDown(int mX, int mY) {
    switch (currWindow) {
        case 0: {//Homescreen
            #define hM homeMenu
            for (int i = 0;i < HOMEMENUBUTTONS;i++) {
                if (((hM.mP.x + hM.bP[i].x) < mX) && (mX < (hM.mP.x + hM.bP[i].x + hM.bL[i].x))
                 && ((hM.mP.y + hM.bP[i].y) < mY) && (mY < (hM.mP.y + hM.bP[i].y + hM.bL[i].y))) {
                    hM.clicked = i;
                    i = HOMEMENUBUTTONS;
                }
            }
            #undef rb
        }
        break;
        case 1: {//Game
            #define rb ribbon
            for (int i = 0;i < RIBBONBUTTONS;i++) {
                if (((rb.mP.x + rb.bP[i].x) < mX) && (mX < (rb.mP.x + rb.bP[i].x + rb.bL[i].x))
                 && ((rb.mP.y + rb.bP[i].y) < mY) && (mY < (rb.mP.y + rb.bP[i].y + rb.bL[i].y))) {
                    rb.clicked = i;
                    i = RIBBONBUTTONS;
                }
            }
            #undef rb
        }
        break;
        case 2: {//Game paused
            #define pM pauseMenu
            if ((pM.mP.x < mX) && (mX < (pM.mP.x + pM.mL.x))
             && (pM.mP.y < mY) && (mY < (pM.mP.y + pM.mL.y))) {
                bool overButton = false;
                for (int i = 0;i < PAUSEMENUBUTTONS;i++) {
                    if (((pM.mP.x + pM.bP[i].x) < mX) && (mX < ((pM.mP.x + pM.bP[i].x) + pM.bL[i].x))
                     && ((pM.mP.y + pM.bP[i].y) < mY) && (mY < ((pM.mP.y + pM.bP[i].y) + pM.bL[i].y))) {
                        pM.clicked = i;
                        overButton = true;
                        i = PAUSEMENUBUTTONS;
                    }
                }
                if (!overButton) {
                    pauseMenuMove = true;
                }
            }
            #undef pM
        }
        break;
        default: {
            
        }
        break;
    }
}

void CApp::OnLButtonUp(int mX, int mY) {
    switch (currWindow) {
        case 0: {//Homescreen
            #define hM homeMenu
            //Get hover
            bool over = false;
            for (int i = 0;i < HOMEMENUBUTTONS;i++) {
                if (((hM.mP.x + hM.bP[i].x) < mX) && (mX < ((hM.mP.x + hM.bP[i].x) + hM.bL[i].x))
                     && ((hM.mP.y + hM.bP[i].y) < mY) && (mY < ((hM.mP.y + hM.bP[i].y) + hM.bL[i].y))) {
                    over = true;
                    hM.hover = i;
                    i = HOMEMENUBUTTONS;
                }
            }
            if (!over) {
                hM.hover = -1;
            }
            //if release over a button, activate button
            hM.selected = hM.clicked;
            if (hM.hover == hM.clicked) {
                switch (hM.hover) {
                    case 0://play
                        GameReset();
                        currWindow = 1;
                        break;
                    case (HOMEMENUBUTTONS - 1)://exit
                        OnExit();
                        break;
                    default:
                        break;
                }
            }
            #undef hM
        }
        break;
        case 1: {//Game
            #define rb ribbon
            //fprintf(debug, "Click Release at: %d, %d\n", mX, mY);
            bool over = false;
            for (int i = 0;i < RIBBONBUTTONS;i++) {
                //fprintf(debug, "Loop[%d]\n", i);
                //fprintf(debug, "((%d + %d) < mX) && (mX < (%d + %d + %d))\n", rb.mP.x, rb.bP[i].x, rb.mP.x, rb.bP[i].x, rb.bL[i].x);
                //fprintf(debug, "(%d < mX) && (mX < %d)\n", (rb.mP.x + rb.bP[i].x), (rb.mP.x + rb.bP[i].x + rb.bL[i].x));
                //fprintf(debug, "((%d + %d) < mY) && (mY < (%d + %d + %d))\n", rb.mP.y, rb.bP[i].y, rb.mP.y, rb.bP[i].y, rb.bL[i].y);
                //fprintf(debug, "(%d < mY) && (mY < %d)\n", (rb.mP.y + rb.bP[i].y), (rb.mP.y + rb.bP[i].y + rb.bL[i].y));
                if (((rb.mP.x + rb.bP[i].x) < mX) && (mX < (rb.mP.x + rb.bP[i].x + rb.bL[i].x))
                 && ((rb.mP.y + rb.bP[i].y) < mY) && (mY < (rb.mP.y + rb.bP[i].y + rb.bL[i].y))) {
                    over = true;
                    rb.hover = i;
                    i = RIBBONBUTTONS;
                }
            }
            if (!over) {
                rb.hover = -1;
            } else if (rb.hover != -1) {
                rb.selected = rb.clicked;
                //fprintf(debug, "Release on: %d\n", rb.clicked);
                switch (rb.clicked) {
                    case 0:
                        currWindow = 2;
                        pauseMenu.hover = -1;
                        pauseMenu.selected = -1;
                        pauseMenu.clicked = -1;
                        break;
                    case 1:
                        rb.hover = -1;
                        rb.selected = -1;
                        rb.clicked = -1;
                    default:
                        break;
                }
            }
            #undef rb
        }
        break;
        case 2: {//Game paused
            #define pM pauseMenu
            //Get hover
            bool over = false;
            for (int i = 0;i < PAUSEMENUBUTTONS;i++) {
                if (((pM.mP.x + pM.bP[i].x) < mX) && (mX < ((pM.mP.x + pM.bP[i].x) + pM.bL[i].x))
                     && ((pM.mP.y + pM.bP[i].y) < mY) && (mY < ((pM.mP.y + pM.bP[i].y) + pM.bL[i].y))) {
                    over = true;
                    pM.hover = i;
                    i = PAUSEMENUBUTTONS;
                }
            }
            if (!over) {
                pM.hover = -1;
            }
            //if release over a button, activate button
            pM.selected = pM.clicked;
            if (pM.hover == pM.clicked) {
                switch (pM.hover) {
                    case 0:
                        currWindow = 1;
                        break;
                    case (PAUSEMENUBUTTONS - 1):
                        currWindow = 0;
                        break;
                    default:
                        break;
                }
            }
            if (pauseMenuMove) {
                pauseMenuMove = false;
            }
            #undef pM
        }
        break;
        default: {
            
        }
        break;
    }
}

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    switch (currWindow) {
        case 0: {//Homescreen
            #define hM homeMenu
            //Get hover
            bool over = false;
            for (int i = 0;i < HOMEMENUBUTTONS;i++) {
                if (((hM.mP.x + hM.bP[i].x) < mX) && (mX < ((hM.mP.x + hM.bP[i].x) + hM.bL[i].x))
                     && ((hM.mP.y + hM.bP[i].y) < mY) && (mY < ((hM.mP.y + hM.bP[i].y) + hM.bL[i].y))) {
                    over = true;
                    hM.hover = i;
                    i = HOMEMENUBUTTONS;
                }
            }
            if (!over) {
                hM.hover = -1;
            }
            #undef hM
        }
        break;
        case 1: {//Game
            
        }
        break;
        case 2: {//Game paused
            #define pM pauseMenu
            //Get hover
            bool over = false;
            for (int i = 0;i < PAUSEMENUBUTTONS;i++) {
                if (((pM.mP.x + pM.bP[i].x) < mX) && (mX < ((pM.mP.x + pM.bP[i].x) + pM.bL[i].x))
                     && ((pM.mP.y + pM.bP[i].y) < mY) && (mY < ((pM.mP.y + pM.bP[i].y) + pM.bL[i].y))) {
                    over = true;
                    pM.hover = i;
                    i = PAUSEMENUBUTTONS;
                }
            }
            if (!over) {
                pM.hover = -1;
            }
            //move pause menu if it is being moved
            if (pauseMenuMove) {
                pM.mP.x = (pM.mP.x + relX);
                pM.mP.y = (pM.mP.y + relY);
                if (0 > pM.mP.x) {pM.mP.x = 0;}
                if (0 > pM.mP.y) {pM.mP.x = 0;}
                if (WIDTH - pM.mL.x < pM.mP.x) {pM.mP.x = WIDTH - pM.mL.x;}
                if (HEIGHT - pM.mL.y < pM.mP.y) {pM.mP.y = HEIGHT - pM.mL.y;}
            }
            #undef pM
        }
        break;
        default: {
            
        }
        break;
    }
}

void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch (currWindow) {
        case 0: {//Homescreen
            switch (sym) {
                case SDLK_ESCAPE:
                    OnExit();
                    break;
                    
                default:
                    break;
            }
        }
        break;
        case 1: {//Game
            switch (sym) {
                case SDLK_ESCAPE:
                    currWindow = 0;
                    break;
                    
                default:
                    break;
            }
        }
        break;
        case 2: {//Game paused
            switch (sym) {
                case SDLK_ESCAPE:
                    currWindow = 1;
                    break;
                    
                default:
                    break;
            }
        }
        break;
        default: {
            
        }
        break;
    }
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch (currWindow) {
        case 0: {//Homescreen
        }
        break;
        case 1: {//Game
            switch (sym) {
                case SDLK_1: case SDLK_KP1:
                    lane[0] = 0;
                    break;
                case SDLK_2: case SDLK_KP2:
                    lane[0] = 1;
                    break;
                case SDLK_3: case SDLK_KP3:
                    lane[0] = 2;
                    break;
                case SDLK_4: case SDLK_KP4:
                    lane[0] = 3;
                    break;
                case SDLK_q:
                    MakeShip(0, 0, lane[0]);
                    break;
                case SDLK_w:
                    MakeShip(0, 1, lane[0]);
                    break;
                case SDLK_e:
                    MakeShip(0, 2, lane[0]);
                    break;
                case SDLK_r:
                    MakeShip(0, 3, lane[0]);
                    break;
                case SDLK_a:
                    MakeShip(0, 4, lane[0]);
                    break;
                case SDLK_s:
                    MakeShip(0, 5, lane[0]);
                    break;
                case SDLK_d:
                    MakeShip(0, 6, lane[0]);
                    break;
                case SDLK_f:
                    MakeShip(0, 7, lane[0]);
                    break;
                case SDLK_v:
                    Upgrade(0);
                    break;
                    
                case SDLK_7: case SDLK_KP7:
                    lane[1] = 0;
                    break;
                case SDLK_8: case SDLK_KP8:
                    lane[1] = 1;
                    break;
                case SDLK_9: case SDLK_KP9:
                    lane[1] = 2;
                    break;
                case SDLK_0: case SDLK_KP0:
                    lane[1] = 3;
                    break;
                case SDLK_u:
                    MakeShip(1, 0, lane[1]);
                    break;
                case SDLK_i:
                    MakeShip(1, 1, lane[1]);
                    break;
                case SDLK_o:
                    MakeShip(1, 2, lane[1]);
                    break;
                case SDLK_p:
                    MakeShip(1, 3, lane[1]);
                    break;
                case SDLK_j:
                    MakeShip(1, 4, lane[1]);
                    break;
                case SDLK_k:
                    MakeShip(1, 5, lane[1]);
                    break;
                case SDLK_l:
                    MakeShip(1, 6, lane[1]);
                    break;
                case SDLK_SEMICOLON:
                    MakeShip(1, 7, lane[1]);
                    break;
                case SDLK_SLASH:
                    Upgrade(1);
                    break;
                
                default:
                    break;
            }
        }
        break;
        case 2: {//Game paused
            switch (sym) {
                default:
                    break;
            }
        }
        break;
        default: {
            
        }
        break;
    }
}

void CApp::MakeShip (int side, int type, int currlane) {
    for (int i = 0;i < 1000;i++) {
        if (Ship[side][i].life == 0 && money[side] >= ShipStats[type].price) {
            if (side == 0) {Ship[side][i].x = 0 - ShipStats[type].sizeX;
            } else {Ship[side][i].x = 1000;}
            Ship[side][i].y = (lane[side] * 100) + 50 - roundDown(ShipStats[type].sizeY / 2);//possibly centre the ship, related to size, same for below
            Ship[side][i].life = ShipStats[type].hp;
            Ship[side][i].type = type;
            Ship[side][i].lane = currlane;
            for(int j = 0;j < ShipStats[type].guns;j++) {
                Ship[side][i].reload[j] = ShipStats[type].RoF;
            }
            money[side] = money[side] - ShipStats[type].price;
            i = 1000;
        }
    }
    return;
}

void CApp::Upgrade (int side) {
    if (money[side] > 50000) {
        money[side] = money[side] - 50000;
        upgrade[side]++;
    }
    return;
}

void CApp::OnExit() {
    Running = false;
}
