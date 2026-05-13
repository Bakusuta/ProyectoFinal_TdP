#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

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

	//metodos
public:
	Enemigo(std::string param_nombreEnemigo, int param_vidaEnemigo, int param_danioEnemigo, int param_experienciaEnemigo, int param_nivelEnemigo) : Personaje(param_nombreEnemigo, param_vidaEnemigo, param_danioEnemigo, param_experienciaEnemigo, param_nivelEnemigo) {} 


};

//clase jugador, hijo de personaje
class Jugador : public Personaje {
	//datos
	std::vector<Item> inventarioPersonal;
	int oro = 500;

	//metodos
public:
	Jugador(std::string param_nombreJugador, int param_vidaJugador, int param_danioJugador) : Personaje(param_nombreJugador, param_vidaJugador, param_danioJugador, 0, 1) {} //el jugador siempre se inicia en nivel 1 con experiencia 0

	void comprarItem(int param_compra) { //metodo que resta el dinero que gastaste en comprar
		oro = oro - param_compra;
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
		std::cout << "----Turno del jugador----" << std::endl;
		std::cout << "Dime, que te gustaria hacer?" << std::endl;
		std::cout << "0. atacar \n1. Ver tu inventario" << std::endl;
		int opcionMenu = ingresarNumero();
		switch (opcionMenu) {
		case 0:
			//atacar --> hacer daño / recibir daño
			param_statsEnemigo.recibirDanio(verDanio());
			break;
		case 1:
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
				}
			}
			break;
		default: 
			break;
		}
		//mostar datos pelea 
		std::cout << "Vida enemigo: " << param_statsEnemigo.verVida() << std::endl;
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


int main() {
	std::srand(std::time(NULL));

	std::string nombreCustom;
	int centinela = 0;

	//entrada del jugador
	std::cout << "Bienvenido a mi tienda de pociones aventurero, dime como te llamas?" << std::endl;
	std::getline(std::cin, nombreCustom);
	Jugador	jugador(nombreCustom, 200, 20);

	//Tienda
	std::cout << "Un gusto joven. tengo mucha mercancia el dia de hoy, no dude en comprar si lo necesita" << std::endl;
	Tienda tienda;
	do {
		std::cout << "Dime, que te gustaria ver?" << std::endl;
		std::cout << "0. Salir de la Tienda \n1. Ver tu inventario \n2. Curaciones \n3. Arrojadizos" << std::endl;
		centinela = ingresarNumero();
		if (centinela == 1) { //muestra el inventario si elegiste la opcion 1
			jugador.mostrarInventario();
		}
		else if (centinela > 1 && centinela <= 3) { //mientras no supere los rangos mantente alli 
			int eleccion = 0;
			do {
				tienda.mostrarItemsCategoria(centinela);
				std::cout << "deseas comprar algun producto?, (escribe 0 si no quieres comprar algo)" << std::endl;
				std::cout << "Tienes " << jugador.mostrarOro() << " Oro" << std::endl;
				eleccion = ingresarNumero();
				if (eleccion != 0) {
					tienda.comprarProductoCategoria(centinela, eleccion, jugador);
				}
			} while (eleccion != 0);
		}
	} while (centinela != 0);

	//caso hipotetico de combate para utilizar items
	Enemigo enemigo("Raiden", 150, 20, 0, 1);
	while (enemigo.estaVivo() && jugador.estaVivo()) {

		//jugador
		jugador.opcionesCombateJugador(enemigo);
		if (!enemigo.estaVivo()) { //esta muerto?
			std::cout << "Has derrotado al enemigo!!" << std::endl;
			break;
		}

		//Turneo Enemigo
		std::cout << "----Turno del enemigo----" << std::endl;
		//atacar
		int danioEnemigo = enemigo.verDanio();
		jugador.recibirDanio(danioEnemigo);
		//mostrar datos pelea
		std::cout << "Vida Jugador: " << jugador.verVida() << std::endl;
		if (!jugador.estaVivo()) { //esta muerto?
			std::cout << "Has sido derrotado" << std::endl;
			break;
		}

	}

	return 0;

}