//
// Created by Tudor Oancea on 28/03/2021.
//

#include "gtest/gtest.h"
#include "Motif.hpp"

#include <filesystem>

using namespace std;
using namespace filesystem;

TEST(MotifTest, Construction) {
	ASSERT_NO_THROW(Motif M1);
	Motif M1;
	ASSERT_TRUE(M1.empty());
//	Construction avec segment aléatoire pour voir s'il n'y a pas d'erreurs lancées ==================================================
	std::cout << "Construction avec segment aléatoire pour voir s'il n'y a pas d'erreurs lancées" << std::endl;
	for (size_t i(0); i < 1000; ++i) {
		ASSERT_NO_THROW(Motif M(segment())) << "Erreur itération n°" << i;
	}
	std::cout << "✅" << std::endl;
//	Construction avec des segments aléatoires et vérification des valeurs ==================================================
	std::cout << "Construction avec des segments aléatoires et vérification des valeurs" << std::endl;
	for (size_t i(0); i < 1000; ++i) {
		Liste seg(segment());
		Motif M(seg);
		ASSERT_EQ(M.size(),seg.size()) << "taille du motif (" << M.size() << ") et du segment (" << seg.size() << ")  ajouté différentes ";
		for (size_t j(0); j < M.size(); ++j) {
			ASSERT_EQ(M[j],seg[j]) << "différence à la position " << j << " : motif (" << X(M[j]) << "," << Y(M[j]) << ") et segment (" << X(seg[j]) << "," << Y(seg[j]) << ")";
		}
	}
	std::cout << "✅" << std::endl;

//  Construction à partir de fichiers  ========================================

}

TEST(MotifTest, Access) {
	Motif* M;
	for (size_t i(0); i < 1000; ++i) {
		Liste seg(segment());
		M = new Motif(seg);
		ASSERT_EQ(seg.size(), M->size()) << "Le motif et le segment n'ont pas la même taille itération n°" << i;
		for (size_t j(0); j < M->size(); ++j) {
			ASSERT_EQ((*M)[j], seg[j]) << "cell différente à la position " << j << " : dans seg (" << X(seg[j]) << "," << Y(seg[j]) << ") et dans motif (" << X((*M)[j]) << "," << Y((*M)[j]) <<") (itération n°" << i;
		}
		delete M;
	}
}

TEST(MotifTest, Modification) {
	Motif* M;
//	Append
	std::cout << "test de la méthode append avec des segments aléatoires" << std::endl;
	for (size_t i(0); i < 1000; ++i) {
		M = new Motif;
		Liste seg1(segment());
		ASSERT_NO_THROW(M->append(seg1)) << "Erreur lorsqu'on append (itération n°" << i;
		delete M;
		M = new Motif;
		Liste seg(segment());
		M->append(seg);
		ASSERT_EQ(seg.size(), M->size()) << "Le motif et le segment n'ont pas la même taille itération n°" << i;
		for (size_t j(0); j < M->size(); ++j) {
			ASSERT_EQ((*M)[j], seg[j]) << "cell différente à la position " << j << " : dans seg (" << X(seg[j]) << "," << Y(seg[j]) << ") et dans motif (" << X((*M)[j]) << "," << Y((*M)[j]) <<") (itération n°" << i;
		}
	}
}