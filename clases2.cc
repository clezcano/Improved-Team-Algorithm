
/************** Clase Conjunto Pareto ******************/

class ConjuntoPareto
{

	int cantSoluciones; 		//cantidad actual de soluciones

	int tamano;			//tamano del array de soluciones

protected:

	// array que contiene las soluciones del frente pareto

	Solucion ** lista;

	SolucionVRP ** listaVRP;
	

public:


	ConjuntoPareto(int numSoluciones);

        ~ConjuntoPareto();

//------------------------------------------------------------------------------------------------------------------
// TSP - QAP
//------------------------------------------------------------------------------------------------------------------
	
        double get_metrica1(Problem * problema, ConjuntoPareto &cp_conocido); //promedio de distancias
   	
        double get_metrica2(Problem * problema, double sigma);                // distribucion
   	
        double get_metrica3(Problem * problema);                              //extension
        
        double get_metrica4(Problem * problema, ConjuntoPareto &cp_conocido); //error
  	
        double get_metrica5(Problem * problema, ConjuntoPareto &cp_conocido);    //cantidad de soluciones aportadas

//------------------------------------------------------------------------------------------------------------------
// VRP 
//------------------------------------------------------------------------------------------------------------------
	
        double get_metrica1_vrp(Problem * problema, ConjuntoPareto &cp_conocido); //promedio de distancias

   	double get_metrica2_vrp(Problem * problema, double sigma);               // distribucion

   	double get_metrica3_vrp(Problem * problema);                             //extension

        double get_metrica4_vrp(Problem * problema, ConjuntoPareto &cp_conocido); //error

   	double get_metrica5_vrp(Problem * problema, ConjuntoPareto &cp_conocido);    //cantidad de soluciones aportadas

//------------------------------------------------------------------------------------------------------------------
// BOMBAS 
//------------------------------------------------------------------------------------------------------------------

   	double get_metrica1_bombas(Problem * problema, ConjuntoPareto cp_conocido); //promedio de distancias

   	double get_metrica2_bombas(Problem * problema, double sigma);               // distribucion

   	double get_metrica3_bombas(Problem * problema);                             //extension

        double get_metrica4_bombas(Problem * problema, ConjuntoPareto cp_conocido); //error

   	int get_metrica5_bombas(Problem * problema, ConjuntoPareto cp_conocido);    //cantidad de soluciones aportadas


//------------------------------------------------------------------------------------------------------------------
// TSP - QAP - BOMBAS 
//------------------------------------------------------------------------------------------------------------------

    
        int agregarNoDominado(Solucion &sol, Problem * prob);

	void eliminarDominados(Solucion &sol, Problem * prob);
	
        void listarSoluciones(Problem * prob, FILE * f);

	void listar(Problem * prob);
		
        void enviar_conjunto_pareto(int tid);

        void enviar_conjunto_pareto_a_esclavo(double q, int tid);

        void enviar_conjunto_pareto_y_sustituir_algoritmo(double q, int tid, int algoritmo);

        void enviar_soluciones_nuevas_al_master(int tid);

        void empaquetar_soluciones_nuevas();

        void empaquetar_conjunto_pareto();
        
        void recibir_conjunto_pareto(int tid);
	
	void desempaquetar_y_copiar_conjunto_pareto();
	
	int esNoDominado(Solucion &sol, Problem *prob);
 
        int esNoDominado(SolucionVRP &sol, Problem * prob);

        void insertar_en_el_conjunto_pareto_global(Solucion &sol);

        void insertar_en_el_conjunto_pareto_global(SolucionVRP &sol);
	
        void encontrarDuplicados(Problem * problema);

	void incorporar_frente(ConjuntoPareto * cpareto, Problem * prob);

        void incorporar_frente_vrp(ConjuntoPareto * cpareto, Problem * prob);
	
        int calcular_soluciones_aportadas(Problem * problema, ConjuntoPareto &cp_conocido); //cantidad de soluciones aportadas

	void seleccionar_soluciones_del_frente(double q);
       
        void seleccionar_soluciones_del_frente_vrp(double q);
   
        void marcar_frente_como_old();

	int sonIguales(double num1, double num2);

//------------------------------------------------------------------------------------------------------------------
// VRP 
//------------------------------------------------------------------------------------------------------------------
	
	
        int agregarNoDominado(SolucionVRP &sol, Problem * prob);
	
        void eliminarDominados(SolucionVRP &sol, Problem * prob);

	void listarSolucionesVRP(Problem * prob, FILE * f);


	void enviar_conjunto_pareto_vrp(int tid);

        void enviar_soluciones_nuevas_vrp_al_master(int tid);

        void enviar_conjunto_pareto_vrp_a_esclavo(double q, int tid);

        void enviar_conjunto_pareto_vrp_y_sustituir_algoritmo(double q, int tid, int algoritmo);	

        void empaquetar_conjunto_pareto_vrp();

        void empaquetar_soluciones_nuevas_vrp();

	void recibir_conjunto_pareto_vrp(int tid);

	void desempaquetar_y_copiar_conjunto_pareto_vrp();

        void marcar_frente_vrp_como_old();
	

	int getSize()
        {
        
         	return cantSoluciones;
    
        };

	Solucion * getSolucion(int i)
        {
        
         	return lista[i];
    
        };

	SolucionVRP * getSolucionVRP(int i)
        {

         	return listaVRP[i];
    
        };


};


ConjuntoPareto :: ConjuntoPareto(int numSoluciones)
{

	cantSoluciones = 0;

	tamano = numSoluciones;

     
        lista = (Solucion **)malloc(sizeof(Solucion *) * numSoluciones);

	listaVRP = (SolucionVRP **)malloc(sizeof(SolucionVRP *) * numSoluciones);
	

				        //lista = new Solucion * [numSoluciones];

				        //listaVRP = new SolucionVRP * [numSoluciones];

	for(int i = 0; i < numSoluciones; i++)
	{

		lista[i] = NULL;

		listaVRP[i] = NULL;
		
	}

}

ConjuntoPareto :: ~ConjuntoPareto()
{
   
    for (int i = 0; i < tamano; i++)
    {

         delete lista[i]; 
        
         delete listaVRP[i]; 

    }

    free(lista);
 
    free(listaVRP);

    //delete[] lista; lista = NULL;

    //delete[] listaVRP; listaVRP = NULL;

}

//----------------------------------------------------------------------------------------------------------
// TSP - QAP 
//----------------------------------------------------------------------------------------------------------

 double ConjuntoPareto :: get_metrica1(Problem * problema, ConjuntoPareto &cp_conocido) //promedio de distancias
 {

        double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido, distancia, menor_distancia, suma = 0.0;


   	for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {
           
                 x1_cpaux = problema -> funcion_obj_1(* getSolucion(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucion(i));

	         y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucion(0));
		
	         y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucion(0));

  	         menor_distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2));
          
	     
                 for (int j = 1; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
                 {

		        y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucion(j));
		
		        y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucion(j));

		        distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2));

		        if (distancia < menor_distancia)
		        {

			          menor_distancia = distancia;

		        }

                 }

                 suma += menor_distancia;
           
        }

        return suma / (double)getSize();

 }
 
 
 
 double ConjuntoPareto :: get_metrica2(Problem * problema, double sigma)                // distribucion
 {


        double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido, distancia;

        int contador, suma = 0;


	
        for (int i = 0; i < getSize(); i++) 
        {
           
                 x1_cpaux = problema -> funcion_obj_1(* getSolucion(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucion(i));

	         contador = 0;

                 for (int j = 0; j < getSize(); j++)
                 {

		      y1_cpconocido = problema -> funcion_obj_1(* getSolucion(j));
		
		      y2_cpconocido = problema -> funcion_obj_2(* getSolucion(j));

		      distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2));

		      if (distancia > sigma)
		      {

			       contador++;

		      }

                 }

	         suma += contador;
           
        }
        
        if(getSize() == 1)
        {

	     return 0.0;				   
      
        }
        else
        {
             
             return suma / (double)getSize();

        }

 }




double ConjuntoPareto :: get_metrica3(Problem * problema)  //extension
{

	
	double x1, y1, x2, y2, distancia, mayor_distancia1 = 0.0, mayor_distancia2 = 0.0;

        
	
	for (int i = 0; i < getSize() - 1; i++) 
        {
 
    
           x1 = problema -> funcion_obj_1(* getSolucion(i));
               
           y1 = problema -> funcion_obj_2(* getSolucion(i));

	
           for (int j = i + 1; j < getSize(); j++)
           {

	
        	x2 = problema -> funcion_obj_1(* getSolucion(j));
		
        	distancia = fabs(x2 - x1);

	    	if (distancia > mayor_distancia1)
	        {

  		     mayor_distancia1 = distancia;

		}

                y2 = problema -> funcion_obj_2(* getSolucion(j));
                
                distancia = fabs(y2 - y1);

	     	if (distancia > mayor_distancia2)
	        {

		     mayor_distancia2 = distancia;

	        }
    
           }//end for
       
       }//end for

     
       return sqrt(pow(mayor_distancia1,2) + pow(mayor_distancia2,2));

 }



double ConjuntoPareto :: get_metrica4(Problem * problema, ConjuntoPareto &cp_conocido) //error
{


            double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido;

	    int contador = 0, pertenece;



	    for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
            {

                 x1_cpaux = problema -> funcion_obj_1(* getSolucion(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucion(i));

  	         pertenece = 0; // 0 es falso

	         for (int j = 0; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
                 {

		         y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucion(j));
		
		         y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucion(j));
			         
	 		 if ((sonIguales(x1_cpaux, y1_cpconocido) == 1) && (sonIguales(x2_cpaux, y2_cpconocido) == 1))
		         {

			         pertenece = 1; // la solucion pertenece al frente pareto global

			         break;

			 }

                }
	         
                if(pertenece == 0)
	        {

	           contador++;

                }

           }//end for

	   return contador / (double)getSize();

 }



 double ConjuntoPareto :: get_metrica5(Problem * problema, ConjuntoPareto &cp_conocido) //cantidad de soluciones aportadas
 {


        double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido;

    	int contador = 0, pertenece;



        for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {

                 x1_cpaux = problema -> funcion_obj_1(* getSolucion(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucion(i));

  	         pertenece = 0; // 0 es falso

	         for (int j = 0; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
                 {

		      y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucion(j));
		
		      y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucion(j));

	 	      if ((sonIguales(x1_cpaux, y1_cpconocido) == 1) && (sonIguales(x2_cpaux, y2_cpconocido) == 1))
		      {

			       pertenece = 1; // la solucion pertenece al frente pareto global

			       break;

		      }

                 }

	         if(pertenece == 1)
	         {

           		 contador++;

                 }

        }

        if(cp_conocido.getSize() == 0)
        {
 
             return 0;                  

        }
        else
        {

	     return contador/(double)cp_conocido.getSize();

        }
  
 }








//---------------------------------------------------------------------------------------------------------------------
//  VRP
//---------------------------------------------------------------------------------------------------------------------

 double ConjuntoPareto :: get_metrica1_vrp(Problem * problema, ConjuntoPareto &cp_conocido) //promedio de distancias
 {

        double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido, distancia, menor_distancia, suma = 0.0;


   	for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {
           
                 x1_cpaux = problema -> funcion_obj_1(* getSolucionVRP(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucionVRP(i));

	         y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucionVRP(0));
		
	         y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucionVRP(0));

  	         menor_distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2));
          
	     
                 for (int j = 1; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
                 {

		        y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucionVRP(j));
		
		        y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucionVRP(j));

		        distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2));

		        if (distancia < menor_distancia)
		        {

			          menor_distancia = distancia;

		        }

                 }

                 suma += menor_distancia;
           
        }

        return suma / (double)getSize();

 }
 
 
 
 double ConjuntoPareto :: get_metrica2_vrp(Problem * problema, double sigma)                // distribucion
 {


        double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido, distancia;

        int contador, suma = 0;


	
        for (int i = 0; i < getSize(); i++) 
        {
           
                 x1_cpaux = problema -> funcion_obj_1(* getSolucionVRP(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucionVRP(i));

	         contador = 0;

                 for (int j = 0; j < getSize(); j++)
                 {

		      y1_cpconocido = problema -> funcion_obj_1(* getSolucionVRP(j));
		
		      y2_cpconocido = problema -> funcion_obj_2(* getSolucionVRP(j));

		      distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2));

		      if (distancia > sigma)
		      {

			       contador++;

		      }

                 }

	         suma += contador;
           
        }
        
        if(getSize() == 1)
        {

	     return 0.0;				   
      
        }
        else
        {
             
             return suma / (double)getSize();

        }

 }




double ConjuntoPareto :: get_metrica3_vrp(Problem * problema)  //extension
{

	
	double x1, y1, x2, y2, distancia, mayor_distancia1 = 0.0, mayor_distancia2 = 0.0;

        
	
	for (int i = 0; i < getSize() - 1; i++) 
        {
 
    
           x1 = problema -> funcion_obj_1(* getSolucionVRP(i));
               
           y1 = problema -> funcion_obj_2(* getSolucionVRP(i));

	
           for (int j = i + 1; j < getSize(); j++)
           {

	
        	x2 = problema -> funcion_obj_1(* getSolucionVRP(j));
		
        	distancia = fabs(x2 - x1);

	    	if (distancia > mayor_distancia1)
	        {

  		     mayor_distancia1 = distancia;

		}

                y2 = problema -> funcion_obj_2(* getSolucionVRP(j));
                
                distancia = fabs(y2 - y1);

	     	if (distancia > mayor_distancia2)
	        {

		     mayor_distancia2 = distancia;

	        }
    
           }//end for
       
       }//end for

     
       return sqrt(pow(mayor_distancia1,2) + pow(mayor_distancia2,2));

 }



double ConjuntoPareto :: get_metrica4_vrp(Problem * problema, ConjuntoPareto &cp_conocido) //error
{


            double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido;

	    int contador = 0, pertenece;



	    for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
            {

                 x1_cpaux = problema -> funcion_obj_1(* getSolucionVRP(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucionVRP(i));

  	         pertenece = 0; // 0 es falso

	         for (int j = 0; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
                 {

		         y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucionVRP(j));
		
		         y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucionVRP(j));
			         
	 		 if ((sonIguales(x1_cpaux, y1_cpconocido) == 1) && (sonIguales(x2_cpaux, y2_cpconocido) == 1))
		         {

			         pertenece = 1; // la solucion pertenece al frente pareto global

			         break;

			 }

                }
	         
                if(pertenece == 0)
	        {

	           contador++;

                }

           }//end for

	   return contador / (double)getSize();

 }



 double ConjuntoPareto :: get_metrica5_vrp(Problem * problema, ConjuntoPareto &cp_conocido) //cantidad de soluciones aportadas
 {


        double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido;

    	int contador = 0, pertenece;



        for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {

                 x1_cpaux = problema -> funcion_obj_1(* getSolucionVRP(i));

	         x2_cpaux = problema -> funcion_obj_2(* getSolucionVRP(i));

  	         pertenece = 0; // 0 es falso

	         for (int j = 0; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
                 {

		      y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucionVRP(j));
		
		      y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucionVRP(j));

	 	      if ((sonIguales(x1_cpaux, y1_cpconocido) == 1) && (sonIguales(x2_cpaux, y2_cpconocido) == 1))
		      {

			       pertenece = 1; // la solucion pertenece al frente pareto global

			       break;

		      }

                 }

	         if(pertenece == 1)
	         {

           		 contador++;

                 }

        }

        if(cp_conocido.getSize() == 0)
        {
 
             return 0;                  

        }
        else
        {

	     return contador/(double)cp_conocido.getSize();

        }
  
 }

//------------------------------------------------------------------------------------------
// BOMBAS 
//------------------------------------------------------------------------------------------

 double ConjuntoPareto :: get_metrica1_bombas(Problem * prob, ConjuntoPareto cp_conocido) //promedio de distancias
 {

        double x1_cpaux, x2_cpaux, x3_cpaux, x4_cpaux, y1_cpconocido, y2_cpconocido, y3_cpconocido, y4_cpconocido, distancia, menor_distancia, suma = 0.0;


        	
        BOMBAS * problema = (BOMBAS *) prob;

	for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {
           
         x1_cpaux = problema -> funcion_obj_1(* getSolucion(i));

	     x2_cpaux = problema -> funcion_obj_2(* getSolucion(i));

	     x3_cpaux = problema -> funcion_obj_3(* getSolucion(i));

	     x4_cpaux = problema -> funcion_obj_4(* getSolucion(i));
	     
	     
	     y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucion(0));
		
	     y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucion(0));

	     y3_cpconocido = problema -> funcion_obj_3(* cp_conocido.getSolucion(0));

	     y4_cpconocido = problema -> funcion_obj_4(* cp_conocido.getSolucion(0));


  	     menor_distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2) + pow(x3_cpaux - y3_cpconocido, 2) + pow(x4_cpaux - y4_cpconocido, 2));
          
	     
             for (int j = 1; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
             {

			y1_cpconocido = problema -> funcion_obj_1(* cp_conocido.getSolucion(j));
		
			y2_cpconocido = problema -> funcion_obj_2(* cp_conocido.getSolucion(j));

            y3_cpconocido = problema -> funcion_obj_3(* cp_conocido.getSolucion(j));

	        y4_cpconocido = problema -> funcion_obj_4(* cp_conocido.getSolucion(j));
			

            distancia = sqrt(pow(x1_cpaux - y1_cpconocido, 2) + pow(x2_cpaux - y2_cpconocido, 2) + pow(x3_cpaux - y3_cpconocido, 2) + pow(x4_cpaux - y4_cpconocido, 2));

			if (distancia < menor_distancia)
			{

				menor_distancia = distancia;

			}

             }

	     suma += menor_distancia;
           
        }

	return suma / getSize();

  }
 



 double ConjuntoPareto :: get_metrica2_bombas(Problem * prob, double sigma)                // distribucion
 {

    
        double w1, x1, y1, z1, w2, x2, y2, z2, distancia;
            
	    int contador, suma = 0;


	BOMBAS * problema = (BOMBAS *) prob;

	    for (int i = 0; i < getSize(); i++) 
        {
 
              w1 = problema -> funcion_obj_1(* getSolucion(i));
               
              x1 = problema -> funcion_obj_2(* getSolucion(i));

              y1 = problema -> funcion_obj_3(* getSolucion(i));
               
              z1 = problema -> funcion_obj_4(* getSolucion(i));

              contador = 0;

              for (int j = 0; j < getSize(); j++)
              {

                    w2 = problema -> funcion_obj_1(* getSolucion(j));
              
                    x2 = problema -> funcion_obj_2(* getSolucion(j));
  
                    y2 = problema -> funcion_obj_3(* getSolucion(j));
               
                    z2 = problema -> funcion_obj_4(* getSolucion(j));

                    distancia = sqrt(pow(w1 - w2, 2) + pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));

			        if (distancia > sigma)
			        {

            				contador++;

			        }

              }

              suma += contador;
           
        }

        return suma / (double)(getSize() - 1);

 }



double ConjuntoPareto :: get_metrica3_bombas(Problem * prob)  //extension
 {

	
    double w1, x1, y1, z1, w2, x2, y2, z2, distancia, mayor_distancia1 = 0.0, mayor_distancia2 = 0.0, mayor_distancia3 = 0.0, mayor_distancia4 = 0.0;

    //	int punto_extremo1 = 0, punto_extremo2 = 0;

    BOMBAS * problema = (BOMBAS *) prob;
	
	for (int i = 0; i < getSize() - 1; i++) 
    {
 
    
           w1 = problema -> funcion_obj_1(* getSolucion(i));
               
           x1 = problema -> funcion_obj_2(* getSolucion(i));

           y1 = problema -> funcion_obj_3(* getSolucion(i));
               
           z1 = problema -> funcion_obj_4(* getSolucion(i));
           
        
           for (int j = i + 1; j < getSize(); j++)
           {

                   
                w2 = problema -> funcion_obj_1(* getSolucion(j));
              
                x2 = problema -> funcion_obj_2(* getSolucion(j));

                y2 = problema -> funcion_obj_3(* getSolucion(j));
               
                z2 = problema -> funcion_obj_4(* getSolucion(j));

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

	return sqrt(mayor_distancia1 + mayor_distancia2 + mayor_distancia1 + mayor_distancia2);

 }

 
 double ConjuntoPareto :: get_metrica4_bombas(Problem * prob, ConjuntoPareto cp_conocido) //error
 {


        double w1, x1, y1, z1, w2, x2, y2, z2;

	    int suma = 0, contador = 0, pertenece;

        BOMBAS * problema = (BOMBAS *) prob;

	    for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {

             w1 = problema -> funcion_obj_1(* getSolucion(i));
               
             x1 = problema -> funcion_obj_2(* getSolucion(i));

             y1 = problema -> funcion_obj_3(* getSolucion(i));
               
             z1 = problema -> funcion_obj_4(* getSolucion(i));

  	         pertenece = 0; // 0 es falso

	         
             for (int j = 0; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
             {


 		          w2 = problema -> funcion_obj_1(* getSolucion(j));
              
                  x2 = problema -> funcion_obj_2(* getSolucion(j));

                  y2 = problema -> funcion_obj_3(* getSolucion(j));
               
                  z2 = problema -> funcion_obj_4(* getSolucion(j));

		    	  if (w1 == w2 && x1 == x2 && y1 == y2 && z1 == z2)
			      {

				        pertenece = 1; // la solucion pertenece al frente pareto global

				        break;

			      }

             }

	         if(pertenece == 0)
	         {

		          contador++;

             }

        }

	    return contador / (double)getSize();

 } 
 
 

 int ConjuntoPareto :: get_metrica5_bombas(Problem * prob, ConjuntoPareto cp_conocido) //cantidad de soluciones aportadas
 {


        double w1, x1, y1, z1, w2, x2, y2, z2;

    	int contador = 0, pertenece;

	
        BOMBAS * problema = (BOMBAS *) prob;

	    for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {

             w1 = problema -> funcion_obj_1(* getSolucion(i));
               
             x1 = problema -> funcion_obj_2(* getSolucion(i));

             y1 = problema -> funcion_obj_3(* getSolucion(i));
               
             z1 = problema -> funcion_obj_4(* getSolucion(i));
  	    
             pertenece = 0; // 0 es falso

	      
             for (int j = 0; j < cp_conocido.getSize(); j++) // j contador para el conjunto pareto global
             {

 		          w2 = problema -> funcion_obj_1(* getSolucion(j));
              
                  x2 = problema -> funcion_obj_2(* getSolucion(j));

                  y2 = problema -> funcion_obj_3(* getSolucion(j));
               
                  z2 = problema -> funcion_obj_4(* getSolucion(j));

		    	  if (w1 == w2 && x1 == x2 && y1 == y2 && z1 == z2)
			      {

				         pertenece = 1; // la solucion pertenece al frente pareto global

				         break;

			      }

             }

	         if(pertenece == 1)
	         {

    	    	 contador++;

             }

        }

     	return contador;
  
 }


//-------------------------------------------------------------------------------------------
// TSP - QAP - BOMBAS
//-----------------------------------------------------------------------------------------

int ConjuntoPareto:: sonIguales(double num1, double num2)
{

       if(fabs(num1-num2) < 0.00001)
           return 1;
       else
           return 0;

}

int ConjuntoPareto::esNoDominado(Solucion &sol, Problem * prob)
{

	double solfuncion1 = prob->funcion_obj_1(sol);	// Evaluacion de la solucion respecto

	double solfuncion2 = prob->funcion_obj_2(sol);	// a las funciones obetivo del problema

	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

        int esNoDominado = 1;

	for (int i = 0; i < cantSoluciones; i++)
	{

		solauxfuncion1 = prob -> funcion_obj_1(* lista[i]);

		solauxfuncion2 = prob -> funcion_obj_2(* lista[i]);
		// ambas funciones objetivo siempre se minimizan
if ( ((solauxfuncion1 < solfuncion1) || (sonIguales(solauxfuncion1, solfuncion1)==1)) && ((solauxfuncion2 < solfuncion2) || (sonIguales(solauxfuncion2, solfuncion2)==1)) )
		{	

			esNoDominado = 0; //sol es dominada por una solucion del conjunto

                        break;

                }

	}

	return esNoDominado;

}

int ConjuntoPareto :: esNoDominado(SolucionVRP &sol, Problem * prob)
{

	double solfuncion1 = prob -> funcion_obj_1(sol);	// Evaluacion de la solucion respecto

	double solfuncion2 = prob -> funcion_obj_2(sol);	// a las funciones obetivo del problema

	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

        int esNoDominado = 1;

	for (int i = 0; i < cantSoluciones; i++)
	{

		solauxfuncion1 = prob -> funcion_obj_1(* listaVRP[i]);

		solauxfuncion2 = prob -> funcion_obj_2(* listaVRP[i]);
		// ambas funciones objetivo siempre se minimizan

if ( ((solauxfuncion1 < solfuncion1) || (sonIguales(solauxfuncion1, solfuncion1) == 1)) && ((solauxfuncion2 < solfuncion2) || (sonIguales(solauxfuncion2, solfuncion2) == 1)) )
		{	

			esNoDominado = 0; //sol es dominada por una solucion del conjunto

                        break;

                }

	}

	return esNoDominado;

}

void ConjuntoPareto::eliminarDominados(Solucion &sol, Problem *prob)
{

	double solfuncion1 = prob->funcion_obj_1(sol);	// Evaluacion de la solucion respecto

	double solfuncion2 = prob->funcion_obj_2(sol);	// a las funciones obetivo del problema

	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

        int i = 0;

	while(i < cantSoluciones)
	{

		solauxfuncion1 = prob -> funcion_obj_1(* lista[i]);

		solauxfuncion2 = prob -> funcion_obj_2(* lista[i]);
		// ambas funciones objetivo siempre se minimizan
if ( ((solauxfuncion1 > solfuncion1) || (sonIguales(solauxfuncion1, solfuncion1) == 1)) && ((solauxfuncion2 > solfuncion2) || (sonIguales(solauxfuncion2, solfuncion2) == 1)) )	
        	{
			
			delete lista[i];

			lista[i] = lista[cantSoluciones - 1];

			lista[cantSoluciones-1] = NULL; //liberar puntero

			cantSoluciones--;

		}
                else
                {    

                        i++;

                }

	}

}

void ConjuntoPareto :: eliminarDominados(SolucionVRP &sol, Problem *prob)
{

	double solfuncion1 = prob -> funcion_obj_1(sol);	// Evaluacion de la solucion respecto

	double solfuncion2 = prob -> funcion_obj_2(sol);	// a las funciones obetivo del problema

	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

        int i = 0;

	while(i < cantSoluciones)
	{

		solauxfuncion1 = prob->funcion_obj_1(* listaVRP[i]);

		solauxfuncion2 = prob -> funcion_obj_2(* listaVRP[i]);
		// ambas funciones objetivo siempre se minimizan
if ( ((solauxfuncion1 > solfuncion1) || (sonIguales(solauxfuncion1, solfuncion1) == 1)) && ((solauxfuncion2 > solfuncion2) || (sonIguales(solauxfuncion2, solfuncion2) == 1)) )	
        	{
			
			delete listaVRP[i];

			listaVRP[i] = listaVRP[cantSoluciones - 1];

			listaVRP[cantSoluciones - 1] = NULL; //liberar puntero

			cantSoluciones--;

		}
                else
                {    

                        i++;

                }

	}

}

void ConjuntoPareto::insertar_en_el_conjunto_pareto_global(Solucion &sol)
{

	if (cantSoluciones==tamano)
	{

		Solucion ** listaAux=lista;

		tamano=tamano*2;

		lista=(Solucion **)malloc(tamano*sizeof(Solucion *));

		for (int i=0;i<cantSoluciones;i++)
		{

			lista[i]=listaAux[i];

                        listaAux[i]=NULL;

		}

		free(listaAux); //liberar el array anterior

	}

	if(lista[cantSoluciones] == NULL)
        {
	
		lista[cantSoluciones] = new Solucion(sol.getSize());

	}

        lista[cantSoluciones] -> solcpy(sol);

        cantSoluciones++;

}

void ConjuntoPareto::insertar_en_el_conjunto_pareto_global(SolucionVRP &sol)
{

	if (cantSoluciones == tamano)
	{

		SolucionVRP ** listaAux = listaVRP;

		tamano = tamano * 2;

		listaVRP = (SolucionVRP **)malloc(tamano * sizeof(SolucionVRP *));

		for (int i = 0; i < cantSoluciones; i++)
		{

			listaVRP[i] = listaAux[i];

                        listaAux[i] = NULL;

		}

		free(listaAux); //liberar el array anterior

	}

	if(listaVRP[cantSoluciones] == NULL)
        {
	
		listaVRP[cantSoluciones] = new SolucionVRP(sol.getSize());

	}

        listaVRP[cantSoluciones] -> solcpy(sol);

        cantSoluciones++;

}

void ConjuntoPareto::encontrarDuplicados(Problem * problema)
{
   double x1_cpaux, x2_cpaux, y1_cpconocido, y2_cpconocido;
       for (int i = 0; i < getSize(); i++) // i contador para el conjunto pareto auxiliar
        {

                 x1_cpaux = problema -> funcion_obj_1(* getSolucion(i));
	         x2_cpaux = problema -> funcion_obj_2(* getSolucion(i));
	         for (int j = 0; j < getSize(); j++) // j contador para el conjunto pareto global
                 {
                       y1_cpconocido = problema -> funcion_obj_1(* getSolucion(j));
		       y2_cpconocido = problema -> funcion_obj_2(* getSolucion(j));
                       if (i!=j && (sonIguales(x1_cpaux, y1_cpconocido) == 1) && (sonIguales(x2_cpaux, y2_cpconocido) == 1))
		          cout << "hay duplicados" << endl;
                 }

        }

}

/*
void ConjuntoPareto :: listar(Problem * prob)
{

                  // printf("%d\n", cantSoluciones);

  	          cout << cantSoluciones << endl;

                  for (int i = 0; i < cantSoluciones; i++)
	          {

 // printf("%lf\t%lf\n", prob -> funcion_obj_1(* lista[i]), prob -> funcion_obj_2(* lista[i]));

cout << prob -> funcion_obj_1(* lista[i]) << "  " << prob -> funcion_obj_2(* lista[i]) << endl;

     	          }
 
}
*/
void ConjuntoPareto::incorporar_frente(ConjuntoPareto * cpareto, Problem * prob)
{
	
        for(int i = 0; i < cpareto -> getSize(); i++)
	{

		if (esNoDominado(* cpareto -> getSolucion(i), prob) == 1)  
		{

			eliminarDominados(* cpareto -> getSolucion(i), prob);

                        insertar_en_el_conjunto_pareto_global(* cpareto -> getSolucion(i));

		}

	}

}

void ConjuntoPareto::incorporar_frente_vrp(ConjuntoPareto * cpareto, Problem * prob)
{
	
        for(int i = 0; i < cpareto -> getSize(); i++)
	{

		if (esNoDominado(* cpareto -> getSolucionVRP(i), prob) == 1)  
		{

			eliminarDominados(* cpareto -> getSolucionVRP(i), prob);

                        insertar_en_el_conjunto_pareto_global(* cpareto -> getSolucionVRP(i));

		}

	}

}

void ConjuntoPareto::seleccionar_soluciones_del_frente(double q)
{

        int i = 0;

        double valor_aleatorio;


	while(i < cantSoluciones)
	{

         	valor_aleatorio = rand() / (double)RAND_MAX;
         
         	if (lista[i] -> esNuevo() == 0 && valor_aleatorio > q)
        	{

				delete lista[i];
		
	                     	lista[i] = lista[cantSoluciones - 1];
			
        	                lista[cantSoluciones - 1] = NULL; //liberar puntero
			
        	                cantSoluciones--;

                }
                else
                {    

                        i++;

                }

	}

}

void ConjuntoPareto::seleccionar_soluciones_del_frente_vrp(double q)
{

        int i = 0;

        double valor_aleatorio;


	while(i < cantSoluciones)
	{

         	valor_aleatorio = rand() / (double)RAND_MAX;
         
         	if (listaVRP[i] -> esNuevo() == 0 && valor_aleatorio > q)
        	{

				delete listaVRP[i];
		
	                     	listaVRP[i] = listaVRP[cantSoluciones - 1];
			
        	                listaVRP[cantSoluciones - 1] = NULL; //liberar puntero
			
        	                cantSoluciones--;

                }
                else
                {    

                        i++;

                }

	}

}

void ConjuntoPareto :: listarSoluciones(Problem * prob, FILE * f)
{

    

    //fprintf(f, "Cantidad de soluciones : %d\n", cantSoluciones);
    
    fprintf(f, "%d\n", cantSoluciones);

    if(prob -> get_column_size() == 0)//tsp, qap
    {    

                  for (int i = 0; i < cantSoluciones; i++)
	          {

		      //fprintf(f, "\nSolucion %d\nValor del objetivo 1 : %lf\tValor del objetivo 2 : %lf\n", i + 1,  prob -> funcion_obj_1(* lista[i]), prob -> funcion_obj_2(* lista[i]));
		      fprintf(f, "%lf\t%lf\n", prob -> funcion_obj_1(* lista[i]), prob -> funcion_obj_2(* lista[i]));
      	    	     
                      // lista[i] -> imprimir(f);

     	          }
 
    }
    else
    {

         BOMBAS * problema = (BOMBAS *) prob; 

         BOMBAS * prob = problema;

     	 for (int i = 0; i < cantSoluciones; i++)
    	 {

      	             fprintf(f, "%lf\t%lf\n", prob -> funcion_obj_1(* lista[i]), prob -> funcion_obj_2(* lista[i]), prob -> funcion_obj_3(* lista[i]), prob -> funcion_obj_4(* lista[i]));
		     lista[i] -> imprimir(f);

         }

    }

    
}

void ConjuntoPareto :: listarSolucionesVRP(Problem * prob, FILE * f)
{

//	FILE * f = fopen(file, "w");

	fprintf(f, "%d\n", cantSoluciones);

	for (int i = 0; i < cantSoluciones; i++)
	{

		fprintf(f, "%lf\t%lf\n", prob -> funcion_obj_1(* listaVRP[i]), prob -> funcion_obj_2(* listaVRP[i]));
		
                //listaVRP[i]->imprimir(f);

	}

//	fclose(f);

}


/*
int ConjuntoPareto::agregarNoDominado(Solucion &sol, Problem *prob)
{
	double solfuncion1=prob->funcion_obj_1(sol);	// Evaluacion de la solucion respecto
	double solfuncion2=prob->funcion_obj_2(sol);	// a las funciones obetivo del problema
	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

	for (int i=0; i<cantSoluciones; i++)
	{
		solauxfuncion1=prob->funcion_obj_1(*lista[i]);
		solauxfuncion2=prob->funcion_obj_2(*lista[i]);
		// ambas funciones objetivo siempre se minimizan
		if (solauxfuncion1 <= solfuncion1 && solauxfuncion2 <= solfuncion2)
			return 0; //sol es dominada por una solucion del conjunto
	}
	//Aumentar el tamaño del conjunto Pareto si éste está lleno
	if (cantSoluciones==tamano)
	{
		Solucion ** listaAux=lista;
		tamano=tamano*2;
		lista=(Solucion **)malloc(tamano*sizeof(Solucion *));
		for (int i=0;i<cantSoluciones;i++)
		{
			lista[i]=listaAux[i];
		}
		free(listaAux);
	}
	if(lista[cantSoluciones]==NULL)
		lista[cantSoluciones]=new Solucion(sol.getSize());
	lista[cantSoluciones]->solcpy(sol);
	cantSoluciones++;
	return 1;

}
*/


//------------------------------------------------------------------------------------------
// VRP 
//-------------------------------------------------------------------------------------------
/*
int ConjuntoPareto :: agregarNoDominado(SolucionVRP &sol, Problem * prob)
{

	double solfuncion1 = prob -> funcion_obj_1(sol);	// Evaluacion de la solucion respecto

	double solfuncion2 = prob -> funcion_obj_2(sol);	// a las funciones obetivo del problema

	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

	for (int i = 0; i < cantSoluciones; i++)
	{

		solauxfuncion1 = prob -> funcion_obj_1(* listaVRP[i]);

		solauxfuncion2 = prob -> funcion_obj_2(* listaVRP[i]);

		// ambas funciones objetivo siempre se minimizan

		if (solauxfuncion1 <= solfuncion1 && solauxfuncion2 <= solfuncion2)

			return 0; //sol es dominada por una solucion del conjunto

	}

	//Aumentar el tama� del conjunto Pareto si �te est�lleno
	if (cantSoluciones == tamano)
	{

		SolucionVRP ** listaAux = listaVRP;

		tamano = tamano * 2;

		listaVRP = (SolucionVRP **)malloc(tamano * sizeof(SolucionVRP *));

		for (int i = 0; i < cantSoluciones; i++)
		{

			listaVRP[i] = listaAux[i];

		}

		free(listaAux);

	}

	if(listaVRP[cantSoluciones] == NULL)

		listaVRP[cantSoluciones] = new SolucionVRP(sol.getSize());

	listaVRP[cantSoluciones] -> solcpy(sol);

	cantSoluciones++;

	return 1;

}
*/
/*
void ConjuntoPareto :: eliminarDominados(SolucionVRP &sol, Problem * prob)
{


	double solfuncion1 = prob -> funcion_obj_1(sol);	// Evaluacion de la solucion respecto

	double solfuncion2 = prob -> funcion_obj_2(sol);	// a las funciones obetivo del problema

	double solauxfuncion1, solauxfuncion2; // Evaluacion de los objetivos de alguna solucion del conjunto

	for (int i = 0; i < cantSoluciones; i++)
	{

		solauxfuncion1 = prob -> funcion_obj_1(* listaVRP[i]);

		solauxfuncion2 = prob -> funcion_obj_2(* listaVRP[i]);

		// ambas funciones objetivo siempre se minimizan
		if ((solauxfuncion1 > solfuncion1 && solauxfuncion2 >= solfuncion2) || (solauxfuncion1 >= solfuncion1 && solauxfuncion2 > solfuncion2))
		{

			delete listaVRP[i];

			listaVRP[i] = listaVRP[cantSoluciones - 1];

			listaVRP[cantSoluciones - 1] = NULL; //liberar puntero

			cantSoluciones--;

			i--;

		}

	}


}
*/


//-------------------------------------------------------------------------------------------
// TSP - QAP - BOMBAS
//-------------------------------------------------------------------------------------------

void ConjuntoPareto :: enviar_conjunto_pareto_a_esclavo(double q, int tid)
{

	pvm_initsend(PvmDataDefault);
	//cout << "manda" << endl;
	pvm_pkdouble(&q, 1, 1);
//cout << "esclavo a master con.pa. envio: " << endl;
	empaquetar_conjunto_pareto();
//cout << "se fue" << endl; 
	pvm_send(tid, CONJUNTO_PARETO);

}

void ConjuntoPareto :: enviar_conjunto_pareto_vrp_a_esclavo(double q, int tid)
{

	pvm_initsend(PvmDataDefault);
	//cout << "manda" << endl;
	pvm_pkdouble(&q, 1, 1);
//cout << "esclavo a master con.pa. envio: " << endl;
	empaquetar_conjunto_pareto_vrp();
//cout << "se fue" << endl; 
	pvm_send(tid, CONJUNTO_PARETO);

}

void ConjuntoPareto :: enviar_conjunto_pareto_y_sustituir_algoritmo(double q, int tid, int algoritmo)
{

	pvm_initsend(PvmDataDefault);
//cout << "master a esclavo con.pa.de susti algor no.: " << algoritmo << endl;
	pvm_pkdouble(&q, 1, 1);	

	pvm_pkint(&algoritmo, 1, 1);
	
	empaquetar_conjunto_pareto();

	pvm_send(tid, SUSTITUIR_ALGORITMO);

}

void ConjuntoPareto :: enviar_conjunto_pareto_vrp_y_sustituir_algoritmo(double q, int tid, int algoritmo)
{

	pvm_initsend(PvmDataDefault);
//cout << "master a esclavo con.pa.de susti algor no.: " << algoritmo << endl;
	pvm_pkdouble(&q, 1, 1);	

	pvm_pkint(&algoritmo, 1, 1);
	
	empaquetar_conjunto_pareto_vrp();

	pvm_send(tid, SUSTITUIR_ALGORITMO);

}


void ConjuntoPareto :: enviar_soluciones_nuevas_al_master(int tid)
{

	pvm_initsend(PvmDataDefault);
	//cout << "manda" << endl;
//cout << "esclavo a master con.pa. envio: " << endl;
	empaquetar_soluciones_nuevas();
//cout << "se fue" << endl; 
	pvm_send(tid, CONJUNTO_PARETO);

}

void ConjuntoPareto :: enviar_soluciones_nuevas_vrp_al_master(int tid)
{

	pvm_initsend(PvmDataDefault);
	//cout << "manda" << endl;
//cout << "esclavo a master con.pa. envio: " << endl;
	empaquetar_soluciones_nuevas_vrp();
//cout << "se fue" << endl; 
	pvm_send(tid, CONJUNTO_PARETO);

}


void ConjuntoPareto :: empaquetar_soluciones_nuevas()
{

	int contador = 0;

	int aux;


	for (int i = 0; i < cantSoluciones; i++)
	{

		if (lista[i] -> esNuevo() == 1)
		{ 

			contador++;			                 

                }

	}

	pvm_pkint(&contador, 1, 1);

	if(contador > 0)
        {

//cout << "CantSolucion : " << cantSoluciones << endl;
		aux = lista[0] -> getSize();
//cout << "tam : " << aux << endl;
		pvm_pkint(&aux, 1, 1);

		for (int i = 0; i < cantSoluciones; i++)
		{
//cout << "solucion : " << endl;
	                if(lista[i] -> esNuevo() == 1)
        	        {

				for (int j = 0; j < lista[i] -> getSize(); j++)
				{

					aux = lista[i] -> get(j);
			
	        	                pvm_pkint(&aux, 1, 1);
//cout << aux << "-"; 
				}
//cout << " " << endl;
	                }

		}

	}

}

void ConjuntoPareto :: empaquetar_soluciones_nuevas_vrp()
{

	int contador = 0;

	int aux;


	for (int i = 0; i < cantSoluciones; i++)
	{

		if (listaVRP[i] -> esNuevo() == 1)
		{ 

			contador++;			                 

                }

	}

	pvm_pkint(&contador, 1, 1);

	if(contador > 0)
        {

		for (int i = 0; i < cantSoluciones; i++)
		{
//cout << "solucion : " << endl;
	                if(listaVRP[i] -> esNuevo() == 1)
        	        {

				aux = listaVRP[i] -> getSize();

				pvm_pkint(&aux, 1, 1);

				aux = listaVRP[i] -> getSizeActual();

				pvm_pkint(&aux, 1, 1);

				aux = listaVRP[i] -> getCamiones();

				pvm_pkint(&aux, 1, 1);

				for (int j = 0; j < listaVRP[i] -> getSize(); j++)
				{

					aux = listaVRP[i] -> get(j);

					pvm_pkint(&aux, 1, 1);

				}
//cout << " " << endl;
		        }

		}

	}

}


void ConjuntoPareto :: empaquetar_conjunto_pareto()
{

	int aux;


	pvm_pkint(&cantSoluciones, 1, 1);
//cout << "CantSolucion : " << cantSoluciones << endl;
        if(cantSoluciones > 0)
        {
		aux = lista[0] -> getSize();
//cout << "tam : " << aux << endl;
		pvm_pkint(&aux, 1, 1);

		for (int i = 0; i < cantSoluciones; i++)
		{
//cout << "solucion : " << endl;
			for (int j = 0; j < lista[i] -> getSize(); j++)
			{

				aux = lista[i] -> get(j);
			
        	                pvm_pkint(&aux, 1, 1);
//cout << aux << "-"; 
			}
//cout << " " << endl;
		}
        }
}

void ConjuntoPareto :: empaquetar_conjunto_pareto_vrp()
{

	
	int aux;

	pvm_pkint(&cantSoluciones, 1, 1);

	for (int i = 0; i < cantSoluciones; i++)
	{

		aux = listaVRP[i] -> getSize();

		pvm_pkint(&aux, 1, 1);

		aux = listaVRP[i] -> getSizeActual();

		pvm_pkint(&aux, 1, 1);

		aux = listaVRP[i] -> getCamiones();

		pvm_pkint(&aux, 1, 1);

		for (int j = 0; j < listaVRP[i] -> getSize(); j++)
		{

			aux = listaVRP[i] -> get(j);

			pvm_pkint(&aux, 1, 1);

		}

	}

}


void ConjuntoPareto :: recibir_conjunto_pareto(int tid)
{
//	cout << "aca" << endl;
	pvm_recv(tid, CONJUNTO_PARETO); //    PVM_NRECV USAR O NO
//	    cout << "actualizo" << endl;
//cout << "esclavo a master con.pa. recibir : " << endl;
        desempaquetar_y_copiar_conjunto_pareto();

}

void ConjuntoPareto :: recibir_conjunto_pareto_vrp(int tid)
{

	
	pvm_recv(tid, CONJUNTO_PARETO);

	desempaquetar_y_copiar_conjunto_pareto_vrp();


}

void ConjuntoPareto :: desempaquetar_y_copiar_conjunto_pareto()
{

	int tamsol = 0, nodo = 0;

	
        pvm_upkint(&cantSoluciones, 1, 1);

        if(cantSoluciones > 0)
        {
	        pvm_upkint(&tamsol, 1, 1);
	
//cout << "tam : " << tamsol << endl;
	        for (int i = 0; i < cantSoluciones; i++)
		{

//cout << "solucion : " << endl;
			if(lista[i] == NULL)

				lista[i] = new Solucion(tamsol);
	
        	    	for (int j = 0; j < tamsol; j++)
			{

				pvm_upkint(&nodo, 1, 1);

				lista[i] -> set(j, nodo);

//cout << nodo << "-"; 
			}
	
//cout << " " << endl;
        	}

       	 }

}


void ConjuntoPareto :: desempaquetar_y_copiar_conjunto_pareto_vrp()
{


	int tamsol = 0, nodo = 0;

	int sizeActual, camiones;

	
	pvm_upkint(&cantSoluciones, 1, 1);

	for (int i = 0; i < cantSoluciones; i++)
	{

		pvm_upkint(&tamsol, 1, 1);

		pvm_upkint(&sizeActual, 1, 1);

		pvm_upkint(&camiones, 1, 1);

		if(listaVRP[i] == NULL)

			listaVRP[i] = new SolucionVRP(tamsol);

		listaVRP[i] -> setSizeActual(sizeActual);

		listaVRP[i] -> setCamiones(camiones);

		for (int j = 0; j < tamsol; j++)
		{

			pvm_upkint(&nodo, 1, 1);

			listaVRP[i] -> set(j, nodo);

		}

	}

}


void ConjuntoPareto :: marcar_frente_como_old()
{

 for (int i = 0; i < cantSoluciones; i++)
 {

      lista[i] -> cambiar_estado_a_old(); 

 }

}

void ConjuntoPareto :: marcar_frente_vrp_como_old()
{

 for (int i = 0; i < cantSoluciones; i++)
 {

      listaVRP[i] -> cambiar_estado_a_old(); 

 }

}




class MOACO
{

protected:

	Problem * prob;

	int criterio, tiempo_ejecucion,tiempoTotal, maxIteraciones, hormigas;

public:

    ConjuntoPareto * pareto;

    MOACO(Problem * p)
    {
    
	prob = p;
	
    	pareto = new ConjuntoPareto(1000);
	
    	srand(time(NULL));

    };

    ~MOACO()
    {
    
	// delete pareto; pareto = NULL;

    };

    virtual int seleccionar_siguiente_estadoTSP(int estOrigen, Solucion &sol) = 0;
	
    virtual int seleccionar_siguiente_estadoQAP(int estOrigen, Solucion &sol) = 0;
	
    virtual int seleccionar_siguiente_estadoVRP(int estOrigen, Solucion &sol, double currentTime, double cargaActual) = 0;
	
    virtual int seleccionar_siguiente_estadoBOMBAS(int intervalo, int bomba) = 0;
	
    virtual void ejecutarTSP(int extension_tiempo) = 0;
	
    virtual void ejecutarQAP(int extension_tiempo) = 0;
	
    virtual void ejecutarVRP(int extension_tiempo) = 0;
    
    virtual void ejecutarBOMBAS(void) = 0;

	
    int condicion_parada(int generacion, clock_t start, clock_t end, int extension_tiempo)
    {
	
    	if(criterio == 1)
     	{
		if (extension_tiempo == TIEMPO_COMPLETO)
                        
			  tiempo_ejecucion = tiempoTotal;                          
        	else
        
                          tiempo_ejecucion = 1;
    		
		if(((end - start) / (double)CLOCKS_PER_SEC) < tiempo_ejecucion)
	
    			return 0;
	
    	}
        else if(generacion < maxIteraciones)
	
    		return 0;
		
	
    	return 1;
	
    };

    Problem * getProblema(void)
    {
            
            return prob;
    
    };
	
    virtual void online_update(int orig, int dest)
    {};
  
    void reemplazar_frente(MOACO * algAnterior)
    {
       
	  delete pareto;
         
          pareto = algAnterior -> pareto;
         
          algAnterior -> pareto = NULL;

    };

};
