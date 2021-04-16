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
//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées" << std::endl;
	for (size_t i(0); i < 1000; ++i) {
		M = new Motif(segment());
		M->append(segment());
		ASSERT_NO_THROW(G = new GameOfLife(*M)) << "Erreur à l'essai n° " << i;
		delete G;
		delete M;
	}
	std::cout << "✅" << std::endl;
	
//	test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées =======================================
	std::cout << "test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées" << std::endl;
	for (size_t i(0); i < 1000; ++i) {
		M = new Motif(segment());
		M->append(segment());
		G = new GameOfLife(*M);
		ASSERT_EQ(G->vivantes().size(), M->size()) << "taille du motif et des cellules vivantes différents (itération n° " << i;
		for (size_t k(0); k < G->vivantes().size(); ++k) {
			ASSERT_EQ(G->vivantes()[k], (*M)[k]) << "cellule mal placée : dans motif (" << X((*M)[k]) << "," << Y((*M)[k]) << ") et dans vivantes : " << X(G->vivantes()[k]) << "," << Y(G->vivantes()[k]) << ")";
		}
		delete G;
		delete M;
	}
	std::cout << "✅" << std::endl;
}

TEST(GameOfLifeTest, AddCell) {
//	Setup des objets ====================================================================================
	GameOfLife* game;
	Motif* motif;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES-1, MAX_LIGNES-1));
	size_t i,j;
	
//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées : " << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i,j));
	}
	delete game;
	std::cout << "✅" << std::endl;
	
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
	std::cout << "✅" << std::endl;
	
//	test sur des cas limites ====================================================================================
	std::cout << "test sur des cas limites" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i, MAX_COLONNES)) << "Erreur lors de l'ajout de (" << i << "," << MAX_COLONNES << ")";
		ASSERT_NO_THROW(game->addCell(MAX_LIGNES,j)) << "Erreur lors de l'ajout de (" << MAX_LIGNES << "," << j << ")";
	}
	delete game;
	std::cout << "✅" << std::endl;
	
//	test sur des Motifs entiers ======================================================================
	std::cout << "test sur des Motifs entiers pour voir s'il n'y a pas d'erreurs lancées :" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		motif = new Motif(segment());
		motif->append(segment());
		ASSERT_NO_THROW(game->addMotif(*motif)) << "Motif non ajoutée itération n°" << k;
		delete motif;
	}
	delete game;
	std::cout << "✅" << std::endl;
}

TEST(GameOfLifeTest, DeleteCell) {
//	Setup des objets ====================================================================================
	GameOfLife* game;
	Motif* motif;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES-1, MAX_LIGNES-1));
	size_t i,j;

//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées : " << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		game->addCell(i,j);
	}
	size_t size1(game->vivantes().size());
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->deleteCell(i,j)) << "La cell (" << i << "," << j << ") a rencontré un problème";
	}
	size_t size2(game->vivantes().size());
	std::cout << size1-size2 << " cells supprimées avec deleteCell()" << std::endl;
	ASSERT_NO_THROW(game->wipe()) << "La grille n'a pas pû être vidée";
	delete game;
	std::cout << "✅" << std::endl;

//	test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé ==========================================
	std::cout << "test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k)  {
		i = distrib(gen);
		j = distrib(gen);
		game->addCell(i,j);
		ASSERT_TRUE(game->deleteCell(i,j)) << "deleteCell(" << i << "," << j << ") a renvoyé false";
		ASSERT_EQ(game->vivantes().size(), 0) << "la cell (" << i << "," << j << ") a été ajoutée mais non supprimée";
	}
	delete game;
	std::cout << "✅" << std::endl;

//	test sur des cas limites ====================================================================================
	std::cout << "test sur des cas limites" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->deleteCell(i,MAX_COLONNES)) << "Erreur lors de la suppression de (" << i << "," << MAX_COLONNES << ")";
		ASSERT_NO_THROW(game->deleteCell(MAX_LIGNES,j)) << "Erreur lors de la suppression de (" << MAX_LIGNES << "," << j << ")";
	}
	delete game;
	std::cout << "✅" << std::endl;

//	test sur des Motifs entiers ======================================================================
	std::cout << "test sur des Motifs entiers pour voir s'il n'y a pas d'erreurs lancées :" << std::endl;
	game = new GameOfLife;
	for (size_t k(0); k < 1000; ++k) {
		motif = new Motif(segment());
		motif->append(segment());
		game->addMotif(*motif);
		ASSERT_NO_THROW(game->deleteMotif(*motif)) << "Motif non supprimé itération n°" << k;
		delete motif;
	}
	delete game;
	std::cout << "✅" << std::endl;
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
		ASSERT_NO_THROW(G = new GameOfLifeView(lmin, lmax, cmin, cmax)) << "echec avec lmin : " << lmin << " | lmax : " << lmax << " | cmin : " << cmin << " | cmax :" << cmax;
		delete G;
	}
	G = nullptr;
}

TEST(GameOfLifeViewTest, AddCell) {
	//	Setup des objets ====================================================================================
	GameOfLifeView* game;
	Motif* motif;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES-1, MAX_LIGNES-1));
	size_t i,j;
	
	//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées : " << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i,j));
	}
	delete game;
	std::cout << "✅" << std::endl;
	
	//	test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé ==========================================
	std::cout << "test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé" << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k)  {
		i = distrib(gen);
		j = distrib(gen);
		bool existeAvant((std::find<Liste::const_iterator, Coord>(game->vivantes().cbegin(), game->vivantes().cend(), {i,j})) != game->vivantes().cend());
		bool ajoute(game->addCell(i,j));
		bool existeApres(std::find<Liste::const_iterator, Coord>(game->vivantes().cbegin(), game->vivantes().cend(), {i,j}) != game->vivantes().cend());
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
			ASSERT_EQ(game->vivantes().back(), std::make_pair(i,j)) << "La cell " << i << "," << j << " n'est pas là où il faut (itération " << k;
		}
	}
	delete game;
	std::cout << "✅" << std::endl;
	
	//	test sur des cas limites ====================================================================================
	std::cout << "test sur des cas limites" << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->addCell(i, MAX_COLONNES)) << "Erreur lors de l'ajout de (" << i << "," << MAX_COLONNES << ")";
		ASSERT_NO_THROW(game->addCell(MAX_LIGNES,j)) << "Erreur lors de l'ajout de (" << MAX_LIGNES << "," << j << ")";
	}
	delete game;
	std::cout << "✅" << std::endl;
	
	//	test sur des Motifs entiers ======================================================================
	std::cout << "test sur des Motifs entiers pour voir s'il n'y a pas d'erreurs lancées :" << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k) {
		motif = new Motif(segment());
		motif->append(segment());
		ASSERT_NO_THROW(game->addMotif(*motif)) << "Motif non ajoutée itération n°" << k;
		delete motif;
	}
	delete game;
	std::cout << "✅" << std::endl;
}

TEST(GameOfLifeViewTest, DeleteCell) {
//	Setup des objets ====================================================================================
	GameOfLifeView* game;
	Motif* motif;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distrib(0, std::min(MAX_COLONNES-1, MAX_LIGNES-1));
	size_t i,j;

//	test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées ==========================================
	std::cout << "test sur échantillon aléatoire pour voir s'il n'y a pas d'erreurs lancées : " << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		game->addCell(i,j);
	}
	size_t size1(game->vivantes().size());
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->deleteCell(i,j)) << "La cell (" << i << "," << j << ") a rencontré un problème";
	}
	size_t size2(game->vivantes().size());
	std::cout << size1-size2 << " cells supprimées avec deleteCell()" << std::endl;
	ASSERT_NO_THROW(game->wipe()) << "La grille n'a pas pû être vidée";
	delete game;
	std::cout << "✅" << std::endl;

//	test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé ==========================================
	std::cout << "test sur échantillon aléatoire pour voir si les bonnes cellules sont ajoutées et si le bon bool est renvoyé" << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k)  {
		i = distrib(gen);
		j = distrib(gen);
		game->addCell(i,j);
		ASSERT_TRUE(game->deleteCell(i,j)) << "deleteCell(" << i << "," << j << ") a renvoyé false";
		ASSERT_EQ(game->vivantes().size(), 0) << "la cell (" << i << "," << j << ") a été ajoutée mais non supprimée";
	}
	delete game;
	std::cout << "✅" << std::endl;

//	test sur des cas limites ====================================================================================
	std::cout << "test sur des cas limites" << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k) {
		i = distrib(gen);
		j = distrib(gen);
		ASSERT_NO_THROW(game->deleteCell(i,MAX_COLONNES)) << "Erreur lors de la suppression de (" << i << "," << MAX_COLONNES << ")";
		ASSERT_NO_THROW(game->deleteCell(MAX_LIGNES,j)) << "Erreur lors de la suppression de (" << MAX_LIGNES << "," << j << ")";
	}
	delete game;
	std::cout << "✅" << std::endl;

//	test sur des Motifs entiers ======================================================================
	std::cout << "test sur des Motifs entiers pour voir s'il n'y a pas d'erreurs lancées :" << std::endl;
	game = new GameOfLifeView;
	for (size_t k(0); k < 1000; ++k) {
		motif = new Motif(segment());
		motif->append(segment());
		game->addMotif(*motif);
		ASSERT_NO_THROW(game->deleteMotif(*motif)) << "Motif non supprimé itération n°" << k;
		delete motif;
	}
	delete game;
	std::cout << "✅" << std::endl;
}
// Tests for class Simulation ============================================================================================