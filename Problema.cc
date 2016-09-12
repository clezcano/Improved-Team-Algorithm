
/************ Clase Problema *************/

class Problem
{

protected:

        double ** matrizAdy;

	int size;

        int column_size;

	virtual void cargar_estado(char * file) = 0;

public:

	Problem(char * file);

	~Problem();

	virtual int get_column_size(void) = 0;

        virtual int getSize(void) = 0;

	virtual double funcion_obj_1(Solucion &sol) = 0;

	virtual double funcion_obj_2(Solucion &sol) = 0;

	virtual double heuristica_1(int estOrigen, int estDest) = 0;

	virtual double heuristica_2(int estOrigen, int estDest) = 0;
	
        double getDistancia(int i,int j)
        {

		return matrizAdy[i][j];

	};
	

        void destruir(void)
        {
             /*
		for(int i = 0; i < size; i++)

			free(matrizAdy[i]);

		free(matrizAdy);
             */
	};

};




Problem :: Problem(char * file)
{
	
    column_size = 0;

}

Problem :: ~Problem()
{
 /* 
    for(int i = 0; i < size; i++)

	free(matrizAdy[i]);

    free(matrizAdy);
  */
};




/************ Clase TSP **************/
class TSP : public Problem
{

	double ** matrizAdy2;

	void cargar_estado(char * file);

public:

	TSP(char * file);

        ~TSP();

	double funcion_obj_1(Solucion &sol);

	double funcion_obj_2(Solucion &sol);
	

    	double heuristica_1(int estOrigen, int estDest)
    	{

		return 1.0 / matrizAdy[estOrigen][estDest];

	};
	
    	double heuristica_2(int estOrigen, int estDest)
    	{

		return 1.0 / matrizAdy2[estOrigen][estDest];

	};
	
    	void imprimir_matrices(void);
	
	int getSize(void)
        { 
        
             return size; 
    
        };
	
        int get_column_size(void)
        { 
        
             return column_size; 
    
        };
    	
        void destruir(void)
    	{
            /*
		for(int i = 0; i < size; i++)

			free(matrizAdy2[i]);
	
    		free(matrizAdy2);
		
		((Problem *)this) -> destruir();
              */
	};

};



TSP :: TSP(char * file) : Problem(file)
{


  	FILE * f = fopen(file, "r");

	// obtener el tama� del problema

	fscanf(f, "%d\n", &size);

	fclose(f);


							        //matrizAdy = new double * [size];

	// inicializar matriz de adyacencia
	matrizAdy = (double **)malloc(sizeof(double *) * size);

	for(int i = 0; i < size; i++)
        {
		matrizAdy[i] = (double *)malloc(sizeof(double) * size);

						        //      matrizAdy[i] = new double[size];
        }

	matrizAdy2 = (double **)malloc(sizeof(double *) * size);
	
							        //matrizAdy2 = new double * [size]; 

    	for(int i = 0; i < size; i++)
        {
		matrizAdy2[i] = (double *)malloc(sizeof(double) * size);

						        //        matrizAdy2[i] = new double[size];
        }
	
        cargar_estado(file);

}


TSP :: ~TSP()
{


    for(int i = 0; i < size; i++)
    {
         
          free(matrizAdy[i]);
         									 //delete[] matrizAdy[i];

          matrizAdy[i] = NULL;
    
    }
    
    free(matrizAdy);

									//    delete[] matrizAdy; matrizAdy = NULL;

    for(int i = 0; i < size; i++)
    {

         free(matrizAdy2[i]);
     
									//            delete[] matrizAdy2[i]; 

         matrizAdy2[i] = NULL;

    }

   free(matrizAdy2);	

									//        delete[] matrizAdy2;

    matrizAdy2 = NULL;

}


void TSP :: cargar_estado(char * file)
{
	// El archivo file posee las dos matrices de adyacencia separadas por '\n'
	// en la primera linea posee el tama� del problema
	FILE * f;

	int i, j;

	f = fopen(file, "r");

	fscanf(f, "%d\n", &i);

	fscanf(f, "%d\n", &i);

	for(i = 0; i < size; i++)

		for(j = 0; j < size; j++)

			fscanf(f, "%lf", &matrizAdy[i][j]);

	for(i = 0; i < size; i++)

		for(j =0 ; j < size; j++)

			fscanf(f, "%lf", &matrizAdy2[i][j]);

	fclose(f);

}


double TSP :: funcion_obj_1(Solucion &sol)
{

	int i;

	double suma = 0.0;

	for(i = 0; i < sol.getSize() - 1; i++)

		suma += matrizAdy[sol.get(i)][sol.get(i + 1)];

        suma += matrizAdy[sol.get(sol.getSize() - 1)][sol.get(0)];

	return suma;

}


double TSP :: funcion_obj_2(Solucion &sol)
{

	int i;

	double suma = 0.0;

	for(i = 0; i < sol.getSize() - 1; i++)

		suma += matrizAdy2[sol.get(i)][sol.get(i + 1)];

	suma += matrizAdy2[sol.get(sol.getSize() - 1)][sol.get(0)];

	return suma;

}


void TSP :: imprimir_matrices(void)
{

	int i, j;

	printf("Matriz Adyacencia 1:\n");

	for(i = 0; i < size; i++)
	{

		for(j = 0; j < size; j++)

			printf("%lf ", matrizAdy[i][j]);

		printf("\n");

	}

	printf("Matriz Adyacencia 2:\n");

	for(i = 0; i < size; i++)
	{

		for(j = 0; j < size; j++)

			printf("%lf ", matrizAdy2[i][j]);

		printf("\n");

	}

}










/************ Clase QAP **************/

class QAP : public Problem
{

	double ** matrizFlujo1;

	double ** matrizFlujo2;

	void cargar_estado(char * file);

public:

	QAP(char * file);

        ~QAP();

	double funcion_obj_1(Solucion &sol);

	double funcion_obj_2(Solucion &sol);

	double heuristica_1(int estOrigen,int estDest)
    	{
           
           return 1;
    
    	}; // No se utilizan

	double heuristica_2(int estOrigen,int estDest)
    	{
           
           return 1;
    
    	}; // heur�ticas

	int getSize(void)
        { 
        
             return size; 
    
        };
	
        int get_column_size(void)
        { 
        
             return column_size; 
    
        };
    	
	void imprimir_matrices(void);

	void destruir(void)
	{

		for(int i = 0; i < size; i++)

			free(matrizFlujo1[i]);

		free(matrizFlujo1);

		for(int i = 0; i < size; i++)

			free(matrizFlujo2[i]);

		free(matrizFlujo2);
		
		((Problem *)this) -> destruir();

	};

};


QAP :: QAP(char * file) : Problem(file)
{


  	FILE * f = fopen(file, "r");

	// obtener el tama� del problema
	fscanf(f, "%d\n", &size);

	fclose(f);

	// inicializar matriz de adyacencia
	matrizAdy = (double **)malloc(sizeof(double *) * size);

	for(int i = 0; i < size; i++)

		matrizAdy[i] = (double *)malloc(sizeof(double) * size);

	matrizFlujo1 = (double **)malloc(sizeof(double *) * size);

	for(int i = 0; i < size; i++)

		matrizFlujo1[i] = (double *)malloc(sizeof(double) * size);

	matrizFlujo2 = (double **)malloc(sizeof(double *) * size);

	for(int i = 0; i < size; i++)

		matrizFlujo2[i] = (double *)malloc(sizeof(double) * size);

	cargar_estado(file);

}

QAP :: ~QAP()
{


    for(int i = 0; i < size; i++)

	free(matrizAdy[i]);

    free(matrizAdy);

	for(int i = 0; i < size; i++)

		free(matrizFlujo1[i]);

	free(matrizFlujo1);

	for(int i = 0; i < size; i++)

		free(matrizFlujo2[i]);

        free(matrizFlujo2);

}


void QAP :: cargar_estado(char * file)
{
	// El archivo file posee las tres matrices: adyacencia,flujo1,flujo2 separadas por '\n'
	// en la primera linea posee informacion adicional
	FILE * f;

	int i, j;

	f = fopen(file, "r");

	fscanf(f, "%d\n", &i);

	for(i = 0; i < this -> size; i++)

		for(j = 0; j < this -> size; j++)

			fscanf(f, "%lf", &matrizAdy[i][j]);

	for(i = 0; i < size; i++)

		for(j = 0; j < size; j++)

			fscanf(f, "%lf", &matrizFlujo1[i][j]);

	for(i = 0; i < this -> size; i++)

		for(j = 0; j < this -> size; j++)

			fscanf(f, "%lf", &matrizFlujo2[i][j]);

	fclose(f);

}



double QAP :: funcion_obj_1(Solucion &sol)
{

	int i, j;

	double suma = 0;

	for(i = 0; i < sol.getSize(); i++)

		for(j = 0; j < sol.getSize(); j++)

			suma += matrizAdy[i][j] * matrizFlujo1[sol.get(i)][sol.get(j)];
			

	return suma;

}



double QAP :: funcion_obj_2(Solucion &sol)
{

	int i, j;

	double suma = 0;

	for(i = 0; i < sol.getSize(); i++)

		for(j = 0; j < sol.getSize(); j++)

			suma += matrizAdy[i][j] * matrizFlujo2[sol.get(i)][sol.get(j)];

			
	return suma;

}



void QAP :: imprimir_matrices(void)
{
	int i, j;
	
	printf("Matriz Adyacencia:\n");

	for(i = 0; i < size; i++)
	{
		
	        for(j = 0; j < size; j++)

			printf("%lf ", matrizAdy[i][j]);

		printf("\n");

	}

	printf("Matriz flujo 1:\n");

	for(i = 0; i < size; i++)
	{

		for(j = 0; j < size; j++)

			printf("%lf ", matrizFlujo1[i][j]);

		printf("\n");

	}

	printf("Matriz flujo 2:\n");

	for(i = 0; i < size; i++)
	{

		for(j = 0; j < size; j++)

			printf("%lf ", matrizFlujo2[i][j]);

		printf("\n");

	}

}





/************ Clase VRPTW **************/

class VRPTW : public Problem
{

	int capacity;

	customerVRP * customers;

	void cargar_estado(char * file);

	void generar_matriz_ady(void);

public:

	VRPTW(char * file);

        ~VRPTW();

	double funcion_obj_1(Solucion &sol)
    	{

                SolucionVRP * soluc = (SolucionVRP *) &sol;

		return soluc -> getCamiones(); // devuelve la cantidad camiones

	};


	double funcion_obj_2(Solucion &sol);


	double heuristica_1(int estOrigen, int estDest)
        {

           return 1;

    	};

	double heuristica_2(int estOrigen, int estDest)
    	{

		return 1.0 / matrizAdy[estOrigen][estDest];

	};


	int getSize(void)
        { 
        
             return size; 
    
        };
	
        int get_column_size(void)
        { 
        
             return column_size; 
    
        };
    	
	int getCapacity(void)
    	{
        	return capacity;
        
    	};

	double getDemanda(int customer)
    	{

		return customers[customer].getDemanda();

	};

	double getTimeStart(int customer)
    	{

		return customers[customer].getTimeStart();

	};

	double getTimeEnd(int customer)
    	{

		return customers[customer].getTimeEnd();

	};

	double getServiceTime(int customer)
    	{

		return customers[customer].getServiceTime();

	};

	void imprimir(void);

};



VRPTW :: VRPTW(char * file) : Problem(file)
{


  	FILE * f = fopen(file, "r");

	// obtener el tama� del problema
	fscanf(f, "%d\n", &size);

	fclose(f);

	// inicializar matriz de adyacencia
	matrizAdy = (double **)malloc(sizeof(double *) * size);

	for(int i = 0; i < size; i++)

		matrizAdy[i] = (double *)malloc(sizeof(double) * size);

	customers = new customerVRP[size];

	cargar_estado(file);

}


VRPTW :: ~VRPTW()
{


    for(int i = 0; i < size; i++)
    {

	free(matrizAdy[i]);

    }
    
    free(matrizAdy);
    
    delete[] customers;

}

void VRPTW :: cargar_estado(char * file)
{
	// El archivo file posee: la cantidad de customers, la capacidad de los camiones
	// y los datos de cada customer: coordenadas, demanda, ventana y tiempo de servicio
	FILE * f;

	double x, y, dem, begin, end, servTime;

	int i;
	
	f = fopen(file, "r");

	fscanf(f, "%d\n", &i);

	fscanf(f, "%d\n", &capacity);



	for(i = 0; i < size; i++)
	{

		fscanf(f, "%lf", &x);

		fscanf(f, "%lf", &x);

		fscanf(f, "%lf", &y);

		fscanf(f, "%lf", &dem);

		fscanf(f, "%lf", &begin);

		fscanf(f, "%lf", &end);

		fscanf(f, "%lf", &servTime);

		customers[i].setCoord(x, y);

		customers[i].setDemanda(dem);

		customers[i].setWindow(begin, end);

		customers[i].setServiceTime(servTime);

	}

	fclose(f);

	generar_matriz_ady();

}




void VRPTW :: generar_matriz_ady(void)
{
	// a partir de las coordenadas de los customers se genera la matriz simetrica
	// de adyacencia con las dinstancias euclideas entre cada par de customers

	double aux;
	
	for(int i = 0; i < size; i++)
	{

		for(int j = i + 1; j < size; j++)
		{

			aux = pow(customers[i].getCoordX() - customers[j].getCoordX(), 2);

			aux += pow(customers[i].getCoordY() - customers[j].getCoordY(), 2);

			matrizAdy[i][j] = sqrt(aux);

			matrizAdy[j][i] = matrizAdy[i][j];

		}

		matrizAdy[i][i] = 0;

	}

}



double VRPTW :: funcion_obj_2(Solucion &sol)
{

	int i;

	double suma = 0;

	SolucionVRP * s = (SolucionVRP *) &sol;

	for(i = 0; i < s -> getSizeActual() - 1; i++)

		suma += matrizAdy[s -> get(i)][s -> get(i + 1)];

	suma += matrizAdy[s -> get(s -> getSizeActual() - 1)][s -> get(0)];				// YA SE SUMO AL DEPOSITO

	return suma; // devolver el "Total Travel Distance"

}



void VRPTW :: imprimir(void)
{

	printf("Matriz Adyacencia:\n");

	for(int i = 0; i < 1; i++)
	{

		for(int j = 0; j < size; j++)

			printf("%lf ", matrizAdy[i][j]);

		printf("\n");

	}

	printf("Size: %d\n", size);

	printf("capacity: %d\n", capacity);

	for(int i = 0; i < 20; i++)
	{

		printf("Customer %d\n", i);

		printf("Demanda: %lf\n", customers[i].getDemanda());

		printf("Service Time: %lf\n", customers[i].getServiceTime());

		printf("Begin: %lf\n", customers[i].getTimeStart());

		printf("End: %lf\n", customers[i].getTimeEnd());

	}

}


/************ Clase BOMBAS **************/

class BOMBAS : public Problem
{

    int ** binario;
    
    int * caudal;
    
    int * potencia;
    
    int * demanda;
    
    double ** heuristica1_costo;
    
    double ** heuristica2_mantenimiento;
    
    double ** heuristica3_nivel;
 
    double * heuristica4_potencia;
    
    int cantidad_intervalos;
    
    int cantidad_combinaciones;
    
    int cantidad_bombas;


    
    void cargar_estado(char * file);

    void calcular_heuristicas();


    
public:

	BOMBAS(char * file);

	double funcion_obj_1(Solucion &sol);

	double funcion_obj_2(Solucion &sol);

	double funcion_obj_3(Solucion &sol);

	double funcion_obj_4(Solucion &sol);

   	int getSize(void)
    {
        
         return cantidad_intervalos;
    
    };

	int get_column_size(void)
    { 
        
         return cantidad_combinaciones; 
    
    };

    int get_cantidad_bombas()
    {
        
         return cantidad_bombas;
        
    };
    
	double heuristica_1(int intervalo, int i)
   	{
           
           return heuristica1_costo[intervalo][i];
    
   	}; // No se utilizan

	double heuristica_2(int bomba, int i)
   	{
           
           return heuristica2_mantenimiento[bomba][i];
    
   	}; 

	double heuristica_3(int intervalo, int i)
   	{
           
           return heuristica3_nivel[intervalo][i];
    
   	}; // No se utilizan

	double heuristica_4(int i)
   	{
           
           return heuristica4_potencia[i];
    
   	}; 

	void imprimir_matrices(void);
     /*
	void destruir(void)
	{

		for(int i = 0; i < size; i++)

			free(matrizFlujo1[i]);

		free(matrizFlujo1);

		for(int i = 0; i < size; i++)

			free(matrizFlujo2[i]);

		free(matrizFlujo2);
		
		((Problem *)this) -> destruir();

	};
     
     */
};



BOMBAS :: BOMBAS(char * file) : Problem(file)
{

    FILE * f = fopen(file, "r");

	fscanf(f, "%d\n", &cantidad_combinaciones);

  	fscanf(f, "%d\n", &cantidad_bombas);

    fclose(f);
    
	
    binario = (int **)malloc(sizeof(int *) * cantidad_combinaciones);

	for(int i = 0; i < cantidad_combinaciones; i++)
    {
	
    	binario[i] = (int *)malloc(sizeof(int) * cantidad_bombas);
    
    }
	
    caudal = (int *)malloc(sizeof(int) * cantidad_combinaciones);
    
    potencia = (int *)malloc(sizeof(int) * cantidad_combinaciones);
    
	
    cargar_estado(file);

    calcular_heuristicas();
    
}


void BOMBAS :: cargar_estado(char * file)
{

    int i, j, aux;

    FILE * f = fopen(file, "r");

	fscanf(f, "%d\n", &i);
	
	fscanf(f, "%d\n", &i);
    	
	
    for(int i = 0; i < cantidad_combinaciones; i++)
    {

	      fscanf(f, "%d", &aux);

          for(int j = 0; j < cantidad_bombas; j++)
          {

	             fscanf(f, "%d", &binario[i][j]);

          }

   	      fscanf(f, "%d", &caudal[i]);

  	      fscanf(f, "%d\n", &potencia[i]);
    
    }

	fscanf(f, "%d\n", &cantidad_intervalos);
	
	for(i = 0; i < cantidad_intervalos; i++)
    {

  	      fscanf(f, "%d", &demanda[i]); 

    }

    fclose(f);
    

}


void BOMBAS :: calcular_heuristicas()
{
     
    int contador = 0;
    
    int diferencia_nivel;
    
    int valor;

 
    heuristica1_costo = (double **)malloc(sizeof(double *) * cantidad_intervalos);

	for(int i = 0; i < cantidad_intervalos; i++)
    {
	
    	heuristica1_costo[i] = (double *)malloc(sizeof(double) * cantidad_combinaciones);
    
    }
    
    
    for(int i = 0; i < cantidad_intervalos; i++)
    {

          heuristica1_costo[i][0] = 0.0001;  

          if((i >= 0 && i <= 16) || (i >= 22 && i <= 23))
          {    
      
                for(int j = 1; j < cantidad_combinaciones; j++)
                {
	
	                     heuristica1_costo[i][j] = 1/(TARIFA_REDUCIDA * potencia[j]);
                            
                }
    
           }
           else if(i >= 17 && i <= 21)
           {

                for(int j = 1; j < cantidad_combinaciones; j++)
                {
            
                        heuristica1_costo[i][j] = 1/(TARIFA_ELEVADA * potencia[j]);
            
                }

           }    
          
    }
   
    heuristica2_mantenimiento = (double **)malloc(sizeof(double *) * cantidad_combinaciones);

	for(int i = 0; i < cantidad_combinaciones; i++)
    {
	
    	    heuristica2_mantenimiento[i] = (double *)malloc(sizeof(double) * cantidad_combinaciones);
    
    }
    
    for(int i = 0; i < cantidad_combinaciones; i++)
    {

            for(int j = 0; j < cantidad_combinaciones; j++)
            {
	
                    contador = 0;
                    
                 	for(int k = 0; k < cantidad_bombas; k++)
                    {
    
                             valor = abs(binario[j][k] - binario[i][k]); 
                             
                             if( valor == 1)
                             {
               
                                   contador++;   
                              
                             }               
           
                    }

                    if(contador == 0)
                    {

                             heuristica2_mantenimiento[i][j] = 1/0.5;

                    }
                    else
                    {

                             heuristica2_mantenimiento[i][j] = 1/contador;

                    }            

            }
          
    }
    
    
    heuristica3_nivel = (double **)malloc(sizeof(double *) * cantidad_intervalos);

	for(int i = 0; i < cantidad_intervalos; i++)
    {
	
    	    heuristica3_nivel[i] = (double *)malloc(sizeof(double) * cantidad_combinaciones);
    
    }
    
    for(int i = 0; i < cantidad_intervalos; i++)
    {

            for(int j = 0; j < cantidad_combinaciones; j++)
            {
	
                     diferencia_nivel = abs(caudal[j] - demanda[i]);
                     
                     if(diferencia_nivel == 0)
                     {

                               heuristica3_nivel[i][j] = 1/(0.26/AREA_RESERVORIO);//0.26 para que la heuris sea 10000

                     }
                     else
                     {          
     
                               heuristica3_nivel[i][j] = 1/(diferencia_nivel/AREA_RESERVORIO);

                     }
                     
            }
          
    }
    
    
    heuristica4_potencia = (double *)malloc(sizeof(double) * cantidad_combinaciones);

    heuristica4_potencia[0] = 0.005;

    for(int i = 1; i < cantidad_combinaciones; i++)
    {

            heuristica4_potencia[i] = 1/potencia[i];
     
    }
   
 
}


double BOMBAS :: funcion_obj_1(Solucion &sol)
{

    double suma = 0.0;
    
    
	for(int i = 0; i < cantidad_intervalos; i++)
    {
    
           if((i >= 0 && i <= 16) || (i >= 22 && i <= 23))
           {    
                   
                   suma += TARIFA_REDUCIDA * potencia[sol.get(i)];
     
           }
           else if(i >= 17 && i <= 21)
           {

                   suma += TARIFA_ELEVADA * potencia[sol.get(i)];
           
           }
            
    }

	return suma;

}



double BOMBAS :: funcion_obj_2(Solucion &sol)
{


	int contador = 0;
	
	double suma = 0.0;
	

  	for(int j = 0; j < cantidad_bombas; j++)
    {
    
        if(binario[sol.get(0)][j] - binario[0][j] == 1)
        {
               
              contador++;
                              
        }               
           
    }

	for(int i = 1; i < cantidad_intervalos; i++)
    {
    
     	for(int j = 0; j < cantidad_bombas; j++)
        {
    
              if(binario[sol.get(i)][j] - binario[sol.get(i - 1)][j] == 1)
              {
               
                    contador++;
                              
              }               
           
        }
            
    }

    suma = contador;
    
    contador = 0;
    
  	for(int j = 0; j < cantidad_bombas; j++)
    {
    
        if(binario[sol.get(0)][j] - binario[sol.get(23)][j] == 1)
        {
               
              contador++;
                              
        }               
           
    }

	return suma + contador/2.0;

}



double BOMBAS :: funcion_obj_3(Solucion &sol)
{


	double suma = 0.0;

        int caudal_suministrado, caudal_demandado;


//    altura_anterior =  ALTURA_INICIAL;
    
	for(int i = 0; i < cantidad_intervalos; i++)
    {
         
         caudal_suministrado = caudal[sol.get(i)];
         
         caudal_demandado = demanda[i];
         
         suma = suma + (abs(caudal_suministrado - caudal_demandado) / AREA_RESERVORIO);
         //altura_actual = altura_anterior + ((caudal_suministrado - caudal_demandado)/ AREA_RESERVORIO);
    
         //suma = suma + abs(altura_actual - altura_anterior);
    
         //altura_anterior = altura_actual;
         
    }

	return suma;

}


double BOMBAS :: funcion_obj_4(Solucion &sol)
{


	double maxima_potencia = potencia[sol.get(0)];


	for(int i = 1; i < cantidad_intervalos; i++)
    {

            if(potencia[sol.get(i)] > maxima_potencia)
            {
            
                    maxima_potencia = potencia[sol.get(i)];               
                           
            }
    
    } 		

	return maxima_potencia;

}










/*
int main()
{
	Problem *p=new VRPTW("c101.txt");
	SolucionVRP sol(p->getSize()+100);
	
	for(int i=0;i<p->getSize();i++)
		sol.add(i);
	double f1=p->funcion_obj_1(sol);
	double f2=p->funcion_obj_2(sol);
	printf("%f*%f\n",f1,f2);
	((VRPTW *)p)->i
*/
