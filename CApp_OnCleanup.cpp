#include "CApp.h"

void CApp::OnCleanup() {
    #define free(a) if (a != NULL) {SDL_FreeSurface(a);}
    free(Surf_Display);
    
    free(Surf_HomeBack);
    free(Surf_HomeMenu);
    
    free(Surf_Background);
    free(Surf_Ribbon);
    
    free(Surf_PauseMenu);
    free(Surf_MHover);
    free(Surf_MSelected);
    free(Surf_MClicked);
    
    free(Surf_HealthBar);
    free(Surf_Win);
    free(Surf_Lose);
    
    for (int i = 0;i < 256;i++) {
        free(Surf_Character[i]);
    }
    
    for (int i = 0;i < 2;i++) {
        free(Surf_Arrow[i]);
        free(Surf_ArrowH[i]);
    }
    
    free(Surf_Bullet);
    
    for (int i = 0;i < SHIPS;i++) {
        for (int j = 0;j < 2;j++) {
            free(Surf_Ship[i][j]);
        }
    }
    #undef free
    
    fclose(debug);
    
    SDL_Quit();
}
