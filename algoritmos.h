

/**********************BICRITERION ANT*************************************/

class BicriterionAnt : public MOACO
{

	TablaFeromona * tabla1, * tabla2, * tabla3, * tabla4; //Matrices de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, F3MAX, F4MAX, NORM1, NORM2, NORM3, NORM4; // utilizados para normalizacion

	int lambda_funcion_1, lambda_funcion_2, lambda_funcion_3, lambda_funcion_4; // utilizado para calcular los pesos lambda
	
        int hormigaActual; // utilizado para calcular los pesos lambda


	void inicializar_tablas(void)
	{

	        if(prob -> get_column_size() == 0)// para tsp, qap, y vrp
       		{
    	     
                    tabla1 = new TablaFeromona(prob -> getSize());

		    tabla2 = new TablaFeromona(prob -> getSize());
       
       		}
	        else //para bombas
       		{
           
                    //fila = cantidad intervalos columnas = cantidad combinaciones
            	   
                    tabla1 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());

	            tabla2 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());

	            tabla3 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());

	            tabla4 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());


        	    tabla3 -> reiniciar(taoInicial);

		    tabla4 -> reiniciar(taoInicial);

       		}

		tabla1 -> reiniciar(taoInicial);

		tabla2 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize,int cantNoDominados);

        void actualizar_feromonas_bombas(Solucion &sol, int solSize,int cantNoDominados);

	int seleccionar_probabilistico(int estOrigen,int visitados[]);

	int seleccionar_probabilistico(int intervalo, int bomba);
	
        void evaporar_feromonas(void);

	void evaporar_feromonas_bombas(void);


	
public:

	BicriterionAnt(Problem * p, char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

             	NORM3 = 1; //por defecto

		NORM4 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		F3MAX = 1; //por defecto

		F4MAX = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tablas();
	};

        ~BicriterionAnt()  
        {

             delete tabla1; tabla1 = NULL;

             delete tabla2; tabla2 = NULL;
             
             delete tabla3; tabla3 = NULL;
             
             delete tabla4; tabla4 = NULL;
            
             delete pareto; pareto = NULL;

        };
	

        int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bombas);

	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);

        void ejecutarBOMBAS(void);
    
};

void BicriterionAnt :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1)
             
             tiempoTotal = valor;

	else 
             
             maxIteraciones = valor;

	// leer parametros especificos del BicriterionAnt
	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

   		else if(stricmp(cad, "F3MAX") == 0)

			fscanf(f, "%lf", &F3MAX);

		else if(stricmp(cad, "F4MAX") == 0)

			fscanf(f, "%lf", &F4MAX);
	
             	else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

    	        else if(stricmp(cad, "D3MAX") == 0)

			fscanf(f, "%lf", &NORM3);

		else if(stricmp(cad, "D4MAX") == 0)

			fscanf(f, "%lf", &NORM4);

		else

			fscanf(f, "%s", cad);

	}	

}



int BicriterionAnt :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{

								// cout << "entra seleccionar" << endl; 

	int sgteEstado, * visitados = (int *) calloc(prob -> getSize(), sizeof(int)); 
	
								// int sgteEstado, * visitados = new int[prob -> getSize()];



	// marcar estados ya visitados, hallar el vecindario

	for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	

	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);


									        //delete[] visitados;

        free(visitados); 

	return sgteEstado;

}



int BicriterionAnt :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;

       
        double * productos = (double *) calloc(prob -> getSize(), sizeof(double));

				                                        //double * productos = new double[prob -> getSize()];
								
									//cout << "prob" <<  endl;	

        double random, suma = 0.0;

	double acum = 0.0;

	double lambda;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;
	


	lambda = hormigaActual / (double)(hormigas - 1); // peso de la hormiga actual

	random = rand() / (double) RAND_MAX; // escoger un valor entre 0 y 1


	// hallar la suma y los productos

	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

			productos[i] = pow(tabla1 -> obtenerValor(estOrigen, i), lambda * alfa) * pow(tabla2 -> obtenerValor(estOrigen, i), (1 - lambda) * alfa) * pow(heuristica1, lambda * beta) * pow(heuristica2, (1 - lambda) * beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{

		// aplicar ruleta

		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}
			}
		}
	}

					        //delete[] productos; producto = NULL;

        free(productos); 

	return sgteEstado;

}

int BicriterionAnt :: seleccionar_probabilistico(int intervalo, int bomba)
{

	int sgteEstado;

	double heuristica1, heuristica2, heuristica3, heuristica4;


     Problem * problema = prob;

      BOMBAS * prob = (BOMBAS *) problema; 
	
		    //double * productos = (double *)calloc(prob -> get_column_size(), sizeof(double));
    
        double * productos = new double[prob -> get_column_size()];

	double random, suma = 0.0;

	double acum = 0.0;

	int sinPorcion[prob -> get_column_size()], cantSinPorcion = 0;


	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

	// hallar la suma y los productos
	for(int i = 0; i < prob -> get_column_size(); i++)
	{

			heuristica1 = prob -> heuristica_1(intervalo, i) * NORM1; 

			heuristica2 = prob -> heuristica_2(bomba, i) * NORM2; 

            heuristica3 = prob -> heuristica_3(intervalo, i) * NORM3; 

			heuristica4 = prob -> heuristica_4(i) * NORM4; 

	        productos[i] = pow(tabla1 -> obtenerValor(intervalo, i), lambda_funcion_1 * alfa) * pow(tabla2 -> obtenerValor(intervalo, i), lambda_funcion_2 * alfa) * pow(tabla3 -> obtenerValor(intervalo, i), lambda_funcion_3 * alfa) * pow(tabla4 -> obtenerValor(intervalo, i), lambda_funcion_4 * alfa) * pow(heuristica1, lambda_funcion_1 * beta) * pow(heuristica2, lambda_funcion_2 * beta)* pow(heuristica3, lambda_funcion_3 * beta)* pow(heuristica4, lambda_funcion_4 * beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{
		// aplicar ruleta
		for (int i = 0; i < prob -> get_column_size(); i++)
		{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

		}

	}

					//free(productos);

        delete[] productos;

        productos = NULL;

	return sgteEstado;

}

int BicriterionAnt :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}



int BicriterionAnt :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
     							   //int sgteEstado, * visitados = new int [prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;
//cout << "A1" << endl;	
	// hallar el vecindario
	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) 
         
                    totalVisitados++; // estado 0 ya se contabilizo

	}
//cout << "A2" << endl;	
	for (int i = 0; i < problem -> getSize(); i++)
	{

		if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));

			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i,0) > problem -> getTimeEnd(0))
			{

				visitados[i] = 1; // marcar como no vecino

				totalVisitados++;

			}

		}

	}
//cout << "totalvisitados :" << totalVisitados << endl;	
	if(totalVisitados >= problem -> getSize())

		sgteEstado = 0; // ir al deposito

	else

		sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		

	free(visitados);
	
       							 //delete[] visitados;

	return sgteEstado;

}


int BicriterionAnt :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bombas)
{ 

    int sgteEstado = seleccionar_probabilistico(intervalo, bombas);


	return sgteEstado;

}


void BicriterionAnt :: actualizar_feromonas(Solucion &sol, int solSize, int cantNoDominados)
{

	double valorTabla;

	int j, k;

	for(int i = 0; i < solSize - 1; i++)
	{	// actualizar ambas tablas en una cantidad 1/cantNoDominados

		j = sol.get(i);

		k = sol.get(i + 1);

		valorTabla = tabla1 -> obtenerValor(j, k);

		tabla1 -> actualizar(j, k, valorTabla + 1.0 / cantNoDominados);
                                                                                                 
		valorTabla = tabla2 -> obtenerValor(j, k);

		tabla2 -> actualizar(j, k, valorTabla + 1.0 / cantNoDominados);

	}

}

void BicriterionAnt :: actualizar_feromonas_bombas(Solucion &sol, int solSize, int cantNoDominados)
{

	double valorTabla;

	int j, k;

	for(int i = 0; i < solSize; i++)
	{	// actualizar ambas tablas en una cantidad 1/cantNoDominados

		j = sol.get(i);

		valorTabla = tabla1 -> obtenerValor(i, j);

		tabla1 -> actualizar(i, j, valorTabla + 1.0 / cantNoDominados);

		valorTabla = tabla2 -> obtenerValor(i, j);

		tabla2 -> actualizar(i, j, valorTabla + 1.0 / cantNoDominados);

	    	valorTabla = tabla3 -> obtenerValor(i, j);

		tabla3 -> actualizar(i, j, valorTabla + 1.0 / cantNoDominados);

		valorTabla = tabla4 -> obtenerValor(i, j);

		tabla4 -> actualizar(i, j, valorTabla + 1.0 / cantNoDominados);

	}

}


void BicriterionAnt :: evaporar_feromonas(void)
{

	for(int i = 0; i < prob -> getSize(); i++)
	{

		for(int j = 0; j < prob -> get_column_size(); j++)
		{

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) * (1 - rho));

			tabla2 -> actualizar(i, j, tabla2 -> obtenerValor(i, j) * (1 - rho));

		}

	}

}


void BicriterionAnt :: evaporar_feromonas_bombas(void)
{

	for(int i = 0; i < prob -> getSize(); i++)
	{

		for(int j = 0; j < prob -> get_column_size(); j++)
		{

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) * (1 - rho));

			tabla2 -> actualizar(i, j, tabla2 -> obtenerValor(i, j) * (1 - rho));

			tabla3 -> actualizar(i, j, tabla3 -> obtenerValor(i, j) * (1 - rho));

			tabla4 -> actualizar(i, j, tabla4 -> obtenerValor(i, j) * (1 - rho));

		}

	}

}

void BicriterionAnt :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

        int indice[hormigas];

        clock_t start, end;

        
        
      //  cout << "BICRITERION ANT" << endl;

        
        							//Solucion ** sols = (Solucion **)malloc(sizeof(Solucion *) * hormigas); 

        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas); 
		
                         					/*
								        Solucion ** sols = new Solucion * [hormigas];

							        for(int i = 0; i < hormigas; i++)
								        {

						               sols[i] = new Solucion(prob -> getSize() + 1);          

								        }
								*/	
							
								//        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

               sols[i].set_array(prob -> getSize() + 1);

        }

	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{
//cout << "i = " << i << endl; 
		        estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i; // utilizado en seleccionar_sgte_estado

			construir_solucionTSP(estOrigen, this, 0, sols[i]);

//cout << "construyo la solucion " << i << endl; 
                }

		evaporar_feromonas();

		for(int i = 0; i < hormigas; i++)
		{
  //     cout << " 662 i = " << i << endl;
			if (pareto -> esNoDominado(sols[i], prob))
			{
    //   cout << " 665 i = " << i << endl;

				pareto -> eliminarDominados(sols[i], prob);
      // cout << " 668 i = " << i << endl;
				pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);
				
                                indice[cantNoDominados++] = i;
       //cout << " 671 i = " << i << endl;

			}
			else
                        { 
         //                 cout << " 675 i = " << i << endl;
				sols[i].resetear();
                         }
//       cout << " 678 i = " << i << endl;

		}
//	cout << " 675 " << endl;	
		
		for(int i = 0; i < cantNoDominados; i++)
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados);

			sols[indice[i]].resetear();

		}
		
		end = clock();

	}//end while
						        //pareto->listarSoluciones(prob,"/home/fuentes/tsp.biant.pareto");
							/*
							for(int i = 0; i < hormigas; i++)
						        {

						              delete sols[i]; sols[i] = NULL;         

						        }
							*/

	free(sols); 

        								// delete[] sols; sols = NULL;

}



void BicriterionAnt :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

	int indice[hormigas];

	clock_t start, end;


	//  cout << "BICRITERION ANT" << endl;

        
        							//Solucion ** sols = (Solucion **)malloc(sizeof(Solucion *) * hormigas); 

        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas); 
		
                         					/*
								        Solucion ** sols = new Solucion * [hormigas];

							        for(int i = 0; i < hormigas; i++)
								        {

						               sols[i] = new Solucion(prob -> getSize() + 1);          

								        }
								*/	
							
								//        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

               sols[i].set_array(prob -> getSize());

        }

	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = rand()%(prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i; // utilizado en seleccionar_sgte_estado

			construir_solucionQAP(estOrigen, this, 0, sols[i]);

		}
		
		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);
      
				pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);
				
                                indice[cantNoDominados++] = i;

			}
			else

				sols[i].resetear();

		}
		
		for(int i = 0; i < cantNoDominados; i++)
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados);

			sols[indice[i]].resetear();

		}
		
		end = clock();

	}
	//pareto->listarSoluciones(prob,"/home/fuentes/qap.biant.pareto");

											/*
										        for(int i = 0; i < hormigas; i++)
										        {

										              delete sols[i];          

										        }
											*/
											//        delete[] sols; 
	free(sols); 
}



void BicriterionAnt :: ejecutarVRP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;
//cout << "BiAnt" << endl;	 
   	int indice[hormigas];

	clock_t start, end;

	
	//SolucionVRP * sols = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas); 

							        //SolucionVRP ** sols = (SolucionVRP **)malloc(sizeof(SolucionVRP *) * hormigas); 

								/*

								        SolucionVRP ** sols = new SolucionVRP * [hormigas]; 

								        for(int i = 0; i < hormigas; i++)
								        {

								               sols[i] = new SolucionVRP(prob -> getSize() * 2);          

								        }
								*/ 

        SolucionVRP sols [hormigas]; 

        for(int i = 0; i < hormigas; i++)
        {

               sols[i].set_array(prob -> getSize() * 2);          

        }
 
	start = clock();

	end = start;
//cout << "E1" << endl;	
	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{
//cout << "E2" << endl;	
		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i; // utilizado en seleccionar_sgte_estado
//cout << "E3" << endl;	
			construir_solucionVRP(estOrigen, this, 0, sols[i]);
//cout << "E4" << endl;	
		}
	//cout << "1" << endl;		
		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);
      
				pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				indice[cantNoDominados++] = i;

			}
			else

				sols[i].resetear();

		}
//cout << "E5" << endl;	

		for(int i = 0; i < cantNoDominados; i++)
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSizeActual(), cantNoDominados);

			sols[indice[i]].resetear();

		}
		
		end = clock();

	}
	//pareto->listarSoluciones(prob,"/home/fuentes/qap.biant.pareto");

											/*
										        for(int i = 0; i < hormigas; i++)
										        {

										              delete sols[i];          

										        }
											*/
//cout << "E6" << endl;											//        delete[] sols; 
	//free(sols); 
}

void BicriterionAnt :: ejecutarBOMBAS(void)
{

	int generacion = 0, estOrigen, cantNoDominados;

  	double deltaTao, taoPrima;

	int indice[hormigas];

   	clock_t start, end;


								//Solucion ** sols = (Solucion **)malloc(sizeof(Solucion *) * hormigas);
					
								/*	
								Solucion ** sols = new Solucion * [hormigas];

							        for (int i = 0; i < hormigas; i++)
							        {

									sols[i] = new Solucion(prob -> getSize());
  
							        }
								*/
	
        Solucion sols [hormigas];

        for (int i = 0; i < hormigas; i++)
        {

		sols[i].set_array(prob -> getSize());
  
        }

	start = clock();

	end = start;

										//	tao = -1;

//	while(!condicion_parada(generacion, start, end))
	{

		generacion++;

        cantNoDominados = 0;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion_2 == 0)
  			    
  			        lambda_funcion_2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion_3 == 0)
  			    
  			        lambda_funcion_3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion_4 == 0)
  			    
  			        lambda_funcion_4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i - 1]);

		}
		
		evaporar_feromonas_bombas();

		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

            	indice[cantNoDominados++] = i;

			}
			else

       			sols[i].resetear();

		}
		
		for(int i = 0; i < cantNoDominados; i++)
		{

			actualizar_feromonas_bombas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados);

			sols[indice[i]].resetear();

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");

												/*
											        for(int i = 0; i < hormigas; i++)
												{

											              delete sols[i];          

											        }
												*/
												//       delete[] sols; 

}






/**********************BICRITERION MC*************************************/
class BicriterionMC : public MOACO
{

	TablaFeromona * tabla1, * tabla2, * tabla3, * tabla4; //Matrices de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, F3MAX, F4MAX, NORM1, NORM2, NORM3, NORM4; // utilizados para normalizacion

	int lambda_funcion_1, lambda_funcion_2, lambda_funcion_3, lambda_funcion_4; // utilizado para calcular los pesos lambda
	
        double lambdaActual; // utilizado para calcular los pesos lambda actual

	void inicializar_tablas(void)
	{

       		if(prob -> get_column_size() == 0)// para tsp, qap, y vrp
        	{
    	     
	            tabla1 = new TablaFeromona(prob -> getSize());

		    tabla2 = new TablaFeromona(prob -> getSize());

       		}
       		else //para bombas
        	{
           
	            tabla1 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());

        	    tabla2 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());

	            tabla3 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());

	            tabla4 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());


        	    tabla3 -> reiniciar(taoInicial);

		    tabla4 -> reiniciar(taoInicial);

       		}

		tabla1 -> reiniciar(taoInicial);

		tabla2 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize, int cantNoDominados, TablaFeromona * tab);

	int seleccionar_probabilistico(int estOrigen, int visitados[]);
	
        int seleccionar_probabilistico(int intervalo, int bomba);

	void evaporar_feromonas(void);

	void evaporar_feromonas_bombas(void);

	int insertar(double * array, double costoActual,int cantNoDominados);

public:

	BicriterionMC(Problem * p,char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

	    	NORM3 = 1; //por defecto

		NORM4 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		F3MAX = 1; //por defecto

		F4MAX = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tablas();

		if(hormigas % 2 != 0) 
                    
                  hormigas++; // se utiliza un numero par de hormigas

	};

	~BicriterionMC()
        {

		delete tabla1; tabla1 = NULL;          

		delete tabla2; tabla2 = NULL;          

		delete tabla3; tabla3 = NULL;          

		delete tabla4; tabla4 = NULL;
              
                delete pareto; pareto = NULL;

        };
	
        int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba);
	
	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};



void BicriterionMC :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1)
          
           tiempoTotal = valor;

	else 
      
           maxIteraciones = valor;

	// leer parametros especificos del BicriterionMC
	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

    			fscanf(f, "%d", &hormigas);
		
        else if(stricmp(cad, "ALFA") == 0)
		
            	fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)
			
                fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "RHO") == 0)
			   
                fscanf(f,"%lf",&rho);

		else if(stricmp(cad, "TAU0") == 0)
		  	    
                fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

        else if(stricmp(cad, "F3MAX") == 0)

			fscanf(f, "%lf", &F3MAX);

		else if(stricmp(cad, "F4MAX") == 0)

			fscanf(f, "%lf", &F4MAX);
	
    	else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

    	else if(stricmp(cad, "D3MAX") == 0)

			fscanf(f, "%lf", &NORM3);

		else if(stricmp(cad, "D4MAX") == 0)

			fscanf(f, "%lf", &NORM4);

 		else
	  		    
                fscanf(f, "%s", cad);

	}	

}



int BicriterionMC :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{

	

        int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
        							       //int sgteEstado, * visitados = new int [prob -> getSize()];


	// marcar estados ya visitados, hallar el vecindario

	for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	

 	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);

       
        free(visitados);

 									       //delete[] visitados; 

									//	visitados = NULL;

	return sgteEstado;

}



int BicriterionMC :: seleccionar_probabilistico(int intervalo, int bomba)
{

return 0;

}

int BicriterionMC :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;

        

        double * productos = (double *)calloc(prob -> getSize(), sizeof(double));
	
       									 //double * productos = new double[prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;
	
	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1



	// hallar la suma y los productos
	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

			productos[i] = pow(tabla1 -> obtenerValor(estOrigen, i), lambdaActual * alfa) * pow(tabla2 -> obtenerValor(estOrigen, i), (1 - lambdaActual) * alfa) * pow(heuristica1, lambdaActual * beta) * pow(heuristica2, (1 - lambdaActual) * beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{
		// aplicar ruleta
		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

			}

		}

	}

	
	free(productos);


        										//delete[] productos; 

								//	productos = NULL;

	return sgteEstado;

}



int BicriterionMC :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}



int BicriterionMC :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
        //int sgteEstado, * visitados = new int[prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	// hallar el vecindario
	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) totalVisitados++; // estado 0 ya se contabilizo

	}

	for (int i = 0; i < problem -> getSize(); i++)
	{

		if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));

			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
			{

				visitados[i] = 1; // marcar como no vecino

				totalVisitados++;

			}

		}

	}
	
	if(totalVisitados >= problem -> getSize())

		sgteEstado = 0; // ir al deposito

	else

		sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		

	free(visitados);

								        //delete[] visitados;
	
	return sgteEstado;

}



int BicriterionMC :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bombas)
{ 

    int sgteEstado = seleccionar_probabilistico(intervalo, bombas);


	return sgteEstado;

}




void BicriterionMC :: actualizar_feromonas(Solucion &sol, int solSize, int cantNoDominados, TablaFeromona * tab)
{

	int j, k;

	for(int i = 0; i < solSize - 1; i++)
	{	

                // actualizar ambas tablas en una cantidad 1/cantNoDominados

		j = sol.get(i);

		k = sol.get(i + 1);

		tab -> actualizar(j, k, tab -> obtenerValor(j, k) + 1.0 / cantNoDominados);

	}

}


void BicriterionMC :: evaporar_feromonas(void)
{

	for(int i = 0; i < prob -> getSize(); i++)
	{

		for(int j = 0; j < prob -> getSize(); j++)
		{

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i,j) * (1 - rho));

			tabla2 -> actualizar(i, j, tabla2-> obtenerValor(i, j) * (1 - rho));

		}

	}

}

void BicriterionMC :: evaporar_feromonas_bombas(void)
{

	for(int i = 0; i < prob -> getSize(); i++)
	{

		for(int j = 0; j < prob -> get_column_size(); j++)
		{

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) * (1 - rho));

			tabla2 -> actualizar(i, j, tabla2 -> obtenerValor(i, j) * (1 - rho));

			tabla3 -> actualizar(i, j, tabla3 -> obtenerValor(i, j) * (1 - rho));

			tabla4 -> actualizar(i, j, tabla4 -> obtenerValor(i, j) * (1 - rho));

		}

	}

}



int BicriterionMC :: insertar(double * array, double costoActual,int cantNoDominados)
{

	for(int i = cantNoDominados - 1; i >= 0; i--)

		if(array[i] <= costoActual)

			return i + 1;
			
	return 0;

}





void BicriterionMC :: ejecutarTSP(int extension_tiempo)
{
	
	
	int generacion = 0, estOrigen, cantNoDominados;

	int indice[hormigas]; // array que contiene el indice en sols de una solucion actual no dominada

    	double f1[hormigas]; // array donde se ordenan las soluciones no dominadas de la generacion por la funcion obj. 1

	double incrementoLambda = 2.0 / 3.0 / (hormigas / 2 - 1); // para problemas bi-objetivos

	double lambda1, lambda2, costoActual;

	int pos;

    	clock_t start, end;


//        cout << "BICRITERION MC" << endl; 

	
                                                	//Solucion ** sols = (Solucion **)malloc(sizeof(Solucion *) * hormigas);
        
        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
	
							/*
						        Solucion ** sols = new Solucion * [hormigas];

						        for (int i = 0; i < hormigas; i++)
							        {

					                sols[i] = new Solucion(prob -> getSize() + 1);
  
						        }
							*/

							//	Solucion sols[hormigas];
	
        for (int i = 0; i < hormigas; i++)
        {

                sols[i].set_array(prob -> getSize() + 1);
  
        }

        start = clock();

	end = start;


	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		lambda1 = -incrementoLambda;

		lambda2 = 1.0 / 3.0 - incrementoLambda;

	
	    	for(int i = 0; i < hormigas / 2; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			lambda1 += incrementoLambda; // utilizado en seleccionar_sgte_estado

			lambdaActual = lambda1;

			construir_solucionTSP(estOrigen, this, 0, sols[i]);

		
        
        	estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			lambda2 += incrementoLambda; // utilizado en seleccionar_sgte_estado

			lambdaActual = lambda2;

		        construir_solucionTSP(estOrigen, this, 0, sols[i + hormigas / 2]);

		}
		

		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);
				
                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);
				
                                costoActual = prob -> funcion_obj_1(sols[i]);

				pos = insertar(f1, costoActual, cantNoDominados); // devuelve su lugar en orden creciente


				for(int j = cantNoDominados; j > pos; j--)
				{

					f1[j] =f1[j - 1];

					indice[j] = indice[j - 1];

				}

				f1[pos] = costoActual;

				indice[pos] = i;

				cantNoDominados++;

			}
			else

				sols[i].resetear();

		}
		
		for(int i = 0; i < cantNoDominados / 2; i++) // la primera region actualiza la tabla 1
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados, tabla1);

			sols[indice[i]].resetear();

		}

		for(int i = cantNoDominados / 2; i < cantNoDominados; i++) // la segunda region actualiza la tabla 2
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados, tabla2);

			sols[indice[i]].resetear();

		}
		
		end = clock();

	}//end while
	//pareto->listarSoluciones(prob,"/home/fuentes/tsp.bimc.pareto");

										/*	
									        for (int i = 0; i < hormigas; i++)
									        {

								               delete sols[i]; sols[i] = NULL;
  
									        }
										*/
        free(sols);         

									        // delete[] sols; sols = NULL;

}







void BicriterionMC :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

    	int indice[hormigas]; // array que contiene el indice en sols de una solucion actual no dominada

    	double f1[hormigas]; // array donde se ordenan las soluciones no dominadas de la generacion por la funcion obj. 1

	double incrementoLambda = 2.0 / 3.0 / (hormigas / 2 - 1); // para problemas bi-objetivos

	double lambda1, lambda2, costoActual;

	int pos;

    	clock_t start, end;

	
//        cout << "BICRITERION MC" << endl; 

	
                                                	//Solucion ** sols = (Solucion **)malloc(sizeof(Solucion *) * hormigas);
        
        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
	
							/*
						        Solucion ** sols = new Solucion * [hormigas];

						        for (int i = 0; i < hormigas; i++)
							        {

					                sols[i] = new Solucion(prob -> getSize() + 1);
  
						        }
							*/

							//	Solucion sols[hormigas];
	
        for (int i = 0; i < hormigas; i++)
        {

                sols[i].set_array(prob -> getSize());
  
        }

	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		lambda1 = -incrementoLambda;

		lambda2 = 1.0 / 3.0 - incrementoLambda;

		for(int i = 0; i < hormigas / 2; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			lambda1 += incrementoLambda; // utilizado en seleccionar_sgte_estado

			lambdaActual = lambda1;

			construir_solucionQAP(estOrigen, this, 0, sols[i]);

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			lambda2 += incrementoLambda; // utilizado en seleccionar_sgte_estado

			lambdaActual = lambda2;

			construir_solucionQAP(estOrigen, this, 0, sols[i + hormigas / 2]);

		}
		
		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);
				
                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);
				
                                costoActual = prob -> funcion_obj_1(sols[i]);

				pos = insertar(f1, costoActual, cantNoDominados); // devuelve su lugar en orden creciente

				for(int j = cantNoDominados; j > pos; j--)
				{

					f1[j] = f1[j - 1];

					indice[j] = indice[j - 1];

				}

				f1[pos] = costoActual;

				indice[pos] = i;

				cantNoDominados++;
			}
			else

				sols[i].resetear();

		}
		
		for(int i = 0; i < cantNoDominados / 2; i++) // la primera region actualiza la tabla 1
		{
		
        		actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados, tabla1);
		
        		sols[indice[i]].resetear();
		
        	}
		
        	for(int i = cantNoDominados / 2; i < cantNoDominados; i++) // la segunda region actualiza la tabla 2
		{
		
        		actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), cantNoDominados, tabla2);
		
	        	sols[indice[i]].resetear();
		
        	}
		
		end = clock();
	}
	
	//pareto->listarSoluciones(prob,"/home/fuentes/qap.bimc.pareto");
										/*	
									        for (int i = 0; i < hormigas; i++)
									        {

									               delete sols[i];
  
									        }
										*/
										//        delete[] sols;         
        free(sols);         
}





void BicriterionMC :: ejecutarVRP(int extension_tiempo)
{
//cout << "BiMC" << endl;
	int generacion = 0, estOrigen, cantNoDominados;

  	int indice[hormigas]; // array que contiene el indice en sols de una solucion actual no dominada

   	double f1[hormigas]; // array donde se ordenan las soluciones no dominadas de la generacion por la funcion obj. 1

	double incrementoLambda = 2.0 / 3.0 /(hormigas / 2 - 1); // para problemas bi-objetivos

	double lambda1, lambda2, costoActual;

	int pos;

   	clock_t start, end;

  
	//SolucionVRP * sols = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas);
								/*	
						        SolucionVRP ** sols = new SolucionVRP * [hormigas];

						        for (int i = 0; i < hormigas; i++)
						        {

						                sols[i] = new SolucionVRP(prob -> getSize() * 2);
  
							       }	
							*/			

        SolucionVRP sols [hormigas];

        for (int i = 0; i < hormigas; i++)
        {

                sols[i].set_array(prob -> getSize() * 2);
  
        }

	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		lambda1 = -incrementoLambda;

		lambda2 = 1.0 / 3.0 - incrementoLambda;

		for(int i = 0; i < hormigas / 2; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			lambda1 += incrementoLambda; // utilizado en seleccionar_sgte_estado

			lambdaActual = lambda1;

			construir_solucionVRP(estOrigen, this, 0, sols[i]);

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			lambda2 += incrementoLambda; // utilizado en seleccionar_sgte_estado

			lambdaActual = lambda2;

			construir_solucionVRP(estOrigen, this, 0, sols[i + hormigas / 2]);

		}
		
		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{
		
				pareto -> eliminarDominados(sols[i], prob);
				
                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				costoActual = prob -> funcion_obj_1(sols[i]);

				pos = insertar(f1, costoActual, cantNoDominados); // devuelve su lugar en orden creciente

				for(int j = cantNoDominados; j > pos; j--)
				{

					f1[j] = f1[j - 1];

					indice[j] = indice[j - 1];

				}

				f1[pos] = costoActual;

				indice[pos] = i;

				cantNoDominados++;

			}
			else

				sols[i].resetear();
		}
		
		for(int i = 0; i < cantNoDominados / 2; i++) // la primera region actualiza la tabla 1
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSizeActual(), cantNoDominados, tabla1);

			sols[indice[i]].resetear();
		}

		for(int i = cantNoDominados / 2; i < cantNoDominados; i++) // la segunda region actualiza la tabla 2
		{

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSizeActual(), cantNoDominados, tabla2);

			sols[indice[i]].resetear();

		}
		
		end = clock();

	}
									//pareto->listarSoluciones(prob,"/home/fuentes/qap.bimc.pareto");
												/*	
											        for (int i = 0; i < hormigas; i++)
											        {

											               delete sols[i];
  
											        }
												*/
												//        delete[] sols;         
        //free(sols);         
}

void BicriterionMC :: ejecutarBOMBAS(void)
{
int c = 1 + 1;
/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


	
	Solucion ** sols = (Solucion **)malloc(sizeof(Solucion *) * hormigas);

        for (int i = 0; i < hormigas; i++)
        {

                sols[i] = new Solucion(prob -> getSize());
  
        }

	start = clock();

	end = start;

	//tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion_2 == 0)
  			    
  			        lambda_funcion_2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion_3 == 0)
  			    
  			        lambda_funcion_3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion_4 == 0)
  			    
  			        lambda_funcion_4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i - 1]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(* (pareto -> getSolucion(i)));

				actualizar_feromonas(pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}



/**********************COMPET ANTS*************************************/
class CompeteAnts : public MOACO
{

	TablaFeromona * tabla1, * tabla2; //Matrices de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, NORM1, NORM2, AVR1, AVR2; // utilizados para normalizacion

	int lambda_funcion_1, lambda_funcion_2, lambda_funcion_3, lambda_funcion_4; // utilizado para calcular los pesos lambda
	
        int esEspia;	// indica si la hormiga actual es espia o no

	int coloniaActual; // indica la colonia actual, 1 o 2

	void inicializar_tablas(void)
	{

		tabla1 = new TablaFeromona(prob -> getSize());

		tabla2 = new TablaFeromona(prob -> getSize());

		tabla1 -> reiniciar(taoInicial);

		tabla2 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize, TablaFeromona * tab, double deltaTao);

	int seleccionar_probabilistico(int estOrigen, int visitados[]);
	
        int seleccionar_probabilistico(int intervalo, int bomba);

	void evaporar_feromonas(TablaFeromona * tab);

	double calcular_media(double array[], int size);

	int adaptar_numero_hormigas(double media1, double media2);

	int adaptar_numero_espias(double best, double bestPrima,int popSize, int colonia);

	int buscar_pos(double * array, int size, double costo);

public:

	CompeteAnts(Problem * p, char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		AVR1 = 1; //por defecto

		AVR2 = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tablas();

	};

	~CompeteAnts()
        {

             delete tabla1; tabla1 = NULL;

             delete tabla2; tabla2 = NULL;

             delete pareto; pareto = NULL;

        };
	
        int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba);
	
	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};


void CompeteAnts :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1) 
            
            tiempoTotal = valor;

	else 
            
	    maxIteraciones = valor;

	// leer parametros especificos del CompeteAnts

	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA")  == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else if(stricmp(cad, "AVR1") == 0)

			fscanf(f, "%lf", &AVR1);

		else if(stricmp(cad, "AVR2") == 0)

			fscanf(f, "%lf", &AVR2);

		else

			fscanf(f, "%s", cad);

	}	

}



int CompeteAnts :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{

  	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
        						//int sgteEstado, * visitados = new int[prob -> getSize()];



	// marcar estados ya visitados, hallar el vecindario

	for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	
	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);


	free(visitados); 

        					//delete[] visitados; visitados = NULL;

	return sgteEstado;

}




int CompeteAnts :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica;

       
        double * productos = (double *)calloc(prob -> getSize(), sizeof(double));
	
        						//double * productos = new double [prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	TablaFeromona * tab;

	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

	// hallar la suma y los productos

	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			if(coloniaActual == 1)
			{

				heuristica = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

				tab = tabla1;

			}
			else
			{

				heuristica = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

				tab = tabla2;
			}	

			if(!esEspia)

				productos[i] = pow(tab -> obtenerValor(estOrigen, i), alfa) * pow(heuristica, beta);

			else

				productos[i] = pow(0.5 * tabla1 -> obtenerValor(estOrigen,i) + 0.5 * tabla2 -> obtenerValor(estOrigen,i), alfa) * pow(heuristica, beta);
			

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{

		// aplicar ruleta
		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

			}

		}

	}

       free(productos); 

								//delete[] productos; productos = NULL;

	return sgteEstado;

}


int CompeteAnts :: seleccionar_probabilistico(int intervalo, int bomba)
{

   return 0;

}



int CompeteAnts :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}




int CompeteAnts :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
      							  //int sgteEstado, * visitados = new int[prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	// hallar el vecindario

	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) 
                 
                 totalVisitados++; // estado 0 ya se contabilizo
	
    }
	
    for (int i = 0; i < problem -> getSize(); i++)
    {
	
    	if(!visitados[i])
	{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
		distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));
	
    		if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
		{
	
    			visitados[i] = 1; // marcar como no vecino
	
    			totalVisitados++;
	
    		}
	
    	}
	
    }
	
	
    if(totalVisitados >= problem -> getSize())
	
    	sgteEstado = 0; // ir al deposito
	
    else
	
    	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		
    
    free(visitados); 

				  //  delete[] visitados;
	
    return sgteEstado;

}

int CompeteAnts :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba)
{

   return 0;

}

void CompeteAnts :: actualizar_feromonas(Solucion &sol, int solSize, TablaFeromona * tab, double deltaTao)
{

	int j, k;

	for(int i = 0; i < solSize - 1; i++)
	{	// actualizar ambas tablas en una cantidad 1/cantNoDominados

		j = sol.get(i);

		k = sol.get( i + 1);
 
		tab -> actualizar(j, k, tab -> obtenerValor(j, k) + deltaTao);

	}

}




void CompeteAnts :: evaporar_feromonas(TablaFeromona * tab)
{

	for(int i = 0; i < prob -> getSize(); i++)

		for(int j = 0; j < prob -> getSize(); j++)

			tab -> actualizar(i, j, tab -> obtenerValor(i, j) * (1 - rho));

}





double CompeteAnts :: calcular_media(double array[], int size)
{

	double media = 0.0;
 
	for(int i = 0; i < size; i++)
 
		media += array[i];
		
	return media / size;

}





int CompeteAnts :: adaptar_numero_hormigas(double media1, double media2)
{

	double random, prob;

	int hormigas1 = 0;

	prob = media2 / AVR2 / (media1/AVR1 + media2 / AVR2); // normalizado

	for(int i = 0; i < hormigas; i++)
	{

		random = (double)(rand() % 10000) / 10000.0;

		if(random < prob)

			hormigas1++;

	}
	
	return hormigas1;

}





int CompeteAnts :: adaptar_numero_espias(double best, double bestPrima, int popSize, int colonia)
{

	double random, prob;

	int espias = 0;
	

	if(colonia == 1)

		prob = best / F1MAX / (best / F1MAX + 4.0 * bestPrima / F2MAX); // normalizado

	else

		prob = best/ F2MAX / (best / F2MAX + 4.0 * bestPrima / F1MAX); // normalizado

	for(int i = 0; i < popSize; i++)
	{

		random = (double)(rand() % 10000) / 10000.0;

		if(random < prob)

			espias++;

	}
	
	return espias;

}



int CompeteAnts :: buscar_pos(double * array, int size, double costo)
{

	for(int i = size; i >= 0; i--)
	{

		if(array[i] <= costo)
		
			return (i + 1);
	
    }
	
    return 0;

}



void CompeteAnts :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0, estOrigen, hormigas1, hormigas2;

        int espias1, espias2, auxEspias, pos;

        double cantBest1, cantBest2, evalAux;

        clock_t start, end;

//cout << "COMPETE ANTS" << endl;        
  	Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
							/*
						        Solucion ** sols = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
						        {

						             sols[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/	

	Solucion * sols2 = (Solucion *)malloc(sizeof(Solucion) * hormigas);

							/*
						        Solucion ** sols2 = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
							       {

					             sols2[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/	

							//	Solucion * solAux = new Solucion(prob -> getSize() + 1);

							//        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() + 1);

        } 

						        //Solucion sols2 [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols2[i].set_array(prob -> getSize() + 1);

        } 

        Solucion solAux(prob -> getSize() + 1);       	


	double evaluaciones1[hormigas], evaluaciones2[hormigas];

	start = clock();

	end = start;

	hormigas1 = hormigas / 2;

	hormigas2 = hormigas - hormigas1;

	espias1 = 0;

	espias2 = 0;
	
	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantBest1 = hormigas1 * 0.0625; //valor tomado de los autores originales del algoritmo

		cantBest2 = hormigas2 * 0.0625; //valor tomado de los autores originales del algoritmo

		auxEspias = espias1;

		esEspia = 1;

		coloniaActual = 1;

		for(int i = 0; i < hormigas1; i++)
		{

			if(auxEspias <= 0)

				esEspia = 0;

			else

				auxEspias--;

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionTSP(estOrigen, this, 0, solAux);

			evalAux = prob -> funcion_obj_1(solAux);

			// insertar en sols ordenado
			pos = buscar_pos(evaluaciones1, i - 1, evalAux);

			for(int k = i; k > pos; k--)
			{

				sols[k].solcpy(sols[k - 1]);

				evaluaciones1[k] = evaluaciones1[k - 1];

			}

			sols[pos].solcpy(solAux);

			evaluaciones1[pos] = evalAux;

			solAux.resetear();

		}

		evaporar_feromonas(tabla1);

		for(int i = 0; i < cantBest1; i++)

			actualizar_feromonas(sols[i], sols[i].getSize(), tabla1, 1.0 - (double)i / (double)cantBest1);
		

		auxEspias = espias2;

		esEspia = 1;

		coloniaActual = 2;

		for(int i = 0; i < hormigas2; i++)
		{

			if(auxEspias <= 0)

				esEspia = 0;

			else

				auxEspias--;

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionTSP(estOrigen, this, 0, solAux);

			evalAux = prob -> funcion_obj_2(solAux);

			// insertar en sols ordenado

			pos = buscar_pos(evaluaciones2, i - 1, evalAux);

			for(int k = i; k > pos; k--)
			{

				sols2[k].solcpy(sols2[k - 1]);

				evaluaciones2[k] = evaluaciones2[k - 1];
			}

			sols2[pos].solcpy(solAux);

			evaluaciones2[pos] = evalAux;

			solAux.resetear();
		
                }

		evaporar_feromonas(tabla2);

		for(int i = 0; i < cantBest2; i++)

			actualizar_feromonas(sols2[i], sols2[i].getSize(), tabla2, 1.0 - (double)i / (double)cantBest2);
		
		// actualizar frente pareto con soluciones de la primera colonia
		for(int i =  0; i < hormigas1; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

			}
			//else
				//sols[i].destruir();
		}

		// actualizar frente pareto con soluciones de la segunda colonia
		for(int i = 0; i < hormigas2; i++)
		{

			if (pareto -> esNoDominado(sols2[i], prob))
			{

				pareto -> eliminarDominados(sols2[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols2[i]);

			}
			//else
				//sols2[i].destruir();

		}
		
		// adaptar numero de hormigas y homigas espias
		hormigas1 = adaptar_numero_hormigas(calcular_media(evaluaciones1, hormigas1), calcular_media(evaluaciones2, hormigas2));

		hormigas2 = hormigas - hormigas1;

		espias1 = adaptar_numero_espias(evaluaciones1[0], evaluaciones2[0], hormigas1, 1);

		espias2 = adaptar_numero_espias(evaluaciones2[0], evaluaciones1[0], hormigas2, 2);
		

		end = clock();

	}
	//pareto->listarSoluciones(prob,"/home/fuentes/tsp.comp.pareto");


        free(sols); 

        free(sols2);

											/*        
										        for(int i = 0; i < hormigas; i++)
										        {

										             delete sols[i]; sols[i] = NULL;

										        } 
											*/	
											//      delete[] sols; sols = NULL;
											/*
										        for(int i = 0; i < hormigas; i++)
										        {

										             delete sols2[i]; sols2[i] = NULL;

										        } 
											*/         
											//      delete[] sols2;	sols2 = NULL;
	
											//      delete solAux; solAux = NULL;

}



void CompeteAnts :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen, hormigas1, hormigas2;

        int espias1, espias2, auxEspias, pos;

        double cantBest1, cantBest2, evalAux;

        clock_t start, end;


//cout << "COMPETE ANTS" << endl;        

  	Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
							/*
						        Solucion ** sols = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
						        {

						             sols[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/	

	Solucion * sols2 = (Solucion *)malloc(sizeof(Solucion) * hormigas);

							/*
						        Solucion ** sols2 = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
							       {

					             sols2[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/	

							//	Solucion * solAux = new Solucion(prob -> getSize() + 1);

							//        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize());

        } 

						        //Solucion sols2 [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols2[i].set_array(prob -> getSize());

        } 

        Solucion solAux(prob -> getSize());       	




	double evaluaciones1[hormigas], evaluaciones2[hormigas];

	start = clock();

	end = start;

	hormigas1 = hormigas / 2;

	hormigas2 = hormigas - hormigas1;

	espias1 = 0;

	espias2 = 0;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantBest1 = hormigas1 * 0.0625; //valor tomado de los autores originales del algoritmo

		cantBest2 = hormigas2 * 0.0625; //valor tomado de los autores originales del algoritmo

		auxEspias = espias1;

		esEspia = 1;

		for(int i = 0; i < hormigas1; i++)
		{

			if(auxEspias <= 0)

				esEspia = 0;

			else

				auxEspias--;

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionQAP(estOrigen, this, 0, solAux);

			evalAux = prob -> funcion_obj_1(solAux);

			// insertar en sols ordenado
			pos = buscar_pos(evaluaciones1, i - 1, evalAux);

			for(int k = i; k > pos; k--)
			{

				sols[k].solcpy(sols[k - 1]);
					//borre por error de compilacion(esta bien)
				//evaluaciones1[k] = evaluaciones1[k - 1];
			}

			sols[pos].solcpy(solAux);

			evaluaciones1[pos] = evalAux;

			solAux.resetear();

		}

		evaporar_feromonas(tabla1);

		for(int i = 0; i < cantBest1; i++)

			actualizar_feromonas(sols[i], sols[i].getSize(), tabla1, 1.0 - (double)i / (double)cantBest1);
		

		auxEspias = espias2;

		esEspia = 1;

		for(int i = 0; i < hormigas2; i++)
		{

			if(auxEspias <= 0)

				esEspia = 0;

			else

				auxEspias--;

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionQAP(estOrigen, this, 0, solAux);

			evalAux = prob -> funcion_obj_2(solAux);

			// insertar en sols ordenado

			pos = buscar_pos(evaluaciones2, i - 1, evalAux);

			for(int k = i; k > pos; k--)
			{

				sols2[k].solcpy(sols2[k - 1]);

				evaluaciones2[k] = evaluaciones2[k - 1];

			}

			sols2[pos].solcpy(solAux);

			evaluaciones2[pos] = evalAux;

			solAux.resetear();

		}

		evaporar_feromonas(tabla2);

		for(int i = 0; i < cantBest2; i++)

			actualizar_feromonas(sols2[i], sols2[i].getSize(), tabla2, 1.0 - (double)i / (double)cantBest2);
		
		// actualizar frente pareto con soluciones de la primera colonia
		for(int i = 0; i < hormigas1; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);


			}
			//else
				//sols[i].destruir();

		}

		// actualizar frente pareto con soluciones de la segunda colonia
		for(int i = 0; i < hormigas2; i++)
		{

			if (pareto -> esNoDominado(sols2[i], prob))
			{

				pareto -> eliminarDominados(sols2[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols2[i]);

			}
			//else
				//sols2[i].destruir();
		}
		
		// adaptar numero de hormigas y homigas espias
		hormigas1 = adaptar_numero_hormigas(calcular_media(evaluaciones1, hormigas1), calcular_media(evaluaciones2, hormigas2));

		hormigas2 = hormigas - hormigas1;

		espias1 = adaptar_numero_espias(evaluaciones1[0], evaluaciones2[0], hormigas1, 1);

		espias2 = adaptar_numero_espias(evaluaciones2[0], evaluaciones1[0], hormigas2, 2);
		

		end = clock();

	}

//	pareto->listarSoluciones(prob,"/home/fuentes/qap.comp.pareto");
									/*        
								        for(int i = 0; i < hormigas; i++)
								        {

								             delete sols[i];

								        } 
									*/	
									//        delete[] sols;
									/*
									for(int i = 0; i < hormigas; i++)
								        {

								             delete sols2[i];

								        } 
									*/         
									//        delete[] sols2;	
	
									//        delete solAux;
 	free(sols); 

        free(sols2);

}




void CompeteAnts :: ejecutarVRP(int extension_tiempo)
{
//cout << "CompAnts" << endl;
	int generacion = 0, estOrigen, hormigas1, hormigas2;

   	int espias1, espias2, auxEspias, pos;

   	double cantBest1, cantBest2, evalAux;

  	clock_t start, end;


        SolucionVRP * sols = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas);
							/*
						        Solucion ** sols = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
						        {

						             sols[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/	

	SolucionVRP * sols2 = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas);

       // SolucionVRP sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() * 2);

        } 
	
							      //SolucionVRP ** sols2 = (SolucionVRP **)malloc(sizeof(SolucionVRP * ) * hormigas);
        //SolucionVRP sols2 [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols2[i].set_array(prob -> getSize() * 2);

        } 
	
	SolucionVRP solAux(prob -> getSize() * 2);



	double evaluaciones1[hormigas], evaluaciones2[hormigas];

	start = clock();

	end = start;

	hormigas1 = hormigas / 2;

	hormigas2 = hormigas - hormigas1;

	espias1 = 0;

	espias2 = 0;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantBest1 = hormigas1 * 0.0625; //valor tomado de los autores originales del algoritmo

		cantBest2 = hormigas2 * 0.0625; //valor tomado de los autores originales del algoritmo

		auxEspias = espias1;

		esEspia = 1;

		for(int i = 0; i < hormigas1; i++)
		{

			if(auxEspias <= 0)

				esEspia = 0;

			else

				auxEspias--;

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionVRP(estOrigen, this, 0, solAux);

			evalAux = prob -> funcion_obj_1(solAux);

			// insertar en sols ordenado

			pos = buscar_pos(evaluaciones1, i - 1, evalAux);

			for(int k = i; k > pos; k--)
			{

				sols[k].solcpy(sols[k - 1]);

				evaluaciones1[k] = evaluaciones1[k - 1];

			}

			sols[pos].solcpy(solAux);

			evaluaciones1[pos] = evalAux;

			solAux.resetear();

		}

		evaporar_feromonas(tabla1);

		for(int i = 0; i < cantBest1; i++)

			actualizar_feromonas(sols[i], sols[i].getSizeActual(), tabla1, 1.0 - (double)i / (double)cantBest1);
		

		auxEspias = espias2;

		esEspia = 1;

		for(int i = 0; i < hormigas2; i++)
		{

			if(auxEspias <= 0)

				esEspia = 0;

			else

				auxEspias--;

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionVRP(estOrigen, this, 0, solAux);

			evalAux = prob -> funcion_obj_2(solAux);

			// insertar en sols ordenado
			pos = buscar_pos(evaluaciones2, i - 1, evalAux);

			for(int k = i; k > pos; k--)
			{

				sols2[k].solcpy(sols2[k - 1]);
				
				evaluaciones2[k] = evaluaciones2[k - 1];

			}

			sols2[pos].solcpy(solAux);

			evaluaciones2[pos] = evalAux;

			solAux.resetear();

		}

		evaporar_feromonas(tabla2);

		for(int i = 0; i < cantBest2; i++)

			actualizar_feromonas(sols2[i], sols2[i].getSizeActual(), tabla2, 1.0 - (double)i / (double)cantBest2);
		
		// actualizar frente pareto con soluciones de la primera colonia
		for(int i = 0; i < hormigas1; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);


			}
			//else
				//sols[i].destruir();
		}

		// actualizar frente pareto con soluciones de la segunda colonia
		for(int i = 0; i < hormigas2; i++)
		{

			if (pareto -> esNoDominado(sols2[i], prob))
			{

				pareto -> eliminarDominados(sols2[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols2[i]);


			}
			//else
				//sols2[i].destruir();
		}
		
		// adaptar numero de hormigas y homigas espias
		hormigas1 = adaptar_numero_hormigas(calcular_media(evaluaciones1, hormigas1), calcular_media(evaluaciones2, hormigas2));

		hormigas2 = hormigas - hormigas1;

		espias1 = adaptar_numero_espias(evaluaciones1[0], evaluaciones2[0], hormigas1, 1);

		espias2 = adaptar_numero_espias(evaluaciones2[0], evaluaciones1[0], hormigas2, 2);
		

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.comp.pareto");
											/*        
											for(int i = 0; i < hormigas; i++)
										        {

										             delete sols[i];

										        } 
											*/
											//        delete[] sols;
											/*
											   for(int i = 0; i < hormigas; i++)
										        {

										             delete sols2[i];

										        } 
         										*/
											//        delete[] sols2;	
	
											//        delete solAux;

        free(sols); 

        free(sols2);
}

void CompeteAnts :: ejecutarBOMBAS(void)
{

int c = 1 + 1;

/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


        
        Solucion ** sols = (Solucion **)malloc(sizeof(Solucion * ) * hormigas);

        for(int i = 0; i < hormigas; i++)
        {

             sols[i] = new Solucion(prob -> getSize());

        } 
	
	start = clock();

	end = start;

	//tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion_2 == 0)
  			    
  			        lambda_funcion_2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion_3 == 0)
  			    
  			        lambda_funcion_3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion_4 == 0)
  			    
  			        lambda_funcion_4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(pareto -> getSolucion(i));

				actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i).getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}


/**********************MAS*************************************/

class MAS : public MOACO
{

	TablaFeromona * tabla1; //Matrices de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, NORM1, NORM2; // utilizados para normalizacion

	int hormigaActual; // utilizado para calcular los pesos lambda

	int noLambdas;

	void inicializar_tabla(void)
	{

		tabla1 = new TablaFeromona(prob -> getSize());

		tabla1 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize, double deltaTao);

	int seleccionar_probabilistico(int estOrigen, int visitados[]);
	
        int seleccionar_probabilistico(int intervalo, int bomba);

	void evaporar_feromonas(void);

public:

	MAS(Problem * p, char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tabla();

		noLambdas = 0;

	};

	~MAS()
        {

            delete tabla1; tabla1 = NULL;

            delete pareto; pareto = NULL;
       
        };

	int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime,double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba);
        
        double calcular_delta_tao(Solucion &sol);
	
	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};



void MAS :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1) 
             
             tiempoTotal = valor;

	else 
             
             maxIteraciones = valor;

	// leer parametros especificos del MAS

	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad,"BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else

			fscanf(f, "%s", cad);

	}	

}




int MAS :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{


	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
        						//int sgteEstado, * visitados = new int [prob -> getSize()];


        // marcar estados ya visitados, hallar el vecindario

	for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	

	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);


        free(visitados); 

                                                                //visitados = NULL;

                                  //delete[] visitados; visitados = NULL;

	return sgteEstado;

}



int MAS :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;


     
	double * productos = (double *)calloc(prob -> getSize(), sizeof(double));
	
        							//double * productos = new double [prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	double lambda1, lambda2;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	if(noLambdas)

		lambda1 = lambda2 = 1;

	else
	{

		lambda1 = hormigaActual;

		lambda2 = hormigas - hormigaActual + 1;

	}

	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

	// hallar la suma y los productos
	
        for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado
		
            		productos[i] = pow(tabla1 -> obtenerValor(estOrigen, i), alfa) * pow(heuristica1, lambda1 * beta) * pow(heuristica2, lambda2 * beta);                                         
	
    			suma += productos[i];
	
    			sinPorcion[cantSinPorcion++] = i;
	
    	}
	
    }
	
    if(suma == 0)
    {
	
    	random = rand() % cantSinPorcion;
	
    	sgteEstado = sinPorcion[(int)random];
	
    }
    else
    {
	
    	// aplicar ruleta
	for (int i = 0; i < prob -> getSize(); i++)
	{
	
    		if (!visitados[i]) // estado i no visitado
		{
	
    			acum += productos[i] / suma;
	
			if(acum >= random)
			{
	
    				sgteEstado = i;
	
    				break;
	
	    		}
	
    		}
	
	}
	
    }
	
    
    free(productos); 

 						   //productos = NULL;

										    //delete[] productos; productos = NULL;
	
    return sgteEstado;

}


int MAS :: seleccionar_probabilistico(int intervalo, int bomba)
{

        return 0;	


}

int MAS :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}





int MAS :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
     							   //int sgteEstado, * visitados = new int [prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;


	// hallar el vecindario
	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) 
                  
                  	totalVisitados++; // estado 0 ya se contabilizo
	
    	}
	
    	for (int i = 0; i < problem -> getSize(); i++)
	{
	
    		if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));
	
    			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia+problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
			{
	
    			visitados[i] = 1; // marcar como no vecino
	
    			totalVisitados++;
	
    		}
	
    	}
	
    }
	
	
    if(totalVisitados >= problem -> getSize())
	
    	sgteEstado = 0; // ir al deposito
	
    else
	
    	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		
  
    free(visitados); 

    									//delete[] visitados;
	
    return sgteEstado;

}


int MAS :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba)
{

    return 0;

}


void MAS :: actualizar_feromonas(Solucion &sol, int solSize, double deltaTau)
{

	int j, k;

	for(int i = 0; i < solSize - 1; i++)
	{	// actualizar ambas tablas en una cantidad 1/cantNoDominados

		j = sol.get(i);

		k = sol.get(i+1);

		tabla1 -> actualizar(j, k, tabla1 -> obtenerValor(j, k) + deltaTau);

	}

}




void MAS :: evaporar_feromonas(void)
{

	for(int i = 0; i < prob -> getSize(); i++)

		for(int j = 0; j < prob -> getSize(); j++)

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) * (1 - rho));

}






double MAS :: calcular_delta_tao(Solucion &sol)
{

	double delta;
	

	delta = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX); //normalizados

	return delta;

}





void MAS :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

    	int indice[hormigas], cambio, noCambio = 0;

    	double deltaTao;

    	clock_t start, end;

        
//cout << "MAS" << endl;        

        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
	
							/*        
						        Solucion ** sols = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
						        {

						             sols[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/

							//        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() + 1);

        } 
	
        start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i =0 ; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado

			construir_solucionTSP(estOrigen, this, 0, sols[i]);

		}
		

		evaporar_feromonas();

		cambio = 0;

		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				indice[cantNoDominados++] = i;

				cambio = 1;

			}
			else

				sols[i].resetear();

		}

		// reiniciar la tabla si es necesario

		if(!cambio)

			noCambio++;

		else

			noCambio = 0;


		if(noCambio >= 500) // no hubo cambios en el frente Pareto por 500 generaciones
		{

			tabla1 -> reiniciar(taoInicial);

			noCambio=0;

		}

		// actualizan la tabla los no dominados de la iteracion

		for(int i = 0; i < cantNoDominados; i++)
		{

			deltaTao = calcular_delta_tao(sols[indice[i]]);

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), deltaTao);

			sols[indice[i]].resetear();

		}

		end = clock();

	}
								//	pareto->listarSoluciones(prob,"/home/fuentes/tsp.mas.pareto");

        free(sols); 
											/*        
										        for(int i = 0; i < hormigas; i++)
											        {

									             delete sols[i]; sols[i] = NULL;

										        } 
											*/      
											//        delete[] sols; sols = NULL;

}






void MAS :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

    	int indice[hormigas], cambio, noCambio = 0;

    	double deltaTao;

    	clock_t start, end;	

        
//cout << "MAS" << endl;        

        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
	
							/*        
						        Solucion ** sols = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
						        {

						             sols[i] = new Solucion(prob -> getSize() + 1);

						        } 
							*/

							//        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize());

        } 
	
    	start = clock();
	
    	end = start;
	
    	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{
	
    		generacion++;
	
    		cantNoDominados = 0;
	
    		for(int i = 0; i < hormigas; i++)
		{
	
    			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio
	
    			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado
	
    			construir_solucionQAP(estOrigen, this, 0, sols[i]);
	
    		}
		
	        evaporar_feromonas();
	
    		cambio = 0;
	
    		for(int i = 0; i < hormigas; i++)
		{
	
    			if (pareto -> esNoDominado(sols[i], prob))
			{
	
    				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				indice[cantNoDominados++] = i;

				cambio = 1;
	
    			}
			else
	
    				sols[i].resetear();
	
    	 	}
	
    		// reiniciar la tabla si es necesario
		if(!cambio)
	
	    		noCambio++;
	
	    	else
	
    			noCambio = 0;

	
    		if(noCambio >= 500) // no hubo cambios en el frente Pareto por 500 generaciones
		{
	
    			tabla1 -> reiniciar(taoInicial);
	
    			noCambio = 0;
	
    		}
	
    		// actualizan la tabla los no dominados de la iteracion
		for(int i = 0; i < cantNoDominados; i++)
		{
	
    			deltaTao = calcular_delta_tao(sols[indice[i]]);
	
    			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSize(), deltaTao);
	
    			sols[indice[i]].resetear();
	
    		}
	
    		end = clock();
	
    	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.mas.pareto");
										/*        
									        for(int i = 0; i < hormigas; i++)
									        {

										             delete sols[i];

									        } 
										*/      
										//        delete[] sols;	

        free(sols); 
}




void MAS :: ejecutarVRP(int extension_tiempo)
{
//cout << "MAS" << endl;
	int generacion = 0, estOrigen, cantNoDominados;

   	int indice[hormigas], cambio, noCambio = 0;

   	double deltaTao;

   	clock_t start, end;

        
	//SolucionVRP * sols = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas);
							      /*
							        SolucionVRP ** sols = new SolucionVRP * [hormigas];

							        for(int i = 0; i < hormigas; i++)
								{

							             sols[i] = new SolucionVRP(prob -> getSize() * 2);

								 } 
								*/
       
        SolucionVRP sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() * 2);

        } 
	
	start = clock();

	end = start;

	noLambdas = 1;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado

			construir_solucionVRP(estOrigen, this, 0, sols[i]);

		}
		
		evaporar_feromonas();

		cambio = 0;

		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				indice[cantNoDominados++] = i;

				cambio = 1;
			}
			else

				sols[i].resetear();
		}

		// reiniciar la tabla si es necesario
		if(!cambio)

			noCambio++;

		else

			noCambio = 0;


		if(noCambio >= 500) // no hubo cambios en el frente Pareto por 500 generaciones
		{

			tabla1 -> reiniciar(taoInicial);

			noCambio = 0;

		}


		// actualizan la tabla los no dominados de la iteracion
		for(int i = 0; i < cantNoDominados; i++)
		{

			deltaTao = calcular_delta_tao(sols[indice[i]]);

			actualizar_feromonas(sols[indice[i]], sols[indice[i]].getSizeActual(), deltaTao);

			sols[indice[i]].resetear();

		}

		end = clock();

	}
								//	pareto->listarSoluciones(prob,"/home/fuentes/qap.mas.pareto");
										/*        
										        for(int i = 0; i < hormigas; i++)
									        {

									             delete sols[i];

									        } 
										*/      
										//        delete[] sols;	
        //free(sols); 
}


void MAS :: ejecutarBOMBAS(void)
{
/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


        
        Solucion ** sols = (Solucion **)malloc(sizeof(Solucion * ) * hormigas);

        for(int i = 0; i < hormigas; i++)
        {

             sols[i] = new Solucion(prob -> getSize());

        } 
	
	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion2 == 0)
  			    
  			        lambda_funcion2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion3 == 0)
  			    
  			        lambda_funcion3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion4 == 0)
  			    
  			        lambda_funcion4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(pareto -> getSolucion(i));

				actualizar_feromonas(pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}





/**********************MOA*************************************/

class MOA : public MOACO
{

	TablaFeromona * tabla1; //Matrices de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double omicron;	// Limite superior impuesto a las feromonas

	double k;		// cantidad de iteraciones entre actualizaciones de la tabla

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, NORM1, NORM2; // utilizados para normalizacion

	int hormigaActual;

	int noLambdas;

	void inicializar_tablas(void)
	{

		tabla1 = new TablaFeromona(prob -> getSize());

		tabla1 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize, int cantNoDominados);

	int seleccionar_probabilistico(int estOrigen, int visitados[]);
	
	int seleccionar_probabilistico(int intervalo, int bomba);

        int seleccionar_probabilisticoBOMBAS(int intervalo, int bomba);

public:

	MOA(Problem * p, char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		noLambdas = 0;

		inicializar_parametros(file);

		inicializar_tablas();

	};

 	~MOA()
        {

             delete tabla1; tabla1 = NULL;

             delete pareto; pareto = NULL;

        }

	int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo , int bomba);

        void construir_TSP(int estOrig, MOACO * aco, int onlineUpdate, Solucion &sol);

	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};





void MOA :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1)

		 tiempoTotal=valor;

	else 

		 maxIteraciones = valor;

	// leer parametros especificos del MOA
	
        while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "OMICRON") == 0)

			fscanf(f, "%lf", &omicron);

		else if(stricmp(cad, "K") == 0)

			fscanf(f, "%lf", &k);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else

			fscanf(f, "%s",cad);

	}	

}





int MOA :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{

//cout << "1t ";

        int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
							// int sgteEstado, * visitados = new int [prob -> getSize()];

       
							       // int sgteEstado;

							       // int visitados[prob -> getSize()];


//cout << "2t ";
	// marcar estados ya visitados, hallar el vecindario

	for (int i = 0; sol.get(i) != -1; i++)
        {
               
		visitados[sol.get(i)] = 1;
	
        }

//cout << "3t ";


	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);

//cout << "4t ";

        free(visitados); 

								//  delete[] visitados; visitados = NULL;

//cout << "5t " << endl;

	return sgteEstado;

}



int MOA :: seleccionar_probabilistico(int intervalo, int bomba)
{

    return 0;

}

int MOA :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;

//cout << "1p ";
        double * productos = (double *)calloc(prob -> getSize(), sizeof(double));

			        // double * productos = new double [prob -> getSize()];

				// double productos[prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	double lambda1, lambda2;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	if(noLambdas)

		lambda1 = lambda2 = 1;

	else
	{

		lambda1 = hormigaActual; // peso de la hormiga actual para el objetivo 1

		lambda2 = hormigas - lambda1 + 1; // peso para el objetivo 2

	}


	// hallar la suma y los productos
	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{


			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

			productos[i] = pow(tabla1 -> obtenerValor(estOrigen, i), alfa) * pow(heuristica1, beta * lambda1) *  pow(heuristica2, beta * lambda2);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

		}

	}

        if(suma == 0)
	{

//cout << "6" << endl;
		random = rand() % cantSinPorcion;
//cout << "7" << endl;

		sgteEstado = sinPorcion[(int)random];

//cout << "8" << endl;
	}
	else
	{

//cout << " 9 ";
		// aplicar ruleta
		random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

//cout << "( 10 )" << endl;
				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

			}

		}

	}

        free(productos);  

//cout << "2p ";


					// delete[] productos; productos = NULL;

	return sgteEstado;

}




int MOA :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}



int MOA :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
    							    //int sgteEstado, * visitados = new int [prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	// hallar el vecindario

	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0)

                        totalVisitados++; // estado 0 ya se contabilizo

	}

	for (int i = 0; i < problem -> getSize(); i++)
	{

		if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));

			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
			{

				visitados[i] = 1; // marcar como no vecino

				totalVisitados++;

			}

		}

	}
	
	if(totalVisitados >= problem -> getSize())

		sgteEstado = 0; // ir al deposito

	else

		sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		
      
        free(visitados); 

     											  //delete[] visitados;
	
	return sgteEstado;

}

int MOA :: seleccionar_siguiente_estadoBOMBAS(int intervalo , int bomba)
{

    return 0;

}



void MOA :: actualizar_feromonas(Solucion &sol, int solSize, int m)
{

	double valorTabla;

	double deltatau = omicron / m;

	int j, k;

	for(int i = 0; i < solSize - 1; i++)
	{	// actualizar cada arco utilizado con el valor Omicron/|pareto|

		j = sol.get(i);

		k = sol.get(i + 1);

		valorTabla = tabla1 -> obtenerValor(j, k);

		tabla1 -> actualizar(j, k, valorTabla + deltatau);

	}

}


void MOA :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0;

	int m;

	int tInicio, tFin, estOrig;

				
//cout << "MOA" << endl;        

									// Solucion * sol = new Solucion(prob -> getSize() + 1);
								/*

							        Solucion * sol = (Solucion *)malloc(sizeof(Solucion) * hormigas);
	

							        for(int i = 0; i < hormigas; i++)
							        {

								             sol[i].set_array(prob -> getSize() + 1);

							        } 
								*/

        Solucion sol(prob -> getSize() + 1);
	
//cout << "MOA////////1" << endl;

	noLambdas=1;
	
        for (int i = 0; i < hormigas; i++)
	{

//cout << "MOA before " << i << endl;
		estOrig = rand() % (prob -> getSize());

// cout << "MOA " << estOrig << endl;

// cout << "MOA hormigaActual : " << hormigaActual << endl;
 
		construir_solucionTSP(estOrig, this, 0, sol);

//cout << "MOA after " << i << endl;
      
		if (pareto -> esNoDominado(sol, prob))
		{
			pareto -> eliminarDominados(sol, prob);

                        pareto -> insertar_en_el_conjunto_pareto_global(sol);

                }
                //else

		sol.resetear();
	}

//cout << "MOA////////2" << endl;

	tInicio = clock();

	tFin = tInicio;

	noLambdas=0;

	while(!condicion_parada(generacion, tInicio, tFin, extension_tiempo))
	{

		generacion += (int)k / hormigas;

		m = pareto -> getSize();

		tabla1 -> reiniciar(taoInicial);

		for (int i = 0; i < pareto -> getSize(); i++)

			actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), m);

		for (int i = 0; i < k; i++)
		{

//cout << "MOA construyo solucion " << i << endl;
			hormigaActual = i % hormigas + 1;

			construir_solucionTSP((int)(rand() % prob -> getSize()), this, 0, sol);

			if (pareto -> esNoDominado(sol, prob))
			{

				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}
                        //else

			sol.resetear();

		}

		tFin = clock();

	}
									//	pareto->listarSoluciones(prob,"/home/fuentes/tsp.moa.pareto");

									//      delete sol; sol = NULL;

									//      free(sol);

//cout << "moa fin" << endl;

}




void MOA :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0;

	int m;

	long int tInicio, tFin;


//cout << "MOA" << endl;        

									// Solucion * sol = new Solucion(prob -> getSize() + 1);
								/*

							        Solucion * sol = (Solucion *)malloc(sizeof(Solucion) * hormigas);
	

							        for(int i = 0; i < hormigas; i++)
							        {

								             sol[i].set_array(prob -> getSize() + 1);

							        } 
								*/

        Solucion sol(prob -> getSize());

	noLambdas=1;

	for (int i = 0; i < hormigas; i++)
	{

		construir_solucionQAP(rand() % prob -> getSize(), this, 0, sol);

		if (pareto -> esNoDominado(sol, prob))
		{
			pareto -> eliminarDominados(sol, prob);

                        pareto -> insertar_en_el_conjunto_pareto_global(sol);

                }
		//else

		sol.resetear();

	}

	tInicio = clock();

	tFin = tInicio;

	noLambdas=0;

	while(!condicion_parada(generacion, tInicio, tFin, extension_tiempo))
	{

		generacion++;

		m = pareto -> getSize();

		tabla1 -> reiniciar(taoInicial);

		for (int i = 0; i < pareto -> getSize(); i++)

			actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), m);

		for (int i = 0; i < k; i++)
		{

			hormigaActual = i % hormigas + 1;

			construir_solucionQAP(rand() % prob -> getSize(), this, 0, sol);

			if (pareto -> esNoDominado(sol, prob))
			{

				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}
			//else

			sol.resetear();

		}

		tFin = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moa.pareto");

											//        delete sol;

}





void MOA :: ejecutarVRP(int extension_tiempo)
{
//cout << "MOA" << endl;
	int generacion = 0;

	int m;

	long int tInicio, tFin;

									//SolucionVRP * sol = new SolucionVRP(prob -> getSize() * 2);

	SolucionVRP sol(prob -> getSize() * 2);

	noLambdas = 1;

	for (int i = 0; i < hormigas; i++)
	{

		construir_solucionVRP(0, this, 0, sol);

		if (pareto -> esNoDominado(sol ,prob))
		{
		
			pareto -> eliminarDominados(sol, prob);

                        pareto -> insertar_en_el_conjunto_pareto_global(sol);

                }

		sol.resetear();

	}

	tInicio = clock();

	tFin = tInicio;

	noLambdas=0;

	while(!condicion_parada(generacion, tInicio, tFin, extension_tiempo))
	{

		generacion++;

		m = pareto -> getSize();

		tabla1 -> reiniciar(taoInicial);

		for (int i = 0; i < pareto -> getSize(); i++)

			actualizar_feromonas(* pareto -> getSolucionVRP(i), pareto -> getSolucionVRP(i) -> getSizeActual(), m);

		for (int i = 0; i < k; i++)
		{

			hormigaActual = i % hormigas + 1;

			construir_solucionVRP(0, this, 0, sol);

			if (pareto -> esNoDominado(sol, prob))
			{
			
				pareto -> eliminarDominados(sol, prob);

	                        pareto -> insertar_en_el_conjunto_pareto_global(sol);

        	        }
			//else

			sol.resetear();

		}

		tFin = clock();

	}
								//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moa.pareto");

												//        delete sol;

}



void MOA :: ejecutarBOMBAS(void)
{
/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


	Solucion * sols = new Solucion[hormigas](prob -> getSize());

	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion2 == 0)
  			    
  			        lambda_funcion2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion3 == 0)
  			    
  			        lambda_funcion3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion4 == 0)
  			    
  			        lambda_funcion4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(pareto -> getSolucion(i));

				actualizar_feromonas(pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}


/**********************MOAQ*************************************/

class MOAQ : public MOACO
{

	TablaFeromona * tabla1; //Matrices de feromonas

	double tmax;	// prob. utilizada en el pseudo-random rule

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double gama;	// Factor de descuento

	double lambda;	// constante utilizada en el pseudo-random rule

	double t;		// valor estocastico utilizado en el pseudo-random rule

	int familiaActual; // indica que objetivo actualiza la colonia actual

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, NORM1, NORM2; // utilizados para normalizacion
	

	void inicializar_tablas(void)
	{

		tabla1 = new TablaFeromona(prob -> getSize());

		tabla1 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize);

	double calcular_max_q(int estOrig, TablaFeromona * tab);

	int seleccionar_mayor(int estOrigen, int visitados[], TablaFeromona * tab);

	int seleccionar_probabilistico(int estOrigen, int visitados[], TablaFeromona * tab);
	
        int seleccionar_probabilistico(int intervalo, int bomba);

	void evaporar_feromonas(void);

public:

	MOAQ(Problem * p,char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tablas();

		t = pow(tmax, 2) / lambda;

		if(hormigas % 2 != 0) 
 
                           hormigas++;

	};

	
	~MOAQ()
        {

             delete tabla1; tabla1 = NULL;
               
             delete pareto; pareto = NULL;

        };

	void setFamiliaActual(int obj)
    	{
	
    		familiaActual = obj;
	
    	};
	
    	int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);
	 
	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);
	
    	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

    	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba);
	
	void ejecutarTSP(int extension_tiempo);
	
    	void ejecutarQAP(int extension_tiempo);
	
    	void ejecutarVRP(int extension_tiempo);
    	
    	void ejecutarBOMBAS(void);

};




void MOAQ :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1) 
               
               tiempoTotal = valor;
	
    	else 
               
               maxIteraciones = valor;

	// leer parametros especificos del MOAQ
	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "GAMA") == 0)

			fscanf(f, "%lf", &gama);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);

		else if(stricmp(cad, "LAMBDA") == 0)

			fscanf(f, "%lf", &lambda);

		else if(stricmp(cad, "TMAX") == 0)

			fscanf(f, "%lf", &tmax);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else

			fscanf(f, "%s", cad);

	}	

}



int MOAQ :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{



	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
							        //int sgteEstado, * visitados = new int [prob -> getSize()];



	double randomValue = rand() / (double)RAND_MAX; // valor aleatorio entre 0 y 1


	// marcar estados ya visitados

	for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	

	if (randomValue <= t)

		sgteEstado = seleccionar_mayor(estOrigen, visitados, tabla1);

	else

		sgteEstado = seleccionar_probabilistico(estOrigen, visitados, tabla1);


        free(visitados); 

										//delete[] visitados; visitados = NULL;

	return sgteEstado;

}




int MOAQ :: seleccionar_mayor(int estOrigen, int visitados[], TablaFeromona * tab)
{

	int sgteEstado;

	double mayorValor = -1; // inicializar a un valor peque�

	double heuristica, valorActual, random;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	for (int i = 0; i < prob -> getSize(); i++)

	{
		if (visitados[i] != 1) // estado i no visitado
		{

			if(familiaActual == 1)

				heuristica = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			else

				heuristica = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

			valorActual = pow(tab -> obtenerValor(estOrigen, i), alfa) * pow(heuristica, beta);

			if(valorActual > mayorValor)

			{

				mayorValor = valorActual;

				sgteEstado = i;

			}

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(mayorValor == -1)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}

	return sgteEstado;

}






int MOAQ :: seleccionar_probabilistico(int estOrigen, int visitados[], TablaFeromona * tab)
{

	int sgteEstado;

	double heuristica;

        
        double * productos = (double *)calloc(prob -> getSize(), sizeof(double));
	
				
								        //double * productos = new double[prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

	// hallar la suma y los productos
	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			if(familiaActual == 1)

				heuristica = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			else

				heuristica = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

			productos[i] = pow(tab -> obtenerValor(estOrigen, i), alfa) * pow(heuristica, beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = 0;

		}

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{

		// aplicar ruleta
		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

			}

		}

	}

        free(productos); 

											//delete[] productos; productos =  NULL;

	return sgteEstado;

}

int MOAQ :: seleccionar_probabilistico(int intervalo, int bomba)
{

   return 0;

}


int MOAQ :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}





int MOAQ :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
       							 //int sgteEstado, * visitados = new int [prob -> getSize()];

	double randomValue = rand() / (double)RAND_MAX; // valor aleatorio entre 0 y 1

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	// hallar el vecindario

	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) 
        
                 totalVisitados++; // estado 0 ya se contabilizo
	
    }
	
    for (int i = 0; i < problem -> getSize(); i++)
    {
	
    	if(!visitados[i])
	{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
		distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));
	
    		if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
		{
	
    			visitados[i] = 1; // marcar como no vecino
	
    			totalVisitados++;
	
    		}
	
    	}
	
    }
	
    if(totalVisitados >= problem -> getSize())
	
        	sgteEstado = 0; // ir al deposito
	
    else
    {
	
    	if (randomValue <= t)
	
    		sgteEstado = seleccionar_mayor(estOrigen, visitados, tabla1);
	
    	else
	
    		sgteEstado = seleccionar_probabilistico(estOrigen, visitados, tabla1);
	
    }
	  
    free(visitados); 

   								 //delete[] visitados;
	
	return sgteEstado;

}



int MOAQ :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba)
{

   return 0;

}

void MOAQ :: actualizar_feromonas(Solucion &sol, int solSize)
{

	double reward;

	int j, k;

	for(int i = 0; i < solSize - 1; i++)
	{	// aplicar reward al arco j,k de ambas tablas

		j = sol.get(i);

		k = sol.get(i + 1);

		reward = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX); // normalizado

		tabla1 -> actualizar(j, k, tabla1 -> obtenerValor(j, k) + rho * reward);

	}

}




double MOAQ :: calcular_max_q(int estOrig, TablaFeromona * tab)
{

	double max = 0, valorTabla;

	for(int i = 0; i < prob -> getSize(); i++)
	{

		valorTabla = tab -> obtenerValor(estOrig, i);

		if(valorTabla > max)

			max = valorTabla;

	}

	return max;

}





void MOAQ :: evaporar_feromonas(void)
{

	double maxQ;

	for(int i = 0; i < prob -> getSize(); i++)
	{

		for(int j = 0; j < prob -> getSize(); j++)
		{

			maxQ = calcular_max_q(j, tabla1);

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) * (1 - rho) + rho * gama * maxQ);

		}

	}

}




void MOAQ :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0, estOrigen;

    	clock_t start, end;

//cout << "MOAQ" << endl;        

	Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
        
    						    //Solucion ** sols = new Solucion * [hormigas];
							/*        
						        for(int i = 0; i < hormigas; i++)
						        {

					             sols[i] = new Solucion(prob -> getSize() + 1);

							        }
							*/
  
						        //Solucion sols[hormigas];
  
        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() + 1);

        }

	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		t = t * lambda / tmax; // actualizar para la regla pseudo-aleatoria

		// familia 1
		familiaActual = 1; //optimizar de acuerdo al objetivo 1

		for(int i = 0; i < hormigas / 2; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionTSP(estOrigen, this, 0, sols[i]);
		}

		// familia 2
		familiaActual = 2; //optimizar de acuerdo al objetivo 2

		for(int i = hormigas / 2; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

		        construir_solucionTSP(estOrigen, this, 0, sols[i]);

		}
		
		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				actualizar_feromonas(sols[i], sols[i].getSize());

			}

			//else
			sols[i].resetear();

		}

		end = clock();

	}
								//	pareto->listarSoluciones(prob,"/home/fuentes/tsp.moaq.pareto");
						

        free(sols); 

									/*
								        for(int i = 0; i < hormigas; i++)
									        {

								             delete sols[i]; sols[i] = NULL;

													        }
									*/
								//        delete[] sols; sols = NULL;
}







void MOAQ :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen;

    	clock_t start, end;

//cout << "MOAQ" << endl;        

	Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
        
    						    //Solucion ** sols = new Solucion * [hormigas];
							/*        
						        for(int i = 0; i < hormigas; i++)
						        {

					             sols[i] = new Solucion(prob -> getSize() + 1);

							        }
							*/
  
						        //Solucion sols[hormigas];
  
        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize());

        }


	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		t = t * lambda / tmax; // actualizar para la regla pseudo-aleatoria

		// familia 1

		familiaActual = 1; //optimizar de acuerdo al objetivo 1

		for(int i = 0; i < hormigas / 2; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionQAP(estOrigen, this, 0, sols[i]);

		}

		// familia 2
		familiaActual = 2; //optimizar de acuerdo al objetivo 2

		for(int i = hormigas / 2; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionQAP(estOrigen, this, 0, sols[i]);
		}
		
		evaporar_feromonas();
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				actualizar_feromonas(sols[i], sols[i].getSize());

			}
			//else

			sols[i].resetear();

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moaq.pareto");
												/*
									        for(int i = 0; i < hormigas; i++)
									        {

									             delete sols[i];

										        }
										*/
										//        delete[] sols;

        free(sols); 

}





void MOAQ :: ejecutarVRP(int extension_tiempo)
{
//cout << "MOAQ" << endl;
	int generacion = 0, estOrigen;

   	clock_t start, end;


	//SolucionVRP * sols = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas);
  							    /*	
						        SolucionVRP ** sols = new SolucionVRP * [hormigas];

							        for(int i = 0; i < hormigas; i++)
						        {

						             sols[i] = new SolucionVRP(prob -> getSize() * 2);

							        }
							*/
      	
        SolucionVRP sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() * 2);

        }

	start = clock();

	end = start;
//cout << "antes condicion parada" << endl;	 
	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		t = t * lambda / tmax; // actualizar para la regla pseudo-aleatoria

		// familia 1

		familiaActual = 1; //optimizar de acuerdo al objetivo 1
//cout << "antes construir" << endl;	 
		for(int i = 0 ; i < hormigas / 2; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionVRP(estOrigen, this, 0, sols[i]);

		}
//cout << "despues contruir 1" << endl;	 
		// familia 2
		familiaActual = 2; //optimizar de acuerdo al objetivo 2

		for(int i = hormigas / 2; i < hormigas; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			construir_solucionVRP(estOrigen, this, 0, sols[i]);

		}
		
		evaporar_feromonas();
//cout << "antes esNoDominado" << endl;	 	
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

				actualizar_feromonas(sols[i], sols[i].getSizeActual());

			}
			//else

			sols[i].resetear();

		}
//cout << "despues esNoDominado" << endl;	 
		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moaq.pareto");
											/*
										        for(int i = 0; i < hormigas; i++)
										        {

									             delete sols[i];

										        }
											*/
											//        delete[] sols;
//cout << "termino MOAQ" << endl;	 
        //free(sols); 

}




void MOAQ :: ejecutarBOMBAS(void)
{
/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


	Solucion * sols = new Solucion[hormigas](prob -> getSize());

	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion2 == 0)
  			    
  			        lambda_funcion2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion3 == 0)
  			    
  			        lambda_funcion3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion4 == 0)
  			    
  			        lambda_funcion4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(pareto -> getSolucion(i));

				actualizar_feromonas(pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}



/**********************PACO*************************************/

class PACO : public MOACO
{

	TablaFeromona * tabla1, * tabla2; //Matrices de feromonas

	double peso1, peso2;	// Necesarios para el calculo de las probabilidades

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Coeficiente de evaporacion

	double q0;		// Probabilidad de pseudoaleatoriedad

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, NORM1, NORM2; // utilizados para normalizacion

	Solucion * mejorObj1,* segundoObj1,* mejorObj2,* segundoObj2;

	SolucionVRP * mejorObj1VRP, * segundoObj1VRP, * mejorObj2VRP, * segundoObj2VRP;

	void inicializar_tablas(void)
	{

		tabla1 = new TablaFeromona(prob -> getSize());

		tabla1 -> reiniciar(taoInicial);

		tabla2 = new TablaFeromona(prob -> getSize());

		tabla2 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void online_update(int origen, int destino);

	void actualizar_feromona_tabla1(Solucion &sol);

	void actualizar_feromona_tabla2(Solucion &sol);

	void actualizar_feromona_tabla1(SolucionVRP &sol);

	void actualizar_feromona_tabla2(SolucionVRP &sol);

	void actualizar_feromona(SolucionVRP *, TablaFeromona *, int objetivo);

	void evaporar_feromonas(void);

	int seleccionar_mayor(int estOrigen, int visitados[]);

	int seleccionar_probabilistico(int estOrigen, int visitados[]);
	
        int seleccionar_probabilistico(int intervalo, int bomba);

	void encontrar_mejores(int objetivo);

	void encontrar_mejoresVRP(int objetivo);

public:

	PACO(Problem * p,char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tablas();

		if(F1MAX > 10000000)
		{//es QAP

			mejorObj1 = new Solucion(prob -> getSize());

			segundoObj1 = new Solucion(prob -> getSize());

			mejorObj2 = new Solucion(prob -> getSize());

			segundoObj2 = new Solucion(prob -> getSize());

		}
		else
		{//no es QAP

			mejorObj1 = new Solucion(prob -> getSize() + 1);

			segundoObj1 = new Solucion(prob -> getSize() + 1);

			mejorObj2 = new Solucion(prob -> getSize() + 1);

			segundoObj2 = new Solucion(prob -> getSize() + 1);

		}

		mejorObj1VRP = new SolucionVRP(prob -> getSize() * 2);

		segundoObj1VRP = new SolucionVRP(prob -> getSize() * 2);

		mejorObj2VRP = new SolucionVRP(prob -> getSize() * 2);

		segundoObj2VRP = new SolucionVRP(prob -> getSize() * 2);

	};

	~PACO()
        {

		delete tabla1; tabla1 = NULL;

                delete tabla2; tabla2 = NULL;

		delete mejorObj1; mejorObj1 = NULL;

		delete segundoObj1; segundoObj1 = NULL;

		delete mejorObj2; mejorObj2 = NULL;

		delete segundoObj2; segundoObj2 = NULL;

		delete mejorObj1VRP; mejorObj1VRP = NULL;

		delete segundoObj1VRP; segundoObj1VRP = NULL;

		delete mejorObj2VRP; mejorObj2VRP = NULL;

		delete segundoObj2VRP; segundoObj2VRP = NULL;

                delete pareto; pareto = NULL;

        };
	
        int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba);
	
	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};





void PACO :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1) 
             
             tiempoTotal = valor;

	else 
              maxIteraciones = valor;

	// leer parametros especificos del PACO

	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "Q0") == 0)

			fscanf(f, "%lf", &q0);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);
 
		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else

			fscanf(f, "%s", cad);

	}	

}





int PACO :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{



	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	

							        //int sgteEstado, * visitados = new int [prob -> getSize()];



	double q;

	// marcar estados ya visitados, hallar el vecindario
	
        for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	

	//Pseudo-random proportional rule

	q = rand() / (double)RAND_MAX; 

	if (q <= q0)

		sgteEstado = seleccionar_mayor(estOrigen, visitados);

	else

		sgteEstado = seleccionar_probabilistico(estOrigen, visitados);



        free(visitados); 

									//delete[] visitados; visitados = NULL;

	return sgteEstado;

}




int PACO :: seleccionar_mayor(int estOrigen, int visitados[])
{

	int sgteEstado;

	double mayorValor = -1; // inicializar a un valor peque�

	double heuristica1, heuristica2, valorActual;

	double tau1, tau2, random;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	for (int i = 0; i < prob -> getSize(); i++)
	{

		if (visitados[i] != 1) // estado i no visitado
		{

			heuristica1 = 1.0 / prob -> heuristica_1(estOrigen, i); // normalizado

			heuristica2 = 1.0 / prob -> heuristica_2(estOrigen, i); // normalizado

			tau1 = tabla1 -> obtenerValor(estOrigen, i) * peso1; // valor de tabla con la preferencia de la hormiga

			tau2 = tabla2 -> obtenerValor(estOrigen,i) * peso2; // valor de tabla con la preferencia de la hormiga

			valorActual = pow(tau1 + tau2, alfa) * pow(2 / (heuristica1 / NORM1 + heuristica2 / NORM2), beta);

			sinPorcion[cantSinPorcion++] = i;

			if(valorActual > mayorValor)
			{

				mayorValor = valorActual;

				sgteEstado = i;

			}

		}

	}

	if(mayorValor == -1)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}

	return sgteEstado;
}





int PACO :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;

	double tau1, tau2;





	double * productos = (double *)calloc(prob -> getSize(), sizeof(double));
	
								        //double * productos = new double [prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;
	
	// hallar la suma y los productos
	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			heuristica1 = 1.0 / prob -> heuristica_1(estOrigen, i); // normalizado

			heuristica2 = 1.0 / prob -> heuristica_2(estOrigen, i); // normalizado

			tau1 = tabla1 -> obtenerValor(estOrigen,i) * peso1; // valor de tabla con la preferencia de la hormiga
	
	    		tau2 = tabla2 -> obtenerValor(estOrigen, i) * peso2; // valor de tabla con la preferencia de la hormiga
	
    			productos[i] = pow(tau1 + tau2, alfa) * pow(2 / (heuristica1 / NORM1 + heuristica2 / NORM2), beta);
	
    			suma += productos[i];
	
    			sinPorcion[cantSinPorcion++] = i;
	
    		}
	
    	}

	
    	if(suma == 0)
    	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

    	}
    	else
    	{

		// aplicar ruleta

		random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

		for (int i = 0; i < prob -> getSize(); i++)
		{	

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{
	
					sgteEstado = i;

					break;

				}

			}

		}

	}

        free(productos); 

											//delete[] productos; productos = NULL;

	return sgteEstado;

}


int PACO :: seleccionar_probabilistico(int intervalo, int bomba)
{

   return 0;

}

int PACO :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}




int PACO :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
      							  //int sgteEstado, * visitados = new int [prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	double q;


	// hallar el vecindario
	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) 

                      totalVisitados++; // estado 0 ya se contabilizo

	}


	for (int i = 0; i < problem -> getSize(); i++)
	{

		if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario

			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));

			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
			{

				visitados[i] = 1; // marcar como no vecino

				totalVisitados++;

			}

		}

	}
	

	if(totalVisitados >= problem -> getSize())

		sgteEstado = 0; // ir al deposito

	else
	{

		//Pseudo-random proportional rule
		q = rand() / (double)RAND_MAX; 

		if (q <= q0)

			sgteEstado = seleccionar_mayor(estOrigen, visitados);

		else

			sgteEstado = seleccionar_probabilistico(estOrigen, visitados);

	}
	 
        free(visitados); 
	
									//delete[] visitados;
	
	return sgteEstado;

}


int PACO :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba)
{

   return 0;

}




void PACO :: evaporar_feromonas(void)
{

	double tau1, tau2;

	for (int i = 0; i < prob -> getSize(); i++)
	{

		for (int j = 0; j < prob -> getSize(); j++)
		{

			tau1 = tabla1 -> obtenerValor(i, j);

			tabla1 -> actualizar(i, j, (1 - rho) * tau1);

			tau2 = tabla2 -> obtenerValor(i, j);

			tabla2 -> actualizar(i, j, (1 - rho) * tau2);

		}

	}

}




void PACO :: online_update(int origen, int destino)
{

	double tau;

	tau =(1 - rho) * tabla1 -> obtenerValor(origen, destino) + rho * taoInicial;

	tabla1 -> actualizar(origen, destino, tau);

	tau = (1 - rho) * tabla2 -> obtenerValor(origen, destino) + rho * taoInicial;

	tabla2 -> actualizar(origen, destino, tau);

}





void PACO :: actualizar_feromona_tabla1(Solucion &sol)
{

	double tau, deltatau;

	deltatau = F1MAX / prob -> funcion_obj_1(sol);
		
	for (int i = 0; i < sol.getSize() - 1; i++)
	{

		tau = tabla1 -> obtenerValor(sol.get(i), sol.get(i + 1)) + rho * deltatau;

		tabla1 -> actualizar(sol.get(i), sol.get(i + 1), tau);

	}

}



void PACO :: actualizar_feromona_tabla2(Solucion &sol)
{

	double tau, deltatau;

	deltatau = F2MAX / prob -> funcion_obj_2(sol);
		
	for (int i = 0; i < sol.getSize() - 1; i++)
	{

		tau = tabla2 -> obtenerValor(sol.get(i), sol.get(i + 1)) + rho * deltatau;

		tabla2 -> actualizar(sol.get(i), sol.get(i + 1), tau);

	}

}




void PACO :: actualizar_feromona_tabla1(SolucionVRP &sol)
{

	double tau, deltatau;

	deltatau = F1MAX / prob -> funcion_obj_1(sol);
		
	for (int i = 0 ; i < sol.getSizeActual() - 1; i++)
	{

		tau = tabla1 -> obtenerValor(sol.get(i), sol.get(i + 1)) + rho * deltatau;

		tabla1 -> actualizar(sol.get(i), sol.get(i + 1), tau);

	}

}




void PACO :: actualizar_feromona_tabla2(SolucionVRP &sol)
{

	double tau, deltatau;

	deltatau = F2MAX / prob -> funcion_obj_2(sol);
		
	for (int i = 0; i < sol.getSizeActual() - 1; i++)
	{

		tau = tabla2 -> obtenerValor(sol.get(i), sol.get(i + 1)) + rho * deltatau;

		tabla2 -> actualizar(sol.get(i), sol.get(i + 1), tau);

	}

}



void PACO :: encontrar_mejores(int objetivo)
{

	double evMejor, evActual;

	double evSeg = RAND_MAX;

	if (objetivo == 1)
	{

		mejorObj1 -> solcpy(* pareto -> getSolucion(0))	;

		evMejor = prob -> funcion_obj_1(* pareto -> getSolucion(0));
	}
	else
	{

		mejorObj2 -> solcpy(* pareto -> getSolucion(0));

		evMejor = prob -> funcion_obj_2(* pareto -> getSolucion(0));

	}
	

	for (int i = 1; i < pareto -> getSize(); i++)
	{

		if (objetivo == 1)
		{

			evActual = prob -> funcion_obj_1(* pareto->getSolucion(i));

			if (evActual < evMejor)
			{

				evSeg = evMejor;

				segundoObj1 -> solcpy(* mejorObj1);
				
				evMejor = evActual;

				mejorObj1 -> solcpy(* pareto -> getSolucion(i));

			}
			else if(evActual < evSeg)
			{

				evSeg = evActual;

				segundoObj1 -> solcpy(* pareto -> getSolucion(i));

			}

		}
		else
		{

			evActual = prob -> funcion_obj_2(* pareto -> getSolucion(i));

			if (evActual < evMejor)
			{

				evSeg = evMejor;

				segundoObj2 -> solcpy(* mejorObj2);
				
				evMejor = evActual;

				mejorObj2 -> solcpy(* pareto -> getSolucion(i));

			}
			else if(evActual < evSeg)
			{

				evSeg = evActual;

				segundoObj2 -> solcpy(* pareto -> getSolucion(i));

			}

		}

	}
	
}



void PACO :: encontrar_mejoresVRP(int objetivo)
{

	double evMejor, evActual;

	double evSeg = RAND_MAX;

	if (objetivo == 1)
	{

		mejorObj1VRP -> solcpy(* pareto -> getSolucionVRP(0));

		evMejor = prob -> funcion_obj_1(* pareto -> getSolucionVRP(0));

	}
	else
	{

		mejorObj2VRP -> solcpy(* pareto -> getSolucionVRP(0));

		evMejor = prob -> funcion_obj_2(* pareto->getSolucionVRP(0));

	}
	

	for (int i = 1; i < pareto -> getSize(); i++)
	{

		if (objetivo == 1)
		{

			evActual = prob -> funcion_obj_1(* pareto -> getSolucionVRP(i));

			if (evActual < evMejor)
			{

				evSeg = evMejor;

				segundoObj1VRP -> solcpy(* mejorObj1VRP);
				
				evMejor = evActual;

				mejorObj1VRP -> solcpy(* pareto -> getSolucionVRP(i));

			}
			else if(evActual < evSeg)
			{

				evSeg = evActual;

				segundoObj1VRP -> solcpy(* pareto -> getSolucionVRP(i));

			}

		}
		else
		{

			evActual = prob -> funcion_obj_2(* pareto -> getSolucionVRP(i));

			if (evActual < evMejor)
			{

				evSeg = evMejor;

				segundoObj2VRP -> solcpy(* mejorObj2VRP);
				
				evMejor = evActual;

				mejorObj2VRP -> solcpy(* pareto -> getSolucionVRP(i));

			}
			else if(evActual < evSeg)
			{

				evSeg = evActual;

				segundoObj2VRP -> solcpy(* pareto -> getSolucionVRP(i));

			}

		}

	}
	
}






void PACO :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0;

	long int tInicio, tFin;

//cout << "PACO" << endl;        

	//	Solucion * sol = new Solucion(prob -> getSize() + 1);
	
        Solucion sol(prob -> getSize() + 1);


	tInicio = clock();

	tFin = tInicio;

	while(!condicion_parada(generacion, tInicio, tFin, extension_tiempo))
	{

		generacion++;

		for (int i = 0; i < hormigas; i++)
		{

			peso1 = rand() / (double)RAND_MAX;

			peso2 = rand() / (double)RAND_MAX;

			construir_solucionTSP((int)(rand() % prob -> getSize()), this, 1, sol);

			if (pareto -> esNoDominado(sol, prob))
			{
				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}			
			//else

			sol.resetear();

		}

		evaporar_feromonas();

		encontrar_mejores(1);

		encontrar_mejores(2);

		actualizar_feromona_tabla1(* mejorObj1);

		actualizar_feromona_tabla2(* mejorObj2);

		if (pareto -> getSize() > 1)
		{

			actualizar_feromona_tabla1(* segundoObj1);

			actualizar_feromona_tabla2(* segundoObj2);

		}

		tFin = clock();

	}
							//	pareto->listarSoluciones(prob,"/home/fuentes/tsp.paco.pareto");

											//      delete sol; sol = NULL;

}




void PACO :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0;

	long int tInicio,tFin;


//cout << "PACO" << endl;        

	//	Solucion * sol = new Solucion(prob -> getSize() + 1);
	
        Solucion sol(prob -> getSize());

	

	tInicio = clock();

	tFin = tInicio;

	while(!condicion_parada(generacion, tInicio, tFin, extension_tiempo))
	{

		generacion++;

		for (int i = 0; i < hormigas; i++)
		{

			peso1 = rand() / (double)RAND_MAX;

			peso2 = rand() / (double)RAND_MAX;

			construir_solucionQAP((int)(rand() % prob -> getSize()), this, 1, sol);

			if (pareto -> esNoDominado(sol, prob))
			{
				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}	
			//else

			sol.resetear();

		}

		evaporar_feromonas();

		encontrar_mejores(1);

		encontrar_mejores(2);

		actualizar_feromona_tabla1(* mejorObj1);

		actualizar_feromona_tabla2(* mejorObj2);

		if (pareto -> getSize() > 1)
		{

			actualizar_feromona_tabla1(* segundoObj1);

			actualizar_feromona_tabla2(* segundoObj2);
		}

		tFin = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.paco.pareto");

												//        delete sol;

}





void PACO :: ejecutarVRP(int extension_tiempo)
{
//cout << "PACO" << endl;
	int generacion = 0;

	long int tInicio, tFin;

								//SolucionVRP * sol = new SolucionVRP(prob -> getSize() * 2);


	SolucionVRP sol(prob -> getSize() * 2);
	
	tInicio = clock();

	tFin = tInicio;

	while(!condicion_parada(generacion, tInicio, tFin, extension_tiempo))
	{

		generacion++;

		for (int i = 0; i < hormigas; i++)
		{

			peso1 = rand() / (double)RAND_MAX;

			peso2 = rand() / (double)RAND_MAX;

			construir_solucionVRP(0, this, 1, sol);

			if (pareto -> esNoDominado(sol, prob))
			{
				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}	
			//else

			sol.resetear();

		}

		evaporar_feromonas();

		encontrar_mejoresVRP(1);

		encontrar_mejoresVRP(2);

		actualizar_feromona_tabla1(* mejorObj1VRP);

		actualizar_feromona_tabla2(* mejorObj2VRP);

		if (pareto -> getSize() > 1)
		{

			actualizar_feromona_tabla1(* segundoObj1VRP);

			actualizar_feromona_tabla2(* segundoObj2VRP);

		}

		tFin = clock();

	}
									//	pareto->listarSoluciones(prob,"/home/fuentes/qap.paco.pareto");

												//	delete sol;

}





void PACO :: ejecutarBOMBAS(void)
{
/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


	Solucion * sols = new Solucion[hormigas](prob -> getSize());

	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion2 == 0)
  			    
  			        lambda_funcion2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion3 == 0)
  			    
  			        lambda_funcion3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion4 == 0)
  			    
  			        lambda_funcion4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 1, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(pareto -> getSolucion(i));

				actualizar_feromonas(pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}






/************M3AS*****************/
class M3AS  : public MOACO
{

	TablaFeromona * tabla1; //Matriz de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double taoInicial;	// valor inicial para las tablas de feromonas

	double F1MAX, F2MAX, NORM1, NORM2; // utilizados para normalizacion

	int hormigaActual; // utilizado para calcular los pesos lambda

	int noLambdas;

	void inicializar_tabla(void)
	{

		tabla1 = new TablaFeromona(prob -> getSize());

		tabla1 -> reiniciar(taoInicial);

	};

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol, int solSize, double deltaTao, double taumin, double taumax);

	int seleccionar_probabilistico(int estOrigen, int visitados[]);
	
        int seleccionar_probabilistico(int intervalo, int bomba);

	void evaporar_feromonas(void);

	double calcular_delta_tao(Solucion &sol);

	double calcular_delta_tao(SolucionVRP &sol);

public:

	M3AS(Problem * p, char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

		inicializar_parametros(file);

		inicializar_tabla();

		noLambdas = 0;

	};

	~M3AS()
        {

            delete tabla1; tabla1 = NULL;

            delete pareto; pareto = NULL;

        };
	
        int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);

	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba);
	
	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};




void M3AS :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1) 
           
             tiempoTotal = valor;

	else 
             maxIteraciones = valor;

	// leer parametros especificos del MAS
	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else

			fscanf(f, "%s", cad);

	}	

}



int M3AS :: seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol)
{


        int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
							        //int sgteEstado, * visitados = new int [prob -> getSize()];


	// marcar estados ya visitados, hallar el vecindario
	
        for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;

	sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		

        free(visitados); 

								//delete[] visitados; visitados = NULL;

	return sgteEstado;

}


int M3AS :: seleccionar_probabilistico(int intervalo, int bomba)
{

   return 0;

}



int M3AS :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;

   

        
        double * productos = (double *)calloc(prob -> getSize(), sizeof(double));
	
							        //double * productos = new double [prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	double lambda1, lambda2;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	if(noLambdas)

		lambda1 = lambda2 = 1;

	else
	{

		lambda1 = hormigaActual; // lambda para heuristica 1

		lambda2 = hormigas - hormigaActual + 1; // lambda para heuristica 2

	}

	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1
	// hallar la suma y los productos

	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

			productos[i] = pow(tabla1 -> obtenerValor(estOrigen, i), alfa) * pow(heuristica1, lambda1 * beta) * pow(heuristica2, lambda2 * beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{

		// aplicar ruleta
		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

			}

		}

	}

       
        free(productos); 

										//delete[] productos; productos = NULL;

	return sgteEstado;

}


int M3AS :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}



int M3AS :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
  						      //int sgteEstado, * visitados = new int [prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	// hallar el vecindario
	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) !=0)
            
                totalVisitados++; // estado 0 ya se contabilizo
	
        }
	
        for (int i = 0; i < problem -> getSize(); i++)
	{
	
	    	if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));
	
    			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))
			{
	
    				visitados[i] = 1; // marcar como no vecino
	
	    			totalVisitados++;
	
    			}
	
    		}
	
    	}
	
	if(totalVisitados >= problem -> getSize())
	
    		sgteEstado = 0; // ir al deposito
	
    	else
	
    		sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
		

	free(visitados); 

							//delete[] visitados;
	
	return sgteEstado;

}


int M3AS :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba)
{

    return 0;

}

void M3AS :: actualizar_feromonas(Solucion &sol, int solSize, double deltaTau, double taumin, double taumax)
{

	int j, k;

	double tjk;

	for(int i = 0; i < solSize - 1; i++)
	{	// actualizar la tabla de feromonas con el valor indicado por deltaTau

		j = sol.get(i);

		k = sol.get(i + 1);

		tjk = tabla1 -> obtenerValor(j, k);

		if (tjk + deltaTau < taumin)

			tabla1 -> actualizar(j, k, taumin);

		else if (tjk + deltaTau > taumax)

			tabla1 -> actualizar(j, k, taumax);

		else

			tabla1 -> actualizar(j, k, tjk + deltaTau);

	}

}

void M3AS :: evaporar_feromonas(void)
{

	for(int i = 0; i < prob -> getSize(); i++)

		for(int j = 0;j < prob -> getSize(); j++)

			tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) * (1 - rho));

}




double M3AS :: calcular_delta_tao(Solucion &sol)
{

	double delta;
	
	delta = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX); //normalizados

	return delta;

}




double M3AS :: calcular_delta_tao(SolucionVRP &sol)
{

	double delta;
	
	delta = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX); //normalizados

	return delta;

}





void M3AS :: ejecutarTSP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

        double deltaTao;

   	double taumin, taumax, f;

        clock_t start, end;

//cout << "M3AS" << endl;        
										//Solucion * sol = new Solucion(prob -> getSize() + 1);



	
        Solucion sol(prob -> getSize() + 1);

	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado

			construir_solucionTSP(estOrigen, this, 0, sol);


			if (pareto -> esNoDominado(sol, prob))
			{

				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}
			//else
			sol.resetear();

		}
		

		evaporar_feromonas();

		// actualizan la tabla las soluciones no dominadas

		for(int i = 0; i < pareto -> getSize(); i++)
		{

			deltaTao = calcular_delta_tao(* pareto -> getSolucion(i));

			taumax = deltaTao / (1 - rho);

			taumin = deltaTao / (2 * hormigas * (1 - rho));
 
			actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao, taumin, taumax);

		}

		end = clock();

	}
									//	pareto->listarSoluciones(prob,"/home/fuentes/tsp.m3as.pareto");

											//      delete sol; sol = NULL;

}



void M3AS :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen, cantNoDominados;

        double deltaTao;

	double taumin, taumax, f;

    	clock_t start, end;

	//cout << "M3AS" << endl;        
										//Solucion * sol = new Solucion(prob -> getSize() + 1);



	
        Solucion sol(prob -> getSize());


	start = clock();

	end = start;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i; // utilizado en seleccionar_sgte_estado

			construir_solucionQAP(estOrigen, this, 0, sol);

			if (pareto -> esNoDominado(sol, prob))
			{

				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}

			sol.resetear();

		}
		
		evaporar_feromonas();
		// actualizan la tabla los no dominados de la iteracion
		for(int i = 0; i < pareto -> getSize(); i++)
		{

			deltaTao = calcular_delta_tao(* pareto -> getSolucion(i));

			taumax = deltaTao / (1 - rho);

			taumin = deltaTao / (2 * hormigas * (1 - rho));

			actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao, taumin, taumax);

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.m3as.pareto");

												//        delete sol;

}




void M3AS :: ejecutarVRP(int extension_tiempo)
{
//cout << "M3AS" << endl;
	int generacion = 0, estOrigen, cantNoDominados;

   	double deltaTao;

	double taumin, taumax, f;

   	clock_t start, end;

								//SolucionVRP * sol = new SolucionVRP(prob -> getSize() * 2);


	SolucionVRP sol(prob -> getSize() * 2);

	start = clock();

	end = start;

	noLambdas = 1;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		cantNoDominados = 0;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i; // utilizado en seleccionar_sgte_estado

			construir_solucionVRP(estOrigen, this, 0, sol);

			if (pareto -> esNoDominado(sol, prob))
			{

				pareto -> eliminarDominados(sol, prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sol);

			}

			sol.resetear();

		}
		
		evaporar_feromonas();

		// actualizan la tabla los no dominados de la iteracion
		for(int i = 0; i < pareto -> getSize(); i++)
		{

			deltaTao = calcular_delta_tao(* pareto -> getSolucionVRP(i));

			taumax = deltaTao / (1 - rho);

			taumin = deltaTao / (2 * hormigas * (1 - rho));
		
            		actualizar_feromonas(* pareto -> getSolucionVRP(i), pareto -> getSolucionVRP(i) -> getSizeActual(), deltaTao, taumin, taumax);
		
        	}
		
        	end = clock();
	
	}
								//	pareto->listarSoluciones(prob,"/home/fuentes/qap.m3as.pareto");

											//        delete sol;

}


void M3AS :: ejecutarBOMBAS(void)
{
/*
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;


								//Solucion * sols = new Solucion[hormigas](prob -> getSize());


	Solucion sols[hormigas];

        sols.set_array(prob -> getSize());


	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion2 == 0)
  			    
  			        lambda_funcion2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion3 == 0)
  			    
  			        lambda_funcion3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion4 == 0)
  			    
  			        lambda_funcion4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 0, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> agregarNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i] -> resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(pareto -> getSolucion(i));

				actualizar_feromonas(pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
*/
}






/*************MOACS************/

class MOACS : public MOACO
{

	TablaFeromona * tabla1; //Matrices de feromonas

	double alfa;	// exponente para las feromonas

	double beta;	// exponente para la visibilidad

	double rho;	// Learning step (coeficiente de evaporacion)

	double taoInicial;	// valor inicial para las tablas de feromonas

	double tao, q0;

	double F1MAX, F2MAX, F3MAX, F4MAX, NORM1, NORM2, NORM3, NORM4; // utilizados para normalizacion

	int lambda_funcion_1, lambda_funcion_2, lambda_funcion_3, lambda_funcion_4; // utilizado para calcular los pesos lambda

	int noLambdas;

        int hormigaActual;

	void inicializar_tabla(void)
	{

 	       if(prob -> get_column_size() == 0)// para tsp, qap, y vrp
               {
    	     
                    tabla1 = new TablaFeromona(prob -> getSize());
        
               }
               else //para bombas
               {
           
                    tabla1 = new TablaFeromona(prob -> getSize(), prob -> get_column_size());
             
               }

               tabla1 -> reiniciar(taoInicial);
        
 	};

	void online_update(int origen, int destino);

	void inicializar_parametros(char * file);

	void actualizar_feromonas(Solucion &sol,int solSize,double deltaTao);

	int seleccionar_mayor(int estOrigen,int visitados[]);

	int seleccionar_mayor(int intervalo, int bomba);
	
        int seleccionar_probabilistico(int estOrigen,int visitados[]);

        int seleccionar_probabilistico(int intervalo, int bomba);
	
        double calcular_delta_tao(Solucion &sol);

	double calcular_delta_tao(SolucionVRP &sol);

	double calcular_tao_prima(double avr1, double avr2);

        double calcular_tao_prima(double avr1, double avr2, double avr3, double avr4);

	double calcular_average(int obj);

	double calcular_averageVRP(int obj);

public:

	MOACS(Problem * p, char * file) : MOACO(p)
	{

		NORM1 = 1; //por defecto

		NORM2 = 1; //por defecto

                NORM3 = 1;

                NORM4 = 1;

		F1MAX = 1; //por defecto

		F2MAX = 1; //por defecto

                F3MAX = 1;

                F4MAX = 1;

		inicializar_parametros(file);

		inicializar_tabla();

		noLambdas = 0;

	};
	
        ~MOACS()
        {

            delete tabla1; tabla1 = NULL;

            delete pareto; pareto = NULL;

        };

	int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol);

	int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual);
	
	int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bombas);


	void ejecutarTSP(int extension_tiempo);

	void ejecutarQAP(int extension_tiempo);

	void ejecutarVRP(int extension_tiempo);
	
	void ejecutarBOMBAS(void);

};


void MOACS :: inicializar_parametros(char * file)
{

	FILE * f = fopen(file, "r");

	char cad[200];

	int valor;

	fgets(cad, 199, f); // leer comentario

	fscanf(f, "%s = %d\n", cad, &criterio); // leer criterio de parada

	fscanf(f, "%s = %d\n", cad, &valor); // leer limite de parada

	if(criterio == 1) 
      
            tiempoTotal = valor;
 
 	else 

            maxIteraciones = valor;


	// leer parametros especificos del MOACS
	while(!feof(f))
	{

		fscanf(f, "%s", cad);

		fscanf(f, " =");

		if(stricmp(cad, "HORMIGAS") == 0)

			fscanf(f, "%d", &hormigas);

		else if(stricmp(cad, "ALFA") == 0)

			fscanf(f, "%lf", &alfa);

		else if(stricmp(cad, "BETA") == 0)

			fscanf(f, "%lf", &beta);

		else if(stricmp(cad, "RHO") == 0)

			fscanf(f, "%lf", &rho);

		else if(stricmp(cad, "TAU0") == 0)

			fscanf(f, "%lf", &taoInicial);

		else if(stricmp(cad, "Q0") == 0)

			fscanf(f, "%lf", &q0);

		else if(stricmp(cad, "F1MAX") == 0)

			fscanf(f, "%lf", &F1MAX);

		else if(stricmp(cad, "F2MAX") == 0)

			fscanf(f, "%lf", &F2MAX);

        else if(stricmp(cad, "F3MAX") == 0)

			fscanf(f, "%lf", &F3MAX);

		else if(stricmp(cad, "F4MAX") == 0)

			fscanf(f, "%lf", &F4MAX);

		else if(stricmp(cad, "D1MAX") == 0)

			fscanf(f, "%lf", &NORM1);

		else if(stricmp(cad, "D2MAX") == 0)

			fscanf(f, "%lf", &NORM2);

		else if(stricmp(cad, "D3MAX") == 0)

			fscanf(f, "%lf", &NORM3);

		else if(stricmp(cad, "D4MAX") == 0)

			fscanf(f, "%lf", &NORM4);
		else

			fscanf(f, "%s", cad);

	}	

}


int MOACS :: seleccionar_siguiente_estadoTSP(int estOrigen,Solucion &sol)
{ 



	int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
							        //int sgteEstado, * visitados = new int [prob -> getSize()];


	double q;

	// marcar estados ya visitados, hallar el vecindario

	for (int i = 0; sol.get(i) != -1; i++)

		visitados[sol.get(i)] = 1;
	
	//Pseudo-random proportional rule

	q = rand() / (double)RAND_MAX;

	if (q <= q0)

		sgteEstado = seleccionar_mayor(estOrigen, visitados);

	else

		sgteEstado = seleccionar_probabilistico(estOrigen, visitados);

        

        free(visitados); 


										//delete[] visitados; visitados = NULL;

	return sgteEstado;

}

int MOACS :: seleccionar_mayor(int intervalo, int bomba)
{

	int sgteEstado;

	double mayorValor = -1; // inicializar a un valor peque�

	double heuristica1, heuristica2, heuristica3, heuristica4, valorActual;

	double tau1, tau2;

	double  random;

				//int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

        BOMBAS * problema = (BOMBAS *) prob;

        BOMBAS * prob = problema;

	for (int i = 0; i < prob -> get_column_size(); i++)
	{

			heuristica1 = prob -> heuristica_1(intervalo, i) * NORM1; 

			heuristica2 = prob -> heuristica_2(bomba, i) * NORM2; 

                        heuristica3 = prob -> heuristica_3(intervalo, i) * NORM3; 

			heuristica4 = prob -> heuristica_4(i) * NORM4; 

         	
            valorActual = pow(tabla1 -> obtenerValor(intervalo, i), alfa) * pow(heuristica1, lambda_funcion_1 * beta) * pow(heuristica2, lambda_funcion_2 * beta)* pow(heuristica3, lambda_funcion_3 * beta)* pow(heuristica4, lambda_funcion_4 * beta);


			if(valorActual > mayorValor)
			{

				mayorValor = valorActual;

				sgteEstado = i;
			}

///			sinPorcion[cantSinPorcion++] = i;

	 }

	
/*
	if(mayorValor == -1)
	{

		random = rand() % cantSinPorcion;						// en que caso se uso. cuando es -1

		sgteEstado = sinPorcion[(int)random];

	}
*/
	return sgteEstado;

}

int MOACS :: seleccionar_mayor(int estOrigen, int visitados[])
{

	int sgteEstado;

	double mayorValor = -1; // inicializar a un valor peque�

	double heuristica1, heuristica2, valorActual;

	double tau1, tau2;

	double lambda1, lambda2, random;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;


	if(noLambdas)

		lambda1 = lambda2 = 1;

	else
	{

		lambda1 = hormigaActual; // peso de la hormiga actual para el objetivo 1

		lambda2 = hormigas - lambda1 + 1; // peso para el objetivo 2

	}

	for (int i = 0; i < prob -> getSize(); i++)
	{

		if (visitados[i] != 1) // estado i no visitado
		{
														// ask why 1 and 50 for NORM2
			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado      

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

         	valorActual = pow(tabla1 -> obtenerValor(estOrigen, i), alfa) * pow(heuristica1, lambda1 * beta) * pow(heuristica2, lambda2 * beta);

			if(valorActual > mayorValor)
			{

				mayorValor = valorActual;

				sgteEstado = i;
			}

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(mayorValor == -1)
	{

		random = rand() % cantSinPorcion;						// en que caso se uso. cuando es -1

		sgteEstado = sinPorcion[(int)random];

	}

	return sgteEstado;

}

int MOACS :: seleccionar_probabilistico(int intervalo, int bomba)
{

	int sgteEstado;

	double heuristica1, heuristica2, heuristica3, heuristica4;

	
				          //double * productos = (double *)calloc(prob -> get_column_size(), sizeof(double));
     
        double * productos = new double [prob -> get_column_size()];

	double random, suma = 0.0;

	double acum = 0.0;

	int sinPorcion[prob -> get_column_size()], cantSinPorcion = 0;

        BOMBAS * problema = (BOMBAS *) prob;
       
        BOMBAS * prob = problema;

	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

	// hallar la suma y los productos

 	for(int i = 0; i < prob -> get_column_size(); i++)
	{

			heuristica1 = prob -> heuristica_1(intervalo, i) * NORM1; 

			heuristica2 = prob -> heuristica_2(bomba, i) * NORM2; 

                        heuristica3 = prob -> heuristica_3(intervalo, i) * NORM3; 

			heuristica4 = prob -> heuristica_4(i) * NORM4; 

	        productos[i] = pow(tabla1 -> obtenerValor(intervalo, i), alfa) * pow(heuristica1, lambda_funcion_1 * beta) * pow(heuristica2, lambda_funcion_2 * beta)* pow(heuristica3, lambda_funcion_3 * beta)* pow(heuristica4, lambda_funcion_4 * beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{
		// aplicar ruleta
		for (int i = 0; i < prob -> get_column_size(); i++)
		{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

		}

	}

	delete[] productos; productos = NULL;

	return sgteEstado;

}

int MOACS :: seleccionar_probabilistico(int estOrigen, int visitados[])
{

	int sgteEstado;

	double heuristica1, heuristica2;

	

        double * productos = (double *)calloc(prob -> getSize(), sizeof(double));

								 	//double * productos = new double [prob -> getSize()];


	double random, suma = 0.0;

	double acum = 0.0;

	double lambda1, lambda2;

	int sinPorcion[prob -> getSize()], cantSinPorcion = 0;

	if(noLambdas)

		lambda1 = lambda2 = 1;

	else
	{		

		lambda1 = hormigaActual; // peso de la hormiga actual para el objetivo 1

		lambda2 = hormigas - lambda1 + 1; // peso para el objetivo 2

	}

	random = rand() / (double)RAND_MAX; // escoger un valor entre 0 y 1

	// hallar la suma y los productos

	for(int i = 0; i < prob -> getSize(); i++)
	{

		if(!visitados[i])
		{

			heuristica1 = prob -> heuristica_1(estOrigen, i) * NORM1; // normalizado

			heuristica2 = prob -> heuristica_2(estOrigen, i) * NORM2; // normalizado

         	productos[i] = pow(tabla1 -> obtenerValor(estOrigen, i), alfa) * pow(heuristica1, lambda1 * beta) * pow(heuristica2, lambda2 * beta);

			suma += productos[i];

			sinPorcion[cantSinPorcion++] = i;

		}

	}

	if(suma == 0)
	{

		random = rand() % cantSinPorcion;

		sgteEstado = sinPorcion[(int)random];

	}
	else
	{
		// aplicar ruleta
		for (int i = 0; i < prob -> getSize(); i++)
		{

			if (!visitados[i]) // estado i no visitado
			{

				acum += productos[i] / suma;

				if(acum >= random)
				{

					sgteEstado = i;

					break;

				}

			}

		}

	}

       
        free(productos); 

											//delete[] productos; productos = NULL;

	return sgteEstado;

}


int MOACS :: seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol)
{

	int sgte = seleccionar_siguiente_estadoTSP(estOrigen, sol);

	return sgte;

}

int MOACS :: seleccionar_siguiente_estadoBOMBAS(int intervalo, int bombas)
{ 

    int sgteEstado;

	//Pseudo-random proportional rule

	double q = rand() / (double)RAND_MAX;

	if (q <= q0)

		sgteEstado = seleccionar_mayor(intervalo, bombas);

	else

		sgteEstado = seleccionar_probabilistico(intervalo, bombas);


	return sgteEstado;

}

///////////////////////////////

//               CAMBIAR EL ESTADO DE ORIGEN DEPOSITO A CUALQUIER NODO


/////////////////////////////////////

int MOACS :: seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual)
{

        int sgteEstado, * visitados = (int *)calloc(prob -> getSize(), sizeof(int));
	
    							    //int sgteEstado, * visitados = new int [prob -> getSize()];

	SolucionVRP * soluc = (SolucionVRP *) &sol;

	VRPTW * problem = (VRPTW *)prob;

	int totalVisitados = 1; // necesariamente se visito el deposito 1 vez

	double distancia;

	double q;

	// hallar el vecindario

	for(int i = 0; i < soluc -> getSizeActual(); i++)
	{

		visitados[soluc -> get(i)] = 1;

		if(soluc -> get(i) != 0) 

	                 totalVisitados++; // estado 0 ya se contabilizo
	
    	}

    	for (int i = 0; i < problem -> getSize(); i++)
    	{

    		if(!visitados[i])
		{	// controlar si se cumplira la ventana, la capacidad
			// y si se podra volver a tiempo al deposito si fuera necesario
			distancia = max(currentTime + problem -> getDistancia(estOrigen, i), problem -> getTimeStart(i));
	
    			if(cargaActual + problem -> getDemanda(i) > problem -> getCapacity() ||
				currentTime + problem -> getDistancia(estOrigen, i) > problem -> getTimeEnd(i) ||     // verificar timestart
				distancia + problem -> getDistancia(i, 0) > problem -> getTimeEnd(0))                  // el tiempo de servicio
			{
	
    				visitados[i] = 1; // marcar como no vecino
	
    				totalVisitados++;
	
    			}
	
    		}
	
    	}

    	if(totalVisitados >= problem -> getSize())
	
    		sgteEstado = 0; // ir al deposito
	
    	else
	{
		//Pseudo-random proportional rule
		q = rand() / (double)RAND_MAX; 
	
    		if (q <= q0)
	
    			sgteEstado = seleccionar_mayor(estOrigen, visitados);
	
	    	else
	
    			sgteEstado = seleccionar_probabilistico(estOrigen, visitados);
	
    	}
	
        free(visitados); 
	
								//delete[] visitados; visitados = NULL;
	
	return sgteEstado;

}


void MOACS :: actualizar_feromonas(Solucion &sol, int solSize, double deltaTau)
{

	int j, k;

    if(prob -> get_column_size() == 0)// tsp, qap, vrp
    {
       
        for(int i = 0; i < solSize - 1; i++)
	    {	// actualizar ambas tablas en una cantidad 1/cantNoDominados

		    j = sol.get(i);

		    k = sol.get(i + 1);
                                               ///////////////////////////////////////                                             
		    tabla1 -> actualizar(j, k, tabla1 -> obtenerValor(j, k) + deltaTau); //NO SE USO LA FORMULA FALTO RO
                                     							//DE ESTA FORMA SE CONVERGE MUY RAPIDO
	    }
      
    }
    else// bombas
    {
        
        for(int i = 0; i < solSize; i++)
	    {

		    j = sol.get(i);

		    tabla1 -> actualizar(i, j, tabla1 -> obtenerValor(i, j) + deltaTau);//NO SE USO LA FORMULA FALTO RO
                                     							//DE ESTA FORMA SE CONVERGE MUY RAPIDO
	    }
    
    }                                                                                   ///////////////////////////////////////

}

  
double MOACS :: calcular_delta_tao(Solucion &sol)
{

	double delta;
	
    if(prob -> get_column_size() == 0)// tsp, qap
    {
   
        delta = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX); //normalizados
    
    }
    else//bombas
    {
        

        BOMBAS * problema = (BOMBAS *) prob;
        BOMBAS * prob = problema;

        delta = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX + prob -> funcion_obj_3(sol) / F3MAX + prob -> funcion_obj_4(sol) / F4MAX); //normalizados
    
    }
	
    return delta;

}


double MOACS :: calcular_delta_tao(SolucionVRP &sol)
{

	double delta;
	
	delta = 1.0 / (prob -> funcion_obj_1(sol) / F1MAX + prob -> funcion_obj_2(sol) / F2MAX); //normalizados		// SE SUMA Y NO SE MULTIP

	return delta;

}


double MOACS :: calcular_average(int obj)
{

	double avr = 0;

	for(int i = 0; i < pareto -> getSize(); i++)
	{

		if(obj == 1)

			avr += prob -> funcion_obj_1(* pareto -> getSolucion(i));

                else if(obj == 2)

			avr += prob -> funcion_obj_2(* pareto -> getSolucion(i));

                else if(obj == 3)
                { 

                        BOMBAS * problema = (BOMBAS *) prob;
        
                        BOMBAS * prob = problema;

			avr += prob -> funcion_obj_3(* pareto -> getSolucion(i));
              
                }
		else
                {     

                        BOMBAS * problema = (BOMBAS *) prob;
             
                        BOMBAS * prob = problema;

			avr += prob -> funcion_obj_4(* pareto -> getSolucion(i));
       
                }
	
        }
	
	return (avr / (double)pareto -> getSize());

}


double MOACS :: calcular_averageVRP(int obj)
{

	double avr = 0;

	for(int i = 0; i < pareto -> getSize(); i++)
	{

		if(obj ==1)

			avr += prob -> funcion_obj_1(* pareto -> getSolucionVRP(i));

		else

			avr += prob -> funcion_obj_2(* pareto -> getSolucionVRP(i));

	}
	
	return (avr / (double)pareto -> getSize());

}


double MOACS :: calcular_tao_prima(double avr1, double avr2)
{

	return (1.0 / (avr1 * avr2));

}


double MOACS :: calcular_tao_prima(double avr1, double avr2, double avr3, double avr4)
{

	return (1.0 / (avr1 * avr2 * avr3 * avr4));

}

void MOACS :: online_update(int origen, int destino)
{

	double tau;

	tau = (1 - rho) * tabla1 -> obtenerValor(origen, destino) + rho * taoInicial;//que pasa cuando tao es menor que tao inicial( se aumenta las feromonas en vez de evaporar

	tabla1 -> actualizar(origen, destino, tau);

}


void MOACS :: ejecutarTSP(int extension_tiempo)
{

//cout << " MOACS " << endl;
	int generacion = 0, estOrigen;

	double deltaTao, taoPrima;

	clock_t start, end;




        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
        
							        //Solucion ** sols = new Solucion * [hormigas];
									/*        
							        for(int i = 0; i < hormigas; i++)
							        {

						             sols[i] = new Solucion(prob -> getSize() +	1);

							        }
									*/

							        //Solucion sols[hormigas];
        
        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() + 1);

        }

	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado

			construir_solucionTSP(estOrigen, this, 1, sols[i]);
//cout << "construirMOACS " << i << endl;
		}
		
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

			}
			//else

			sols[i].resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2));
		
		if(taoPrima > tao)
		{

			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(* pareto -> getSolucion(i));

				actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
								//	pareto->listarSoluciones(prob,"/home/fuentes/tsp.moacs.pareto");
				
        free(sols); 
			

									/*
								        for(int i = 0; i < hormigas; i++)
								        {

								             delete sols[i]; sols[i] = NULL;

									        }
										*/
								//      delete[] sols; sols = NULL;

}


void MOACS :: ejecutarQAP(int extension_tiempo)
{

	int generacion = 0, estOrigen;

    	double deltaTao, taoPrima;

    	clock_t start, end;



        Solucion * sols = (Solucion *)malloc(sizeof(Solucion) * hormigas);
        
							        //Solucion ** sols = new Solucion * [hormigas];
									/*        
							        for(int i = 0; i < hormigas; i++)
							        {

						             sols[i] = new Solucion(prob -> getSize() +	1);

							        }
									*/

							        //Solucion sols[hormigas];
        
        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize());

        }

	start = clock();

	end = start;

	tao = -1;

	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = rand() % (prob -> getSize()); // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado

			construir_solucionQAP(estOrigen, this, 1, sols[i]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

			}

			//else
			sols[i].resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(* pareto -> getSolucion(i));

				actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
											/*
									        for(int i = 0; i < hormigas; i++)
										        {

									             delete sols[i];

										        }
											*/
											  //      delete[] sols;

        free(sols); 
}



void MOACS :: ejecutarVRP(int extension_tiempo)
{
//cout << "MOACS" << endl;
	int generacion = 0, estOrigen;

    	double deltaTao, taoPrima;

    	clock_t start, end;



//        SolucionVRP * sols = (SolucionVRP *)malloc(sizeof(SolucionVRP) * hormigas)	;
							 /*    
						        SolucionVRP ** sols = new SolucionVRP * [hormigas];

								        for(int i = 0; i < hormigas; i++)
							        {

						             sols[i] = new SolucionVRP(prob -> getSize() * 2);

							        }
								*/

      
        SolucionVRP sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize() * 2);

        }

	start = clock();

	end = start;

	tao = -1;

	noLambdas = 1;

//cout << "antes condicion parada" << endl;	 
	while(!condicion_parada(generacion, start, end, extension_tiempo))
	{

		generacion++;

		for(int i = 0; i < hormigas; i++)
		{

			estOrigen = 0; // colocar a la hormiga en un estado inicial aleatorio

			hormigaActual = i + 1; // utilizado en seleccionar_sgte_estado
//cout << "antes construir" << endl;	 
			construir_solucionVRP(estOrigen, this, 1, sols[i]);
//cout << "despues construir" << endl;	 

		}
//cout << "antes esNoDominado" << endl;	 
		for(int i = 0; i < hormigas; i++)
		{

			if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

                                pareto -> insertar_en_el_conjunto_pareto_global(sols[i]);

			}

			//else
			sols[i].resetear();

		}
//cout << "despues esNoDominado" << endl;	 
		taoPrima = calcular_tao_prima(calcular_averageVRP(1), calcular_averageVRP(2));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(* pareto -> getSolucionVRP(i));
			
                		actualizar_feromonas(* pareto -> getSolucionVRP(i), pareto -> getSolucionVRP(i) -> getSizeActual(), deltaTao);
			
            		}
		
        	}

        	end = clock();
	
    	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
												/*
									        for(int i = 0; i < hormigas; i++)
										        {

									             delete sols[i];

										        }
											*/
										//        delete[] sols;
//cout << "despues iteracion" << endl;	 
        //free(sols); 
}



void MOACS :: ejecutarBOMBAS(void)
{
	int generacion = 0, estOrigen;

  	double deltaTao, taoPrima;

   	clock_t start, end;



						        //Solucion ** sols = (Solucion **)malloc(sizeof(Solucion * ) * hormigas)	;
  								     /* 
							        Solucion ** sols = new Solucion * [hormigas];

						        for(int i = 0; i < hormigas; i++)
							        {

						             sols[i] = new Solucion(prob -> getSize());

							        }

								*/
        Solucion sols [hormigas];

        for(int i = 0; i < hormigas; i++)
        {

             sols[i].set_array(prob -> getSize());

        }

	start = clock();

	end = start;

	tao = -1;

	//while(!condicion_parada(generacion, start, end))
	{

		generacion++;


        lambda_funcion_2 = 6; // utilizado en seleccionar_sgte_estado
  			    
   	    lambda_funcion_3 = 11; // utilizado en seleccionar_sgte_estado
  			      			    
  	    lambda_funcion_4 = 16; // utilizado en seleccionar_sgte_estado


		for(int i = hormigas; i > 0; i--)
		{

  			    lambda_funcion_1 = i; // utilizado en seleccionar_sgte_estado

  			    lambda_funcion_2--; // utilizado en seleccionar_sgte_estado
  			    
  			    if(lambda_funcion_2 == 0)
  			    
  			        lambda_funcion_2 = hormigas;

                lambda_funcion_3--; // utilizado en seleccionar_sgte_estado
  			    
                if(lambda_funcion_3 == 0)
  			    
  			        lambda_funcion_3 = hormigas;    

  			    lambda_funcion_4--; // utilizado en seleccionar_sgte_estado   			   

                if(lambda_funcion_4 == 0)
  			    
  			        lambda_funcion_4 = hormigas;
               

                estOrigen = rand() % (prob -> get_column_size()); // colocar a la hormiga en un estado inicial aleatorio

			    construir_solucionBOMBAS(estOrigen, this, 1, sols[i - 1]);

		}
		
		for(int i = 0; i < hormigas; i++)
		{
		
        	if (pareto -> esNoDominado(sols[i], prob))
			{

				pareto -> eliminarDominados(sols[i], prob);

			}

			//else
			sols[i].resetear();

		}
		
		taoPrima = calcular_tao_prima(calcular_average(1), calcular_average(2), calcular_average(3), calcular_average(4));
		
		if(taoPrima > tao)
		{
			// reiniciar tabla de feromonas
			tao = taoPrima;

			tabla1 -> reiniciar(tao);

		}
		else
		{

			// actualizan la tabla las soluciones del frente Pareto
			for(int i = 0; i < pareto -> getSize(); i++)
			{

				deltaTao = calcular_delta_tao(* pareto -> getSolucion(i));

				actualizar_feromonas(* pareto -> getSolucion(i), pareto -> getSolucion(i) -> getSize(), deltaTao);

			}

		}

		end = clock();

	}
//	pareto->listarSoluciones(prob,"/home/fuentes/qap.moacs.pareto");
														/*
									        for(int i = 0; i < hormigas; i++)
									        {

								             delete sols[i];

										        }
										*/
											//      delete[] sols;


}
