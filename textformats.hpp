class TextFile{
    /*----------------------------------
    Classe mère qui représente les fichiers contenant du texte, d'extension quelconque.
    -----------------------------------*/

    public:
    /*______Constructeurs_______*/
    TextFile():_location("noname.txt"),_header(""),_body(""){}

    TextFile(std::string body,std::string header, std::string location):_location(location),_header(header),_body(body){}
    /*_________________________*/

    /*______Constructeurs_______*/
    ~TextFile(){}
    /*_________________________*/

    /*_______Accesseurs________*/
    std::string const& getLocation() const{
        return _location;
    }
    
    std::string const& getHeader() const{
        return _header;
    }
    
    std::string const& getBody() const{
        return _body;
    }    
    /*_________________________*/

    /*_______Mutateurs_________*/
    void setLocation(std::string location){
        _location = location;
    }

    void setHeader(std::string header){
        _header = header;
    }
    
    void setBody(std::string body){
        _body = body;
    }
    /*_________________________*/

    /*__________Opérateurs_________*/
    TextFile & operator+=(std::string const& str){
        _body += str;
        return *this;
    }
    TextFile & operator+=(TextFile const& txt){
        _body += txt._body;
        return *this;
    }
    TextFile operator +(std::string const& str){
        TextFile res;
        res._body = this->_body + str;
        return res;
    }
    TextFile operator+(TextFile const& txt){
        TextFile res;
        if(this->_location != txt._location){
            res._header = this->_header + txt._header;
            res._body = this->_body + txt._body;
        }
        return res;
    }
    /*_________________________*/

    /*_________Méthodes__________*/
    void save(std::string location="same",bool verbose=true){
        if(location != "same"){
            _location = location;
        }
        std::ofstream f_txt (_location,std::ios::out);
        if(f_txt){
            f_txt << _header<<"\n";
            f_txt << _body;
        }
        f_txt.close();
        if(verbose){
            std::cout << "Solutions were saved\n";
        }
    }
    /*________________________*/

    private:
    /*_______Attributs________*/
    std::string _location;
    std::string _header;
    std::string _body;
    /*_________________________*/
};

class CSV : public TextFile{
    /*----------------------------------
    Classe fille de TextFile, représente les fichier de type CSV
    ----------------------------------*/

    public:
    /*_________Constructeurs___________*/
    CSV():TextFile("","","noname.csv"){}

    CSV(std::string data):TextFile(data,"","noname.csv"){}

    CSV(std::string data, std::string columns):TextFile(data,columns,"noname.csv"){};

    CSV(std::string data, std::string columns,std::string location):TextFile(data,columns,location){};

    CSV(std::initializer_list<Vector> column_values, double index_step = 1):TextFile("","","noname.csv"){
        /*
        Constructeur de CSV par liste de vecteurs
        Paramètres: 
            - column_values: liste (brace-enclosed list) de vecteurs, sont les données qui seront en colonne.
            - index_step: double qui sert à mettre à scale la colonne de position (x) du csv. 
                e.g. si index_step = 0.1, x vaudra 0.0, 0.1, 0.2, 0.3,...
        */

        std::initializer_list<Vector>::iterator it = column_values.begin();

        //Vérifie que les vecteurs de la liste aient la même taille        
        int len = (*it).getSize(); 
        for(;it!=column_values.end();it++){
            if(len != (*it).getSize()){
                std::cout << "Column values must have the same size.\n";
                break;
            }
        } 

        //Genere chaque ligne du csv et l'ajoute à chaque fois dans un string.
        std::ostringstream data; 
        for(int i = 0; i < len; i++ ){
             data <<i*index_step << ",";
            it = column_values.begin();
            int counter = 1;
            for (; it != column_values.end();it++){
               data<< (*it)[i] << ( (counter != column_values.size())?(","):(""));
                counter ++;
            }
            data << ((i != len-1)?("\n"):(""));
        }
        this->setBody(data.str());
    }
    /*_________________________________*/
    
    /*___________Destructeur___________*/
    ~CSV(){}
    /*________________________________*/

    /*_________Muttateurs____________*/
    void addCol(std::string col_name){
        this->setHeader(this->getHeader() + std::string(",") + col_name);
    }
    /*________________________________*/

    /*_______Opérateurs (pour merge)_______*/
    CSV & operator *=(Vector const& v){  //Ajoute un vecteur en colonne (inplace)
        std::ostringstream new_data;
        std::stringstream data_ss(this->getBody());
        std::string current_line;
        for(int i=0; i<v.getSize();i++){
            data_ss >> current_line;
            new_data << current_line <<","<<v[i]<<"\n";
        }
        this->setBody(new_data.str());
        return *this;
    }
    CSV operator *(Vector const& v){  //Ajoute un vecteur en colonne
        std::ostringstream new_data;
        std::stringstream data_ss(this->getBody());
        std::string current_line;
        for(int i=0; i<v.getSize();i++){
            data_ss >> current_line;
            new_data << current_line <<","<<v[i]<<"\n";
        }
        CSV csv_res(new_data.str(),this->getHeader(),this->getLocation());
        return csv_res;
    }
    /*________________________________*/
};

class VTK:public TextFile{
    /*----------------------------------
    Classe fille de TextFile, représente les fichier de type VTK avec une STRUCTURED_GRID
    ----------------------------------*/
    public:
    VTK():TextFile(
        "",
        "# vtk DataFile Version 2.0\nvtk output\nASCII\nDATASET STRUCTURED_GRID",
        "noname.vtk"
    ){}

    VTK(std::string body):TextFile(
        body,
        "# vtk DataFile Version 2.0\nvtk output\nASCII\nDATASET STRUCTURED_GRID",
        "noname.vtk"
    ){}

    VTK(std::string body, std::string location):TextFile(
        body,
        "# vtk DataFile Version 2.0\nvtk output\nASCII\nDATASET STRUCTURED_GRID",
        location
    ){}

    VTK(Ailette ail):TextFile(
        "",
        "# vtk DataFile Version 2.0\nvtk output\nASCII\nDATASET STRUCTURED_GRID",
        "noname.vtk"
    ){
        buildMesh(ail._Mx,ail._My,ail._Mz);
        addData(ail._T);
        for(int i=1;i<ail._My*ail._Mz;i++){
            this->appendData(ail._T);
        }
    }

    void buildMesh(int dim1,int dim2,int dim3){
        //Construit le maillage (sous forme de string) selon des dimensions données
        _nb_points = dim1*dim2*dim3;
        std::ostringstream mesh_oss;
        mesh_oss << "DIMENSIONS " << dim1 << " " << dim2 << " " << dim3 << "\n";
        mesh_oss << "POINTS " << _nb_points << " float\n";
        for(int k =0; k<dim3; k++){
            for(int j =0; j<dim2;j++){
                for(int i =0; i<dim1;i++){
                    mesh_oss << i <<" "<<j<<" "<<k<<"\n";
                }   
            }
        }
        std::string tmp_body = this->getBody() + mesh_oss.str();
        this->setBody(tmp_body);
    }


    void addData(Vector T){        
        if(_nb_points == 0){
            throw std::string("Le maillage doit être construit avant d'ajouter des données");
        }

        std::ostringstream data_oss;

        data_oss << "POINT_DATA " << _nb_points << "\n" 
                << "FIELD FieldData 1\n"
                << "sol1 1 "<<_nb_points<<" float\n";
        for(int i = 0; i<T.getSize();i++){
            data_oss << T[i] << "\n";
        }
        std::string tmp_body = this->getBody() + data_oss.str();
        this->setBody(tmp_body);
    }
    void appendData(Vector T){
        std::ostringstream data_oss;
        for(int i = 0; i<T.getSize();i++){
            data_oss << T[i] << "\n";
        }
        std::string tmp_body = this->getBody() + data_oss.str();
        this->setBody(tmp_body);
    }

    private:
    int _nb_points = 0;
};
