#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

#include "Game.h"
#include "raymath.h"


/*

	CÍL: Implementovat karetní hru, tahová, rougelite, pseudonáhodná generace nepřátel, herní mapy a karet, ukládání postupu.
	- jednoduchá herní plocha
	- interaktivní prostředí
	- generace balíčku karet
	- generace nepřátel
	- vytvoření základního souboje
	- akce nepřátel - dmg
	- akce karet - dmg
	- odměny za poražení
	- efekty pro konec tahu -> negativní / pozitivní jak pro karty tak enemy
	- více akcí karet / nepřátel
	- interakce mezi kartami / nepřáteli 
	- schopnosti pro kolo 0
	- vizualizace boje pro přehlednost
	- herní plocha - výpis textu, ošetření kolizí, vykreslování, animce
	- karty - výpis textu, ošetření kolizí, vykreslování, uložení stavu, animace
	- mapa - PSN generace, interaktivnost, typy místností, animace
	- nepřátelé - stejný princip jako karty, načítání přes JSON parser
	- ukládání stavu hry do JSON souboru, odemykání karet
	- možnost vytvoření balíčku karet pro "playstyle" 
	- obchod / loot místnost -> nákup / získání nových karet, odemknutí karet pro další kola
	- encounters - náhodná dobrá / špatná akce, implementace nového typu místnosti
	- vytvoření předmětů - aktivní, pasivní, jednorázové -> nutnost implementovat nový systém napříč x třídami
	- epický / boss místnosti -> psudonáhodné / pevně určené speciální místnosti
	- vytvoření spritů
	- animace
	- zvětšení "databáze"
	- bugfixy
	
	-Konečný stav: karetní hra s možností tvořit balíčky karet ovlivnňující průběh hry, získávání nových karet, předmětů a odemykání alternativních cest a možností dokončit hru, implementace boss a elitních postav, rozlíčné schopnosti typu 
	, freeze, boost etc napříč herní plochou, interakce mezi nepřáteli, kartami. funkční parodie na "databázi", místnosti s možností nákupu, voleb, vylepšování etc.
*/

int main(void) {


	enum gameState {
		ROOM,
		MAP
	};
	gameState state = ROOM;


	static const int windowWidth = 1920;
	static const int windowHeight = 1000;
	SetTargetFPS(60);
	InitWindow(windowWidth, windowHeight, "Test, test: I am alive!");

	Game game;
	game.create();

	while (!WindowShouldClose()) {
		// update: 
		game.update();
		// draw: 
		BeginDrawing();
		game.draw();

		EndDrawing();
	}
}

/*
	efekt po nezabití enemy, screen s odměnou za ukončení kola, dodělat obchod, zákl stats screen, imolementovat heal / mana recharge, start armor staty pro hráče
*/

/*
TODO: dokončit schopnosti enemy classy

KONEC:
	5.12 -> enemy vidí sousedy dle indexu, vybarvení zeleně, nastavení aktivního stavu dle kliknuti (předělat na akitvní stav kola), protikolizní opatření na cardShape, prozatím plně funkční, potřeba opravit I shape, jen lehkej offset dolu nebo nahoru o size nebo size/2

*/