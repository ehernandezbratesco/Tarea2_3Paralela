#include <iostream>
#include <fstream>
#include <sstream>
#include <mpi.h>

using namespace std;

struct Contar{
 int cont;
 string nombre;
};

int LlenarArreglo(string archivo[],char nombre[]){
	ifstream infile(nombre); //leer el archivo
	string linea, token; //string para guardar cada linea del archivo
	int i=0, posicion; //contador para recorrer el arreglo
	getline(infile,linea); //avanzar la primera linea para omitir el titulo del csv
	char delimitador = ',';
	char delimitador2 = ' '; //para tomar solo el último dato
	while(getline(infile,linea)){
		for(int j=0;j<2;j++){
			posicion = linea.find(delimitador);
			token = linea.substr(0,posicion);
			linea.erase(0,posicion+1);
		}
		archivo[i] = token;
		i++;
	}
	return i;
}
void LimpiarArreglo(string carrera1[],string carrera2[],string carrera3[]){
	for(int i=0;i<100;i++){
		carrera1[i] = "0";
		carrera2[i] = "0";
		carrera3[i] = "0";
	}
}

void ProcesarParalelo(int largo,string archivo[],string carrera1[],string carrera2[],string carrera3[],Contar cantidad[]){
    int rank, size, limites[2], p_a1=0,p_a2=0,p_a3=0,receptor1[2],receptor2[2],receptor3[2];

    bool var1,var2,var3, termino1=false,termino2=false,termino3=false;
    LimpiarArreglo(carrera1,carrera2,carrera3);
  	MPI::Init();
    rank = MPI::COMM_WORLD.Get_rank(); //Se le asigna el procesador que se está usando
    size = MPI::COMM_WORLD.Get_size();
    if(rank==0){
    	limites[0] = 0;
      limites[1] = largo/3;
      MPI_Send(limites, 2, MPI_INT,1,0, MPI_COMM_WORLD);
      limites[0] = largo/3;
      limites[1] = 2*(largo/3);
      MPI_Send(limites, 2, MPI_INT,2, 0, MPI_COMM_WORLD);
      limites[0] = limites[1];
      limites[1] = largo+1;
      MPI_Send(limites,2, MPI_INT,3, 0, MPI_COMM_WORLD);
    }else if(rank==1){
    	MPI_Recv(receptor1, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	int n=0;
    	carrera1[0] = archivo[0];
    	cantidad[0].cont = 1;
    	cantidad[0].nombre = archivo[1];
    	p_a1 = 0;
  	  for(int i=receptor1[0]+1;i<=receptor1[1];i++){
    		var1 = true;
    		while(var1){
    			if(carrera1[n] == archivo[i]){
    				cantidad[n].cont = cantidad[n].cont + 1;
    				cantidad[n].nombre = archivo[i];
    				var1=false;
    			}else{
    				if(carrera1[n] == "0"){
    					termino1=true;
    					var1=false;
    			}
  			}
  			if(termino1){
  				cantidad[n].cont = cantidad[n].cont + 1;
  				cantidad[n].nombre = archivo[i];
  				carrera1[p_a1+1] = archivo[i];
  				p_a1++;
  				termino1=false;
  			}
  			n++;
  		}
  		n=0;
  	}
  	int aux=0;
  	for(int j = 0;j<p_a1;j++){
  	cout<<"El Procesador 1 contó "<<cantidad[j].cont<<" personas de la carrera:  "<<cantidad[j].nombre<<endl;
  	aux = cantidad[j].cont + aux;
  	}
  	cout<<"Total de personas en el procesador 1: "<<aux<<endl;
  }else if(rank==2){
  	MPI_Recv(receptor2, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	int n=0;
  	carrera2[0] = archivo[receptor2[0]];
  	cantidad[0].cont =1;
  	cantidad[0].nombre = archivo[receptor2[0]];
  	p_a2 = 0;
  	for(int i=receptor2[0]+1;i<=receptor2[1];i++){
  		var2 = true;
  		while(var2){
  			if(carrera2[n] == archivo[i]){
  				cantidad[n].cont = cantidad[n].cont + 1;
  				cantidad[n].nombre = archivo[i];
  				var2=false;
  			}else{
  				if(carrera2[n] == "0"){
  					termino2=true;
  					var2=false;
  				}
  			}
  			if(termino2){
  				cantidad[n].cont = cantidad[n].cont + 1;
  				cantidad[n].nombre = archivo[i];
  				carrera2[p_a2+1] = archivo[i];
  				p_a2++;
  				termino2=false;
  			}
  			n++;
  		}
  		n=0;
  	}
  	int aux=0;
  	for(int j = 0;j<p_a2;j++){
  	cout<<"El Procesador 2 contó "<<cantidad[j].cont<<" personas de la carrera: "<<cantidad[j].nombre<<endl;
  	aux = cantidad[j].cont + aux;
  	 	}
  	 	cout<<"Total de personas en el procesador 2: "<<aux<<endl;
  }else if(rank==3){
  	MPI_Recv(receptor3, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	int n=0;
  	carrera3[0] = archivo[receptor3[0]];
  	cantidad[0].cont =1;
  	cantidad[0].nombre = archivo[receptor3[0]];
  	p_a3 = 0;
  	for(int i=receptor3[0]+1;i<=receptor3[1];i++){
  		var3 = true;
  		while(var3){
  			if(carrera3[n] == archivo[i]){
  				cantidad[n].cont = cantidad[n].cont + 1;
  				cantidad[n].nombre = archivo[i];
  				var3=false;
  			}else{
  				if(carrera3[n] == "0"){
  					termino3=true;
  					var3=false;
  				}
  			}

  			if(termino3){
  				cantidad[n].cont = cantidad[n].cont + 1;
  				cantidad[n].nombre = archivo[i];
  				carrera3[p_a3+1] = archivo[i];
  				p_a3++;
  				termino3=false;
  			}
  			n++;
  		}
  		n=0;
  	}
  	int aux=0;
  	for(int j = 0;j<p_a3;j++){
  	cout<<"El procesador 3 contó "<<cantidad[j].cont<<" personas de la carrera: "<<cantidad[j].nombre<<endl;
  	aux = cantidad[j].cont + aux;
  	}
  	cout<<"Total de personas en el procesador 3: "<<aux<<endl;
  }
  	MPI_Finalize();
}

int main(int argc, char* argv[]){
    string archivo[50000];
    string carrera1[100],carrera2[100],carrera3[100];
    int l;
    Contar cantidad[150];
    l = LlenarArreglo(archivo,argv[1]); //devuelve el largo del archivo
    ProcesarParalelo(l,archivo,carrera1,carrera2,carrera3,cantidad);
}
