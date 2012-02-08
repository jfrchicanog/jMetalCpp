//  ssGA_main.cpp
//
//  Author:
//       Esteban L�pez <esteban@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <Problem.h>
#include <Algorithm.h>
#include <Solution.h>
#include <RealSolutionType.h>
#include <SBXCrossover.h>
#include <PolynomialMutation.h>
#include <BinaryTournament.h>
#include <iostream>
#include <ssGA.h>
#include <time.h>
#include <ProblemFactory.h>

int main(int argc, char ** argv) {

  clock_t t_ini, t_fin;

  Problem   * problem   ; // The problem to solve
  Algorithm * algorithm ; // The algorithm to use
  Operator  * crossover ; // Crossover operator
  Operator  * mutation  ; // Mutation operator
  Operator  * selection ; // Selection operator

  if (argc>=2) {
    problem = ProblemFactory::getProblem(argc, argv);
  } else {
    cout << "No problem selected." << endl;
    cout << "Default problem will be used: Sphere" << endl;
//    char * defaultProblem;
//    strcpy(defaultProblem, "Fonseca");
    problem = ProblemFactory::getProblem("Sphere");
  }

  algorithm = new ssGA(problem); // Steady-state GA

  cout << "Steady-state genetic algorithm initialized." << endl;

  // Algorithm parameters
  int populationSize = 100;
  int maxEvaluations = 250000;
  algorithm->setInputParameter("populationSize",&populationSize);
  algorithm->setInputParameter("maxEvaluations",&maxEvaluations);

  // Mutation and Crossover for Real codification
  map<string, void *> parameters;
  double crossoverProbability = 0.9;
  double distributionIndex1 = 20.0;
  parameters["probability"] =  &crossoverProbability ;
  parameters["distributionIndex"] = &distributionIndex1 ;
  crossover = new SBXCrossover(parameters);

  parameters.clear();
  double mutationProbability = 1.0/problem->getNumberOfVariables();
  double distributionIndex2 = 20.0;
  parameters["probability"] = &mutationProbability;
  parameters["distributionIndex"] = &distributionIndex2 ;
  mutation = new PolynomialMutation(parameters);

  // Selection Operator
  parameters.clear();
  selection = new BinaryTournament(parameters) ;

  // Add the operators to the algorithm
  algorithm->addOperator("crossover",crossover);
  algorithm->addOperator("mutation",mutation);
  algorithm->addOperator("selection",selection);

  // Add the indicator object to the algorithm
  //algorithm->setInputParameter("indicators", indicators) ;

  // Execute the Algorithm
  t_ini = clock();
  SolutionSet * population = algorithm->execute();
  t_fin = clock();
  double secs = (double) (t_fin - t_ini);

  // Result messages
  cout << "Total execution time: " << secs << "s" << endl;
  cout << "Variables values have been written to file VAR" << endl;
  population->printVariablesToFile("VAR");
  cout << "Objectives values have been written to file FUN" << endl;
  population->printObjectivesToFile("FUN");

  delete crossover;
  delete mutation;
  delete selection;
  delete population;
  delete algorithm;

} // main
