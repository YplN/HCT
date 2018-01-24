#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;

int gagnant(int j1, int j2, double matchup[][16], bool decks[][16])
{
        int p1 = 0; // parties gagnées par j1
        int p2 = 0; // parties gagnées par j2
        int s1, s2; //score du j1 et du j2 à chaque partie
        int d1, d2; // deck joué par j1 et j2

        int deckj1[4]; //contient les indices des 4 decks joués
        int deckj2[4];

        double r;

        int k=0;
        int l=0;
        for (size_t i = 0; i < 16; i++) {
                if(decks[j1][i])
                        deckj1[k++]=i;
                if(decks[j2][i])
                        deckj2[l++]=i;
        }


        while(p1+p2<1000000)
        {
                s1=0;
                s2=0;

                // on melange aléatoirement l'ordre dans lequel les decks vont etre joués (par défaut, le deckji[3] sera le deck ban)
                random_shuffle(&deckj1[0], &deckj1[4]);
                random_shuffle(&deckj2[0], &deckj2[4]);

                while(s1<3 && s2 <3)
                {
                        d1 = deckj1[s1];
                        d2 = deckj2[s2];

                        r = ((double) rand() / (RAND_MAX));
                        if(matchup[d1][d2]>r)
                                s2++;
                        else
                                s1++;
                }

                if(s1==3)
                        p1++;
                else
                        p2++;
        }

        cout<<"("<<p1<< " "<<p2<<") ";
        p1>p2 ? cout<<(int) (100*(double) p1/(p1+p2)) : cout<<(int) (100*(double) p2/(p1+p2));
        cout<<endl;
        return p1>p2 ? j1 : j2;
}


int choixban(double wr[][4])
{
        // principe : on choisit de ban le deck qui laisse le moins de chances à l'adversaire d'avoir un bon ban.
        double sommecolonne;
        double sommemax = 0;
        int ban=0;

        for (size_t i = 0; i < 4; i++) {
                sommecolonne = 0;
                for (size_t j = 0; j < 4; j++) {
                        sommecolonne+=wr[j][i];
                }
                if(sommecolonne>sommemax)
                {
                        sommemax=sommecolonne;
                        ban=i;
                }
        }
        return ban;
}


int gagnantv2(int j1, int j2, double matchup[][16], bool decks[][16])
{
        int p1, p2; // parties gagnées par j1 et j2
        int s1, s2; //score du j1 et du j2 à chaque partie
        int d1, d2; // deck joué par j1 et j2
        int b1, b2; // deck ban du j1 et j2
        int deckj1[4]; //contient les indices des 4 decks joués (sera modifié)
        int deckj2[4];
        int D1[4]; //contient les indices des 4 decks joués (ne sera pas modifié)
        int D2[4];

        double r;

        int k=0;
        int l=0;
        for (size_t i = 0; i < 16; i++) {
                if(decks[j1][i])
                {
                        deckj1[k]=i;
                        D1[k]=i;
                        k++;
                }
                if(decks[j2][i])
                {
                        deckj2[l]=i;
                        D2[l]=i;
                        l++;
                }
        }

        double r1[4][4] = {0};
        double r2[4][4] = {0};
        int i, j;
        for (size_t k = 0; k < 4; k++) {
                for (size_t l = 0; l < 4; l++) {
                        // b1 est le deck ban du joueur 1, et b2 celui du joueur 2
                        b1 = D1[k];
                        b2 = D2[l];
                        p1 = 0;
                        p2 = 0;
                        while(p1+p2<5000000)
                        {
                                s1=0;
                                s2=0;
                                i=0;
                                j=0;

                                // on melange aléatoirement l'ordre dans lequel les decks vont etre joués
                                random_shuffle(&deckj1[0], &deckj1[4]);
                                random_shuffle(&deckj2[0], &deckj2[4]);

                                while(s1<3 && s2 <3)
                                {
                                        while(deckj1[i]==b1) // on vérifie que le deck n'est pas ban
                                                i++;
                                        d1 = deckj1[i];
                                        while(deckj2[j]==b2) // on vérifie que le deck n'est pas ban
                                                j++;
                                        d2 = deckj2[j];

                                        //  r = ((double) rand() / (RAND_MAX));
                                        if(matchup[d1][d2]>0.5) // on regarde le matchup
                                                s1++;
                                        else
                                                s2++;
                                }

                                if(s1==3)
                                        p1++;
                                else
                                        p2++;
                        }

                        r1[k][l]=(double) p1/(p1+p2); // r1[k][l] stocke le pourcentage de victoire avec comme ban le deck k du j1, et le deck l du j2
                        r2[l][k]=(double) p2/(p1+p2);

                }
        }


        b1 = choixban(r1);
        b2 = choixban(r2);

        return (r1[b1][b2]>0.5) ? j1 : j2;

}

void tournoi(double matchup[][16], bool decklists[][16], string joueurs[16])
{
        vector<vector <int> > resultats;
        resultats.push_back(vector<int>());
        int j = 16;
        for (size_t i = 0; i < j; i++) {
                resultats[0].push_back(i);
        }
        int r=0; //numéro de round
        int g; //ID du gagnant
        int j1, j2; // ID j1, j2

        while (j!=1) {
                resultats.push_back(vector<int>());
                for (size_t i = 0; i < j/2; i++) {
                        j1=resultats[r][2*i];
                        j2=resultats[r][2*i+1];
                        g=gagnantv2(j1, j2, matchup, decklists);
                        resultats[r+1].push_back(g);
                }
                r++;
                j=j/2;
        }

        for (size_t i = 0; i < r; i++) {
                cout<<"ROUND "<<i<<endl;
                for (size_t j = 0; j < resultats[i].size(); j=j+2) {
                        cout<<joueurs[resultats[i][j]]<<" vs. "<<joueurs[resultats[i][j+1]]<<endl;
                }
                cout<<endl;
        }
        cout<<"VAINQUEUR :"<<joueurs[resultats[r][0]]<<endl;
}


int main(){
        srand(time(NULL));
        int a = 16; // nombre d'archétypes de decks
        int j = 16; // nomdre de joueurs
        int d = 4; // nombre de decks par joueur
        int b = 1; // nombre de bans par match

        double matchup[16][16] = { // 16 => a
                //JD   AD    MH    CM    MP    CP    AP    HP    TR    PR    ES    HW    CW    ZW    TW    NW
                {0.5, 0.43, 0.47, 0.56, 0.41, 0.58, 0.61, 0.56, 0.48, 0.48, 0.49, 0.63, 0.63, 0.49, 0.66, 0.66},
                {0.56, 0.5, 0.51, 0.46, 0.46, 0.48, 0.53, 0.45, 0.53, 0.53, 0.51, 0.43, 0.43, 0.52, 0.51, 0.51},
                {0.53, 0.49, 0.5, 0.58, 0.38, 0.45, 0.39, 0.49, 0.42, 0.42, 0.38, 0.57, 0.57, 0.43, 0.58, 0.67},
                {0.44, 0.53, 0.41, 0.5, 0.47, 0.44, 0.44, 0.44, 0.45, 0.45, 0.51, 0.58, 0.58, 0.44, 0.43, 0.43},
                {0.59, 0.54, 0.61, 0.53, 0.5, 0.61, 0.46, 0.48, 0.58, 0.58, 0.45, 0.44, 0.44, 0.46, 0.77, 0.77},
                {0.41, 0.51, 0.54, 0.55, 0.38, 0.5, 0.50, 0.37, 0.37, 0.37, 0.46, 0.53, 0.53, 0.50, 0.50, 0.50},
                {0.38, 0.47, 0.60, 0.56, 0.54, 0.50, 0.5, 0.47, 0.46, 0.46, 0.42, 0.47, 0.47, 0.47, 0.57, 0.67},
                {0.43, 0.55, 0.50, 0.56, 0.52, 0.61, 0.52, 0.5, 0.52, 0.52, 0.54, 0.59, 0.59, 0.52, 0.60, 0.58},
                {0.52, 0.46, 0.57, 0.54, 0.59, 0.62, 0.54, 0.55, 0.5, 0.50, 0.51, 0.57, 0.57, 0.54, 0.50, 0.60},
                {0.52, 0.46, 0.57, 0.54, 0.59, 0.62, 0.54, 0.55, 0.50, 0.5, 0.51, 0.57, 0.57, 0.54, 0.50, 0.60},
                {0.50, 0.49, 0.62, 0.48, 0.55, 0.53, 0.57, 0.46, 0.49, 0.49, 0.5, 0.39, 0.39, 0.44, 0.44, 0.50},
                {0.37, 0.57, 0.43, 0.42, 0.56, 0.46, 0.52, 0.40, 0.42, 0.42, 0.61, 0.5, 0.50, 0.53, 0.48, 0.47},
                {0.37, 0.57, 0.43, 0.42, 0.56, 0.46, 0.52, 0.40, 0.42, 0.42, 0.61, 0.50, 0.5, 0.53, 0.48, 0.47},
                {0.50, 0.47, 0.57, 0.55, 0.54, 0.50, 0.53, 0.48, 0.46, 0.46, 0.56, 0.47, 0.47, 0.5, 0.57, 0.67},
                {0.33, 0.49, 0.42, 0.57, 0.22, 0.50, 0.43, 0.40, 0.49, 0.49, 0.56, 0.52, 0.52, 0.43, 0.5, 0.50},
                {0.33, 0.49, 0.33, 0.57, 0.22, 0.50, 0.33, 0.42, 0.40, 0.40, 0.50, 0.53, 0.53, 0.33, 0.50, 0.5}
        };

        bool decklists [16][16] = { // 16 16 => j a
                {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, // 0 Trunks
                {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1}, // 1 Oldboy
                {1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0}, // 2 Nalguidan
                {1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0}, // 3 Tom60229
                {1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0}, // 4 Purple
                {1,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0}, // 5 uya
                {1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0}, // 6 Amyks
                {1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0}, // 7 JasonZhou
                {1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0}, // 8 Surrender
                {1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0}, // 9 YAYtears
                {0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0}, // 10 OmegaZero
                {1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0}, // 11 Orange
                {1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0}, // 12 Pavel
                {0,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0}, // 13 BaiZe
                {1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0}, // 14 cocosasa
                {1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0}  // 15 Empanizado
        };


        string joueurs [16] = {
                "Trunks", "Oldboy", "Nalguidan", "Tom60229",
                "Purple", "uya", "Amyks", "JasonZhou",
                "Surrender", "YAYtears", "OmegaZero", "Orange",
                "Pavel", "BaiZe", "cocosasa", "Empanizado"
        };

        //tournoi(matchup, decklists, joueurs);
        int j1=4;
        int j2=6;
        cout<<joueurs[j1]<<" vs. "<<joueurs[j2]<<": ";
        cout<<joueurs[gagnantv2(j1,j2,matchup, decklists)]<<endl;
        return 0;
}
