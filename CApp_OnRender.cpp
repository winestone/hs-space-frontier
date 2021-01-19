#include "CApp.h"

void CApp::OnRender() {
    //fprintf(debug, "Start render\n");
    switch (currWindow) {
        case 0: {//Homescreen
            #define hM homeMenu
            //fprintf(debug, "x and y of box: %d %d\n", hM.mP.x, hM.mP.y);
            CSurface::OnDraw(Surf_Display, Surf_HomeBack, 0, 0);
            CSurface::OnDraw(Surf_Display, Surf_HomeMenu, hM.mP.x, hM.mP.y);
            CSurface::OnDraw(Surf_Display, Surf_HomeLogo, 100, 25);
            //screenPrint(500, 250, "Click anywhere to PLAY");
            //fprintf(debug, "hover(%d), selected(%d), clicked(%d)\n", hM.hover, hM.selected, hM.clicked);
            #define ifdraw(a,b) if (b >= 0) {CSurface::OnDraw(Surf_Display, a, (hM.mP.x + hM.bP[b].x), (hM.mP.y + hM.bP[b].y));}
            ifdraw(Surf_MHover, hM.hover);
            ifdraw(Surf_MSelected, hM.selected);
            ifdraw(Surf_MClicked, hM.clicked);
            #undef ifdraw
            #undef hM
        }
        break;
        case 1: {//Game
            RenderGame();
            char dummy[500];
            //Handle Buttons
            #define rb ribbon
            if (rb.selected > 1) {
                sprintf(dummy, "HP: %d", ShipStats[rb.selected-2].hp);
                screenPrint((rb.mP.x+6), (rb.mP.y+26), dummy);
                sprintf(dummy, "Damage: %d", ShipStats[rb.selected-2].dmg);
                screenPrint((rb.mP.x+6), (rb.mP.y+40), dummy);
                sprintf(dummy, "Range: %d", ShipStats[rb.selected-2].range);
                screenPrint((rb.mP.x+6), (rb.mP.y+54), dummy);
                sprintf(dummy, "RoF: %d", ShipStats[rb.selected-2].RoF);
                screenPrint((rb.mP.x+6), (rb.mP.y+68), dummy);
                sprintf(dummy, "Cost: %d", ShipStats[rb.selected-2].price);
                screenPrint((rb.mP.x+103), (rb.mP.y+29), dummy);
            }
            #undef rb
        }
        break;
        case 2: {//Game paused
            RenderGame();
            //Render Pause Menu
            #define pM pauseMenu
            CSurface::OnDraw(Surf_Display, Surf_PauseMenu, pM.mP.x, pM.mP.y);
            
            #define ifdraw(a,b) if (b >= 0) {CSurface::OnDraw(Surf_Display, a, (pM.mP.x + pM.bP[b].x), (pM.mP.y + pM.bP[b].y));}
            ifdraw(Surf_MHover, pM.hover);
            ifdraw(Surf_MSelected, pM.selected);
            ifdraw(Surf_MClicked, pM.clicked);
            #undef ifdraw
            #undef pM
        }
        break;
        default: {
            
        }
        break;
    }
    
    SDL_Flip(Surf_Display);
}

void CApp::RenderGame () {
    CSurface::OnDraw(Surf_Display, Surf_Background, 0, 0);
    CSurface::OnDraw(Surf_Display, Surf_Ribbon, 0, 400);
    
    CSurface::OnDraw(Surf_Display, Surf_HealthBar, 7, 487, 0, 0, health[0], 6);
    CSurface::OnDraw(Surf_Display, Surf_HealthBar, 192, 487, 0, 0, health[1], 6);
    
    switch (win) {
        case 0:
            CSurface::OnDraw(Surf_Display, Surf_Lose, 192, 487);
            CSurface::OnDraw(Surf_Display, Surf_Win, 7, 487);
            break;
        case 1:
            CSurface::OnDraw(Surf_Display, Surf_Lose, 7, 487);
            CSurface::OnDraw(Surf_Display, Surf_Win, 192, 487);
            break;
        default:
            break;
    }
    
    for (int i = 0;i < 2;i++) {
        RenderMoney ((6+(i*97)), 407, money[i]);
    }
    /*OLD RENDERING NUMBERS CODE
    for (int i = 1;i < (moneyLength[0] + moneyComma[0] + 1);i++) {
        if ((i % 4) != 0) {
            RenderNumber(6 + ((moneyLength[0] + moneyComma[0] - i) * 8), 407, moneyChar[0][moneyLength[0] - i + roundDown(i / 4)]);
            
        } else {
            CSurface::OnDraw(Surf_Display, Surf_Comma, 6 + ((moneyLength[0] + moneyComma[0] - i) * 8), 407);
        }
    }
    for (int i = 1;i < (moneyRightLength + moneyRightComma + 1);i++) {
        if ((i % 4) != 0) {
            RenderNumber(103 + ((moneyRightLength + moneyRightComma - i) * 8), 407, moneyRightC[moneyRightLength - i + roundDown(i / 4)]);
        } else {
            CSurface::OnDraw(Surf_Display, Surf_Comma, 103 + ((moneyRightLength + moneyRightComma - i) * 8), 407);
        }
    }
    */
    
    for (int i = 0;i < 2;i++) {
        for (int j = 0;j < 1000;j++) {
            if (Bullets[i][j].life == 1) {
                switch (i) {
                    case 0:
                        CSurface::OnDraw(Surf_Display, Surf_Bullet, Bullets[i][j].x, Bullets[i][j].y);
                        break;
                    case 1:
                        CSurface::OnDraw(Surf_Display, Surf_Bullet, (Bullets[i][j].x - 5), Bullets[i][j].y);
                        break;
                }
            }
        }
    }
    
    for (int i = 0;i < 2;i++) {
        for (int j = 0;j < 1000;j++) {
            if (Ship[i][j].life > 0) {
                CSurface::OnDraw(Surf_Display, Surf_Ship[Ship[i][j].type][i], roundDown(Ship[i][j].x), Ship[i][j].y);
            }
        }
    }
    //for above, need to make them spawn at 0 and 1000 in relation to size for both sides
    //this means render differently which means render x, (y-size of ship)
    //make their variables for x more easier and realistic for the range
    //i think now need to edit make ship
    
    for (int i = 0;i < 2;i++) {
        for (int j = 0;j < 301;j = j + 100) {//arrow left
            CSurface::OnDraw(Surf_Display, Surf_Arrow[i], 100+(i*760), j + 30);
        }
        CSurface::OnDraw(Surf_Display, Surf_ArrowH[i], 100+(i*760), (lane[i] * 100) + 30);
    }
    /*
    for (int i = 0;i < 301;i = i + 100) {//arrow left
        CSurface::OnDraw(Surf_Display, Surf_Arrow[0], 100, i + 30);
    }
    CSurface::OnDraw(Surf_Display, Surf_ArrowH[0], 100, (lane[0] * 100) + 30);
    
    for (int i = 0;i < 301;i = i + 100) {//arrow right
        CSurface::OnDraw(Surf_Display, Surf_Arrow[1], 860, i + 30);
    }
    CSurface::OnDraw(Surf_Display, Surf_ArrowH[1], 860, (lane[1] * 100) + 30);
    */
    
    char dummy[50];
    sprintf(dummy, "%d fps", prev_fps);
    screenPrint(920, 480, dummy);
    fps++;
    if (prevTime < time(NULL)) {
        prev_fps = fps;
        fps = 0;
        time(&prevTime);
    }
    //fprintf(debug, "End render\n");
    return;
}

void CApp::RenderMoney (int x, int y, int amount) {
    char moneyS[20], moneyI[20];
    sprintf(moneyI, "%d", amount);
    int moneyC = strlen(moneyI) / 3;
    if ((strlen(moneyI) % 3) == 0) {
        moneyC--;
    }
    int read_m = 0, read_i = 0;
    
    for (int i = 0;i < ((strlen(moneyI) + moneyC) % 4);i++) {
        moneyS[read_m] = moneyI[read_i];
        read_m++; read_i++;
    }
    
    if (strlen(moneyI) > 3) {
        for (int i = 0;i < (strlen(moneyI) + moneyC - ((strlen(moneyI) + moneyC) % 4));i++) {
            if ((i % 4) != 0) {
                moneyS[read_m] = moneyI[read_i];
                read_m++;read_i++;
            } else {
                moneyS[read_m] = ',';
                read_m++;
            }
        }
    }
    moneyS[read_m] = 0; read_m++;
    
    //fprintf(debug, "amount: %d\n", amount);
    //fprintf(debug, "moneyC: %d\n", moneyC);
    //fprintf(debug, "moneyI: %s\n", moneyI);
    //fprintf(debug, "moneyS: %s\n", moneyS);
    screenPrint(x, y, moneyS);
    return;
}
