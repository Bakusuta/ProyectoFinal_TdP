#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <conio.h>
#include <limits>

#define NOMINMAX

#include <Windows.h>

/*
camelCase -> variables y funciones(incluidos metodos)
snake_Camel_Case -> parametros
Pascal_Case -> clases
SCREAM_SNAKE_CASE -> constantes
*/

int generarNumeroAleatorio(int param_numeroMaximo, int param_numeroMinimo) { //genera un numero aleatorio a partir de un limite maximo y minimo
	int numRandom = rand() % (param_numeroMaximo - param_numeroMinimo + 1) + param_numeroMinimo;
	return numRandom;
}

int ingresarNumero() {
	int valor = 0;
	while (!(std::cin >> valor)) {
		std::cout << "Esto es una palabra, por favor dime un numero" << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return valor;
}

void textoOmitible(std::string texto) {
	for (int i = 0; i < texto.size(); i++) {

		if (_kbhit()) { //se omite el dialogo si se presiona una tecla
			(void)_getch(); 
			std::cout << texto.substr(i);
			std::cout.flush();
			return;
		}

		std::cout << texto[i];
		std::cout.flush();
		Sleep(20);
	}
}

class Item { //clase item
	//datos
	std::string nombreItem;
	std::string descripcionItem;
	int precioItem;
	int cantidadItem;
	int efectoItem; //efecto se aplica a cuanto se cura el jugador y cuanto daño recibe el enemigo cuando se ve afectado
	int tipoItem; //esta variable le indica al sistema si es una curación o un debuff

	//metodos
public:
	Item(std::string param_nombreItem, std::string param_descripcionItem, int param_precioItem, int param_cantidadItem, int param_efectoItem, int param_tipoItem) {
		nombreItem = param_nombreItem;
		descripcionItem = param_descripcionItem;
		precioItem = param_precioItem;
		cantidadItem = param_cantidadItem;
		efectoItem = param_efectoItem;
		tipoItem = param_tipoItem;
	}

	void mostrarItem() { //muestra una descripción detallada del juego
		std::cout << "-----------------------------------------------------------------" << std::endl;
		std::cout << "Nombre: " << nombreItem << std::endl;
		std::cout << "Descripcion: " << descripcionItem << std::endl;
		std::cout << "Precio: " << precioItem << std::endl;
		std::cout << "Cantidad: " << cantidadItem << std::endl;
		std::cout << "-----------------------------------------------------------------" << std::endl;
	}

	std::string mostrarNombreItem() {
		return nombreItem;
	}

	int mostrarCantidadItem() {
		return cantidadItem;
	}

	int mostrarPrecioItem() {
		return precioItem;
	}

	int mostrarEfectoItem() {
		return efectoItem;
	}

	int mostrarTipoItem() {
		return tipoItem;
	}

	void sobrescribirCantidadItem(int param_item) { //cambia la cantidad del item y se usa especificamente en la tienda
		cantidadItem = param_item;
	}

	void usarItem() { //resta especificamente un item y este se aplica al jugador cuando utiliza un item
		cantidadItem--;
	}

};


class Personaje { //clase padre de personaje 

	//datos
protected:
	std::string nombre;
	int vidaMaxima;
	int vida;
	int danio;
	int experiencia;
	int nivel;


	//metodos
public:
	//constructor
	Personaje(std::string param_nombre, int param_vida, int param_danio, int param_experiencia, int param_nivel) {
		nombre = param_nombre;
		vida = param_vida;
		vidaMaxima = param_vida;
		danio = param_danio;
		experiencia = param_experiencia;
		nivel = param_nivel;
	}

	void recibirDanio(int param_danio) { //mecanica atacar
		vida = vida - param_danio;

		if (vida <= 0) { //murio
			vida = 0;
		}
	}

	bool estaVivo() { //metodo verificar vida
		if (vida <= 0) {
			return false;
		}
		else {
			return true;
		}
	}

	int verDanio() { //metodo de ver el daño
		return danio;
	}

	int verVida() { //metodo de ver vida
		return vida;
	}

	int verVidaMaxima() { //metodo para ver vida maxima
		return vidaMaxima;
	}

	std::string verNombre() {
		return nombre;
	}

	void curarse(int param_curacion) { //metodo de curarse
		vida += param_curacion;
		if (vida > vidaMaxima) {
			vida = vidaMaxima;
		}
	}
};


//clase enemigo, hijo de personaje
class Enemigo : public Personaje {
	//datos
	int curaciones = 1;


	//metodos
public:
	Enemigo(std::string param_nombreEnemigo, int param_vidaEnemigo, int param_danioEnemigo, int param_experienciaEnemigo, int param_nivelEnemigo) : Personaje(param_nombreEnemigo, param_vidaEnemigo, param_danioEnemigo, param_experienciaEnemigo, param_nivelEnemigo) {} 

	int opcionesCombateEnemigo(){
		if (vida <= 60 && curaciones >= 1) {
			curaciones--;
			return 2;
		}
		else {
			return 1;
		}
	}
};


//clase jugador, hijo de personaje
class Jugador : public Personaje {
	//datos
	std::vector<Item> inventarioPersonal;
	int oro = 100;
	int cooldownEspecial = 0;

	//metodos
public:
	Jugador(std::string param_nombreJugador, int param_vidaJugador, int param_danioJugador) : Personaje(param_nombreJugador, param_vidaJugador, param_danioJugador, 0, 1) {} //el jugador siempre se inicia en nivel 1 con experiencia 0

	void comprarItem(int param_compra) { //metodo que resta el dinero que gastaste en comprar
		oro = oro - param_compra;
	}

	void ganarOro(int param_ganancia) { //metodo utilizado para cuando ganas dinero en un combate
		oro = oro + param_ganancia;
		std::cout << "Has ganado " << param_ganancia << " piezas de oro" << std::endl;
		std::cout << "Oro actual: " << oro << "g" << std::endl;
	}

	void agregarItemAlInventario(Item& itemComprado) { //guarda el objeto comprado en el inventario del jugador
		for (int contador = 0; contador < inventarioPersonal.size(); contador++) {
			if (inventarioPersonal[contador].mostrarNombreItem() == itemComprado.mostrarNombreItem()) {
				inventarioPersonal[contador].sobrescribirCantidadItem(inventarioPersonal[contador].mostrarCantidadItem() + itemComprado.mostrarCantidadItem());
				return;
			}
		}

		inventarioPersonal.push_back(itemComprado);
	}

	void mostrarInventario() { //muestra los objetos que poses en tu inventario
		if (inventarioPersonal.size() == 0) {
			std::cout << "Parece que no tengo objetos..." << std::endl;
		}
		else {
			for (int contador = 0; contador < inventarioPersonal.size(); contador++) {
				std::cout << "Objeto: " << contador + 1 << std::endl;
				inventarioPersonal[contador].mostrarItem();
			}
		}
	}

	int usarItemInventario(int param_posicionItem, Enemigo& param_statsEnemigo) { //permite utilizar un objeto del inventario
		if (param_posicionItem < inventarioPersonal.size() && param_posicionItem >= 0) { //verifica que sea menor al tamaño de la categoria
			inventarioPersonal.at(param_posicionItem).usarItem();
			switch (inventarioPersonal.at(param_posicionItem).mostrarTipoItem()) {
			case 1:
				curarse(inventarioPersonal.at(param_posicionItem).mostrarEfectoItem());
				break;
			case 2:
				param_statsEnemigo.recibirDanio(inventarioPersonal.at(param_posicionItem).mostrarEfectoItem());
				break;
			default:
				break;
			}
			std::cout << "Has usado el item " << inventarioPersonal.at(param_posicionItem).mostrarNombreItem() << std::endl;

			if (inventarioPersonal.at(param_posicionItem).mostrarCantidadItem() <= 0) {
				std::cout << "Te quedaste sin " << inventarioPersonal.at(param_posicionItem).mostrarNombreItem() << std::endl;
				inventarioPersonal.erase(inventarioPersonal.begin() + param_posicionItem);
			}
			return 1;
		}
		else {
			std::cout << "Esta opcion no existe" << std::endl;
			return 0;
		}
	}

	int mostrarOro() {
		return oro;
	}

	void opcionesCombateJugador(Enemigo& param_statsEnemigo) { //menu de combate de jugador 
		//Inicia Combate
		int centinela = 0;
		std::cout << "----Turno de " << nombre << "----" << std::endl;
		std::cout << "Dime, que te gustaria hacer?" << std::endl;
		std::cout << "0. atacar" << std::endl;
		if (cooldownEspecial == 0) {
			std::cout << "1. Ataque Fuerte" << std::endl;
		}
		if (inventarioPersonal.size() != 0) {
			std::cout << "2. ver tu inventario" << std::endl;
		}
		while (centinela == 0) {
			int opcionMenu = ingresarNumero();
			switch (opcionMenu) {
			case 0:
				//atacar --> hacer daño / recibir daño
				param_statsEnemigo.recibirDanio(verDanio());
				centinela = 1;
				break;
			case 1:
				if (cooldownEspecial == 0) {
					param_statsEnemigo.recibirDanio(verDanio() * 2);
					centinela = 1;
					cooldownEspecial += 3;
				}
				else {
					std::cout << "Habilidad aun no disponible..." << std::endl;
				}
				break;
			case 2:
				if (inventarioPersonal.size() == 0) {
					std::cout << "Parece que no tengo objetos..." << std::endl;
				}
				else {
					int usoObjeto = 0;
					while (usoObjeto == 0) {
						mostrarInventario();
						std::cout << "que objeto quieres utilizar?" << std::endl;
						int opcionObjeto;
						opcionObjeto = ingresarNumero();
						usoObjeto = usarItemInventario(opcionObjeto - 1, param_statsEnemigo);
						centinela = 1;
					}
				}
				break;
			default:
				break;
			}
			if (cooldownEspecial != 0) { //reduce el cooldown hasta que llegue a 0
				cooldownEspecial -= 1;
			}
		}
	}

	void subirNivel() {
		experiencia += generarNumeroAleatorio(6, 2);
		if (experiencia >= 10) {
			danio += 10;
			vidaMaxima += 20;
			vida += 20;
			nivel += 1;
			experiencia -= 10;
			std::cout << "Subiste a nivel: " << nivel << std::endl;
			mostrarInfoJugador();
		}
	}

	void mostrarInfoJugador() {
		std::cout << "----------------------------------------" << std::endl;
		std::cout << "Nombre jugador: " << nombre << std::endl;
		std::cout << "vida: " << vida << "/" << vidaMaxima << std::endl;
		std::cout << "danio: " << danio << std::endl;
		std::cout << "nivel: " << nivel << std::endl;
	}

};


class Categoria { //clase categoria de los items
	//datos
	std::string nombreItemCategoria;
	std::string descripcionItemCategoria;
	int precioItemCategoria = 0;
	int cantidadItemCategoria = 0;
	int efectoItemCategoria = 0;

	std::vector<Item> categoriaItem;

	//aqui se guardan los nombres, descripciones y precios de los objetos
	
	//nombres objetos
	std::vector<std::string> Curaciones = {
		"Hervovitalizante",
		"Hervovitalizante extremo"
	};
	std::vector<std::string> Debuffs = {
		"Bomba",
		"Cuchillo",
		"Poción de Muerte"
	};

	//descripciones
	std::vector<std::string> descripcionItemCategoriaesSanación = {
		"Devuelve un poco la salud. Su elaboracion es sencilla si eres aventurero pero nunca viene mal comprar de mas (cura 60 puntos de salud)",
		"Devuelve mucha vida casi que te sientes totalmente restaurado(cura 200 puntos de salud)"
	};
	std::vector<std::string> descripcionItemCategoriaesDebuffs = {
		"Aplica daño al enemigo (quita 60 puntos de vida al enemigo)",
		"Clava un cuchillo en el enemigo, generandole algo de danio (quita 100 puntos de vida al enemigo)",
		"Aplica mucho danioo al enemigo que lo puede conducir a la muerte (quita 200 puntos de vida al enemigo)"
	};

	//precios
	std::vector<int> precioItemCategoriaesSanación = {
		50,
		200
	};
	std::vector<int> precioItemCategoriaesDebuffs = {
		100,
		150,
		200
	};

	//efecto
	std::vector<int> efectoItemCategoriaesCuración = {
		100,
		200
	};
	std::vector<int> efectoItemCategoriaesDebuffs = {
		60,
		100,
		200
	};

public:
	//Metodos

	Categoria(int param_tipoItem) { //crea la categoria que la tienda pida
		switch (param_tipoItem) {
		case 1: //sanación
			for (int contador = 0; contador < Curaciones.size(); contador++) {
				nombreItemCategoria = Curaciones[contador];
				descripcionItemCategoria = descripcionItemCategoriaesSanación[contador];
				precioItemCategoria = precioItemCategoriaesSanación[contador];
				cantidadItemCategoria = generarNumeroAleatorio(20, 1);
				efectoItemCategoria = efectoItemCategoriaesCuración[contador];
				Item item(nombreItemCategoria, descripcionItemCategoria, precioItemCategoria, cantidadItemCategoria, efectoItemCategoria, 1);
				categoriaItem.push_back(item);
			}
			break;
		case 2: //debuffs
			for (int contador = 0; contador < Debuffs.size(); contador++) {
				nombreItemCategoria = Debuffs[contador];
				descripcionItemCategoria = descripcionItemCategoriaesDebuffs[contador];
				precioItemCategoria = precioItemCategoriaesDebuffs[contador];
				cantidadItemCategoria = generarNumeroAleatorio(20, 1);
				efectoItemCategoria = efectoItemCategoriaesDebuffs[contador];
				Item item(nombreItemCategoria, descripcionItemCategoria, precioItemCategoria, cantidadItemCategoria, efectoItemCategoria, 2);
				categoriaItem.push_back(item);
			}
			break;
		default:
			std::cout << "Error en constructor de categoria, este caso no existe" << std::endl;
			break;
		}
	}

	void mostrarItemsCategoria() { //muestra los objetos de la categoria
		for (int centinela = 0; centinela < categoriaItem.size(); centinela++) {
			std::cout << "Producto: " << centinela + 1 << std::endl;
			categoriaItem[centinela].mostrarItem();
		}
	}

	void comprarProducto(int param_tipoProducto, Jugador& param_statsJugador) { //permite comprar un producto
		if (param_tipoProducto - 1 < categoriaItem.size()) { //verifica que sea menor al tamaño de la categoria
			if (categoriaItem.at(param_tipoProducto - 1).mostrarCantidadItem() <= 0) { //verifica que haya cantidad
				std::cout << "Lo siento, se me acabo este producto\n" << std::endl;
			}
			else {
				if (categoriaItem.at(param_tipoProducto - 1).mostrarPrecioItem() <= param_statsJugador.mostrarOro()) { //verifica que tengas dinero

					categoriaItem.at(param_tipoProducto - 1).sobrescribirCantidadItem(categoriaItem.at(param_tipoProducto - 1).mostrarCantidadItem() - 1);
					param_statsJugador.comprarItem(categoriaItem.at(param_tipoProducto - 1).mostrarPrecioItem());
					Item itemComprado = categoriaItem.at(param_tipoProducto - 1);
					itemComprado.sobrescribirCantidadItem(1);
					param_statsJugador.agregarItemAlInventario(itemComprado);

				}
				else {
					std::cout << "Lo siento pero no tienes dinero suficiente para comprar\n" << std::endl;
				}
			}
		}
	}
};


class Tienda { //clase de la tienda
	//datos
	std::vector<Categoria> stock;

	//metodos
public:
	Tienda() {
		for (int contador = 1; contador <= 2; contador++) { //crea las 2 categorias para añadirlas al stock de la tienda
			Categoria categoriasItems(contador);
			stock.push_back(categoriasItems);
		}
	}

	void mostrarItemsCategoria(int param_busqueda) { //permite mostrar los items de la categoria elegida
		stock[param_busqueda - 2].mostrarItemsCategoria();
	}

	void comprarProductoCategoria(int param_tipoCategoria, int param_tipoProducto, Jugador& param_statsJugador) { //permite comprar un producto de la categoria
		stock[param_tipoCategoria - 2].comprarProducto(param_tipoProducto, param_statsJugador);
	}
};


class Mundo {
	//datos
	int completado = 0;

public:
	//metodos
	Mundo() {
		aperturaHistoria();
		std::string nombreCustom;
		std::getline(std::cin, nombreCustom);
		Jugador	jugador(nombreCustom, 200, 20);

		//capitulo 1
		while (completado == 0) {
			acto1(jugador);
			if (completado != 1) {
				textoOmitible("Presiona cualquier tecla para volver a jugar...");
				(void)_getch(); 
				system("cls");
				jugador.curarse(jugador.verVidaMaxima());
			}
		}

		//capitulo 2
		completado = 0;
		while (completado == 0) {
			acto2(jugador);
			if (completado != 1) {
				textoOmitible("Presiona cualquier tecla para volver a jugar...");
				(void)_getch(); 
				system("cls");
				jugador.curarse(jugador.verVidaMaxima());
			}
		}


	}

	void modoTienda(Jugador& param_statsJugador){
		//Tienda
		int centinela = 0;
		std::cout << "Un gusto joven. tengo mucha mercancia el dia de hoy, no dude en comprar si lo necesita" << std::endl;
		Tienda tienda;
		do {
			std::cout << "Dime, que te gustaria ver?" << std::endl;
			std::cout << "0. Salir de la Tienda \n1. Ver tu inventario \n2. Curaciones \n3. Arrojadizos" << std::endl;
			centinela = ingresarNumero();
			if (centinela == 1) { //muestra el inventario si elegiste la opcion 1
				param_statsJugador.mostrarInventario();
			}
			else if (centinela > 1 && centinela <= 3) { //mientras no supere los rangos mantente alli 
				int eleccion = 0;
				do {
					tienda.mostrarItemsCategoria(centinela);
					std::cout << "deseas comprar algun producto?, (escribe 0 si no quieres comprar algo)" << std::endl;
					std::cout << "Tienes " << param_statsJugador.mostrarOro() << " Oro" << std::endl;
					eleccion = ingresarNumero();
					if (eleccion != 0) {
						tienda.comprarProductoCategoria(centinela, eleccion, param_statsJugador);
					}
				} while (eleccion != 0);
			}
		} while (centinela != 0);
		
	}

	int modoCombate(std::string param_nombreEnemigo, int param_vidaEnemigo, int param_danioEnemigo, int param_curacionEnemigo, Jugador& param_statsJugador) {
		Enemigo enemigo(param_nombreEnemigo, param_vidaEnemigo, param_danioEnemigo, 0, 1);
		std::cout << "Vida Jugador: " << param_statsJugador.verVida() << std::endl;
		std::cout << "Vida Enemigo: " << enemigo.verVida() << std::endl;
		while (enemigo.estaVivo() && param_statsJugador.estaVivo()) {

			//jugador
			param_statsJugador.opcionesCombateJugador(enemigo);
			system("cls");
			
			if (!enemigo.estaVivo()) { //esta muerto?
				std::cout << "Has derrotado a " << enemigo.verNombre() << std::endl;
				param_statsJugador.subirNivel();
				param_statsJugador.ganarOro(50);
				std::cout << "Presiona cualquier tecla para continuar..." << std::endl;
				(void)_getch(); 
				system("cls");
				return 1;
				break;
			}

			//Turneo Enemigo
			std::cout << "----Turno de " << enemigo.verNombre() << "----" << std::endl;
			//opciones de combate
			int eleccionEnemigo = enemigo.opcionesCombateEnemigo();
			switch (eleccionEnemigo) {

			case 1:
				std::cout << enemigo.verNombre() << " te ha hecho " << enemigo.verDanio() << " danio" << std::endl;
				param_statsJugador.recibirDanio(enemigo.verDanio());
				break;
			case 2:
				enemigo.curarse(param_curacionEnemigo);
				std::cout << "El enemigo se a curado" << std::endl;
				break;
			default:
				std::cout << "Este caso no existe en opciones de combate del enemigo Devmin" << std::endl;
				break;

			}
			//mostrar datos pelea
			std::cout << "Vida Jugador: " << param_statsJugador.verVida() << std::endl;
			std::cout << "Vida Enemigo: " << enemigo.verVida() << std::endl;
			if (!param_statsJugador.estaVivo()) { //esta muerto?
				std::cout << "Has sido derrotado" << std::endl;
				std::cout << "Presiona cualquier tecla para continuar..." << std::endl;
				(void)_getch(); 
				system("cls");
				return 0;
				break;
			}

		}
	}

	void aperturaHistoria() {
		textoOmitible("Guardian del tiempo: Tras liberar el Reloj de Arena y convertirme en Guardian del Tiempo, \nmi siguiente tarea fue reiniciar la historia. Libre de ataduras al pasado, tenia la libertad de crear una nueva era. \nCon humildad y autocontrol me acerque a este lienzo en blanco. Tras una cuidadosa preparacion, comence mi trabajo, \npintando sobre la oscuridad. Tras eones, esboce los reinos. Tras eones mas, les di vida. En mi nueva era, \ntodos los seres tendran la oportunidad de vivir en paz. Que lo aprovechen o no, sera su responsabilidad. \nMi poder solo me permite empezar la tarea. Es deber de los mortales terminarla.\n \nGuardian del tiempo: Dime, cual es tu nombre, mortal?\n");
	}

	void acto1(Jugador& param_statsJugador) {
		system("cls");
		int centinela = 0;

		textoOmitible("El sol comenzaba a ocultarse sobre los campos de Fengjian. El aire estaba impregnado del olor de la tierra humeda y el sonido de las herramientas golpeando el suelo se mezclaba con las respiraciones agotadas de los campesinos \n\n" + param_statsJugador.verNombre() + ": Esta es una buena cosecha, Kung Lao \n\nKung Lao: Si... *mmmph*... Es una bendicion, " + param_statsJugador.verNombre() + "\n\n1.Prefieres que nos muramos de hambre? \n2.Te preocupa algo? \n3.Te entiendo, a veces es agotador...\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Prefieres que nos muramos de hambre?\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Te preocupa algo?\n\n");
				centinela = 1;
				break;
			case 3:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Te entiendo, a veces es agotador...\n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Kung Lao: No, no. Por supuesto que no. Pero hemos trabajado estos campos desde que eramos ninios. Dentro de cuarenta anios seguiremos aqui, encorvados y debiles como el viejo Wei \n\nlogras ver a lo lejos al viejo wei con un rostro de amargura y disgusto hacia ustedes \n\nWei: Hmm? *escupe* \n\n1. Desde que te conozco, has soniado con irte de Fengjian. Por que no puedes ser feliz aqui? \n2.ja... yo tambien he querido irme de aqui\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Desde que te conozco, has soniado con irte de Fengjian. Por que no puedes ser feliz aqui?\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": ja... yo tambien he querido irme de aqui\n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Kung Lao: Nuestros antepasados lucharon en grandes guerras. Alcanzaron honor y gloria... y nosotros? \n\n" + param_statsJugador.verNombre() + ": Puede que no sea glorioso, pero lo que hacemos ayuda a que el pueblo prospere. \n\nKung Lao: Lo se. Pero acaso esta mal querer algo de emocion? \n\n" + param_statsJugador.verNombre() + ": Si eso es lo que quieres, te propongo una apuesta? Cena en el restaurante de la seniora Bo esta noche. El ultimo en llenar su carrito paga \n\nKung Lao: Trabajas mas rapido que yo? Ja! Desde cuando? Acepto el reto \n\nDespues de pasar toda la maniana recolectando coles llegan al restaurante de la seniora Bo \n\nPresiona cualquier tecla para continuar...");
		(void)_getch(); 
		system("cls");

		textoOmitible("El restaurante de Madam Bo estaba iluminado con calidas lamparas y lleno de vida. Apenas cruzaron la entrada, Kung Lao cerro los ojos y aspiro profundamente el aroma \n\nKung Lao: Este olor es increible \n\n" + param_statsJugador.verNombre() + ": Me muero de hambre. Me pregunto que estara preparando la seniora Bo esta noche \n\nSeniora Bo : Para ustedes? Lo que quieran \n\nKung Lao: Gracias, seniora Bo. Nos malcria \n\nLa mujer solto una pequenia risa \nSeniora Bo: Como los hijos que nunca tuve \n\nMientras servia el te, observo a ambos con seriedad \nSeniora Bo: Han estado practicando mis lecciones? \n\nKung Lao: Cuando podemos. La cosecha nos deja poco tiempo para las artes marciales \n\nSeniora Bo: Desde tus primeras palabras, Kung Lao, no has dicho mas que excusas. Si no te preparas... \n\nKung Lao: lo se... tus preparativos no funcionaran \n\nSeniora Bo: Bien. No lo has olvidado. Pero ya veremos si te preparo algo especial esta noche \n\n1. Me alegra que pagues tu. La seniora Bo esta tan enojada que seguro nos cobrara mas \n2. Por como lucia la seniora Bo suena a que te cobrara mas de lo normal\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Me alegra que pagues tu. La seniora Bo esta tan enojada que seguro nos cobrara mas\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Por como lucia la seniora Bo suena a que te cobrara mas de lo normal\n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Kung Lao : Yo pago? Tu terminaste el trabajo al ultimo \n\n" + param_statsJugador.verNombre() + ": Pero yo entregue mi carrito primero \n\nKung Lao: Entregado? Eh?! La apuesta era llenar\n\n" + param_statsJugador.verNombre() + ": Eso no es lo que recuerdo \n\nFinalmente, Kung Lao se levanto de golpe \nKung Lao: Asi que asi son las cosas, eh? Que te parece si resolvemos esto de otra forma? Hmm? \n\n1. Espera. Te refieres a pelear? \n2. Esto es para demostrarle a la seniora Bo que has estado entrenando?\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Espera. Te refieres a pelear?\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + "Esto es para demostrarle a la seniora Bo que has estado entrenando?\n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Kung Lao: Si. Aqui y Ahora. La seniora Bo puede observar y criticar nuestra tecnica \n\n" + param_statsJugador.verNombre() + ": Debemos tener cuidado. Nos echaran si rompemos algo \n\nKung Lao: (riendo) Esto no durara lo suficiente como para que eso suceda. Caeras en un abrir y cerrar de ojos\n\nContinuar...");
		(void)_getch(); 
		system("cls");

		switch (modoCombate("Kung Lao", 200, 20, 40, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("Kung Lao : Todavia no me has vencido \n\n" + param_statsJugador.verNombre() + ": El dia se acerca, Kung Lao \n\nKung Lao: *riendo* Sigue diciendote eso \n\nAl Kung Lao haber ganado la apuesta perdiste 50 piezas de oro \n\n\n");
			param_statsJugador.comprarItem(50);
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Parece que alguien tendra que pagar la cuenta \n\nKung Lao : *riendo* Se nota que has practicado\n\n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		param_statsJugador.curarse(param_statsJugador.verVidaMaxima());
		textoOmitible("A partir de aqui cuando pierdas significara derrota absoluta y deberas empezar el capitulo actual desde 0 \nPresiona cualquier tecla para continuar...");
		(void)_getch(); 
		system("cls");

		textoOmitible("Despues de la pelea, ambos comenzaron a devorar la comida servida por la seniora Bo \n\n" + param_statsJugador.verNombre() + ": Todo estaba delicioso, seniora Bo \n\nSeniora Bo: Siempre es un placer cocinar para ustedes \n\nTodo parecia tranquilo hasta que la puerta del restaurante se abrio violentamente. \nUn hombre vestido de negro avanzo lentamente entre el silencio del lugar. \n\nSmoke: Seniora Bo. Los Lin Kuei esperan su respuesta \n\nSeniora Bo : He estado ocupada, Smoke \n\n1. Los Lin Kuei? Quienes son? \n2. Seran triadas? \n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": los Lin Kuei? Quienes son?\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Seran triadas?\n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Kung Lao: No lo se. Pero no me gusta hacia donde va esto \n\n" + param_statsJugador.verNombre() + ": Que hacemos? \n\nKung Lao: Mantente preparado \n\nSmoke: Seniora Bo, la decision es sencilla. Pague por nuestra proteccion o arriesgue la integridad de este excelente establecimiento \n\nSeniora Bo: No obtendras nada de mi \n\nSmoke : Seniora Bo, tomo una mala decision \n\nDe repente, una espesa nube de humo cubrio todo el restaurante \n\nEntre el humo aparecieron varios guerreros misteriosos \n\nKung Lao: Seniora Bo! \n\nSmoke: Ustedes dos son valientes. Pero no pueden contra nosotros \n\nKung Lao: No nos importa quien seas ni con quien estes! No amenazas a la seniora Bo! \n\n" + param_statsJugador.verNombre() + ": Ayuda a los clientes Kung Lao, yo me encargo de el \n\nPresiona cualquier tecla para continuar...");
		(void)_getch(); 
		system("cls");

		switch (modoCombate("Smoke", 60, 20, 0, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("observas impotente como saqueaban la casa de te. Has decepcionado tanto a la seniora Bo\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible("Kung Lao: Tu hiciste eso? \n\n" + param_statsJugador.verNombre() + ": Honestamente tambien me sorprende \n\nKung Lao: Vamos. Ayudemos a la seniora Bo \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Sub-Zero: Congelate donde estas! \n\nObservan como el hielo se extiende por el suelo \n\n" + param_statsJugador.verNombre() + ": Como lo hace? \n\nKung Lao: (preocupado) No tengo idea. \n\nSub-Zero: Te entrometes en los asuntos de Lin Kuei. Vete o enfrentaras nuestra ira \n\nKung Lao: Abandonar a la seniora Bo? No va a suceder \n\nSientes como una cadena abraza tu pierna \n\nEscorpion: (enojado) Ven aqui! \n\nEres arrastrado hasta donde esta el sujeto con la cadena y ves como desprende fuego de las manos \n\nEscorpion: Deberias haber huido cuando pudiste. Preparate para arrepentirte de haberte cruzado con los Lin Kuei \n\nPresiona cualquier tecla para continuar...");
		(void)_getch(); 
		system("cls");

		switch (modoCombate("Escorpion", 80, 30, 40, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("Cuando Scorpion abandono la casa de te saqueada, solo pudieron maravillarse con su habilidad. Kung Lao y tu se dieron cuenta de que la seniora Bo tenía razon: tienen mucho que aprender\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Tu seras quien se arrepienta de haberme desafiado \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Logras ver a lo lejos a Kung Lao congelado de las manos sin poder moverse mientras el hombre de hielo se acerca a ti \n\nSub-Zero: ya he tenido suficiente de ti \n\n" + param_statsJugador.verNombre() + ": Entonces vete, antes de que te acabe como al resto de tus Lin Kuei \n\nSub-Zero: No tienes ninguna posibilidad contra el gran maestro de los Lin Kuei \n\nPresiona cualquier tecla para continuar...");
		(void)_getch(); 
		system("cls");

		switch (modoCombate("Sub-Zero", 100, 40, 40, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("Despues de perder rotundamente ante Sub-Zero, te preguntas si alguna vez tuviste oportunidad contra el. Es evidente que todavia tienes mucho por aprender\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Interesante. Estas seguro de que eres un gran maestro? \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Kung Lao: " + param_statsJugador.verNombre() + ", por aqui! \n\nTe diriges para donde se encuentra Kung Lao y encuentras a Madam Bo inconsciente entre los restos del restaurante \n\n" + param_statsJugador.verNombre() + ": esta inconsciente \n\nKung lao: Oh no. No estara...? \n\nSeniora Bo : Muerta? Todavia no \n\nAntes de que pudieran reaccionar, una figura rodeada de fuego aparecio detras de ellos. Sus ojos brillaban como brasas ardientes \n\nLiu Kang : Entonces, se seniora Bo, estan listos? \n\nSeniora Bo : Un poco cabezones, tal vez... Pero estan listos \n\n1. Sus ojos... \n2. quien eres? \n3. Acaso eres otro Lin Kuei?\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Sus ojos...\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Quien eres?\n\n");
				centinela = 1;
				break;
			case 3:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Acaso eres otro Lin Kuei?\n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Liu Kang: Soy el Senior Liu Kang, Dios del Fuego y Protector del Reino de la tierra \n\nKung Lao: Dios? Reino de la Tierra? \n\nLiu Kang: La seniora Bo los ha estado preparando para este momento desde que eran ninos. Hoy han demostrado ser dignos de unirse a mis campeones \n\n" + param_statsJugador.verNombre() + ": Esta pelea fue... una prueba? \n\nLiu Kang: De tu habilidad y caracter, si. Bi-Han, Kuai Liang, vengan \n\nKung Lao: Asi que estos dos no son matones? \n\nLiu Kang: El clan Lin Kuei es un clan centenario dedicado a la defensa del Reino de la Tierra \n\n" + param_statsJugador.verNombre() + ": Sigues diciendo Reino de la Tierra. Hablas del planeta? \n\nSeniora Bo: Tienen mucho que aprender \n\nLiu Kang: La Tierra es uno de muchos reinos que existen. Juntos conforman el universo \n\nSeniora Bo: Los reinos pueden ser feroces rivales. Por eso necesitamos campeones que defiendan el nuestro \n\nLiu Kang: Se acerca el gran torneo de artes marciales entre el Reino de la Tierra y el Reino del Mundo Exterior. Celebrado una vez cada siglo, permite a cada reino demostrar su poderio. Si bien nuestros reinos estan en paz, hay habitantes del Mundo Exterior que preferirian que estuvieramos en guerra. Nuestra victoria en el torneo... apaciguara su fervor \n\nSeniora Bo: les he enseniado todo lo que he podido. Deben terminar su entrenamiento con el Senior Liu Kang \n\nLiu Kang: Vengan. Los monjes de la Academia Wu Shi los esperan para continuar su formacion \n\nKung Lao: Estoy listo, senior Liu Kang \n\nLiu Kang: Y tu, Raiden? \n\n1. Lo entiendo, ire con usted senior Liu Kang \n2. Dejar Fengjian? Me necesitan aqui \n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Lo entiendo, ire con usted senior Liu Kang\n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Dejar Fengjian? Me necesitan aqui \n\nSeniora Bo: El Reino de la Tierra te necesita, Raiden. La mejor manera de servir a la aldea es convirtiendote en uno de sus campeones \n\n" + param_statsJugador.verNombre() + ": Lo entiendo \n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Excelente. No hay tiempo que perder, partimos en la maniana asi que preparense \n\nAntes de que se fueran a la academia Wu Shi fueron a comprar unos suministros para el viaje \n\nPresiona cualquier tecla para continuar...");
		(void)_getch(); 
		system("cls");
		modoTienda(param_statsJugador);

		system("cls");
		std::cout << "Estado actual del jugador" << std::endl;
		param_statsJugador.mostrarInfoJugador();
		std::cout << "Presiona cualquier tecla para iniciar" << std::endl;
		(void)_getch(); 

		completado = 1;

	}

	void acto2(Jugador& param_statsJugador) {
		param_statsJugador.curarse(param_statsJugador.verVidaMaxima());
		system("cls");
		int centinela = 0;

		textoOmitible("En la Academia Wu Shi, Raiden y Kung Lao conocieron a Johnny Cage y Kenshi quienes se conocieron cuando kenshi intento entrar a arrebatarle la sento, una katana del clan Taira, la cual Kenshi buscaba liberar a su clan con la misma katana para luego ambos ser irrumpidos por Liu Kang para que se unieran a los campeones de la tierra. Meses despues los 4 llevan entrenando con los monjes shaolin diligentemente en preparacion para el proximo torneo contra el mundo exterior. Mientras tanto Kung Lao porfin habia perfeccionado su arma el sombrero de hoja el cual iba a presumirselo a " + param_statsJugador.verNombre() + "\n\nKung lao: Es una idea brillante, la proxima vez que vea a Sub-Zero tendre que agradecerle por inspirarme \n\nKung Lao se veia orgulloso de su extranio sobrero metalico \n\n1. Esta no es una mala idea \n2. Estas seguro que es buena idea? \n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Esta no es una mala idea \n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Estas seguro que es buena idea? \n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Kung Lao lanzo el sombrero contra uno de los muniecos de entrenamiento. El arma atraveso completamente el objetivo antes de regresar girando a sus manos \n\nKung Lao: Tal vez sea demasiado efectivo \n\nCerca de ellos, Johnny Cage observaba aburrido mientras descansaba sobre unas escaleras \n\nJohnny Cage: Tenemos que salir de aqui pronto. Llevo meses encerrado entrenando. Como voy a crear la mejor pelicula de la historia sobre el mundo exterior si nunca lo he visto? \n\nJohnny nos conto que era actor famoso del cine y que acepto formar parte de los campeones por la idea de filmar una pelicula baasada en el mundo exterior y el torneo. tambien nos conto que habia tenido una mala racha en el cine ultimamente y que por eso hacia esto. pero como provengo de un Fengjian, un pueblo alejado de la ciudad no estoy muy enterado de lo que habla... \n\nKenshi lo observaba con evidente fastidio \n\nKenshi: Por eso estas aqui? \n\nJohnny Cage: Liu Kang dijo que este trabajo cambiaria el rumbo de mi vida. Asi es como se hacen las cosas \n\nKenshi : Que desinteresado \n\nJohnny Cage : Oh, por favor, Tatuajes. Solo estas aqui para recuperar tu espada \n\n" + param_statsJugador.verNombre() + ": Todos tenemos nuestras razones. Pero no estamos aqui para pelear entre nosotros. Estamos aqui para defender la tierra \n\nEn ese instante, un enorme gong resono por toda la academia. Liu Kang aparecio frente a ellos acompaniado por varios monjes \n\nJohnny Cage: Ya era hora \n\nLiu Kang: Hoy uno de ustedes sera elegido para representar al Reino Terrenal en el torneo\n\n" + param_statsJugador.verNombre() + ": solo uno de nosotros luchara?\n\nLiu Kang: Las reglas del torneo otorgan esa ventaja al reino anfitrion. Y ahora le toca al mundo exterior ser el anfitrion \n\nJohnny Cage: Pregunta rapida... cuantas veces hemos ganado esto? \n\nLiu Kang: El Reino de la Tierra y el Mundo Exterior han estado igualados durante mucho tiempo. Pero el Mundo Exterior esta ganando fuerza. y si ganan quienes desean la guerra se sentiran libres de actuar \n\n1. Creia que admirabas al mundo exterior \n2. Suena que es un lugar clandestino \n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Creia que admirabas el mundo exterior \n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Suena que es un lugar clandestino \n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Liu Kang: Es un lugar de gran conocimiento, riqueza y belleza. Pero nuestros reinos no comparten objetivos ni creencias. Coexistimos pacificamente porque el mundo exterior respeta nuestra fuerza. Si mostraramos debilidad... nuestro rival se convertiria en nuestro enemigo \n\nKung Lao apretaba los punios \n\nKung Lao: No perdere, senior Liu Kang \n\nLiu Kang: Primero deberan ganar el derecho a representarnos. Quien de ustedes resista mas sera elegido. Raiden, da un paso al frente \n\nJohnny Cage: (rie) Buena idea. Primero hay que deshacerse de los debiles \n\nLiu Kang: Tendras que enfrentarte a el, Johnny Cage \n\nKenshi: Listo para tu primer acercamiento? \n\nJohnny Cage: Muy bien, hagamoslo. Pero recuerda, muchacho de granja, soy un icono mundial de las artes marciales. No creo que puedas soportar tanto estruendo \n\nPresiona cualquier tecla para continuar...");
		(void)_getch();
		system("cls");

		switch (modoCombate("Johnny Cage", 180, 30, 40, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("A pesar del feroz esfuerzo y la practica diligente, Te quedas corto. Sin embargo, tu fracaso no fue una derrota. Solo el primer paso a algo mas grande\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Esta pelea fue un honor \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Liu Kang: Kenshi Takahashi, tu eres el siguiente \n\nKenshi : Tienes talento, Raiden, pero te falta experiencia \n\n" + param_statsJugador.verNombre() + ": Cualquier experiencia que me falte, la compenso con mi corazon \n\nKenshi: Lo que gana las peleas es el coraje, no el corazon \n\nAntes de comenzar el combate decides tomar un poco de aire para recuperar algo de fuerzas (recuperaste 60pts de salud) \n\npresiona cualquier tecla para continuar...");
		param_statsJugador.curarse(60);
		(void)_getch();
		system("cls");

		switch (modoCombate("Kenshi Takahashi", 180, 40, 40, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("A pesar del feroz esfuerzo y la practica diligente, Te quedas corto. Sin embargo, tu fracaso no fue una derrota. Solo el primer paso a algo mas grande\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Todavia dudas de mi osadia? \n\nKenshi: No. La tuya fue una victoria bien merecida \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Liu Kang: Puedes descansar, Raiden. Esta noche te enfrentaras a tu ultimo oponente \n\nHas recuperado toda la vida \n\nCayendo la silencio cubrio el patio de entrenamiento mientras ambos amigos se observaban frente a frente \n\nJohnny Cage: Apuesto cincuenta dolares a que gana Kung Lao \n\nKenshi : Hazlo interesante. Apuesta por Sento \n\nJohnny Cage: Ja! Ni en suenios \n\nLiu Kang: Raiden. Kung Lao. El ganador de este combate representara a la tierra contra el mundo exterior. Es un deber grave que no debe tomarse a la ligera \n\nKung Lao: (rie) Rindete, Raiden. Nunca me has vencido en un combate de verdad. Eso no cambiara hoy \n\nRaiden: Si los monjes nos han enseniado algo... es que la unica constante en el universo es el cambio \n\nPresiona cualquier tecla para continuar...");
		param_statsJugador.curarse(param_statsJugador.verVidaMaxima());
		(void)_getch();
		system("cls");

		switch (modoCombate("Kung Lao", 300, 40, 60, param_statsJugador)) {
		case 0: //derrota
			textoOmitible("Aunque habia perdido, todavia estaba orgulloso de mi mejor amigo. Como nuevo campeon de la tierra, Kung Lao estaba ahora un gran salto mas cerca de la gloria que anhelaba\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Siempre hay una primera vez para todo, Kung Lao \n\nKung Lao bajo la mirada unos segundos antes de sonreir \n\nFelicidades, " + param_statsJugador.verNombre() + "\n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Liu Kang: Si bien todos ustedes han entrenado bien, Raiden ha sobresalido. Este resultado no me sorprende. Para el torneo, necesitaras esto\n\n");
		Item medallon("Medallon Electrico", "Forjado por los Dioses antiguos, otorga el poder del rayo y el trueno a quien lo porta (realiza 80 pts de danio)", 10000, 20, 80, 2);
		medallon.mostrarItem();
		param_statsJugador.agregarItemAlInventario(medallon);
		param_statsJugador.curarse(param_statsJugador.verVidaMaxima());

		textoOmitible("Observas fascinado la energia moviendose entre tus manos \n\nJohnny Cage: Santa Arca de la Alianza! \n\nLiu Kang: los habitantes del Mundo Exterior a los que te enfrentaras tambien manejan una magia poderosa. El amuleto equilibra la balanza \n\nPreiona cualquier tecla para continuar..."); 
		(void)_getch();
		system("cls");

		modoTienda(param_statsJugador);
		system("cls");

		textoOmitible("Siete dias despues, los campeones partieron hacia el mundo exterior.Cuando cruzaron el portal dimensional, quedan completamente maravillados \n\nJohnny Cage : Tatuajes(refiriendose a kenshi), tengo la sensación de que ya no estamos en Kansas \n\nFrente a ellos aparecieron las princesas del mundo exterior.Mileena y Kitana \n\nLiu Kang : Permatanme presentarles a Johnny Cage, Kung Lao, Kenshi Takahashi y al campeon del Reino de la Tierra... " + param_statsJugador.verNombre() + "\n\nMileena: Senior Liu Kang, bienvenido.Espero que esten preparados.Nuestros campeones estan decididos a ganar \n\nCriaturas gigantes caminaban por las calles.Guerreros de multiples brazos custodiaban enormes palacios dorados.Bestias imposibles recorrian la ciudad como si fueran algo normal \n\n1.Esos son centauros ? \n2.Los monjes los describieron.Pero verlos en persona...\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Esos son centauros? \n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Los monjes lo describieron. Pero verlos en persona... \n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Mileena: Los seres de seis brazos son Naknadans \n\nLiu Kang: Veo que Su Majestad, una vez mas, no escatima en gastos para el festival \n\nMileena: Es un homenaje a mi difunto padre \n\nLogras notar como una figura imponente de los Naknadans se acerca a ustedes \n\nJohnny Cage: Alerta roja. Se acerca el villano de serie B \n\nLiu Kang: General Shao. No dejes que te provoque \n\nGeneral Shao: (riendo) Soy yo, o el campeon de Earthrealm esta mas flaco de lo normal?. Destruiremos a tu campeon, Liu Kang. No saboreara la victoria \n\nLlegando al palacio de la emperatriz donde se realizara el torneo logras ver a una mujer frente al trono mostrando firmeza \n\nSindel : Bienvenidos, estimados invitados del Reino de la Tierra. Nos reunimos una vez mas para honrar el legado de mi difunto esposo. Para continuar el torneo que el fundo con Lord Liu Kang con la esperanza de que fomentara la paz entre los reinos. Que el alma de Jerrod nos vele con orgullo desde su lugar de descanso en el Bosque Viviente \n\nLiu Kang: Emperatriz Sindel. Me complace ser su huesped una vez mas \n\nSindel: Aqui siempre seras bienvenido. Ahora, conozcamos a tu campeon. \n\nLiu Kang : El campeon del Reino de la Tierra es " + param_statsJugador.verNombre() + ", Su Majestad. Se ha ganado su lugar al encarnar las mejores cualidades de la gente del Reino de la Tierra \n\nSindel : Pareces nervioso, jovencito \n\n1. Soy un extranio en tierra desconocida \n2. Estoy aqui para competir contra sus mejores luchadores \n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Soy un extranio en tierra desconocida. Asi que si estoy nervioso \n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Estoy aqui para competir contra sus mejores luchadores. Asi que si estoy nervioso \n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("Sindel: Como debe ser. Te espera un camino dificil. Ya ha comenzado! Como manda la tradicion, la primera rival del Mundo Exterior seráala Primera Alguacil de Sun Do... Li Mei. Que defienda la gloria de nuestro reino y preserve el orden de nuestra capital \n\nLi Mei: Su Majestad. Honrare tanto a la casa real como a todo el mundo exterior con mi combate \n\nLiu Kang: Lo unico que tienes que hacer es dar lo mejor de ti. El resto se solucionara solo \n\nLi Mei: Veamos que puedes hacer? \n\nPresiona cualquier tecla para continuar...");
		(void)_getch();
		system("cls");

		switch (modoCombate("Li Mei", 280, 40, 60, param_statsJugador)) {
		case 0: //derrota
			textoOmitible(param_statsJugador.verNombre() + ": Quede atonito al haber perdido el primer partido. No puedo evitar preguntarme si Liu Kang se habia equivocado al nombrarme campeon...\n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible("Sindel: Mis felicitaciones por un combate muy disputado. Veremos como te va en el proximo. General Shao. A quien has elegido como nuestro proximo competidor? \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("General Shao: Majestad, he elegido a Reiko como mi segundo al mando. De ninio, Reiko quedo huerfano durante la Guerra de Kafallah. Aunque fue capturado, su espíritu permanecio intacto. Lucho con unias y dientes y sobrevivio. Tras la guerra, lo acogi. Lo forme hasta convertirlo en el soldado perfecto. Pocos estan tan versados ​​en las artes de la guerra como yo \n\nReiko: Este sera tu ultimo campo de batalla \n\nPresiona cualquier tecla para continuar... \n\n");
		(void)_getch();
		system("cls");

		switch (modoCombate("Reiko", 300, 40, 60, param_statsJugador)) {
		case 0: //derrota
			textoOmitible(param_statsJugador.verNombre() + " quedo desconcertado por la satisfaccion burlona que obtuvo Reiko con su victoria. Como podria un hombre que nunca habia conocido considerarlo con tanto desden? \n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Derribado por un simple granjero. Estoy listo para mi proximo oponente, Su Majestad. A quien me enfrentare? \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Sindel: Paciencia. La sesion se aplaza hasta el amanecer. Espero verlos a todos en el banquete de esta noche \n\nEn el banquete de la noche todos se reunen para celebrar la llegada de los embajadores de la tierra \n\nSindel: Mi esposo Jerrod creia que el futuro de nuestros reinos residia en la union. Avancemos con un dialogo abierto, sin permitir que ningun secreto rompa nuestros lazos \n\nRaiden: Su Majestad. Es un honor estar aqui y conocer a su gente. Ya puedo ver que hay mas cosas que nos unen que las que nos dividen \n\nGeneral Shao: (risa burlona) Nuestros pueblos no podrian ser mas diferentes, campeon. Los habitantes de la Tierra carecen de todo... \n\nSindel: Ya basta, General \n\nGeneral Shao: Mis disculpas, Su Majestad. Quizas he disfrutado demasiado de este excelente vino. Con su permiso... \n\nSindel: Puedes irte \n\nTras ese incidente con el general Shao estuvimos festejando un rato más antes de que todos nos fueramos y prepararamos para el siguiente enfrentamiento \n\nPresiona cualquier tecla para continuar..."); 
		(void)_getch();
		system("cls");

		modoTienda(param_statsJugador);
		system("cls");
		
		textoOmitible("Sindel: Sigues sorprendiendome, Raiden.Solo quedan dos combates.a continuacion, te enfrentaras a mi hija Kitana ¿Estas preparado, habitante de la Tierra ? \n\nPresiona cualquier tecla para continuar...");
		param_statsJugador.curarse(param_statsJugador.verVidaMaxima());

		switch (modoCombate("Kitana", 340, 40, 80, param_statsJugador)) {
		case 0: //derrota
			textoOmitible(param_statsJugador.verNombre() + " se dio cuenta, demasiado tarde, de que habia subestimado a la princesa Kitana. Prometio no volver a dejarse seducir tanto por el encanto y la belleza \n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible(param_statsJugador.verNombre() + ": Luchas bien, Princesa \n\nKitana: Tu tambien, terricola. Sorprendentemente \n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}
		textoOmitible("Sindel: Ha llegado el momento del combate final. El hecho de que hayas llegado hasta aqui es una prueba de tu valia. Pero ahora debes enfrentarte al General Shao. Vencedor de la Guerra de Tervaria. Conquistador de las Llanuras de Kuatan. Defensor de la Costa de Navala. Al igual que su familia durante generaciones, el General Shao nos defiende con fervor y tenacidad. Todo el mundo exterior le agradece sus servicios \n\n1. Esa es una lista de logros notable, General \n2. Sera un honor pelear contra alguien con una lista de logros así de notable, General\n\n");
		do {
			switch (ingresarNumero()) {
			case 1:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Esa es una lista de logros notable, General \n\n");
				centinela = 1;
				break;
			case 2:
				textoOmitible("\n" + param_statsJugador.verNombre() + ": Sera un honor pelear contra alguien con una lista de logros así de notable, General \n\n");
				centinela = 1;
				break;
			default:
				centinela = 0;
				break;
			}
		} while (centinela == 0);

		textoOmitible("General Shao: Es solo una pequenia parte de ellos. Una recitacion completa llevaria dias \n\nEste sera tu ultimo combate por lo que procura darlo todo por el bien de la tierra \nPresiona cualquier tecla para continuar... \n\n");
		(void)_getch();
		system("cls");

		switch (modoCombate("General Shao", 400, 50, 80, param_statsJugador)) {
		case 0: //derrota
			textoOmitible(param_statsJugador.verNombre() + " nunca se sintio tan deprimido como cuando el general Shao reclamo la victoria. Como habia predicho Liu Kang, su exito animo al mundo exterior a actuar agresivamente contra la tierra... \n\n");
			return;
			break;
		case 1: //victoria
			textoOmitible("Sindel: Felicitaciones, " + param_statsJugador.verNombre() + ". La Tierra a ganado \n\nLiu Kang: Bien hecho! Has superado con creces mis mayores expectativas Gracias a ti, los Forasteros que intentan perturbar la paz volveran a estar a raya. Ven. Despidamonos de nuestros anfitriones y regresemos al Reino de la Tierra.\n\nTras tu victoria las cosas en el mundo exterior se han calmado aunque aun habia un invasor ahi afuera tanto tu como los demas podran encargarse de ello... tristemente la historia acaba aqui pero no te desanimes ganaste el torneo y detuviste a los que querian perturbar la paz por lo que completaste el juego!\n\n");
			break;
		default:
			std::cout << "Este caso no existe Devmin, error en los return de combate";
			break;
		}

		completado = 1;
	}
};


int main() {
	std::srand(std::time(NULL));
	Mundo aventura;

	return 0;

}