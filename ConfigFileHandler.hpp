/*________________________________________*/
// Fichier qui permet de traiter les fichiers de configuration,
// pour extraire les paramètres de celui ci. 
/*________________________________________*/


template <typename T>
struct Wrapper{
    /*Enveloppe permettant à une fonction de retourner deux objets*/
    T _params;
    T _values;
};

Wrapper<std::string> readConfig(char* path){
    /*
    Paramètres: 
        path: le chemin d'accès au fichier de configuration. e.g. "./simu.cfg"  
    Returns: (un objet Wrapper contenant deux strings comme attributs).
        {
            _params: noms des paramètres du fichier,
            _values: valeurs associées aux paramètres du fichier
        }
        e.g. Retourne {"Lx Ly Lz N M","0.04 0.004 0.01"}
    */

    std::ifstream f_config(path,std::ios::in);
    if (f_config){
        std::ostringstream params;
        std::ostringstream values;

        while( 1 ){
            std::string param_holder;
            std::string val_holder;

            f_config >> param_holder;
            f_config >> val_holder;
            if(f_config.eof()){
                break;
            }
            params << param_holder << " ";
            values << val_holder << " ";
        }

        Wrapper<std::string> wrapper;
        wrapper = {params.str(),values.str()};
        return wrapper;
    }
    else{ //Handle error
        Wrapper<std::string> wrapper;
        std::cout << "File not found \n";
        return wrapper;
    }

}

int wordCount(std::string str){
    /* 
    Compte le nombre de mots d'un string (séparés par des espaces ou autres séparateur par défault de std)
    Paramètres:
        str: String dont on veut compter le nombre de mots 
    Returns: 
        Un entier qui représente la quantité de mots 
    */
    std::string holder;
    std::stringstream reader(str);
    int len =0;
    while(reader >> holder){
        len ++;
    }
    return len;
}

std::string * tokenizer(std::string str){
    /*
    Sépare un string dans une liste
    Paramètres:
        str: string dont on veut séparer
    Returns:
        Une liste de strings
    */
    std::string holder;
    std::stringstream reader(str);

    int len = wordCount(str);
    int i =0;
    
    std::string * tokens= new std::string[len];

    while(reader >> holder){
        tokens[i] = holder;
        i++;
    }
    return tokens;
}

template <typename T=double>
T fetchParamValue(std::string target, std::string params, std::string values){
    /*
    Cherche l'élément d'un string associé à un élément d'un premier string
    Paramètres:
        target: string avec l'élément qu'on veut chercher
        params: string contenant un tous les targets (éléments dont on veut associer une valeur)
        values: string contenant les valeurs associés (dans l'ordre) aux params
    Returns: 
        valeur (typée) associé au target
        e.g. Si params = "Lx Ly Lz", values="0.1 0.2 0.3", et target = "Ly"
            alors elle retourne 0.2 
    */
    std::string *param_tokens;
    std::string *values_tokens;
   
    param_tokens = tokenizer(params);
    values_tokens = tokenizer(values);
    int count = 0;
    
    for(int i = 0; i<wordCount(params);i++){
        if(target == param_tokens[i]){
            break;
        }
        count ++;
    }
    std::istringstream temp_value(values_tokens[count]);
    T value;
    temp_value >> value;
    delete [] param_tokens;
    delete [] values_tokens;

    return value;
}

template<typename T=double>
std::string toStr(T obj){
    /*
    Convertit un objet (qui possède l'opérateur << ) en std::string 
    Paramètres:
        obj: objet de type T qui veut être convertit
    Returns:
        l'équivalent de l'objet en string
    */
    std::ostringstream ss; 
    ss << obj;
    std::string str =ss.str();
    return str;
}
