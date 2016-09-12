#include "/usr/share/pvm3/include/pvm3.h"
#include <iostream>
	using namespace std;
#include <stdio.h>


#define max(i, j) (i >= j) ? i : j


#include <stdlib.h>

#include <math.h>

#include <time.h>

#include <string.h>

#include <ctype.h>                   


#define SLAVENAME "slave"

#define CBIANT	  1

#define CBIMC	  2

#define CCOMP		     3

#define CMAS		     4

#define CM3AS		     5

#define CMOACS		     6

#define CMOAQ		     7

#define CMOA		     8

#define CPACO		     9

#define KROAB		     10

#define KROAC		     11

#define QAP1		     12

#define QAP2		     13

#define C101		     14

#define RC101		     15

#define BOMBAS1          16

#define NTASKS		     9

#define MSGINI		     20

#define MSGDATA		     30

#define MSGCONT		     40

#define CONJUNTO_PARETO      50

#define SUSTITUIR_ALGORITMO  60

#define TERMINAR	     1000

#define CANTIDAD_METRICAS    5

#define CANTIDAD_BOMBAS    5


#define TARIFA_REDUCIDA 395 //GS. POR KW/H

#define TARIFA_ELEVADA 790


#define ALTURA_MAXIMA 7 // METROS

#define ALTURA_MINIMA 1 // METROS

#define ALTURA_INICIAL 3 // METROS

#define AREA_RESERVORIO 2600 //METROS CUADRADOS

#define VOLUMEN 15600 //METROS CUBICOS
#define TIEMPO_COMPLETO 200
#define TIEMPO_REDUCIDO 300
// #define CAUDAD_TOTAL_DEMANDADO 54788 //M CUBICOS POR DIA


#include "clases.cc"                   

#include "Problema.cc"               

#include "clases2.cc"               



/* Comparador de cadenas no sensible a maysculas ni minsculas,
//   retorna: -1 si cad es mayor que cad2
			 0 si son iguales
			 1 si cad2 es mayor que cad */
int stricmp(char cad[], char cad2[])
{

	int comp;

	int i;

	char c2[strlen(cad2)];

	strcpy(c2, cad2);

	
	for(i = 0; i < strlen(cad); i++)

		cad[i] = tolower(cad[i]);


	for(i = 0; i < strlen(c2); i++)

		c2[i] = tolower(c2[i]);


	comp = strcmp(cad,c2);



	return comp;

}

/* Tipos de definidos */

typedef struct estado
{

	int algoritmo;		// Algoritmo asignado
	
	int solAportadas;	// Nro. de soluciones aportadas al Frente Pareto

} state;




/* Variables globales */   

double metricas[NTASKS][CANTIDAD_METRICAS];// contiene las metricas correspondientes a cada conjunto parento proporcionado por los esclavos

int frentes[NTASKS][NTASKS];

int cantidad_de_frentes = 0;

int cantidad_procesos_seleccionados = 0;

int frente_actual[NTASKS];

int procesos_seleccionados[NTASKS];    // registra los procesos ya asignados a algunos de los frentes pareto


     
ConjuntoPareto cp(500);  		// Conjunto Pareto Global

ConjuntoPareto * cp_new = NULL;

ConjuntoPareto ** lista_de_conjuntos_pareto;  // contenedor de todos los conjuntos pareto de los esclavos

state estado[NTASKS];	        	// Almacena el estado de un esclavo

int algoritmos_anterior[NTASKS];

int criterioFin;			// Indica el criterio de parada del proceso Master: por tiempo (1), por iteraciones (2)

int tiempoTotal;			// Guarda el tiempo de ejecucion del proceso Master

int maxIteraciones;			// Guarda la maxima cantidad de iteraciones a ejecutar el proceso Master

int metodo_reemplazo;       // elista o probabilistico del proceso Master

Problem * prob;				// Problema a resolver






/*	
        Inserta en cp global las soluciones cpaux si es no dominada
	Devuelve la cantidad de soluciones que entraron al cp global
*/


int sonIguales(double num1, double num2)
{

       if(fabs(num1-num2) < 0.00001)
           return 1;
       else
           return 0;

}
/*
void actualizar_frente_global(ConjuntoPareto * cpaux)
{

	for(int i = 0; i < cpaux -> getSize(); i++)
	{

		if (cp.esNoDominado(* cpaux -> getSolucion(i), prob)==1)      // true si la solucion se incorpora al conjunto pareto global
		{
        		cp.eliminarDominados(* cpaux -> getSolucion(i), prob);        //elimina las soluciones del conjunto pareto global dominados
                  
		        cp.insertar_en_el_conjunto_pareto_global(* cpaux -> getSolucion(i));

		}

	}
	
}
*/

//------------------------------------------------------------------------------------------
// Actualizacion del conjunto pareto global con los con. pare. de los 9 procesos
//------------------------------------------------------------------------------------------
/*
void actualizar_conjunto_pareto_global(int problema)
{

		if(problema == C101 || problema == RC101)		// Actualizar frente global 
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
		
}
*/
//------------------------------------------------------------------------------------------

void actualizar_frente_global_NEW(ConjuntoPareto * cpaux)
{

	for(int i = 0; i < cpaux -> getSize(); i++)
	{

		if (cp_new -> esNoDominado(* cpaux -> getSolucion(i), prob)==1)      // true si la solucion se incorpora al conjunto pareto global
		{

        		cp_new -> eliminarDominados(* cpaux -> getSolucion(i), prob);  //elimina las soluciones del conjunto pareto global dominados
                  
		        cp_new -> insertar_en_el_conjunto_pareto_global(* cpaux -> getSolucion(i));

		}

	}
	
}

void actualizar_frente_global_NEW_VRP(ConjuntoPareto * cpaux)
{


	for(int i = 0; i < cpaux -> getSize(); i++)
	{

		if (cp_new -> esNoDominado(* cpaux -> getSolucionVRP(i), prob)==1)      // true si la solucion se incorpora al conjunto pareto global
		{

        		cp_new -> eliminarDominados(* cpaux -> getSolucionVRP(i), prob);  //elimina las soluciones del conjunto pareto global dominados
                  
		        cp_new -> insertar_en_el_conjunto_pareto_global(* cpaux -> getSolucionVRP(i));

		}

	}

}

void actualizar_conjunto_pareto_global_NEW(int problema)
{

		delete cp_new;

                cp_new = new ConjuntoPareto(1000);
          	
                if(problema == C101 || problema == RC101)		/* Actualizar frente global */
		{
		
			for (int i = 0; i < NTASKS; i++)
			{
				
				actualizar_frente_global_NEW_VRP(lista_de_conjuntos_pareto[i]);
				
			}
		
		}
    		else
		{		
			
			for (int i = 0; i < NTASKS; i++)
			{

				actualizar_frente_global_NEW(lista_de_conjuntos_pareto[i]);
			
			}
			
		}

}

//------------------------------------------------------------------------------------------
// calculo de sigma para la metrica 2 para vrp
//------------------------------------------------------------------------------------------

double calcular_sigma_vrp()// tsp, qap
{

    double x1, x2, menor_objetivo_1 = 0.0, menor_objetivo_2 = 0.0, mayor_distancia1, mayor_distancia2;

    int punto_extremo_1, punto_extremo_2;
    
    


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

    mayor_distancia1 = fabs(prob -> funcion_obj_1(* cp.getSolucionVRP(punto_extremo_1)) - prob -> funcion_obj_1(* cp.getSolucionVRP(punto_extremo_2)));

    mayor_distancia2 = fabs(prob -> funcion_obj_2(* cp.getSolucionVRP(punto_extremo_1)) - prob -> funcion_obj_2(* cp.getSolucionVRP(punto_extremo_2)));

    return 0.1 * sqrt(pow(mayor_distancia1, 2) + pow(mayor_distancia2, 2));

 }


//------------------------------------------------------------------------------------------
// calculo de sigma para la metrica 2 para bombas
//------------------------------------------------------------------------------------------

 double calcular_sigma_bombas()
 {

    double w1, x1, y1, z1, w2, x2, y2, z2, distancia, mayor_distancia1 = 0.0, mayor_distancia2 = 0.0, mayor_distancia3 = 0.0, mayor_distancia4 = 0.0;

   
     BOMBAS * problema = (BOMBAS *) prob;	
 	
     for (int i = 0; i < cp.getSize() - 1; i++) 
    {
 
           w1 = problema -> funcion_obj_1(* cp.getSolucion(i));
               
           x1 = problema -> funcion_obj_2(* cp.getSolucion(i));

           y1 = problema -> funcion_obj_3(* cp.getSolucion(i));
               
           z1 = problema -> funcion_obj_4(* cp.getSolucion(i));
           
        
           for (int j = i + 1; j < cp.getSize(); j++)
           {

                   
                w2 = problema -> funcion_obj_1(* cp.getSolucion(j));
              
                x2 = problema -> funcion_obj_2(* cp.getSolucion(j));

                y2 = problema -> funcion_obj_3(* cp.getSolucion(j));
               
                z2 = problema -> funcion_obj_4(* cp.getSolucion(j));

        		distancia = fabs(w2 - w1);

		    	if (distancia > mayor_distancia1)
			    {

  				     mayor_distancia1 = distancia;

			    }

                distancia = fabs(x2 - x1);

		     	if (distancia > mayor_distancia2)
			    {

				     mayor_distancia2 = distancia;

			    }

        		distancia = fabs(y2 - y1);

		    	if (distancia > mayor_distancia3)
			    {

  				     mayor_distancia3 = distancia;

			    }

                distancia = fabs(z2 - z1);

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

    int punto_extremo_1, punto_extremo_2;
    
    


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

    mayor_distancia1 = fabs(prob -> funcion_obj_1(* cp.getSolucion(punto_extremo_1)) - prob -> funcion_obj_1(* cp.getSolucion(punto_extremo_2)));

    mayor_distancia2 = fabs(prob -> funcion_obj_2(* cp.getSolucion(punto_extremo_1)) - prob -> funcion_obj_2(* cp.getSolucion(punto_extremo_2)));

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
		
       		//sigma = calcular_sigma_vrp();    
		
		for(int i = 0; i < NTASKS; i++)
        	{

    	       		metricas[i][0] =  0;//lista_de_conjuntos_pareto[i] -> get_metrica1_vrp(prob, * cp_new);//distancia

    	       		metricas[i][1] =  0;//lista_de_conjuntos_pareto[i] -> get_metrica2_vrp(prob, sigma);//distribucion

    	       		metricas[i][2] =  lista_de_conjuntos_pareto[i] -> get_metrica3_vrp(prob);//extension
 
    	       		metricas[i][3] =  0;//lista_de_conjuntos_pareto[i] -> get_metrica4_vrp(prob, * cp_new);//error
 
    	       		metricas[i][4] =  lista_de_conjuntos_pareto[i] -> get_metrica5_vrp(prob, * cp_new);//cantidad de soluciones aportadas

		}
	
	}
 	else if(problema == BOMBAS1)//bombas
	{
		
     		  // sigma = calcular_sigma_bombas();    
		
      		  for(int i = 0; i < NTASKS; i++)
      		  {

    	       		metricas[i][0] =  lista_de_conjuntos_pareto[i] -> get_metrica1_bombas(prob, * cp_new);//distancia

    	       		metricas[i][1] =  lista_de_conjuntos_pareto[i] -> get_metrica2_bombas(prob, sigma);//distribucion

    	       		metricas[i][2] =  lista_de_conjuntos_pareto[i] -> get_metrica3_bombas(prob);//extension
 
    	       		metricas[i][3] =  lista_de_conjuntos_pareto[i] -> get_metrica4_bombas(prob, * cp_new);//error
 
    	       		metricas[i][4] =  lista_de_conjuntos_pareto[i] -> get_metrica5_bombas(prob, * cp_new);//cantidad de soluciones aportadas

		}
	
	}
	else
	{	
//cout << "MATRIZ DE METRICA" << endl;                
                // sigma = calcular_sigma();    

		for(int i = 0; i < NTASKS; i++)
                {

	   	           metricas[i][0] = 0; //lista_de_conjuntos_pareto[i] -> get_metrica1(prob, * cp_new);
//cout << " " << metricas[i][0];
    			   metricas[i][1] = 0; //lista_de_conjuntos_pareto[i] -> get_metrica2(prob, sigma);
    			   
//cout << " " << metricas[i][1];
            		   metricas[i][2] = lista_de_conjuntos_pareto[i] -> get_metrica3(prob);
 
//cout << " " << metricas[i][2];
    			   metricas[i][3] = 0; //lista_de_conjuntos_pareto[i] -> get_metrica4(prob, * cp_new);
 
//cout << " " << metricas[i][3];
      			   metricas[i][4] = lista_de_conjuntos_pareto[i] -> get_metrica5(prob, * cp_new);

//cout << " " << metricas[i][4] << endl;

 		}

	}

}

void unir_frenteNEW_y_frenteGlobal(int problema)
{

	if(problema == C101 || problema == RC101)// vrp
	{

	       cp.incorporar_frente_vrp(cp_new, prob);
	
        }
	else
	{

	       cp.incorporar_frente(cp_new, prob);

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
          
                  if(((metricas[j][0] < metricas[i][0] || sonIguales(metricas[j][0], metricas[i][0])) && (metricas[j][1] > metricas[i][1] || sonIguales(metricas[j][1], metricas[i][1])) && (metricas[j][2] > metricas[i][2] || sonIguales(metricas[j][2], metricas[i][2])) && (metricas[j][3] < metricas[i][3] || sonIguales(metricas[j][3], metricas[i][3])) && (metricas[j][4] > metricas[i][4] || sonIguales(metricas[j][4], metricas[i][4]))) && (metricas[j][0] < metricas[i][0] || metricas[j][1] > metricas[i][1] || metricas[j][2] > metricas[i][2] || metricas[j][3] < metricas[i][3] || metricas[j][4] > metricas[i][4]))                    
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
          
                  if(((metricas[i][0] < metricas[j][0] || sonIguales(metricas[i][0], metricas[j][0])) && (metricas[i][1] > metricas[j][1] || sonIguales(metricas[i][1], metricas[j][1])) && (metricas[i][2] > metricas[j][2] || sonIguales(metricas[i][2], metricas[j][2])) && (metricas[i][3] < metricas[j][3] || sonIguales(metricas[i][3], metricas[j][3])) && (metricas[i][4] > metricas[j][4] || sonIguales(metricas[i][4], metricas[j][4]))) && (metricas[i][0] < metricas[j][0] || metricas[i][1] > metricas[j][1] || metricas[i][2] > metricas[j][2] || metricas[i][3] < metricas[j][3] || metricas[i][4] > metricas[j][4]))                    
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
     
     double valor, suma, probabilidad_eleccion_primer_frente;
     
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
             
          probabilidades[i] = numerador / (double)denominador;//se calcula la prob. de cada frente

          numerador--;

     }
     
     for(int i = 0; i < NTASKS; i++)
     {
     
          valor = rand() / (double)RAND_MAX;
     
          suma = 0.0;

          numero_frente = 0;

          probabilidad_eleccion_primer_frente = 0.5;
          
	  if(valor <= probabilidad_eleccion_primer_frente)
          {
		
                valor = rand() / (double)RAND_MAX;
          	
                for(int j = 0; j < cantidad_frentes_local; j++)// se elije el frente
	        {
             
        	       suma += probabilidades[j];
            
        	       if(valor <= suma)
	               {
          
        	               numero_frente = j;
                       
        	               break;            
            
        	       }        
     
        	}
          
	  }

          if(existe_frente_extra == 1 && numero_frente == cantidad_frentes_local - 1)
          {
                              
                estado[i].algoritmo = algoritmos_no_utilizados[rand() % cantidad];
               // cout << " numero frente : " << numero_frente << endl;
               // cout << " i : " << i << "algoritmo : " << estado[i].algoritmo << endl;     
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
             //cout << " i : " << i << " proceso No. "<< numero_proceso << endl;
          }
              
     }//end for

     delete[] probabilidades;     
       
}



void realizar_reemplazo()
{

     if(metodo_reemplazo == 1)
     {
     
          reemplazo_elitista();
     
     }     
     else if(metodo_reemplazo == 2) 
     {
     
          reemplazo_probabilistico();
          
     }
     else

          cout << "METODO DE REEMPLAZO NO SELECCIONADO" << endl;
     
}


/*
int continuar_esclavos(int problema, int tids[])
{

	for(int i = 0; i < NTASKS; i++)
	{

	       if(problema == C101 || problema == RC101)
	       {

	              cp.enviar_conjunto_pareto_vrp_y_sustituir_algoritmo(tids[i], estado[i].algoritmo);

           }
           else
           {
                  //cout << "proceso : " << i << " reemp. con alg no.: " << estado[i].algoritmo << endl;
                  cp.enviar_conjunto_pareto_y_sustituir_algoritmo(tids[i], estado[i].algoritmo);
               
           }
           
	}

}
*/

int continuar_esclavos_sin_sinergia(int tids[])
{
cout << "sin sinergia" << endl; 
	for(int i=0;i<NTASKS;i++)
	{
		pvm_initsend(PvmDataDefault);
		if(algoritmos_anterior[i] == estado[i].algoritmo)
			pvm_send(tids[i],MSGCONT);
		else
		{
			pvm_pkint(&estado[i].algoritmo,1,1);
			pvm_send(tids[i],MSGCONT*20); // enviar con un tag diferente a MSGCONT
		}
	}
}

int continuar_esclavos_con_sinergia(double q, int problema, int tids[])
{
cout << "con sinergia" << endl; 
	for(int i = 0; i < NTASKS; i++)
	{

	       if(problema == C101 || problema == RC101)
	       {

			if(algoritmos_anterior[i] == estado[i].algoritmo)
			{
			
			      cp_new -> enviar_conjunto_pareto_vrp_a_esclavo(q, tids[i]);//cout << "clases2 157" << endl; 

        	        }
			else
			{

        	              cp_new -> enviar_conjunto_pareto_vrp_y_sustituir_algoritmo(q, tids[i], estado[i].algoritmo);//cout << "clases2 161" << endl; 

			}
			
               }
               else
               {
//cout << "clases2 154" << endl; 
			if(algoritmos_anterior[i] == estado[i].algoritmo)
			{
			
			      cp_new -> enviar_conjunto_pareto_a_esclavo(q, tids[i]);//cout << "clases2 157" << endl; 

        	        }
			else
			{

        	              cp_new -> enviar_conjunto_pareto_y_sustituir_algoritmo(q, tids[i], estado[i].algoritmo);//cout << "clases2 161" << endl; 

			}

                      //cout << "proceso : " << i << " reemp. con alg no.: " << estado[i].algoritmo << endl;
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

		if(((end - start) / (double)CLOCKS_PER_SEC) < tiempoTotal)
		
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

	double q = 0;



	

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

			prob = new TSP("/home/test/KROAB100.TXT.tsp");

			strcpy(p, "KROAB100.TXT.tsp");

		}
		else if(stricmp(args[1], "kroac") == 0)
		{

			problema = KROAC;

			prob = new TSP("/home/test/kroac100.txt.tsp");

			strcpy(p, "kroac100.txt.tsp");

		}
		else if(stricmp(args[1], "qap1") == 0)
		{

			problema = QAP1;

			prob = new QAP("/home/test/qapUni.75.0.1.qap");

			strcpy(p, "qapUni.75.0.1.qap");

		}
		else if(stricmp(args[1], "qap2") == 0)                                     // ver la e antes de qap2    ATENCION
		{

			problema = QAP2;

			prob = new QAP("/home/test/qapUni.75.p75.1.qap");

			strcpy(p, "qapUni.75.p75.1.qap");

		}
		else if(stricmp(args[1], "c101") == 0)
		{

			problema = C101;

			prob = new VRPTW("/home/test/c101.txt");

			strcpy(p, "c101.txt");

		}
		else if(stricmp(args[1], "rc101") == 0)
		{
		
        		problema = RC101;
		
        		prob = new VRPTW("/home/test/rc101.txt");
		
        		strcpy(p, "rc101.txt");
		
        }
        else if(stricmp(args[1], "bombas") == 0)
        {
             
                problema = BOMBAS1;     
             
                prob = new BOMBAS("/home/test/bombas.txt");
                
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

		sprintf(cad, "/home/test/%s.TA.%s.pareto", p, args[4]);
        
    
        metodo_reemplazo = atoi(args[5]);

if(metodo_reemplazo == 1)
    cout << "ELISTISTA-------------------------------------------" << endl;		
else if(metodo_reemplazo == 2)
    cout << "PROBABILISTICO-------------------------------------------" << endl;		
     
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

//cout << "hola" << endl;    
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
	
	algoritmos_anterior[i] = 0;

    	algoritmo++; 				// sgte. en orden secuencial (ver defines)

    }
//cout << "hola1" << endl;    
    //-----------------------------------------------------------------------------  


    /* Ejecutar cuerpo del maestro */

    generacion = 0;
   
		    // lista_de_conjuntos_pareto = new ConjuntoPareto * [NTASKS];
    
    lista_de_conjuntos_pareto = (ConjuntoPareto **)malloc(sizeof(ConjuntoPareto *) * NTASKS);
  
    for(int i = 0; i < NTASKS; i++)
    {
 
        lista_de_conjuntos_pareto[i] = NULL;

    }
 
    start = clock();
    
    end = start;

    FILE * f = fopen(cad, "w");
   
   // FILE * faux = fopen("/home/test/hora", "w");

    FILE * falg = fopen("/home/test/alg", "w");


    while(!condicion_parada(generacion, start, end))
    {
	
    	generacion++;

	fprintf(falg, "INICIO DE LA ITERACION : %d\n", generacion);

	for(int i = 0; i < NTASKS; i++)
	{
	
        	fprintf(falg, "%d\n", estado[i].algoritmo);	

	}

    	//------------------------------------------------------------------- 
        //    Recepción de los datos de los 9 procesos esclavos
    	//-------------------------------------------------------------------
//cout << "hola4" << endl;    
     	for(int i = 0; i < NTASKS ; i++ )
        {
	
	      cpaux = new ConjuntoPareto(1000);
    		
              if(problema == C101 || problema == RC101)// Instancias del VRP
              {
	    	
  		   cpaux -> recibir_conjunto_pareto_vrp(tids[i]);
//cout << "hola2" << endl;	
              }
              else // Instancias del TSP o QAP o BOMBAS
	      {

 		   cpaux -> recibir_conjunto_pareto(tids[i]);
                   //fprintf(f, "Recibe el master Frente del algoritmo : %d\n", i + 1 );
                   //cpaux -> listarSoluciones(prob, f);
     		   //cout << "llego" << endl;
              }

              delete lista_de_conjuntos_pareto[i];
            
              lista_de_conjuntos_pareto[i] = cpaux; // se reciben los 9 conjuntos paretos de los 9 procesos
        
        } //end for
//cout << "hola3" << endl;	
    	//-------------------------------------------------------------------	

	actualizar_conjunto_pareto_global_NEW(problema);// todos los conjuntos pareto actualizan el conjunto pareto global
       
//cout << "actualizo" << endl;
	calcular_metricas(problema);// se cuantifica la calidad de cada uno de los 9 frente paretos
//cout << "paso actualizo" << endl;
        unir_frenteNEW_y_frenteGlobal(problema);
//cout << "metricas" << endl;
        clasificar_procesos(); // de acuerdo a las metricas del con. par. de cada proceso, se los distribuyen en frentes par. de forma a clasificarlos en mejores y peores
//cout << "clasificar" << endl;
	for(int i = 0; i < NTASKS; i++)
	{
	
	      	algoritmos_anterior[i] = estado[i].algoritmo;	

	}
//cout << "FRENTE" << endl;
/*        for(int i = 0; i < NTASKS; i++)
	{
	
		for(int j = 0; j < NTASKS; j++)
		{
	
	 		cout << frentes[i][j] << " " ;

		}
		cout << "" << endl;
	}
*/
    	realizar_reemplazo();	

//cout << "reemplazo" << endl;
        q = q + 1/(double)maxIteraciones;

cout << "valor de q : " << q << endl;
	
        continuar_esclavos_con_sinergia(q, problema, tids);	// Enviar mensajes de continuacion a esclavos
        	
    	//continuar_esclavos(problema, tids);	// Enviar mensajes de continuacion a esclavos

//cout << "continuar esclavos " << endl;
        //------------------------------------------------------
        //  Mostrar resultados, lista el conjunto pareto global
        //------------------------------------------------------
        

      

        end = clock();
        
        if(generacion % 1 == 0)
        {
     
            //fprintf(faux, "%lf\n", clock() / (double)CLOCKS_PER_SEC); 
        
            //fprintf(f, "Frente pareto global iteracion : %d\n", generacion);
            
            if(problema == C101 || problema == RC101)
            {	
	
                 cp.listarSolucionesVRP(prob, f);
    
            }    
            else
            {	
    
                 cp.listarSoluciones(prob, f);
    
            }
       
        }
        //--------------------------------------------

    
        cout << "termino de la  generacion //////////////: " << generacion << endl;
    
    }//end while
   
    fclose(f);
    fclose(faux);
    fclose(falg);
   cout << "fin master(HOST 6)" << endl; 

    terminar_esclavos(tids); // Terminar ejecucion
    
    
    for(int i = 0; i < NTASKS; i++)
    {
 
        delete lista_de_conjuntos_pareto[i]; 

    }
  
    							//delete[] lista_de_conjuntos_pareto; lista_de_conjuntos_pareto = NULL;

    free(lista_de_conjuntos_pareto); 
 
    delete prob; 
   
    pvm_exit();
    

    return 0;

}
