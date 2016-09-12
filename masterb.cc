#include "/usr/share/pvm3/include/pvm3.h"

#include "varios.h"

#include "Problema.cc"

#include "clases2.cc"

#include "clases.cc"


/* Tipos de definidos */

typedef struct estado
{

	int algoritmo;		// Algoritmo asignado
	
	int solAportadas;	// Nro. de soluciones aportadas al Frente Pareto

} state;




/* Variables globales */   

float metricas[NTASKS][CANTIDAD_METRICAS];// contiene las metricas correspondientes a cada conjunto parento proporcionado por los esclavos

int frentes[NTASKS][NTASKS];

int cantidad_de_frentes = 0;

int cantidad_procesos_seleccionados = 0;

int frente_actual[NTASKS];

int procesos_seleccionados[NTASKS];    // registra los procesos ya asignados a algunos de los frentes pareto


     
ConjuntoPareto cp(500);  		// Conjunto Pareto Global

ConjuntoPareto * lista_de_conjuntos_pareto;  // contenedor de todos los conjuntos pareto de los esclavos

state estado[NTASKS];	        	// Almacena el estado de un esclavo

int criterioFin;			// Indica el criterio de parada del proceso Master: por tiempo (1), por iteraciones (2)

int tiempoTotal;			// Guarda el tiempo de ejecucion del proceso Master

int maxIteraciones;			// Guarda la maxima cantidad de iteraciones a ejecutar el proceso Master

int metodo_reemplazo;       // elista o probabilistico del proceso Master

Problem * prob;				// Problema a resolver





/*	
        Inserta en cp global las soluciones cpaux si es no dominada
	Devuelve la cantidad de soluciones que entraron al cp global
*/

void actualizar_frente_global(ConjuntoPareto * cpaux)
{

	for(int i = 0; i < cpaux -> getSize(); i++)
	{

		if (cp.agregarNoDominado(* (cpaux -> getSolucion(i)), prob))      // true si la solucion se incorpora al conjunto pareto global
		{

			cp.eliminarDominados(* (cpaux -> getSolucion(i)), prob);        //elimina las soluciones del conjunto pareto global dominados

		}

	}
	
}


void actualizar_frente_globalVRP(ConjuntoPareto * cpaux)
{


	for(int i = 0; i < cpaux -> getSize(); i++)
	{

		if (cp.agregarNoDominado(* (cpaux -> getSolucionVRP(i)), prob))
		{

			cp.eliminarDominados(* (cpaux -> getSolucionVRP(i)), prob);


		}

	}

}

//------------------------------------------------------------------------------------------
// Actualizacion del conjunto pareto global con los con. pare. de los 9 procesos
//------------------------------------------------------------------------------------------

void actualizar_frente_pareto_global(int problema)
{

		if(problema == C101 || problema == RC101)		/* Actualizar frente global */
		{
	
			for (int i = 0; i < NTASKS; i++)
			{
				
				actualizar_frente_globalVRP(lista_de_conjuntos_pareto[i]);
				
			}
			
		}
    	else
		{		
			
			for (int i = 0; i < NTASKS; i++)
			{

				actualizar_frente_global(lista_de_conjuntos_pareto[i]);
			
			}
			
		}
		//  estado[i].solAportadas = cantSols;
		
}

//------------------------------------------------------------------------------------------













//------------------------------------------------------------------------------------------
// calculo de sigma para la metrica 2 para vrp
//------------------------------------------------------------------------------------------

 double calcular_sigma_vrp()
 {

	double x1, x2, menor_objetivo_1 = 0.0, menor_objetivo_2 = 0.0, mayor_distancia1, mayor_distancia2;

    int punto_extremo_1, punto_extramo_2;
    
    


    menor_objetivo_1 = prob -> funcion_obj_1(* cp.getSolucionVRP(0));
    
    punto_extremo_1 = 0;           
               
    menor_objetivo_2 = prob -> funcion_obj_2(* cp.getSolucionVRP(0));
   	
    punto_extremo_2 = 0;
    
    
	for (int i = 1; i < cp.getSize(); i++) 
    {
 
           x1 = prob -> funcion_obj_1(* cp.getSolucionVRP(i));
               
           if (x1 < menor_objetivo_1)
		   {

  				menor_objetivo_1 = x1;
  				
   		        punto_extremo_1 = i;

		   }

           x2 = prob -> funcion_obj_2(* cp.getSolucionVRP(i));

           if (x2 < menor_objetivo_2)
		   {

  				menor_objetivo_2 = x2;
  				
			    punto_extremo_2 = i;

		   }

    }//end for

    mayor_distancia1 = abs(prob -> funcion_obj_1(* cp.getSolucionVRP(punto_extremo_1)) - prob -> funcion_obj_1(* cp.getSolucionVRP(punto_extremo_2)));

    mayor_distancia2 = abs(prob -> funcion_obj_2(* cp.getSolucionVRP(punto_extremo_1)) - prob -> funcion_obj_2(* cp.getSolucionVRP(punto_extremo_2)));

	return 0.1 * sqrt(pow(mayor_distancia1, 2) + pow(mayor_distancia2, 2));

 }


//------------------------------------------------------------------------------------------
// calculo de sigma para la metrica 2 para bombas
//------------------------------------------------------------------------------------------

 double calcular_sigma_bombas()
 {

    double w1, x1, y1, z1, w2, x2, y2, z2, distancia, mayor_distancia1 = 0.0, mayor_distancia2 = 0.0, mayor_distancia3 = 0.0, mayor_distancia4 = 0.0;

   	
	for (int i = 0; i < cp.getSize() - 1; i++) 
    {
 
    
           w1 = prob -> funcion_obj_1(* cp.getSolucion(i));
               
           x1 = prob -> funcion_obj_2(* cp.getSolucion(i));

           y1 = prob -> funcion_obj_3(* cp.getSolucion(i));
               
           z1 = prob -> funcion_obj_4(* cp.getSolucion(i));
           
        
           for (int j = i + 1; j < getSize(); j++)
           {

                   
                w2 = prob -> funcion_obj_1(* cp.getSolucion(j));
              
                x2 = prob -> funcion_obj_2(* cp.getSolucion(j));

                y2 = prob -> funcion_obj_3(* cp.getSolucion(j));
               
                z2 = prob -> funcion_obj_4(* cp.getSolucion(j));

        		distancia = abs(w2 - w1);

		    	if (distancia > mayor_distancia1)
			    {

  				     mayor_distancia1 = distancia;

			    }

                distancia = abs(x2 - x1);

		     	if (distancia > mayor_distancia2)
			    {

				     mayor_distancia2 = distancia;

			    }

        		distancia = abs(y2 - y1);

		    	if (distancia > mayor_distancia3)
			    {

  				     mayor_distancia3 = distancia;

			    }

                distancia = abs(z2 - z1);

		     	if (distancia > mayor_distancia4)
			    {

				     mayor_distancia4 = distancia;

			    }
    
          }//end for
       
    }//end for


    //ver si los dos puntos coinciden        

	return 0.1 * sqrt(pow(mayor_distancia1, 2) + pow(mayor_distancia2, 2) + pow(mayor_distancia1, 2) + pow(mayor_distancia2, 2));

 }


//------------------------------------------------------------------------------------------
// calculo de sigma para la metrica 2 para tsp y qap
//------------------------------------------------------------------------------------------

double calcular_sigma()// tsp, qap
{

	double x1, x2, menor_objetivo_1 = 0.0, menor_objetivo_2 = 0.0, mayor_distancia1, mayor_distancia2;

    int punto_extremo_1, punto_extramo_2;
    
    


    menor_objetivo_1 = prob -> funcion_obj_1(* cp.getSolucion(0));
    
    punto_extremo_1 = 0;           
               
    menor_objetivo_2 = prob -> funcion_obj_2(* cp.getSolucion(0));
   	
    punto_extremo_2 = 0;
    
    
	for (int i = 1; i < cp.getSize(); i++) 
    {
 
           x1 = prob -> funcion_obj_1(* cp.getSolucion(i));
               
           if (x1 < menor_objetivo_1)
		   {

  				menor_objetivo_1 = x1;
  				
   		        punto_extremo_1 = i;

		   }

           x2 = prob -> funcion_obj_2(* cp.getSolucion(i));

           if (x2 < menor_objetivo_2)
		   {

  				menor_objetivo_2 = x2;
  				
			    punto_extremo_2 = i;

		   }

    }//end for

    mayor_distancia1 = abs(prob -> funcion_obj_1(* cp.getSolucion(punto_extremo_1)) - prob -> funcion_obj_1(* cp.getSolucion(punto_extremo_2)));

    mayor_distancia2 = abs(prob -> funcion_obj_2(* cp.getSolucion(punto_extremo_1)) - prob -> funcion_obj_2(* cp.getSolucion(punto_extremo_2)));

	return 0.1 * sqrt(pow(mayor_distancia1, 2) + pow(mayor_distancia2, 2));

 }



//------------------------------------------------------------------------------------------------------------------------------
// CALCULO DE LAS 5 METRICAS DE CADA UNO DE LOS CONJUNTOS PARETOS DE LOS 9 PROCESOS
//------------------------------------------------------------------------------------------------------------------------------

void calcular_metricas(int problema)
{
	

	double sigma;
	
	
	if(problema == C101 || problema == RC101)// vrp
	{
		
        sigma = calcular_sigma_vrp();    
		
		for(int i = 0; i < NTASKS; i++)
        {

    	       		metricas[i][0] =  lista_de_conjuntos_pareto[i] -> get_metrica1_vrp(prob, cp);//distancia

    	       		metricas[i][1] =  lista_de_conjuntos_pareto[i] -> get_metrica2_vrp(prob, sigma);//distribucion

    	       		metricas[i][2] =  lista_de_conjuntos_pareto[i] -> get_metrica3_vrp(prob);//extension
 
    	       		metricas[i][3] =  lista_de_conjuntos_pareto[i] -> get_metrica4_vrp(prob, cp);//error
 
    	       		metricas[i][4] =  lista_de_conjuntos_pareto[i] -> get_metrica5_vrp(prob, cp);//cantidad de soluciones aportadas

		}
	
	}
    else if(problema == BOMBAS1)//bombas
	{
		
        sigma = calcular_sigma_bombas();    
		
        for(int i = 0; i < NTASKS; i++)
        {

    	       		metricas[i][0] =  lista_de_conjuntos_pareto[i] -> get_metrica1_bombas(prob, cp);//distancia

    	       		metricas[i][1] =  lista_de_conjuntos_pareto[i] -> get_metrica2_bombas(prob, sigma);//distribucion

    	       		metricas[i][2] =  lista_de_conjuntos_pareto[i] -> get_metrica3_bombas(prob);//extension
 
    	       		metricas[i][3] =  lista_de_conjuntos_pareto[i] -> get_metrica4_bombas(prob, cp);//error
 
    	       		metricas[i][4] =  lista_de_conjuntos_pareto[i] -> get_metrica5_bombas(prob, cp);//cantidad de soluciones aportadas

		}
	
	}
	else
	{	

        sigma = calcular_sigma();    

		for(int i = 0; i < NTASKS; i++)
        {

	   	           metricas[i][0] = lista_de_conjuntos_pareto[i] -> get_metrica1(prob, cp);

    			   metricas[i][1] = lista_de_conjuntos_pareto[i] -> get_metrica2(prob, sigma);

    			   metricas[i][2] = lista_de_conjuntos_pareto[i] -> get_metrica3(prob);
 
    			   metricas[i][3] = lista_de_conjuntos_pareto[i] -> get_metrica4(prob, cp);
 
      			   metricas[i][4] = lista_de_conjuntos_pareto[i] -> get_metrica5(prob, cp);

 		}

	}

}




/* Realiza una ruleta con probabilidades uniformes
   para los miembros del array ids */
int ruleta_uniforme(int ids[], int sizeIds)
{

	return ids[rand() % sizeIds];

}




void inicializaciones()
{
     
     cantidad_de_frentes = 0;

     cantidad_procesos_seleccionados = 0;// cantidad de procesos distribuidos en los distintos frentes

     for(int i = 0; i < NTASKS; i++)
     {

          for(int j = 0; j < NTASKS; j++)
          {
 
                  frentes[i][j] = 0;    
     
          }
         
     }     

     for(int i = 0; i < NTASKS; i++)
     {
     
         procesos_seleccionados[i] = 0;   // 1 para seleccionado y 0 para no seleccionado
         
     }
     
}


void inicializar_frente_actual()
{

     for(int i = 0; i < NTASKS; i++)
     {
                               
            frente_actual[i] = 0;
    
     }

} 

int es_dominado(int i)
{
    
    int is_dominated = 0; // no es dominado
    
    for(int j = 0; j < NTASKS; j++)
    {
           
          if(frente_actual[j] == 1)
          {
          
                  if((metricas[j][0] <= metricas[i][0] && metricas[j][1] >= metricas[i][1] && metricas[j][2] >= metricas[i][2] && metricas[j][3] <= metricas[i][3] && metricas[j][4] >= metricas[i][4]) && (metricas[j][0] < metricas[i][0] || metricas[j][1] > metricas[i][1] || metricas[j][2] > metricas[i][2] || metricas[j][3] < metricas[i][3] || metricas[j][4] > metricas[i][4]))                    
                  {
                                     
                        is_dominated = 1;// si es dominado
                        
                        break;
                                                             
                  }
        
          }
                          
    }
    
    if(is_dominated == 1)
    {
    
          return 1;                
                    
    }
    else
    {
        
          return 0;
        
    }

}

void eliminar_dominados(int i)
{
     
      for(int j = 0; j < NTASKS; j++)
      {
           
            if(frente_actual[j] == 1)
            {
          
                  if((metricas[i][0] <= metricas[j][0] && metricas[i][1] >= metricas[j][1] && metricas[i][2] >= metricas[j][2] && metricas[i][3] <= metricas[j][3] && metricas[i][4] >= metricas[j][4]) && (metricas[i][0] < metricas[j][0] || metricas[i][1] > metricas[j][1] || metricas[i][2] > metricas[j][2] || metricas[i][3] < metricas[j][3] || metricas[i][4] > metricas[j][4]))                    
                  {
                                     
                         frente_actual[j] = 0;  
                                                            
                  }
      
            }
                          
      }
     
}


void registrar_frente_actual()
{
     
         int contador = 0;

         for(int i = 0; i < NTASKS; i++)
         {
    
                 frentes[i][cantidad_de_frentes] = frente_actual[i];// se actualiza por columnas. Cuales son los procesos seleccionados en el frente actual(columna)
                 
                 if(frente_actual[i] == 1)
                 {
                        
                        procesos_seleccionados[i] = 1;
                        
                        contador++;             
                 
                 }

         }//end for                   

         cantidad_de_frentes++;
         
         cantidad_procesos_seleccionados += contador;
         
}

void clasificar_procesos()// distribuye todos los procesos en frentes
{
     
     inicializaciones();
      
     while(cantidad_procesos_seleccionados < NTASKS)
     {
     
               inicializar_frente_actual(); 
                                            
               for(int i = 0; i < NTASKS; i++)//todos los procesos que no fueron seleccionados todavia compiten por entrar en el frente actual
               {
                       
                       if(procesos_seleccionados[i] == 0) //todavia no esta en ningun frente, por lo tanto no fue seleccionado todavia
                       {
                       
                              if(es_dominado(i) == 0)// no es dominado 
                              {
                                     
                                     eliminar_dominados(i);

                                     frente_actual[i] = 1;           
                                     
                              }
                        
                       }//end if
                       
               }//end for
                                            
               registrar_frente_actual(); //copia el frente actual en la matriz de frentes y actualiza vector de procesos seleccionados
               
     }//end while

}//end function


void realizar_reemplazo()
{

     if(metodo_reemplazo == 1)
     {
     
          reemplazo_elitista();
     
     }     
     else 
     {
     
          reemplazo_probabilistico();
          
     }
     
}



void reemplazo_elitista()
{

     int contador = 0;
     
     int frente_auxiliar[NTASKS];
     
     int mejor_proceso, peor_proceso;
     
     
     for(int i = 0; i < NTASKS; i++)
     {
     
           frente_auxiliar[i] = 0;                                                    
     
     }
          
     if (cantidad_de_frentes > 1)
     {
                             
          for(int i = 0; i < NTASKS; i++)
          {
             
              if(frentes[i][0] == 1)
              {
             
                    frente_auxiliar[contador++] = i;                                                    
             
              }
             
          }

          mejor_proceso = frente_auxiliar[rand() % contador];
     

          contador = 0;
     
          for(int j = 0; j < NTASKS; j++)
          {
             
              if(frentes[j][cantidad_de_frentes - 1] == 1)
              {
             
                  frente_auxiliar[contador++] = j;                                                    
             
              }
     
          }
     
          peor_proceso = frente_auxiliar[rand() % contador];
     
          estado[peor_proceso].algoritmo = estado[mejor_proceso].algoritmo;

     }

}



void reemplazo_probabilistico()
{

     int algoritmos_utilizados[NTASKS + 1]; // los algoritmos se numeran del 1 al 9
     
     int algoritmos_no_utilizados[NTASKS];
     
     int cantidad;
     
     double * probabilidades; //se incorpora el frente de los algoritmos no utilizados en la iteracion
     
     int cantidad_frentes_local = 0; //usado solo en reemplazo probabilistico
          
     int denominador = 0, numerador = 0;
     
     double valor, suma;
     
     int numero_frente;
     
     int numero_proceso;
     
     int frente_auxiliar[NTASKS];
          
     int contador;
     
     int existe_frente_extra; 
     
     
     for(int i = 0; i <= NTASKS; i++)
     {

           algoritmos_utilizados[i] = 0;
     
     }
     
     for(int i = 0; i < NTASKS; i++)
     {

           algoritmos_utilizados[estado[i].algoritmo] = 1;// se identifican los algoritmos de los 9 procesos
     
     }
     
     cantidad = 0;
     
     for(int i = 1; i <= NTASKS; i++)
     {

           if(algoritmos_utilizados[i] == 0)//no se uso
           {
           
                   algoritmos_no_utilizados[cantidad++] = i;           
                                       
           }
           
     }

     if(cantidad > 0)// hubo por lo menos un algoritmo sin utilizar
     {
     
           existe_frente_extra = 1;
           
           cantidad_frentes_local = cantidad_de_frentes + 1;
           
     }
     else// todos los algoritmos se utilizaron, no se crea nuevo frente
     {
          
           existe_frente_extra = 0;

           cantidad_frentes_local = cantidad_de_frentes;
           
     }

     numerador = cantidad_frentes_local;

     for(int i = 1; i <= cantidad_frentes_local; i++)
     {
             
           denominador += i;
     
     }

     probabilidades = new double[cantidad_frentes_local];
     
     for(int i = 0; i < cantidad_frentes_local; i++)
     {
             
          probabilidades[i] = numerador / denominador;//se calcula la prob. de cada frente

          numerador--;

     }
     
     for(int i = 0; i < NTASKS; i++)
     {
     
          valor = rand() / (double)RAND_MAX;
     
          suma = 0.0;

          numero_frente = 0;
          
          
          for(int j = 0; j < cantidad_frentes_local; j++)// se elije el frente
          {
             
               suma += probabilidades[j];
            
               if(valor <= suma)
               {
          
                       numero_frente = j;
                       
                       break;            
            
               }        
     
          }
          
          if(existe_frente_extra == 1 && numero_frente == cantidad_frentes_local - 1)
          {
                              
                estado[i].algoritmo = algoritmos_no_utilizados[rand() % cantidad];
     
          }
          else
          {
              
                contador = 0;
          
                for(int j = 0; j < NTASKS; j++)
                {
          
                     if(frentes[j][numero_frente] == 1)  
                     {
                                             
                           frente_auxiliar[contador++] = j;
                                             
                     }
                
                }
          
                numero_proceso = frente_auxiliar[rand() % contador];
     
                estado[i].algoritmo = estado[numero_proceso].algoritmo;
             
          }
              
     }//end for
            
}



int continuar_esclavos(int tids[])
{

	for(int i = 0; i < NTASKS; i++)
	{

	       if(problema == C101 || problema == RC101)
	       {

	              cp.enviar_conjunto_pareto_vrp_&_sustituir_algoritmo(tids[i], estado[i].algoritmo);

           }
           else
           {

                  cp.enviar_conjunto_pareto_&_sustituir_algoritmo(tids[i], estado[i].algoritmo);
               
           }
           
	}

}




/*
	Retorna verdadero si se cumplio el criterio de parada
	falso en caso contrario
*/
int condicion_parada(int generacion, int start, int end)
{

	if(criterioFin == 1) // tiempo
	{

		if((end - start) / CLOCKS_PER_SEC < tiempoTotal)
		
			return 0;

	}
	else if(generacion < maxIteraciones)
		
			return 0;
	
	return 1;
}




/* 
	Termina la ejecucion de todos los esclavos
*/
void terminar_esclavos(int tids[])
{

	for(int i = 0; i < NTASKS; i++)

		pvm_kill(tids[i]);

}




/* Rutina principal */

int main(int argc, char * args[])
{

	
	int mytid;				// TID del maestro

	int tids[NTASKS];			// array de TID'S de los esclavos

	int numtasks;				// Nmero de esclavos lanzados efectivamente

	int problema;				// indica el cï¿½igo del problema

	int generacion;				// contador de generaciones

	int i, mejor, indicePeor;		// varios: ï¿½dices,auxiliares,etc.

	int cantSols;				// varios,etc.

	long start, end;

	char cad[150];				//Archivo de salida

	char p[50];				//Nombre del archivo del problema a resolver
	
	ConjuntoPareto * cpaux;			// puntero a conjunto pareto




	

	srand(time(NULL));

                                   /* leer parametros de usuario */
//.......................................................................................................
//LECTURA DE ARGUMENTOS EN LA LINEA DE COMANDOS
// ARG 1 -> Problema a resolver. Opciones: kroab, kroac, qap1, qap2, c101, rc101, bombas
// ARG 2 -> Criterio de parada del proceso Master. Opciones: 1 - Tiempo, 2 - Iteraciones
// ARG 3 -> Valor del criterio de parada. Segundos para Tiempo y número de iteraciones para Iteraciones
// ARG 4 -> Nombre del archivo de salida
// ARG 5 -> Método de reemplazo. Opciones: 1 - Elitista, 2 - Probabilístico
//.......................................................................................................

    if(argc >= 6)
    {

		if(stricmp(args[1], "kroab") == 0)
		{

			problema = KROAB;

			prob = new TSP("/home/fuentes/KROAB100.TXT.tsp");

			strcpy(p, "KROAB100.TXT.tsp");

		}
		else if(stricmp(args[1], "kroac") == 0)
		{

			problema = KROAC;

			prob = new TSP("/home/fuentes/kroac100.txt.tsp");

			strcpy(p, "kroac100.txt.tsp");

		}
		else if(stricmp(args[1], "qap1") == 0)
		{

			problema = QAP1;

			prob = new QAP("/home/fuentes/qapUni.75.0.1.qap");

			strcpy(p, "qapUni.75.0.1.qap");

		}
		else if(stricmp(args[1], "qap2") == 0)                                     // ver la e antes de qap2    ATENCION
		{

			problema = QAP2;

			prob = new QAP("/home/fuentes/qapUni.75.p75.1.qap");

			strcpy(p, "qapUni.75.p75.1.qap");

		}
		else if(stricmp(args[1], "c101") == 0)
		{

			problema = C101;

			prob = new VRPTW("/home/fuentes/c101.txt");

			strcpy(p, "c101.txt");

		}
		else if(stricmp(args[1], "rc101") == 0)
		{
		
        		problema = RC101;
		
        		prob = new VRPTW("/home/fuentes/rc101.txt");
		
        		strcpy(p, "rc101.txt");
		
        }
        else if(stricmp(args[1], "bombas" == 0)
        {
             
                problema = BOMBAS1;     
             
                prob = new BOMBAS("/home/fuentes/bombas.txt");
                
                strcpy(p, "bombas.txt");
        
        }
     	else
		{
		
        		printf("\nError: problema no válido. Opciones: kroab, kroac, qap1, qap2, c101, rc101, bombas.\n");
		
        		exit(1);
		
       	}	
		
        
        criterioFin = atoi(args[2]);
		
        if(criterioFin != 1 && criterioFin != 2)
		{
		
        		printf("\nError, Criterio inválido. Opciones: 1 - Tiempo, 2 - Iteraciones.\n");
		
        		exit(1);
		
        }

		if(criterioFin == 1)
		{//tiempo
		
        		tiempoTotal = atoi(args[3]);
		
        		if(!tiempoTotal)
			    {
		
        			printf("\nError, Tiempo inválido. Se espera un entero mayor a 0.\n");
		
        			exit(1);
		
        		}
		
		}	
		else
		{//iteraciones
		
        		maxIteraciones = atoi(args[3]);
		
        		if(!maxIteraciones)
		    	{
		
        			printf("\nError, Nro. de iteraciones inválido. Se espera un entero mayor a 0.\n");
		
        			exit(1);
		
        		}
	
		}
		
        /*****ESTABLECER EL NOMBRE DEL ARCHIVO DE SALIDA******/

		sprintf(cad, "/home/fuentes/%s.TA.%s.pareto", p, args[4]);
        
        
        metodo_reemplazo = atoi(args[5]);
		
        if(metodo_reemplazo != 1 && metodo_reemplazo != 2)
		{
		
        		printf("\nError: Método de reemplazo inválido. Opciones: 1 - Elitista, 2 - Probabilístico.\n");
		
        		exit(1);
		
        }

    }
    else
    {
		
       	printf("\nError: Faltan argumentos\n");
		
    	exit(1);
     
    }






	mytid = pvm_mytid(); // introducir tarea en entorno pvm
    
    numtasks = pvm_spawn(SLAVENAME, (char **)0, 0, "", NTASKS, tids); // levantar tareas esclavas

   
    //-----------------------------------------------------------------------------
  	// Tratamiento del error de lanzamiento de procesos esclavos
    //-----------------------------------------------------------------------------
    
    if( numtasks < NTASKS )
    {
    
       		printf("\n Error lanzando esclavos. Abortar. Códigos de error:\n");
     
       		for( i = numtasks ; i < NTASKS ; i++ ) 
       		{
    
          		printf("TID %d %d\n", i, tids[i]);
    
       		}
       
       		for( i = 0 ; i < numtasks ; i++ )
       		{
       
          		pvm_kill( tids[i] );
       
       		}
       
       		pvm_exit();
       
       		exit(1);	//salir por error
    
    }
    //-----------------------------------------------------------------------------  






    //-----------------------------------------------------------------------------  
    //  Hacer broadcast de los parametros a los esclavos
    //-----------------------------------------------------------------------------  

    int algoritmo = 1; //los 9 algoritmos se identifican con los números del 1 al 9

    for(i = 0; i < NTASKS; i++)
    {
	
    	pvm_initsend(PvmDataDefault);

    	pvm_pkint(&tids[i], 1, 1);

    	pvm_pkint(&algoritmo, 1, 1);

    	pvm_pkint(&problema, 1, 1);
	
    	pvm_send(tids[i], MSGINI);		// enviar a esclavo i con tipo de mensaje de inicializacion
	

    	estado[i].algoritmo = algoritmo;	// actualizar estado del esclavo
	
    	estado[i].solAportadas = 0;		// inicializar acumulador del esclavo
	
	
    	algoritmo++; 				// sgte. en orden secuencial (ver defines)

    }

    //-----------------------------------------------------------------------------  





    /* Ejecutar cuerpo del maestro */

    generacion = 0;
   
    lista_de_conjuntos_pareto = new Conjunto Pareto[NTASKS];
 
    start = clock();
    
    end = start;

	
    while(!condicion_parada(generacion, start, end))
    {
	
    	generacion++;


    	//------------------------------------------------------------------- 
        //    Recepción de los datos de los 9 procesos esclavos
    	//-------------------------------------------------------------------

     	for(i = 0; i < NTASKS ; i++ )
	    {
	
		      cpaux = new ConjuntoPareto(1000);
    		
              if(problema == C101 || problema == RC101)// Instancias del VRP
              {
	    	
			      cpaux -> recibir_conjunto_pareto_vrp(tids[i]);

       		  }
              else                                     // Instancias del TSP o QAP o BOMBAS
		      {
	
			      cpaux -> recibir_conjunto_pareto(tids[i]);

		      }

		      if (lista_de_conjuntos_pareto[i] =! NULL)
              {

                  delete(lista_de_conjuntos_pareto[i]);

    	      }

              lista_de_conjuntos_pareto[i] = cpaux; // se reciben los 9 conjuntos paretos de los 9 procesos

        } //end for

    	//-------------------------------------------------------------------	


     	actualizar_conjunto_pareto_global(problema); // todos los conjuntos pareto actualizan el conjunto pareto global
	
	   	calcular_metricas(problema);// se cuantifica la calidad de cada uno de los 9 frente paretos
	
        clasificar_procesos();// de acuerdo a las metricas del con. par. de cada proceso, se los distribuyen en frentes par. de forma a clasificarlos en mejores y peores
		
    	realizar_reemplazo();	
	
    	continuar_esclavos(problema, tids);	// Enviar mensajes de continuacion a esclavos


      	end = clock();

    }//end while
 

    //------------------------------------------------------
    //  Mostrar resultados, lista el conjunto pareto global
    //------------------------------------------------------

    if(problema == C101 || problema == RC101)
    {	
	
        cp.listarSolucionesVRP(prob, cad);
    
    }    
    else
    {	
    
       	cp.listarSoluciones(prob, cad);
    
    }
    
    //--------------------------------------------
    
    
    terminar_esclavos(tids); // Terminar ejecucion
    
    pvm_exit();
    
    return 0;

}
