

#include "/usr/share/pvm3/include/pvm3.h"

#include "varios.h"

#include "algoritmos.h"


int main(int argc, char *args[])
{

    	int mytid;					// TID del esclavo

    	int master;					// TID del maestro

    	int problema;					// indica el codigo del problema 

    	char parametros[100];				// indica el archivo de parametros a utilizar 

    	int generacion = 0;				// contador de generaciones

    	int bufid, type, bytes, source;			// varios.

    	int algoritmo;					// algoritmo a ejecutar

    	MOACO * alg;					// instancia del algoritmo

    	Problem * prob;					// instancia del problema a resolver

	
	
	
	
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
    	
        

        if(problema == KROAB)        // instanciar el problema y el algoritmo a utilizar
        {
		
	        prob = new TSP("/home/fuentes/KROAB100.TXT.tsp");

         	strcpy(parametros, "/home/fuentes/parametros_tsp.txt");
	
    	}
     	else if(problema == KROAC)
      	{

       		prob = new TSP("/home/fuentes/kroac100.txt.tsp");

         	strcpy(parametros, "/home/fuentes/parametros_tsp.txt");

       	}
        else if(problema == QAP1)
        {

        	prob = new QAP("/home/fuentes/qapUni.75.0.1.qap");

         	strcpy(parametros, "/home/fuentes/parametros_qap.txt");

       	}
        else if(problema == QAP2)
        {

        	prob = new QAP("/home/fuentes/qapUni.75.p75.1.qap");

         	strcpy(parametros, "/home/fuentes/parametros_qap.txt");

       	}
        else if(problema == C101)
        {

        	prob = new VRPTW("/home/fuentes/c101.txt");

         	strcpy(parametros, "/home/fuentes/parametros_vrp.txt");

       	}
        else if(problema == RC101)
        {

        	prob = new VRPTW("/home/fuentes/rc101.txt");

         	strcpy(parametros, "/home/fuentes/parametros_vrp.txt");
		
    	}
     	else if(problema == BOMBAS1)
      	{

	    	prob = new BOMBAS("/home/fuentes/bombas.txt");

      		strcpy(parametros, "/home/fuentes/parametros_bombas.txt");

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

    /* iniciar labor del esclavo */
/*    mytid=1;
    problema=KROAB;
    prob=new TSP("/home/fuentes/KROAB100.TXT.tsp");
    strcpy(parametros,"/home/fuentes/parametros_tsp.txt");
    algoritmo=CBIANT;
    alg=new PACO(prob,parametros);
*/	

	while(1)
   	{	// es infinito porque para terminar la ejecucion actualmente el master invoca a pvm_kill

		generacion ++;

		// Ejecutar el algoritmo indicado
		if (problema == KROAB || problema == KROAC)

			alg -> ejecutarTSP();


		else if(problema == QAP1 || problema == QAP2)

			alg -> ejecutarQAP();


		else if(problema == C101 || problema == RC101)

		  	alg -> ejecutarVRP();

		
        else if(problema == BOMBAS1)
		
        	alg -> ejecutarBOMBAS();
        	

		// enviar conjunto pareto al master
		if(problema == C101 || problema == RC101)

			alg -> pareto -> enviar_conjunto_pareto_vrp(master);

		else

			alg -> pareto -> enviar_conjunto_pareto(master);




		bufid = pvm_recv(master, -1); // recibir datos del master

		pvm_bufinfo(bufid, &bytes, &type, &source);

		if (type == SUSTITUIR_ALGORITMO)
		{

			pvm_upkint(&algoritmo, 1, 1);
			// instanciar el nuevo algoritmo indicado
			// delete alg ??????????

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
		else
		{
            
            printf("\nError: No se sustituyen los algoritmos\n");
		
    	    exit(1);
    	    
        }

        if(problema == C101 || problema == RC101)
        {
		 
             alg -> pareto -> desempaquetar_&_copiar_conjunto_pareto_vrp();
        
        }            
		else
        {

		     alg -> pareto -> desempaquetar_&_copiar_conjunto_pareto();

        }

	}

	return 0;
}
