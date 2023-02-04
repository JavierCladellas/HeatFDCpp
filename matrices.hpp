#include <cmath>

class Vector{
    /*--------------------------------------------------
    Le but de cette classe est d'étendre les objet tableaux c++, pour faciliter la syntaxe de:
        - L'affichage de tableaux,
        - opérations entre vecteurs et normes,
        - Affectectation et comparation
        - Modification des éléments
    ---------------------------------------------------*/

    public:
    /*___________Constructeurs___________*/
    Vector():_size(0){}

    Vector(int size):_size(size), _vect(new double[size]){} 

    Vector(int size, float cst):_size(size),_vect(new double[size]){
        for(int i=0; i<size; i++){
            _vect[i] = cst;
        }
    }

    Vector(int size, double * array):_size(size), _vect(new double[size]){
        for(int i=0; i<size;i++){
            _vect[i] = array[i];
        }
    }
    Vector(Vector const& v):_size(v._size),_vect(new double[v._size]){
        for(int i=0; i<v._size;i++){
            _vect[i] = v._vect[i];
        }
    }
    /*____________________________________*/

    /*_____Destructeur______*/
    ~Vector(){
        delete _vect;
    }
    /*_____________________*/

    /*______Affectation et comparation_____*/
    Vector & operator=(Vector const& v){
        if(this != &v){
            if(this->_size == v._size){
                for(int i=0;i<_size;i++){
                    _vect[i] = v._vect[i];
                }
            }
        }
        return *this;
    }
    bool operator==(Vector const& v){
        if(this->_size == v._size){
            for(int i =0; i<this->_size;i++){
                if(this->_vect[i] != v._vect[i]){
                    return false;
                }
            }
        }
        else{
            return false;
        }
        return true;
    }
    /*_____________________________________*/

    /*_________Accesseurs________*/
    int const& getSize() const{
        return _size;
    }

    double * toArray() const{
        return _vect;
    } 
    
    double const& operator()(int i) const {
        return _vect[i]; 
    }
    double const& operator[](int i) const { 
        return _vect[i]; 
    }
    /*____________________________*/

    /*__________Mutateurs____________*/
    void setCst(double cst){ // Modifie les coefficients du vecteur avec la valeur de cst
        for(int i = 0 ; i<_size;i++){
            _vect[i] = cst;
        }
    }

    double & operator()(int i) { 
        return _vect[i]; 
    }
    double & operator[](int i) { 
        return _vect[i]; 
    }
    /*______________________________*/

    /*________Opérations entre vecteurs_______*/
    Vector & operator+=(Vector const& v){
        if(this->_size == v._size){
            for(int i=0; i<_size;i++){
                _vect[i] += v._vect[i];
            }
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return *this;
    }
    
    Vector & operator-=(Vector const& v){
        if(this->_size == v._size){
            for(int i=0; i<_size;i++){
                _vect[i] -= v._vect[i];
            }
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return *this;
    }
    
    Vector operator-(Vector const& v){
        Vector res(_size);
        if(this->_size == v._size){
            for(int i = 0; i<_size;i++){
                res._vect[i] = this->_vect[i] - v._vect[i];
            }
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return res;
    }
    
    Vector operator+(Vector const& v){
        Vector res(_size);
        if(this->_size == v._size){
            for(int i = 0; i<_size;i++){
                res._vect[i] = this->_vect[i] + v._vect[i];
            }
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return res;
    }
    
    Vector operator*(Vector const& v){ //Produit éléments un à un.
        Vector res(_size);
        if(this->_size == v._size){
            for(int i=0;i<_size;i++){
                res._vect[i] = this->_vect[i] * v._vect[i];
            }
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return res;
    }


    Vector operator*(double scal){ //Produit par un scalaire
        Vector res(_size);
        for(int i=0;i<_size;i++){
            res._vect[i] = this->_vect[i] * scal;
        }
        return res;
    }
    /*_____________________________*/

    /*___________Calcul de norme__________*/ 
    double norm(int type=2) const{
        double norme = 0.0;
        switch (type){
            case 2:
                for(int i =0 ; i<_size ; i++){
                    norme += _vect[i]*_vect[i];
                }
                norme = std::sqrt(norme);
                break;
            default:
                std::cout << "Does not exist or has not been implemented.\n";
                break;
            }
        
        return norme;
    }
    /*__________________________________*/

    /*______________Affichage______________*/
    friend std::ostream& operator << (std::ostream& o, Vector const& v){  //Affichage
        for(int i = 0; i< v._size;i++){
            o << v._vect[i] << "\n";
        }
        return o;
    }
    /*___________________________________*/
    
    private:
    /*________Attributs__________*/
    int _size;
    double * _vect;
    /*__________________________*/
};

class Tridiagonal; //Forward declaration

struct LUWrapper{
    /*Conteneur des matrices tridiagonales.
     Utilisé pour stocker les matrices de la 
     décomposition LU d'une tridiagonale*/
    Tridiagonal * L;
    Tridiagonal * U;
};

class Tridiagonal{
    /*-------------------------------------------------
    - Simule une matrice tridiagonale, 
    - pour des questions d'optimisation, que les trois diagonales sont stockées.
    - La classe permet d'effectuer les opérations classiques entre matrices. 
      Ainsi que d'effectuer une factorisation LU et trouver la solution 
      d'un système linéaire comportant une telle matrice.
    --------------------------------------------------*/

    public:
    /*__________Constructeurs________*/
    Tridiagonal():_size(0),_inf(new double[0]),_diag(new double[0]),_sup(new double [0]){}
    
    Tridiagonal(int size):_size(size),_inf(new double[size-1]),_diag(new double[size]),_sup(new double[size-1]){}
    
    Tridiagonal(int size, double * inf, double * diag,double * sup):_size(size),_inf(new double[size-1]),_diag(new double[size]),_sup(new double [size-1]){
        for(int i=0; i<size-1;i++){
            _inf[i] = inf[i];
            _diag[i] = diag[i];
            _sup[i] = sup[i];
        }
        _diag[size-1] = diag[size-1];
    }
    
    Tridiagonal(int size, double inf, double diag,double sup):_size(size),_inf(new double[size-1]),_diag(new double[size]),_sup(new double [size-1]){
        for(int i=0; i<size-1;i++){
            _inf[i] = inf;
            _diag[i] = diag;
            _sup[i] = sup;
        }
        _diag[size-1] = diag;
    }

    Tridiagonal(int size, double inf, double * diag,double * sup):_size(size),_inf(new double[size-1]),_diag(new double[size]),_sup(new double [size-1]){
        for(int i=0; i<size-1;i++){
            _inf[i] = inf;
            _diag[i] = diag[i];
            _sup[i] = sup[i];
        }
        _diag[size-1] = diag[size-1];
    }

    Tridiagonal(int size, double * inf, double diag,double * sup):_size(size),_inf(new double[size-1]),_diag(new double[size]),_sup(new double [size-1]){
        for(int i=0; i<size-1;i++){
            _inf[i] = inf[i];
            _diag[i] = diag;
            _sup[i] = sup[i];
        }
        _diag[size-1] = diag;
    }
    
    Tridiagonal(int size, double * inf, double * diag,double sup):_size(size),_inf(new double[size-1]),_diag(new double[size]),_sup(new double [size-1]){
        for(int i=0; i<size-1;i++){
            _inf[i] = inf[i];
            _diag[i] = diag[i];
            _sup[i] = sup;
        }
        _diag[size-1] = diag[size-1];
    }

    Tridiagonal(Tridiagonal const& t):_size(t._size),_inf(new double[t._size-1]),_diag(new double[t._size]),_sup(new double[t._size-1]){
        for(int i=0; i<t._size-1;i++){
            _inf[i] = t._inf[i];
            _diag[i] = t._diag[i];
            _sup[i] = t._sup[i];
        }
        _diag[t._size-1] = t._diag[t._size-1];
    }
    /*________________________________*/

    /*___________Destructeur___________*/
    ~Tridiagonal(){
        delete _inf;
        delete _diag;
        delete _sup;
    }
    /*__________________________*/

    /*_________Accesseurs________*/
    int const& getSize() const{
        return _size;
    }

    double const& operator()(int i,int j)const{
        
        if(i==j+1){
            return this->_inf[j];
        }
        else if(j==i+1){
            return this->_sup[i];
        }
        else if(i == j){
            return this->_diag[i];
        }
        else{
            std::cout << "Cannot change value of non tridiagonal element.\n";
            throw;
        }
    }

    /*________Affectation_________*/
    Tridiagonal & operator=(Tridiagonal const& trid){
        if(this != &trid){
            if(this->_size == trid._size){
                for(int i=0;i<_size-1;i++){
                    _inf[i] = trid._inf[i];
                    _diag[i] = trid._diag[i];
                    _sup[i] = trid._sup[i];
                }
                _diag[_size-1] = trid._diag[_size-1];
            }
        }
        return *this;
    }

    /*________Comparaison_________*/
    bool operator==(Tridiagonal const& trid){
        if(this->_size == trid._size){
            for(int i =0; i<this->_size-1;i++){
                if(
                    this->_inf[i] != trid._inf[i] ||
                    this->_diag[i] != trid._diag[i] ||
                    this->_sup[i] != trid._sup[i]
                ){
                    return false;
                }
            }
            if(this->_diag[_size-1] != trid._diag[_size-1]){
                return false;
            }
        }
        else{
            return false;
        }
        return true;
    }

    /*_________Opérations matricielles________*/
    Tridiagonal & operator+=(Tridiagonal const& trid){
        if(this->_size == trid._size){
            for(int i=0; i<_size-1;i++){
                _inf[i] += trid._inf[i];
                _diag[i] += trid._diag[i];
                _sup[i] += trid._sup[i];
            }
            _diag[this->_size] += trid._diag[trid._size];

        }
        else{
            std::cout << "Tridiagonals must have the same size.\n";
        }
        return *this;
    }
    Tridiagonal & operator-=(Tridiagonal const& trid){
        if(this->_size == trid._size){
            for(int i=0; i<_size-1;i++){
                _inf[i] -= trid._inf[i];
                _diag[i] -= trid._diag[i];
                _sup[i] -= trid._sup[i];
            }
            _diag[this->_size] -= trid._diag[trid._size];
        }
        else{
            std::cout << "Tridiagonals must have the same size.\n";
        }
        return *this;
    }
    Tridiagonal & operator*=(Tridiagonal const& trid){
        if(this->_size == trid._size){
            _inf[0] = _inf[0]*trid._diag[1] + _diag[0]*trid._inf[0];
            _diag[0] = _diag[0]*trid._diag[0] + _sup[0]*trid._inf[0];
            _sup[0] = _diag[0]*trid._sup[0] + _sup[0]*trid._diag[1];
            for(int i=1; i<_size-1;i++){
                _inf[i] = _inf[i]*trid._sup[i-1] + _diag[i+1]*trid._diag[i] + _sup[i+1]*trid._inf[1];
                _diag[i] = _inf[i-1]*trid._sup[i-1] + _diag[i]*trid._diag[i]+_sup[i]*trid._inf[i];
                _sup[i] = _diag[i]*trid._sup[i] + _sup[i]*trid._diag[i+1] + _sup[i+1]*trid._inf[1];
            }
            _inf[_size-1] = _inf[_size-1]*trid._diag[_size-2] + _diag[_size-1]*trid._inf[_size-1];
            _diag[_size-1] = _inf[_size-2]*trid._sup[_size-1] + _diag[_size-1]*trid._diag[_size-1];
            _sup[_size-1] = _diag[_size-2]*trid._sup[_size-1] + _sup[_size-1]*trid._diag[_size-1];
        }
        else{
            std::cout << "Tridiagonals must have the same size.\n";
        }
        return *this;
    }

    Tridiagonal operator+(Tridiagonal const& trid){
        Tridiagonal res(_size);
        if(this->_size == trid._size){
            for(int i = 0; i<_size-1;i++){
                res._inf[i] = this->_inf[i] + trid._inf[i];
                res._diag[i] = this->_diag[i] + trid._diag[i];
                res._sup[i] = this->_sup[i] + trid._sup[i];
            }
            res._diag[this->_size] = this->_diag[this->_size] + trid._diag[this->_size];
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return res;
    }
    Tridiagonal operator-(Tridiagonal const& trid){
        Tridiagonal res(_size);
        if(this->_size == trid._size){
            for(int i = 0; i<_size-1;i++){
                res._inf[i] = this->_inf[i] - trid._inf[i];
                res._diag[i] = this->_diag[i] - trid._diag[i];
                res._sup[i] = this->_sup[i] - trid._sup[i];
            }
            res._diag[this->_size] = this->_diag[this->_size] - trid._diag[this->_size];
        }
        else{
            std::cout << "Vectors must have the same size.\n";
        }
        return res;
    }
    Tridiagonal operator*(Tridiagonal const& trid){
        Tridiagonal res(_size);
        if(this->_size == trid._size){
            res._inf[0] = _inf[0]*trid._diag[1] + _diag[0]*trid._inf[0];
            res._diag[0] = _diag[0]*trid._diag[0] + _sup[0]*trid._inf[0];
            res._sup[0] = _diag[0]*trid._sup[0] + _sup[0]*trid._diag[1];
            for(int i=1; i<_size-1;i++){
                res._inf[i] = _inf[i]*trid._sup[i-1] + _diag[i+1]*trid._diag[i] + _sup[i+1]*trid._inf[1];
                res._diag[i] = _inf[i-1]*trid._sup[i-1] + _diag[i]*trid._diag[i]+_sup[i]*trid._inf[i];
                res._sup[i] = _diag[i]*trid._sup[i] + _sup[i]*trid._diag[i+1] + _sup[i+1]*trid._inf[1];
            }
            res._inf[_size-1] = _inf[_size-1]*trid._diag[_size-2] + _diag[_size-1]*trid._inf[_size-1];
            res._diag[_size-1] = _inf[_size-2]*trid._sup[_size-1] + _diag[_size-1]*trid._diag[_size-1];
            res._sup[_size-1] = _diag[_size-2]*trid._sup[_size-1] + _sup[_size-1]*trid._diag[_size-1];
        }
        else{
            std::cout << "Tridiagonals must have the same size.\n";
        }
        return res;
    } 

    /*_________Mutateurs_________*/
    void setCst(double constant, int diagonal = 0){
        /*
            Modifie une subdiagonale de la tridiagonale 
            Params:
                constant: constante avec laquelle modifier la subdiagonale
                diagonal: indique la subdiagonale a modifier : -1:lower, 0:diagonal, 1:upper
        */
        switch(diagonal){
            case(-1):{
                for(int i=0; i<this->_size-1;i++){
                    this->_inf[i] = constant;
                }
                break;
            }
            case(1):{
                for(int i=0; i<this->_size-1;i++){
                    this->_sup[i] = constant;
                }
                break;
            }
            case(0):{
                for(int i=0; i<this->_size;i++){
                    this->_diag[i] = constant;
                }
                break;
            }
            default:{
                std::cout << "On ne peut modifier que des éléments des trois diagonales.";
            }
        }
    }
    double & operator()(int i,int j){
        if(i==j+1){
            return this->_inf[j];
        }
        else if(j==i+1){
            return this->_sup[i];
        }
        else if(i == j){
            return this->_diag[i];
        }
        else{
            std::cout << "Cannot change value of non tridiagonal element. \n";
            throw;
        }
    }

    /*__________Méthodes___________*/ 
    LUWrapper decompositionLU(bool inplace=true){
        /*
        Effectue la decomposition LU d'une matrice tridiagonale. 
        Paramètres
            -inplace: booléan indiquant si la decomposition se stocke dans le même objet ou non
        Le stockage de la décomposition se fait sur l'objet tridiagonal de la facon suivante:
            La diagonale inférieur de L (qui a des 1 sur la diagonales) est stockée sur _inf
            La diagonale et la diagonale supérieure de U sont stockées sur _diag et _sup
        */
        if(inplace){
            LUWrapper lu= {};
            for (int i=1; i<_size;i++){
                _inf[i-1]=_inf[i-1]/_diag[i-1];
                _diag[i] = _diag[i] - _inf[i-1]*_sup[i-1];
            }
            return lu;
        }
        else{
            double inf[_size-1];
            double diag[_size];
            double sup[_size-1];
            diag[0] = _diag[0];
            for (int i=1; i<_size;i++){
                inf[i-1] = _inf[i-1];
                diag[i] = _diag[i];
                sup[i-1] = _sup[i-1];
            }
            for (int i=1; i<_size;i++){
                inf[i-1]=inf[i-1]/diag[i-1];
                diag[i] = diag[i] - inf[i-1]*sup[i-1];
            }
            
            Tridiagonal L(_size-1,inf,inf,inf);
            L.setCst(1,0);
            L.setCst(0,1);
            Tridiagonal U(_size-1,diag,diag,sup);
            U.setCst(0,-1);

            LUWrapper lu;
            lu.L = &L;
            lu.U = &U;
            return lu;
        }
    }
    Vector solveLU( Vector const& F){
        /*
        Trouve la solution X (vecteur) du système AX=F.
        A étant l'objet tridiagonal (this), auquel une décomposition LU a déjà été faite.
        */
        int n = _size;
        Vector x(n);
        x[0]=F[0];

        //Méthode de descente pour résoudre Ly=F pour y (y est stockée dans x)
        for(int i=1; i<n;i++){
            x[i] = F[i]-_inf[i-1]*x[i-1];
        }

        //Méthode de remontée pour résoudre Ux=y pour x
        x[n-1] = x[n-1]/_diag[n-1];
        for(int i=2;i<n+1;i++){
            x[n-i] = (x[n-i] - _sup[n-i]*x[n-i+1])/_diag[n-i];
        }
        return x;
    }
    /*_________________________________*/
    
    /*____________Affichage____________*/
    friend std::ostream & operator << (std::ostream& o, Tridiagonal const& tridiag){  //Affichage
        for(int i=0; i<tridiag._size;i++){
            o << "|";
            for(int j=0; j<tridiag._size;j++){
                if(j<tridiag._size-1){
                    if(j==i+1){
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<std::setprecision(3)
                            <<std::fixed
                            << tridiag._sup[i];
                    }
                    else if(i==j){
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<std::setprecision(3)
                            <<std::fixed
                            << tridiag._diag[i];
                    }
                    else if (i == j+1){
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<std::setprecision(3)
                            <<std::fixed
                            <<  tridiag._inf[i-1];
                    }
                    else{
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<  "0.00";
            
                    }
                }
                else{
                    if(j==i+1){
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<std::setprecision(3)
                            <<std::fixed
                            <<tridiag._sup[i] << "|" ;
                    }
                    else if(i==j){
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<std::setprecision(3)
                            <<std::fixed
                            <<tridiag._diag[i] << "|";
                    }
                    else if (i == j+1){
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<std::setprecision(3)
                            <<std::fixed
                            <<tridiag._inf[i-1] << "|";
                    }
                    else{
                        o<< std::setw(15)<<std::left
                            <<std::setfill(' ')
                            <<"0.00" << "|";
                    }
                }
            }    
            o << std::endl;
        }
        return o << std::endl;
    }
    /*_____________________________________*/


    private:
    /*________Attributs________*/
    int _size;
    double * _inf;
    double * _diag;
    double * _sup;
    /*__________________________*/

};

class Matrix{
    //To implement
};
