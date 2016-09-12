
#include "/usr/share/pvm3/include/pvm3.h"
#include <iostream>
#include <exception>
   using namespace std;

#include <stdio.h>

/*
void debug(char s[])
{
	
    FILE * f = fopen("/usr/share/pvm3/bin/LINUXI386/debug.txt", "a");
	
    fprintf(f, "%s\n", s);
	
    fclose(f);

}
*/

#define max(i, j) (i >= j) ? i : j


#include <stdlib.h>

#include <math.h>

#include <time.h>

#include <string.h>

#include <ctype.h>                   





#define SLAVENAME "slave"

#define CBIANT	  1

#define CBIMC	  2

#define CCOMP	  3

#define CMAS	  4

#define CM3AS	  5

#define CMOACS	  6

#define CMOAQ	  7

#define CMOA	  8

#define CPACO	  9

#define KROAB	  10

#define KROAC	  11

#define QAP1	  12

#define QAP2	  13

#define C101	  14

#define RC101	  15

#define BOMBAS1   16

#define NTASKS	  9

#define MSGINI	  20

#define MSGDATA	  30

#define MSGCONT	 40

#define CONJUNTO_PARETO  50

#define SUSTITUIR_ALGORITMO  60

#define TERMINAR	1000

#define CANTIDAD_METRICAS    5

#define CANTIDAD_BOMBAS    5

//#define CANTIDAD_INTERVALOS  24   

#define TARIFA_REDUCIDA 395 //GS. POR KW/H

#define TARIFA_ELEVADA 790

//#define CANTIDAD_COMBINACIONES 32

#define ALTURA_MAXIMA 7 // METROS

#define ALTURA_MINIMA 1 // METROS

#define ALTURA_INICIAL 3 // METROS

#define AREA_RESERVORIO 2600 //METROS CUADRADOS

#define VOLUMEN 15600 //METROS CUBICOS

#define TIEMPO_COMPLETO 200

#define TIEMPO_REDUCIDO 300

//#define CAUDAD_TOTAL_DEMANDADO 54788 //M CUBICOS POR DIA


void set_outfile(char out[], char prob[], char inst[], char alg[])
{

	strcpy(out, prob);

	strcat(out, ".");

	strcat(out, inst);

	strcat(out, alg);

}



/* Comparador de cadenas no sensible a maysculas ni minsculas,
   retorna: -1 si cad es mayor que cad2
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



#include "clases.cc"                   

#include "Problema.cc"                   

#include "clases2.cc"                   

#include "construir_solucion.cc"

#include "algoritmos.h"

char outFile[255];







int main(int argc, char *args[])
{

    	int mytid;					// TID del esclavo

    	int master;					// TID del maestro

    	int problema;					// indica el codigo del problema 

    	char parametros[100];				// indica el archivo de parametros a utilizar 

    	int generacion = 0;				// contador de generaciones

    	int bufid, type, bytes, source;			// varios.

    	int algoritmo;	       				// algoritmo a ejecutar

        double q;
  
        MOACO * algAnterior;	
    	
        MOACO * alg;					// instancia del algoritmo

    	Problem * prob;					// instancia del problema a resolver

        ConjuntoPareto * conjuntoAuxiliar;
	       

	
    	mytid = pvm_mytid();				// Registrarse en PVM
	
    	master = pvm_parent();				// Guardar TID del master

	
        //----------------------------------------------------------------------
	    //  Recepción de los datos del proceso Master
	    //----------------------------------------------------------------------

    	pvm_recv(master, MSGINI); 			// Recibir datos del master
	
    	pvm_upkint(&mytid, 1, 1);
	
    	pvm_upkint(&algoritmo, 1, 1);

    	pvm_upkint(&problema, 1, 1);
	
        //----------------------------------------------------------------------    
    	
//cout << "S1" << endl;	

        if(problema == KROAB)        // instanciar el problema y el algoritmo a utilizar
        {
		
	        prob = new TSP("/home/test/KROAB100.TXT.tsp");

         	strcpy(parametros, "/home/test/parametros_tsp.txt");

    	}
     	else if(problema == KROAC)
      	{

       		prob = new TSP("/home/test/kroac100.txt.tsp");

         	strcpy(parametros, "/home/test/parametros_tsp.txt");

       	}
        else if(problema == QAP1)
        {

        	prob = new QAP("/home/test/qapUni.75.0.1.qap");

         	strcpy(parametros, "/home/test/parametros_qap.txt");

       	}
        else if(problema == QAP2)
        {

        	prob = new QAP("/home/test/qapUni.75.p75.1.qap");

         	strcpy(parametros, "/home/test/parametros_qap.txt");

       	}
        else if(problema == C101)
        {

        	prob = new VRPTW("/home/test/c101.txt");

         	strcpy(parametros, "/home/test/parametros_vrp.txt");

       	}
        else if(problema == RC101)
        {

        	prob = new VRPTW("/home/test/rc101.txt");

         	strcpy(parametros, "/home/test/parametros_vrp.txt");
		
    	}
     	else if(problema == BOMBAS1)
      	{

	    	prob = new BOMBAS("/home/test/bombas.txt");

      		strcpy(parametros, "/home/test/parametros_bombas.txt");

        }

        
        if(algoritmo == CBIANT)// LOS PROBLEMAS SE ENCUENTRAN INSTANCIADOS. INSTANCIAR LOS ALGORITMOS
        {

	    	alg = new BicriterionAnt(prob, parametros);
      	
        }
       	else if(algoritmo == CBIMC)
        {

        	alg = new BicriterionMC(prob, parametros);

        }
        else if(algoritmo == CCOMP)
        {
		
        	alg = new CompeteAnts(prob, parametros);

	    }
     	else if(algoritmo == CM3AS)
      	{

       		alg = new M3AS(prob, parametros);

        }
        else if(algoritmo == CMOACS)
        {

        	alg = new MOACS(prob, parametros);

        }
        else if(algoritmo == CMOAQ)
        {

        	alg = new MOAQ(prob, parametros);

        }
        else if(algoritmo == CMOA)
        {

        	alg = new MOA(prob, parametros);

        }
        else if(algoritmo == CPACO)
        {

        	alg = new PACO(prob, parametros);

        }
        else if(algoritmo == CMAS)
        {
		
        	alg = new MAS(prob, parametros);
	
    	}

//cout << "S2" << endl;	
//FILE * faux = fopen("/home/test/frenteesclavo", "w");
        conjuntoAuxiliar = new ConjuntoPareto(1000);
	
        while(1)
   	{// es infinito porque para terminar la ejecucion actualmente el master invoca a pvm_kill
//cout << "F1" << endl;	 
		generacion++;

		// Ejecutar el algoritmo indicado
		if (problema == KROAB || problema == KROAC)
 		{
			alg -> ejecutarTSP(TIEMPO_COMPLETO);
                }
		else if(problema == QAP1 || problema == QAP2)
                {
			alg -> ejecutarQAP(TIEMPO_COMPLETO);
                }
		else if(problema == C101 || problema == RC101)
                {
//cout << "Tamano : " << alg -> pareto -> getSize() << endl;
//cout << "antes completo" << endl;	 
		  	alg -> ejecutarVRP(TIEMPO_COMPLETO);
//cout << "despues completo" << endl;	 
                }
                else if(problema == BOMBAS1)
		{
        	        alg -> ejecutarBOMBAS();
        	}
                // fprintf(faux, "Envia el esclavo Frente del algoritmo : %d\n", algoritmo);
                // cout << "Envia el esclavo Frente del algoritmo" << algoritmo << endl; 
                
                // alg -> pareto -> listar(prob);
		
                // enviar conjunto pareto al master
		
                if(problema == C101 || problema == RC101)
		{

			alg -> pareto -> enviar_soluciones_nuevas_vrp_al_master(master);
//cout << "despues enviar soluciones nuevas al master" << endl;	
			alg -> pareto -> marcar_frente_vrp_como_old();
//cout << "S5" << endl;	

		}
		else
		{

                     	alg -> pareto -> enviar_soluciones_nuevas_al_master(master);

			alg -> pareto -> marcar_frente_como_old();

		}
		
		//cout << "termino de ejecutar el algoritmo " << algoritmo << endl;

                // delete alg;
		
           	bufid=pvm_nrecv(master,-1);                
//cout << "S6" << endl;		
                while(bufid == 0)
                {
		        //cout << "continua" << endl; 
                 	if (problema == KROAB || problema == KROAC)
			{

 				alg -> ejecutarTSP(TIEMPO_REDUCIDO);

			}
			else if(problema == QAP1 || problema == QAP2)
			{

				alg -> ejecutarQAP(TIEMPO_REDUCIDO);

			}
			else if(problema == C101 || problema == RC101)
			{
//cout << "antes reducido" << endl;	
				alg -> ejecutarVRP(TIEMPO_REDUCIDO);
//cout << "despues reducido" << endl;	

			}

        	        bufid = pvm_nrecv(master, -1);

		}//cout << "S10" << endl;	 
		algAnterior = alg;

		pvm_bufinfo(bufid, &bytes, &type, &source);

		if(type == CONJUNTO_PARETO || type == SUSTITUIR_ALGORITMO)
		{
                      
                        pvm_upkdouble(&q, 1, 1);

                }
//cout << "S11" << endl;	 
		if (type == MSGCONT*20 || type == SUSTITUIR_ALGORITMO)//Los dos tipos que sustituyen algoritmos
		{
                        
			pvm_upkint(&algoritmo, 1, 1);
	
			if(algoritmo == CBIANT)
			{

				alg = new BicriterionAnt(prob, parametros);

			}
			else if(algoritmo == CBIMC)
			{

				alg = new BicriterionMC(prob, parametros);

			}
			else if(algoritmo == CCOMP)
			{

				alg = new CompeteAnts(prob, parametros);

			}
			else if(algoritmo == CM3AS)
			{

				alg = new M3AS(prob, parametros);

			}
			else if(algoritmo == CMOACS)
			{

				alg = new MOACS(prob, parametros);

			}
			else if(algoritmo == CMOAQ)
			{

				alg = new MOAQ(prob, parametros);

			}
			else if(algoritmo == CMOA)
			{

				alg = new MOA(prob, parametros);

			}
			else if(algoritmo == CPACO)
			{

				alg = new PACO(prob, parametros);

			}
			else if(algoritmo == CMAS)
			{

				alg = new MAS(prob, parametros);

			}

		}
//cout << "S12" << endl;	 
                if(type == MSGCONT*20)
	        {

			alg -> reemplazar_frente(algAnterior);

        	       	delete algAnterior;
//cout << "S14" << endl;	 
        	}
	        else if(type == SUSTITUIR_ALGORITMO)
        	{
		
			if(problema == C101 || problema == RC101)
			{

	                	alg -> pareto -> desempaquetar_y_copiar_conjunto_pareto_vrp();
			
	                        alg -> pareto -> marcar_frente_vrp_como_old();
	                
	                        alg -> pareto -> incorporar_frente_vrp(algAnterior -> pareto, prob);

	                        alg -> pareto -> seleccionar_soluciones_del_frente_vrp(q);
			
			}
			else
			{

	                	alg -> pareto -> desempaquetar_y_copiar_conjunto_pareto();
			
	                        alg -> pareto -> marcar_frente_como_old();
	                
	                        alg -> pareto -> incorporar_frente(algAnterior -> pareto, prob);

	                        alg -> pareto -> seleccionar_soluciones_del_frente(q);
                     	
			}

        	        delete algAnterior;
//cout << "S15" << endl;	 
        	}
	        else if(type == CONJUNTO_PARETO)
        	{

			if(problema == C101 || problema == RC101)
			{

				conjuntoAuxiliar -> desempaquetar_y_copiar_conjunto_pareto_vrp();

				conjuntoAuxiliar -> marcar_frente_vrp_como_old();

		   		alg -> pareto -> incorporar_frente_vrp(conjuntoAuxiliar, prob);

				alg -> pareto -> seleccionar_soluciones_del_frente_vrp(q);

                        }
			else
			{

				conjuntoAuxiliar -> desempaquetar_y_copiar_conjunto_pareto();

				conjuntoAuxiliar -> marcar_frente_como_old();

		   		alg -> pareto -> incorporar_frente(conjuntoAuxiliar, prob);

				alg -> pareto -> seleccionar_soluciones_del_frente(q);

			}
//cout << "S127" << endl;	 
        	}

//cout << "termina iteracion" << endl;	 
                //  cout << "Recibe del master Frente del algoritmo" << algoritmo << endl; 
                //  fprintf(faux, "Recibe del master Frente global algoritmo : %d\n", algoritmo);
                //cout << "primera f solucion o : " << prob -> funcion_obj_1(* alg -> pareto -> getSolucionVRP(0)) << endl;


	}//end while

 // fclose(faux);  
 
        delete conjuntoAuxiliar;
    
        delete prob; 

	return 0;

}
