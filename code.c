#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>  
#include "structH.h"

#include<limits.h>
#define MAX 6
#define maxi 32767

int TourGeant(float **matrixdistance,int premierclient,int NbClients,int *tour, float *tourBis){
    
    float min;
    int i, courant, next, nbVisited;
    int visited[NbClients]; // inutile de faire un malloc

    for ( i = 0; i < NbClients; i++){
         visited[i]=0;
    }
       
    int found = 0;
    courant   = premierclient;
    next      = premierclient;
    visited[0]=1;
    nbVisited = 1;
    tour[0]    = courant;
    tourBis[0] = 0;
    visited[courant]     = 1;
   
    while (nbVisited<NbClients){ // tant qu'on a pas visité toutes lzs lignes
        min=INT_MAX;
        found = 0;
        for ( i = 0; i < NbClients; i++){ 
            if(visited[i]==0 && matrixdistance[courant][i]<min && matrixdistance[courant][i]!=0){
                min=matrixdistance[courant][i];
                next=i;
                found = 1;
            }
        }
        visited[next]     = 1;
        tour[nbVisited]    = next;
        courant              = next;
        if(found){
            tourBis[nbVisited] = min; // Optionnel
            printf("%f\n",min);
        }
       

        nbVisited++;  

        
    }
    

    return 0;
}




/* Fonction qui remplie la structure aux H avec la table Head, des successeurs et des coûts associés */
/* Données  : Q : entier { capacité des camions} 
              n : entier { nombre de clients à livrer}
              dist : matrice de réel de taille (n+1)*(n+1) {matrice constitué des distances entre les divers clients et le dépôt }
              q : vecteur d'entier de taille n { contient la quantité à livrer pour chaque client } */
/* Résultat : H : struct de type aux { composée du vecteur Head et de succ de type struct données } */
 
void split (int Q, int n, int *T, float **dist, int *q, struct table* H) {
    float cost; /* Distance parcourue par le vÃ©hicule courant*/
    int load;  /*Chargement du vÃ©hicule courant*/
    int j,compt; /* indice de boucle et compteur */

    compt = 0; 
    
    for (int i = 0; i < n; i++){ 
            H->head[i] = compt;
            j = i; 
            load = 0;
            
            while ( j <n && load <Q) 
            {
                load = load + q[T[j]-1];
                if ( i == j ) 
                    cost = dist[0][T[i]] + dist[T[i]][0];
                else
                    cost = cost - dist[T[j-1]][0] + dist[T[j-1]][T[j]] + dist[T[j]][0];
                
                if ( load <= Q)
                {
                    H->succ[compt].sommet = j+1;
                    H->succ[compt].cout = cost;
                    compt++;
                }
                j++;
            }
    }
    H->head[n] = H->head[n-1]+1 ; 
    H->head = realloc(H->head,compt+1);
 }


//// Main ////
int main(void){
    
    
    FILE *fichierR = fopen("exemple.txt","r");
    if(fichierR == NULL) {
        perror(0);
        return 1;
    }
    int i, j;
    float **dist= NULL;
    int *Quantity;
    int UnitCamion;
    int NbClients;
    int premierclient;
    struct H* retoursplit;
    
    fscanf(fichierR,"%d",&NbClients);
    printf("%d\n",NbClients);
    fscanf(fichierR,"%d",&UnitCamion);
    printf("%d\n",UnitCamion);

    //allocation de memoire 
    Quantity=malloc((NbClients+1) * sizeof(int));
    dist=malloc((NbClients+1) * sizeof(int*));
    for (int i = 0; i < NbClients+1; i++)
      {
           dist[i]=malloc((NbClients+1) * sizeof(int));
      }

// lecture
    for (int i = 1; i < NbClients+1; i++)
    {
            fscanf(fichierR, "%d", &Quantity[i]);
            
    }
    for (int i = 1; i < NbClients+1; i++)
    {
            printf("%d ", Quantity[i]);
            
    }
    printf("\n");
    
    for (i = 0; i < NbClients+1; i++){
        for (j = 0; j < (NbClients+1); j++){
            fscanf(fichierR, "%f", &dist[i][j]);
        }
    }
    
    for (i = 0; i < NbClients+1; i++){
        for (j = 0; j < NbClients+1; j++){
            printf("%f ",dist[i][j]);
        }
        printf("\n");
    }
    
    float  result;
    int *tour;
    float *tourBis;
    tour=malloc((NbClients+1) * sizeof(int));
    tourBis=malloc((NbClients+1) * sizeof(float));
    printf("Indiquer le numéro du premier client que vous souhaitez livrer : \n");
    scanf("%d",&premierclient);
    if (premierclient>NbClients){
        printf("Le numéro client indiqué n'est pas valide veuillez indiquer un numéro entre 1 et %d\n",NbClients);
        scanf("%d",&premierclient);
        
    }
    
    else{
       result= TourGeant(dist,premierclient,NbClients+1, tour,tourBis);
    }
    //split(UnitCamion, NbClients, tour, dist, Quantity,retoursplit);

    printf("Voici le tour géant qu'il faudra effectuer en commencant par le client %d\n" ,premierclient);
    float comp=0;
    for (int i = 1; i < NbClients; i++){

        printf("(%d : %f)\t",tour[i],tourBis[i]);
        comp+=tourBis[i];


    }
    
   
    printf("\n");
    printf("cosssst %f\n",comp);
    //free(tour);
    //free(tourBis);
    //free(Quantity);
    for (int i = 0; i < NbClients; i++) {
        printf("%d",tour[i]);
        
    }


    for (int i = 0; i < NbClients+1; i++) {
        
        free(dist[i]);
    }

    
    return 0;
}
