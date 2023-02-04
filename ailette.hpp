struct Ailette{
    /*___________
        Corréspond aux caractéristiques d'une ailette du dissipateur dans un certain temps.
    ___________*/

    /*______Constructeurs_____*/
    Ailette():_Lx(0),_Ly(0),_Lz(0),_Mx(0),_My(0),_Mz(0),_T(0){}
    
    Ailette(double Lx,double Ly,double Lz):_Lx(Lx),_Ly(Ly),_Lz(Lz),_Mx(0),_My(0),_Mz(0),_T(1){}
    
    Ailette(double Lx,double Ly,double Lz,int Mx,int My,int Mz):_Lx(Lx),_Ly(Ly),_Lz(Lz),_Mx(Mx),_My(My),_Mz(Mz),_T(Mx){}
    /*________________________*/

    /*_____Déstructeur____*/
    ~Ailette(){}
    /*____________________*/

    /*______Muttateurs______*/
    void setDimensions(double x,double y,double z){
        _Lx = x;
        _Ly = y;
        _Lz = z;
    }
    void setMesh(int x,int y,int z){
        _Mx = x;
        _My = y;
        _Mz = z;
    }
    void setTemperatures(Vector T){
        double x_i00;
        int k;
        double x_k,x_k1;
        double a,b;
        for(int i = 0;i<_Mx;i++){
            x_i00 = i*(_Lx/_Mx);

            //Localisation de x_i00 dans le maillage
            k = 0;
            x_k = 0;
            x_k1 = 0;
            while(x_k1<=x_i00){
                x_k = x_k1;
                x_k1 = k*(_Lx/T.getSize());
                k++;
            }

            //Calcul des coefs de y=ax+b
            a=(T[k]-T[k+1])/(x_k-x_k1); 
            b=T[k]-a*x_k;
            
            _T[i]= a*x_i00 +b; 
        }
    }
    /*____________________*/

    /*_____Attributs_____*/
    double _Lx,_Ly,_Lz;
    int _Mx,_My,_Mz;
    Vector _T; 
    /*____________________*/
};