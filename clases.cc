//#include "varios.h"

/*********** Clase Solucion **************/

class Solucion
{

protected:

	int * array;

	int size;

        int nuevo;

public:

    Solucion()
    {};

    Solucion(int tam)
    {

	nuevo = 1;

	array = (int *)calloc(sizeof(int), tam);

									        //array = new int[tam];         

    	for(int i = 0; i < tam; i++) 
        {

                  array[i] = -1;
        }
  
    	size = tam;

    };

    int esNuevo()
    {

        return nuevo;

    };

    void cambiar_estado_a_old()
    {

        nuevo = 0; // no es nuevo 

    };

    void set_array(int tam)
    {

        nuevo = 1;
    	
        array = (int *)calloc(sizeof(int), tam);

										        //array = new int[tam];         

    	for(int i = 0; i < tam; i++) 
        {

                  array[i] = -1;
        }
  
    	size = tam;

    };


    ~Solucion()
    { 
   
	 free(array);

									         //delete[] array; array = NULL;

    }

    void resetear()
    {

    	for(int i = 0; i < size; i++)
	{

                  array[i] = -1;
        
        }
	
    };


    int esIgual(Solucion &sol)
    {
        
        int igual = 1; 
	
        for(int i = 0; i < sol.getSize(); i++)
	{
    	
            if(array[i] != sol.get(i))
	    {
                
                igual = 0;
                
                break;
                         
            }
        }

        return igual;  

    };


    void set(int pos, int valor)
    {

    	array[pos] = valor;

    };


    int get(int pos)
    {
	
    	return array[pos];
	
    };
	
    
    int getSize()
    {
	
    	return size;
	
    };
	
    
    void imprimir(FILE * f)
    {
     
    	for (int i = 0; i < size - 1; i++)
	{
    		fprintf(f, "%d - ", array[i]);
	}		
	
    	fprintf(f, "%d\n", array[size - 1]);
	
    };
	
    
    
    void imprimir()
    {
    	for (int i = 0; i < size - 1; i++)
	{
    		
             //printf("%d-", array[i]);
 
             cout << array[i] << " ";          

	}		
	
    	//printf("%d\n", array[size - 1]);
	
        cout << array[size - 1] << endl;

    };



    void destruir(void)
    {
     
	 free(array);

    };


    void solcpy(Solucion &sol)
    {
	
        nuevo = sol.esNuevo();       
 
    	for(int i = 0; i < sol.getSize(); i++)
	
    		array[i] = sol.get(i);
	
    };

};





/*********** Clase SolucionVRP ************/
class SolucionVRP : public Solucion
{

    int camiones;

    int sizeActual;

    void duplicar_size(void);

public:

    SolucionVRP();

    SolucionVRP(int tam);

   ~SolucionVRP();

    void setCamiones(int nro)
    {
	
    	camiones = nro;
	
    };
	
    void incCamiones(void)
    {
	
    	camiones++;
	
    };
	
    int getCamiones(void)
    {
	
    	return camiones;
	
    };


    void add(int valor)
    {
	
    	if(sizeActual + 1 >= size)
	
    		duplicar_size();
	
    	array[sizeActual] = valor;
	
    	sizeActual++;
	
    };

    
    int getSizeActual(void)
    {
	
    	return sizeActual;
	
    };
	

    void setSizeActual(int s)
    {
	
    	sizeActual = s;
	
    };
	

    void imprimir(FILE * f)
    {
		/*int ruta=1;
		printf("Ruta 1:0");
		for (int i=1;i<sizeActual-1;i++)
		{
			printf("-%d",array[i]);
			if(array[i]==0)
				printf("\nRuta %d:0",++ruta);
		}
		printf("-%d\n",array[sizeActual-1]);*/
		
	for (int i = 0; i < sizeActual - 1; i++)
	
    		fprintf(f, "%d-", array[i]);
			
	
    	fprintf(f, "%d\n", array[sizeActual - 1]);
	
    };
	
   		
    void resetear()
    {
	
    	for(int i = 0; i < size; i++) 
          
                  array[i] = -1;
	
    	sizeActual = 0;
	
    	camiones = 0;
	
    };
	

    void solcpy(SolucionVRP &sol)
    {
	
    	for(int i = 0; i < sol.getSizeActual(); i++)
	
    		array[i] = sol.get(i);
	
    	for(int i = sol.getSizeActual(); i < size; i++)
	
    		array[i] = -1;
	
    	sizeActual = sol.getSizeActual();
	
    	camiones = sol.getCamiones();
	
    };

};



SolucionVRP :: SolucionVRP() : Solucion() 
{

	camiones = 1;

	sizeActual = 0;

}

SolucionVRP :: SolucionVRP(int tam) : Solucion(tam)
{

	camiones = 1;

	sizeActual = 0;

}



SolucionVRP :: ~SolucionVRP()
{

    free(array); 

    array = NULL;

    //   delete[] array; array = NULL;

}


void SolucionVRP :: duplicar_size(void)
{ 

	int * arrayAnterior = array;
	

	array = (int *)malloc(sizeof(int) * size *2);

        //array = new int[size * 2];

	for(int i = 0; i < size; i++)

		array[i] = arrayAnterior[i];

	for(int i = size; i < size * 2; i++) 
     
               array[i] = -1;

	size *= 2;

	
        free(arrayAnterior);

        //delete[] arrayAnterior; arrayAnterior = NULL; 

}





/**********Clase Auxiliar para el VRPTW************/
class customerVRP
{

    double x, y, serviceTime, demanda;

    double timeStart, timeEnd;

public:

    customerVRP(void)
    {};


    void setCoord(double x, double y)
    {

	 this -> x = x;

	 this -> y = y;

    };


    double getCoordX(void)
    {
	
    	return x;
	
    };
	

    double getCoordY(void)
    {
	
    	return y;
	
    };
	

    void setServiceTime(double servTime)
    {
	
    	serviceTime = servTime;
	
    };
	

    double getServiceTime(void)
    {
	
    	return serviceTime;
	
    };
	

    void setDemanda(double dem)
    {
	
    	demanda = dem;
	
    };
	

    double getDemanda(void)
    {
	
    	return demanda;
	
    };
	

    void setWindow(double begin, double end)
    {
	
    	timeStart = begin;
	
    	timeEnd = end;
	
    };
	

    double getTimeStart(void)
    {
	
    	return timeStart;
	
    };
	

    double getTimeEnd(void)
    {
	
    	return timeEnd;
	
    };

};






/************Clase Tabla Feromonas****************/
class TablaFeromona
{

	int size;
	
	int column_size;

	double ** tabla;

public:

	TablaFeromona(int tam);

        TablaFeromona(int fila, int columna);
    
        ~TablaFeromona();

	double obtenerValor(int estOrigen, int estDestino);

	void actualizar(int estOrigen, int estDestino, double tau);

	void reiniciar(double tau0);

	void imprimir();
	
    void destruir(void)
    {
      /*	
    	for (int i = 0; i < size; i++)
	
    		free(tabla[i]);
	
    	free(tabla);
      */	
    };

};



TablaFeromona :: TablaFeromona(int tam)
{

	size = tam;

        column_size = 0;

        // tabla = new double * [tam];       
	
       
        tabla = (double **)malloc(tam * sizeof(double *));

	for (int i = 0; i < tam; i++)
	{

	    tabla[i] = (double *)malloc(tam * sizeof(double));

						            //tabla[i] = new double[tam];

	}

}


TablaFeromona :: TablaFeromona(int fila, int columna)
{

	size = fila;
	
	column_size = columna;

						        //tabla = new double * [fila];


        tabla = (double **)malloc(fila * sizeof(double *));

	for (int i = 0; i < fila; i++)
	{

	    tabla[i] = (double *)malloc(columna * sizeof(double));

					            //tabla[i] = new double[columna];
 

	}

}


TablaFeromona :: ~TablaFeromona()
{
  
       for (int i = 0; i < size; i++)
       { 	
         
					            //delete[] tabla[i]; tabla[i] = NULL;
       
            free(tabla[i]);
       
       }	
   
       free(tabla);
     
      							//delete[] tabla;

       tabla = NULL;

}
	
double TablaFeromona :: obtenerValor(int estOrigen, int estDestino)
{

	return tabla[estOrigen][estDestino];

}



void TablaFeromona :: actualizar(int estOrigen, int estDestino, double tau)
{

	tabla[estOrigen][estDestino] = tau;

}



void TablaFeromona :: reiniciar(double tau0)
{

    int fila, columna;


    fila = size;
    
    if(column_size == 0)
    {
                   
         columna = size;          
    
    }
    else
    {
        
         columna = column_size;    
    
    }

	for (int i = 0; i < fila; i++)
	{

		for (int j = 0; j < columna; j++)
		{

			tabla[i][j] = tau0;

		}

	}

}



void TablaFeromona :: imprimir()
{
    
    
    int fila, columna;


    fila = size;
    
    if(column_size == 0)
    {
                   
         columna = size;          
    
    }
    else
    {
        
         columna = column_size;    
    
    }
	for (int i = 0; i < fila; i++)
	{

		for (int j = 0; j < columna; j++)
		{

			printf("%lf ", tabla[i][j]);

		}

		printf("\n");

	}

}

