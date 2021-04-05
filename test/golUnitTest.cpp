//
// Created by Tudor Oancea on 28/03/2021.
//

#include <gtest/gtest.h>
#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "termcolor.hpp"

#include <filesystem>
#include <random>
using namespace std;

// Tests for class GameOfLife ============================================================================================

TEST(GameOfLifeTest, Construction) {
	auto G(new GameOfLife);
	delete G;
	static GameOfLife G2;
	Motif* M;
	for (size_t i(0); i < 5; ++i) {
		std::cerr << termcolor::red << "Essai n° " << i <<  termcolor::reset << std::endl;
		M = new Motif(segment());
		M->append(segment());
		G = new GameOfLife(*M);
		delete G;
		delete M;
	}
	G = nullptr;
	M = nullptr;
}

TEST(GameOfLifeTest, AddCell) {
//	Setup des objets ====================================================================================
	auto game(new GameOfLife);
	std::unique_ptr<Motif> motif;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES-1, MAX_LIGNES-1));
	size_t i,j;
	
//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées" << std::endl;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i,j));
	}
	delete game;
	
//	test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé ==========================================
	std::cout << "test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k)  {
		i = distrib(gen);
		j = distrib(gen);
		bool existeAvant((std::find<Liste::const_iterator, Coord>(game->vivantes().cbegin(), game->vivantes().cend(), {i+50,j+50})) != game->vivantes().cend());
		bool ajoute(game->addCell(i,j));
		bool existeApres(std::find<Liste::const_iterator, Coord>(game->vivantes().cbegin(), game->vivantes().cend(), {i+50,j+50}) != game->vivantes().cend());
		if (existeAvant) {
			if (ajoute) {
				FAIL() << "La cell " << i << "," << j << " est marquée ajoutée alors qu'elle existait déjà (itération " << k;
			}
			if (!existeApres) {
				FAIL() << "La cell " << i << "," << j << " n'existe plus c'est zarb (itération " << k;
			}
		} else {
			if (!ajoute) {
				FAIL() << "La cell " << i << "," << j << " n'existait pas avant mais n'est pas marquée ajoutée (itération " << k;
			}
			if (k > 0 && !existeApres) {
				FAIL() << "La cell " << i << "," << j << " n'existe pas après l'ajout (itération " << k;
			}
		}
		if (ajoute) {
			ASSERT_EQ(game->vivantes().back(), std::make_pair(i+50,j+50)) << "La cell " << i << "," << j << " n'est pas là où il faut (itération " << k;
		}
	}
	delete game;
	
//	test sur des cas limites ====================================================================================
	std::cout << "test sur des cas limites" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i, MAX_COLONNES));
		ASSERT_NO_THROW(game->addCell(MAX_LIGNES,j));
	}
}

TEST(GameOfLifeTest, DeleteCell) {
//	Setup des objets ====================================================================================
	auto game(new GameOfLife);
	std::unique_ptr<Motif> motif;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES-1, MAX_LIGNES-1));
	size_t i,j;

//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées" << std::endl;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		game->addCell(i,j);
	}
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->deleteCell(i,j));
	}
	delete game;

//	test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé ==========================================
//	std::cout << "test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé" << std::endl;
//	game = new GameOfLife;
//	for (size_t k(0); k < 1000; ++k) {
//		i = distrib(gen);
//		j = distrib(gen);
//		ASSERT_NO_THROW(game->addCell(i,j));
//	}
//	while (!game->vivantes().empty()) {
//
//	}
//	for (size_t k(0); k < 1000; ++k)  {
//		i = distrib(gen);
//		j = distrib(gen);
//		bool existeAvant((std::find<Liste::const_iterator, Coord>(game->vivantes().cbegin(), game->vivantes().cend(), {i+50,j+50})) != game->vivantes().cend());
//		bool ajoute(game->addCell(i,j));
//		bool existeApres(std::find<Liste::const_iterator, Coord>(game->vivantes().cbegin(), game->vivantes().cend(), {i+50,j+50}) != game->vivantes().cend());
//		if (existeAvant) {
//			if (ajoute) {
//				FAIL() << "La cell " << i << "," << j << " est marquée ajoutée alors qu'elle existait déjà (itération " << k;
//			}
//			if (!existeApres) {
//				FAIL() << "La cell " << i << "," << j << " n'existe plus c'est zarb (itération " << k;
//			}
//		} else {
//			if (!ajoute) {
//				FAIL() << "La cell " << i << "," << j << " n'existait pas avant mais n'est pas marquée ajoutée (itération " << k;
//			}
//			if (k > 0 && !existeApres) {
//				FAIL() << "La cell " << i << "," << j << " n'existe pas après l'ajout (itération " << k;
//			}
//		}
//		if (ajoute) {
//			ASSERT_EQ(game->vivantes().back(), std::make_pair(i+50,j+50)) << "La cell " << i << "," << j << " n'est pas là où il faut (itération " << k;
//		}
//	}
//	delete game;

//	test sur des cas limites ====================================================================================
	std::cout << "test sur des cas limites" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i, MAX_COLONNES));
		ASSERT_NO_THROW(game->addCell(MAX_LIGNES,j));
	}
}
// Tests for class GameOfLifeView ============================================================================================
TEST(GameOfLifeViewTest, Construction) {
	auto G(new GameOfLifeView);
	delete G;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES, MAX_LIGNES));
	size_t lmin, lmax, cmin, cmax;
	for (size_t i(0); i < 5; ++i) {
		lmin =  distrib(gen);
		lmax = distrib(gen);
		cmin = distrib(gen);
		cmax = distrib(gen);
		std::cerr << termcolor::red << "Essai n° " << i << std::endl << "lmin : " << lmin << " | lmax : " << lmax << " | cmin : " << cmin << " | cmax :" << cmax <<  termcolor::reset << std::endl;
		G = new GameOfLifeView(lmin, lmax, cmin, cmax);
		delete G;
	}
	G = nullptr;
}
// Tests for class Simulation ============================================================================================