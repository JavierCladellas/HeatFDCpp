#include <iostream>
#include <fstream>
#include <iomanip>

#include "ConfigFileHandler.hpp"

#include "matrices.hpp"
#include "ailette.hpp"
#include "textformats.hpp"


int main(int argc, char** argv){

    /*__________________Lecture des paramètres________________*/ 
    std::string params, values;
    if(argc>1){
        params = readConfig(argv[1])._params;
        values = readConfig(argv[1])._values;
    }
    else{
        std::cout << "Rentrez le nom du fichier de configuration en ligne de commande. \n"; 
        return 1;
    }

    bool    statio  = fetchParamValue<bool>("stationary",params,values);// Booléan indiquant type de modèle         1->stationnaire/0->instationnaire 
    //Paramètres géomètriques
    double  Lx      = fetchParamValue<double>("Lx", params,values);     // Composante en x de l'ailette             m
    double  Ly      = fetchParamValue<double>("Ly", params,values);     // Composante en y de l'ailette             m
    double  Lz      = fetchParamValue<double>("Lz", params,values);     // Composante en z de l'ailette             m
    double  s       = Ly*Lz;                                            //  Surface de l'ailette                    m^2
    double  p       = 2*(Ly+Lz);                                        //  Périmètre de l'ailette                  m
    //Paramètres et physiques 
    double  Te      = fetchParamValue<double>("Te", params,values);     // Température ambiante                     °C
    double  Phip    = fetchParamValue<double>("Phi", params,values);    // Flux de chaleur du processeur            W/m2
    double  hc      = fetchParamValue<double>("hc", params,values);     // Coeff de transfert de chaleur surfacique W/(m2*K)    
    double  ro      = fetchParamValue<double>("ro", params,values);     // Densité                                  kg/m3
    double  Cp      = fetchParamValue<double>("Cp", params,values);     // Chaleur spécifique à pressión constante  J/(kg*K) 
    double  kappa   = fetchParamValue<double>("kappa", params,values);  // Conductivité thérmique                   W/(m*K)
    //Maillage
    int     M       = fetchParamValue<int>("M", params,values);         // Maillage spatial pour température        (nombre de points)
    int     Mx      = fetchParamValue<int>("Mx", params,values);        // Maillage spatial en x de l'ailette       (nombre de points)
    int     My      = fetchParamValue<int>("My", params,values);        // Maillage spatial en y de l'ailette       (nombre de points)
    int     Mz      = fetchParamValue<int>("Mz", params,values);        // Maillage spatial en z de l'ailette       (nombre de points)
    double  h       = Lx/M;                                             // Pas spatial de x pour la température     m                              
    int     N       = fetchParamValue<int>("N",params,values);          // Maillage temporel                        (nombre de points)
    double  tfinal  = fetchParamValue<double>("TFinal",params,values);  // Durée finale de la simulation            s
    double  deltat  = tfinal/N;                                         // Pas temporel                             s
    /*__________________________________________________________*/

    /*_________Construction du système linéaire AT = F__________*/
    //Construction de la matrice tridiagonale A
    
    Tridiagonal trid(
        M+1,
        -kappa/(h*h), //ai
        2*kappa/(h*h) + (hc*p/s) + ( (!statio) ? (ro*Cp/deltat) : (0.0) ), //bi
        -kappa/(h*h) //ci
    );
    trid(M,M-1) = -kappa/h;
    trid(M,M) = kappa/h;
    trid(0,0) = kappa/h;
    trid(0,1) = -kappa/h;

    //Construction du vecteur correspondant au second membre, F 
    Vector F(
        M+1,
        (hc*p/s)*Te + ( (!statio) ? ( (ro*Cp/deltat)*Te ): (0.0) ) 
    );
    F[0] = Phip;
    F[M]=0.0;
    /*___________________________________________________________*/

    /*____________________Résolution du système__________________*/
    //Décomposition LU (stockée inplace)
    trid.decompositionLU(true);

    //Résolution du système LUT=F
    Vector T = trid.solveLU(F); 
    /*____________________________________________________________*/

    /*_________________Modèle stationnaire___________________*/
    if(statio){
        //Calcul de la solution exacte
        Vector T_exact(M+1);
        double a_exact = hc*p/(kappa*s);
        for(int i=0; i<M+1;i++){
            T_exact[i] =  Te +  
                        (
                            Phip *
                            std::cosh(std::sqrt(a_exact)*Lx) *
                            std::cosh(std::sqrt(a_exact)*(Lx - (i*h)))
                        ) /
                        (
                            kappa * 
                            std::sqrt(a_exact) *
                            std::sinh(std::sqrt(a_exact)*Lx) *
                            std::cosh(std::sqrt(a_exact)*Lx)
                        );
        }

        //Construction et exportation du fichier CSV
        CSV csv({T,T_exact},h);
        csv.setHeader("x,temperature_simulee,temperature_exacte");
        csv.save(std::string("./2Dsolutions/statio.csv"));
   
        Ailette ailette(Lx,Ly,Lz,Mx,My,Mz); 
        //Construction et exportation du fichier VTK
        ailette.setTemperatures(T);
        VTK vtk(ailette);
        vtk.save(std::string("./3Dsolutions/statio.vtk"));
    }
    /*_____________________________________________________*/
    /*________________Modèle intationnaire_________________*/
    else{
        Vector current_step(M+1);

        Ailette ailette1(Lx,Ly,Lz,Mx,My,Mz); 
        Ailette ailette2(Lx,Ly,Lz,Mx,My,Mz); 

        CSV csv_instatio_1;
        CSV csv_instatio_2;

        Vector Tn(M+1); 
        Vector Tn2(M+1);
        Vector F2 = F;

        Vector T1(M+1,Te);
        Vector T2(M+1,Te);
        for (int i = 1; i<N+1;i++ ){
            /*________Scénario 1 _________*/
            //Calcul de la solution (Tn+1 en fonction de Tn)
            for(int j = 1; j<M;j++){
                F[j] = (hc*p/s)*Te + (ro*Cp/deltat)*T1[j];
            }
            Tn = trid.solveLU(F); 

            //Ajoute les résultats au CSV
            current_step.setCst((i-1)*deltat);
            CSV csv_s1_tmp({current_step,T1},h);
            csv_instatio_1 += csv_s1_tmp;
            csv_instatio_1 += "\n";

            //Ajoute les résultats au VTK
            ailette1.setTemperatures(T1);
            VTK vtk1(ailette1);
            vtk1.save(std::string("./3Dsolutions//S1//sol."+toStr<int>(i-1)+".vtk"),false);

            T1 = Tn;

            /*_____________________________*/

            /*__________Scénario 2_________*/
            //Calcul de la solution (Tn+1 en fonction de Tn)
            for(int j = 1; j<M;j++){
                F2[j] = (hc*p/s)*Te + (ro*Cp/deltat)*T2[j];
            }
            if(
                (i*deltat >=30 && i*deltat <= 60) ||
                (i*deltat >=90 && i*deltat <= 120) ||
                (i*deltat >=150 && i*deltat <= 180) ||
                (i*deltat >=210 && i*deltat <= 240) ||
                (i*deltat >=270 && i*deltat <= 300)
            ){
                F2[0]= 0;
            }
            else{
                F2[0]= Phip;
            }
            Tn2 = trid.solveLU(F2);

            //Ajoute les résultats au CSV
            csv_instatio_2 += (toStr<double>((i-1)*deltat) + "," 
                                + toStr<double>(T2[0]) + "," 
                                + toStr<double>(T2[M/2]) + "," 
                                + toStr<double>(T2[M]) + "\n");

            //Ajoute les résultats au VTK
            ailette2.setTemperatures(T2);
            VTK vtk2(ailette2);
            vtk2.save(std::string("./3Dsolutions//S2//sol."+toStr<int>(i-1)+".vtk"),false);

            T2 = Tn2;
            /*_____________________________*/

            //Loading progress status
            bool entered_flag;
            if((100*i/N+1) % 15==0 || i==1 || (100*i/N+1) == 100){
                if(!entered_flag){
                    entered_flag = true;
                    std::cout << "Progress: "<<100*i/N+1<<"%\n";
                }
            }
            else{
                entered_flag = false;
            }
        }

        // Ajoute le dernier vecteur et enregistre -----scénario 1------
        current_step.setCst(N*deltat);
        CSV csv_s1_tmp({current_step,T},h);
        csv_instatio_1 += csv_s1_tmp;

        csv_instatio_1.setHeader("x,t,temperature_simulee");
        csv_instatio_1.save(std::string("./2Dsolutions//instatio_1.csv"));

        // Ajoute le dernier vecteur et enregistre ------scénario 2-------
        csv_instatio_2+=(toStr<double>(N*deltat) + "," 
                            + toStr<double>(T2[0]) + "," 
                            + toStr<double>(T2[M/2]) + "," 
                            + toStr<double>(T2[M]) + "\n");

        csv_instatio_2.setHeader("t,x_0,x_M/2,x_M");
        csv_instatio_2.save(std::string("./2Dsolutions//instatio_2.csv"));
    }
    /*_____________________________________________________*/

    return 0;
}
