


void construir_solucionTSP(int estOrig, MOACO * aco, int onlineUpdate, Solucion &sol)
{

//cout << estOrig; 
	
        int estVisitados = 0;

	int sgteEstado = 0;

	int estActual = estOrig;

	sol.set(estVisitados, estOrig);

	estVisitados++;


	while(estVisitados < (aco -> getProblema() -> getSize()))
   	{

// cout << " & ";
		sgteEstado = aco -> seleccionar_siguiente_estadoTSP(estActual, sol);

//cout << sgteEstado << " ";

		if(onlineUpdate)

			aco -> online_update(estActual, sgteEstado);

		estActual = sgteEstado;

		sol.set(estVisitados, sgteEstado);

		estVisitados++;

	}

	sol.set(estVisitados, estOrig);

//cout << " " << estOrig << endl; 
}



void construir_solucionQAP(int estOrigen, MOACO * aco, int onlineUpdate, Solucion &sol)
{

	int estVisitados = 0;

	int sgteEstado;

	int estActual = estOrigen;


	sol.set(estVisitados, estOrigen);

	estVisitados++;

	while(estVisitados < aco -> getProblema() -> getSize())
	{

		sgteEstado = aco -> seleccionar_siguiente_estadoQAP(estActual, sol);

		if(onlineUpdate)

			aco -> online_update(estActual, sgteEstado);

		estActual = sgteEstado;

		sol.set(estVisitados, sgteEstado);

		estVisitados++;

	}


}




void construir_solucionVRP(int estOrigen, MOACO * aco, int onlineUpdate, SolucionVRP &sol)
{


	int sgteEstado;

	double cargaActual;

	double currentTime;



	int estVisitados = 0;

	int estActual = estOrigen;


	VRPTW * vrp = (VRPTW *)(aco -> getProblema());
	

	sol.add(estOrigen);

	estVisitados++;

	currentTime = 0;

	cargaActual = 0;
//cout << "C1" << endl;	
	while(estVisitados < vrp -> getSize())
	{

		sgteEstado = aco -> seleccionar_siguiente_estadoVRP(estActual, sol, currentTime, cargaActual);
//cout << "des" << endl;	
		sol.add(sgteEstado);

		if(sgteEstado != 0) //0 representa el deposito, no ir al deposito
		{	

			estVisitados++;

			currentTime = max(currentTime + vrp -> getDistancia(estActual, sgteEstado), vrp -> getTimeStart(sgteEstado));									                 			// NO SE USO SERVICE TIME
			cargaActual += vrp -> getDemanda(sgteEstado);							/////////////////////////

			if(onlineUpdate)

             			aco -> online_update(estActual, sgteEstado);
//cout << "C3" << endl;	
		}
		else // ir al deposito
		{

			currentTime = 0;

			cargaActual = 0;

			sol.incCamiones();

		}

		estActual = sgteEstado;

	}
//cout << "C" << endl;	
	sol.add(estOrigen); // volver al deposito

}


void construir_solucionBOMBAS(int bombas, MOACO * aco, int onlineUpdate, Solucion &sol)
{

	int intervalo = 0;
    
    int bomba_actual;


	sol.set(intervalo, bombas);

	if(onlineUpdate)

			aco -> online_update(intervalo, bombas);

	intervalo++;

    bomba_actual = bombas;
    

	while(intervalo < aco -> getProblema() -> getSize())
	{

		bombas = aco -> seleccionar_siguiente_estadoBOMBAS(intervalo, bomba_actual);

		sol.set(intervalo, bombas);

		if(onlineUpdate)

			aco -> online_update(intervalo, bombas);

		intervalo++;
		
		bomba_actual = bombas;

	}


}
