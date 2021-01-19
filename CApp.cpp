#include "CApp.h"
 
CApp::CApp() {
    Surf_Display = NULL;
    
    Surf_HomeBack = NULL;
    Surf_HomeMenu = NULL;
    Surf_HomeLogo = NULL;
    
    Surf_Background = NULL;
    Surf_Ribbon = NULL;
    
    Surf_PauseMenu = NULL;
    Surf_MHover = NULL;
    Surf_MSelected = NULL;
    Surf_MClicked = NULL;
    
    Surf_HealthBar = NULL;
    Surf_Win = NULL;
    Surf_Lose = NULL;
    
    for (int i = 0;i < 256;i++) {
        Surf_Character[i] = NULL;
    }
    
    Surf_Bullet = NULL;
    
    for (int i = 0;i < SHIPS;i++) {
        for (int j = 0;j < 2;j++) {
        Surf_Ship[i][j] = NULL;
        }
    }
    
    Running = true;
}

int CApp::OnExecute() {
    if(OnInit() == false) {
        fprintf(debug, "Initialisation failed.\n");
        return -1;
    }
    
    SDL_Event Event;
    
    if (HPETsupported) {
        LARGE_INTEGER liTime, liPrevTime;
        LARGE_INTEGER liStart, liEnd;
        QueryPerformanceCounter(&liPrevTime);
        while(Running) {
            QueryPerformanceCounter(&liTime);
            //fprintf(debug, "liTime:%lld, liPrevTime:%lld  ", liTime.QuadPart, liPrevTime.QuadPart);
            if ((liPrevTime.QuadPart+PCFreq) <= liTime.QuadPart) {
                QueryPerformanceCounter(&liPrevTime);
                QueryPerformanceCounter(&liStart);
                while(SDL_PollEvent(&Event)) {
                    OnEvent(&Event);
                }
                OnLoop();
                OnRender();
                QueryPerformanceCounter(&liEnd);
                //fprintf(debug, "Process time:%f", ((liEnd.QuadPart - liStart.QuadPart)/PCFreq));
            }
            //fprintf(debug, "\n");
        }
    } else {
        while(Running) {
            while(SDL_PollEvent(&Event)) {
                OnEvent(&Event);
            }
            OnLoop();
            OnRender();
        }
    }
    
    OnCleanup();
    
    return 0;
}

int main (int argc, char* argv[]) {
    CApp theApp;
    
    return theApp.OnExecute();
}
