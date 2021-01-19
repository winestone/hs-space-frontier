#include "CApp.h"

void CApp::OnLoop() {
    switch (currWindow) {
        case 0: {//Homescreen
            
        }
        break;
        case 1: {//Game
            for (int i = 0;i < 2;i++) {
                for (int j = 0;j < SHIPSTRUCT;j++) {
                    if (Ship[i][j].life > 0) {
                        for (int k = 0;k < ShipStats[Ship[i][j].type].guns;k++) {
                            if (Ship[i][j].reload[k] < ShipStats[Ship[i][j].type].RoF) {
                                Ship[i][j].reload[k]++;
                            }
                        }
                        //fprintf(debug, "Player 1 compare: x(%d), cmp(%d)\n", roundDown(Ship[i][j].x), -Surf_Ship[Ship[i][j].type][i]->w);
                        if (((i == 0) && (roundDown(Ship[i][j].x) > WIDTH)) || 
                            ((i == 1) && (roundDown(Ship[i][j].x) < (-(ShipStats[Ship[i][j].type].sizeX))))) {//size of ship
                            Ship[i][j].life = 0;
                            if (health[!i] > 0) {health[!i] = health[!i] - 1;}//maybe take away more hp depending on ship
                            //<WIN WAS HERE>
                        }
                    }
                }
                //this code was above at: <WIN WAS HERE>
                if (health[!i] <= 0) {win = i;}
            }
            for (int i = 0;i < 2;i++) {
                for (int j = 0;j < SHIPSTRUCT ;j++) {
                    Ship[i][j].engaged = 0;
                }
            }
            for (int i = 0;i < SHIPSTRUCT;i++) {
                if (Ship[0][i].life > 0) {
                    for (int j = 0;j < SHIPSTRUCT;j++) {
                        if ((Ship[1][j].life > 0) && (Ship[0][i].lane == Ship[1][j].lane)) {
                            for (int l = 0;l < ShipStats[Ship[0][i].type].guns;l++) {
                                if ((Ship[1][j].x <= (Ship[0][i].x + ShipStats[Ship[0][i].type].gunstats[l].x + ShipStats[Ship[0][i].type].range))
                                 && (Ship[0][i].x < Ship[1][j].x)) {
                                    if (Ship[0][i].reload[l] >= ShipStats[Ship[0][i].type].RoF) {
                                        Shoot(0, ShipStats[Ship[0][i].type].dmg, Ship[0][i].type, l, Ship[0][i].lane, roundDown(Ship[0][i].x), Ship[0][i].y);
                                        //Ship[0][i].life = Ship[0][i].life - ShipStats[Ship[1][j].type].dmg;
                                        Ship[0][i].reload[l] = 0;
                                    }
                                    Ship[0][i].engaged = 1;
                                }
                            }
                            for (int l = 0;l < ShipStats[Ship[1][j].type].guns;l++) {
                                if ((Ship[0][i].x >= (Ship[1][j].x - ShipStats[Ship[1][j].type].gunstats[l].x - ShipStats[Ship[1][j].type].range))
                                    /*extended version of above line: ((Ship[0][i].x + ShipStats[Ship[0][i].type].sizeX) >= (Ship[1][j].x + ShipStats[Ship[1][j].type].sizeX - ShipStats[Ship[1][j].type].gunstats[l].x - ShipStats[Ship[0][i].type].range))*/
                                 && (Ship[0][i].x < Ship[1][j].x)) {
                                    if (Ship[1][j].reload[l] >= ShipStats[Ship[1][j].type].RoF) {
                                        Shoot(1, ShipStats[Ship[1][j].type].dmg, Ship[1][j].type, l, Ship[1][j].lane, roundDown(Ship[1][j].x), Ship[1][j].y);
                                        //Ship[1][j].life = Ship[1][j].life - ShipStats[Ship[0][i].type].dmg;
                                        Ship[1][j].reload[l] = 0;
                                    }
                                    Ship[1][j].engaged = 1;
                                }
                            }
                        }
                    }
                }
            }
            
            //move ships
            int direction;
            for (int i = 0;i < 2;i++) {
                switch (i) {
                    case 0:
                        direction = +1;
                        break;
                    case 1:
                        direction = -1;
                        break;
                }
                for (int j = 0;j < SHIPSTRUCT;j++) {
                    if (Ship[i][j].life > 0) {
                        if (Ship[i][j].engaged == 0) {
                            Ship[i][j].x = Ship[i][j].x + (direction*ShipStats[Ship[i][j].type].speed);
                        }
                    }
                }
            }
            
            for (int j = 0;j < BULLETSTRUCT;j++) {
                if (Bullets[0][j].life > 0) {
                    Bullets[0][j].x = Bullets[0][j].x + Bullets[0][j].speed;
                    for (int k = 0;k < SHIPSTRUCT;k++) {
                        if ((Ship[1][k].life > 0) && (Bullets[0][j].lane == Ship[1][k].lane)) {
                            if ((Bullets[0][j].x > Ship[1][k].x) && (Ship[1][k].y < Bullets[0][j].y) && (Bullets[0][j].y < (Ship[1][k].y + ShipStats[Ship[1][k].type].sizeY))) {
                                //need to update how dmg is handled and same for below
                                Ship[1][k].life = Ship[1][k].life - ShipStats[Bullets[0][j].shiptype].dmg;
                                Bullets[0][j].life--;
                            }
                        }
                        if (Bullets[0][j].x > WIDTH) {
                            Bullets[0][j].life = 0;
                        }
                    }
                }
                if (Bullets[1][j].life > 0) {
                    Bullets[1][j].x = Bullets[1][j].x - Bullets[1][j].speed;
                    for (int k = 0;k < SHIPSTRUCT;k++) {
                        if ((Ship[0][k].life > 0) && (Bullets[1][j].lane == Ship[0][k].lane)) {
                            if (((Bullets[1][j].x + 5) < (Ship[0][k].x + ShipStats[Ship[0][k].type].sizeX)) && (Ship[0][k].y < Bullets[1][j].y) && (Bullets[1][j].y < (Ship[0][k].y + ShipStats[Ship[0][k].type].sizeY))) {
                                //update needed here
                                Ship[0][k].life = Ship[0][k].life - ShipStats[Bullets[1][j].shiptype].dmg;
                                Bullets[1][j].life--;
                            }
                        }
                        if (Bullets[1][j].x < -5) {
                            Bullets[1][j].life = 0;
                        }
                    }
                }
            }
            
            if (win == -1) {
                for (int i = 0;i < 2;i++) {
                    money[i] = money[i] + upgrade[i];
                }
            //otherwise someone has won
            } else {//add losing animation, eg. ships blow up, base blow up, etc
                for (int i = 0;i < SHIPSTRUCT;i++) {Ship[!win][i].life = 0;}
            }
        }
        break;
        case 2: {//Game paused
            
        }
        break;
        default: {
            
        }
        break;
    }
    return;
}

void CApp::Shoot(int side, int damage, int type, int gun, int currlane, int x, int y) {
    //fprintf(debug, "Start shoot with side(%d), damage(%d), type(%d), gun(%d), currlane(%d), x(%d), y(%d).\n", side, damage, type, gun, currlane, x, y);
    for (int i = 0;i < 1000;i++) {
        if (Bullets[side][i].life <= 0) {
            #define set(a,b) Bullets[side][i].a = b;fprintf(debug, "Set Bullets[%d][%d].%s to %d\n", side, i, #a, b)
            switch (side) {
                case 0:
                    set(x, (x + ShipStats[type].gunstats[gun].x));
                    break;
                case 1:
                    set(x, ((x + ShipStats[type].sizeX) - ShipStats[type].gunstats[gun].x));
                    break;
            }
            set(shiptype, type);
            set(guntype, gun);
            set(y, (y + ShipStats[type].gunstats[gun].y));
            set(dmg, damage);
            set(speed, 1);
            set(lane, currlane);
            set(life, 1);
            #undef set
            fprintf(debug, "Found, end shoot.\n");
            return;
        }
    }
    fprintf(debug, "Not Found, end shoot.\n");
    return;
}

int CApp::roundDown (double num) {
    int answer = (int)floor(num);
    return answer;
}
