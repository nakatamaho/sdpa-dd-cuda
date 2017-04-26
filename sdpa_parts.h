/* -------------------------------------------------------------

This file is a component of SDPA
Copyright (C) 2004 SDPA Project

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

------------------------------------------------------------- */

#ifndef __sdpa_parts_h__
#define __sdpa_parts_h__

#include <sdpa_newton.h>


namespace sdpa {

class Newton;

class Solutions;
class InputData;
class Residuals;
class WorkVariables;

class ComputeTime;
class Parameter;
class StepLength;
class DirectionParameter;
class Switch;
class RatioInitResCurrentRes;
class SolveInfo;
class Phase;
class AverageComplementarity;


class ComputeTime
{
public:
  double Predictor;
  double Corrector;
  double StepPredictor;
  double StepCorrector;
  double xMatTime;
  double zMatTime;
  double invzMatTime;
  double xMatzMatTime;
  double EigxMatTime;
  double EigzMatTime;
  double EigxMatzMatTime;
  double makerMat;
  double makebMat;
  double B_DIAG;
  double B_F1;
  double B_F2;
  double B_F3;
  double B_PRE;
  double makegVecMul;
  double makegVec;
  double choleskybMat;
  double solve;
  double sumDz;
  double makedX;
  double symmetriseDx;
  double makedXdZ;
  double updateRes;
  double MainLoop;
  double FileRead;
  double FileCheck;
  double FileChange;
  double TotalTime;
  ComputeTime();
  ~ComputeTime();
  void display(FILE* fpout=stdout);
};

class Parameter
{
public:
  enum parameterType {PARAMETER_DEFAULT,
		      PARAMETER_UNSTABLE_BUT_FAST,
		      PARAMETER_STABLE_BUT_SLOW};
  int    maxIteration;
  double epsilonStar;
  double lambdaStar;
  double omegaStar;
  double lowerBound;
  double upperBound;
  double betaStar;
  double betaBar;
  double gammaStar;
  double epsilonDash;
  Parameter();
  Parameter(FILE* parameterFile);
  ~Parameter();
  void setDefaultParameter(parameterType type
			   = PARAMETER_DEFAULT);
  void readFile(FILE* parameterFile);
  void display(FILE* fpout=stdout);
};

class StepLength
{
public:

  dd_real primal;
  dd_real dual;
  StepLength();
  StepLength(dd_real alphaP, dd_real alphaD, int nBlock,
	      int* blockStruct);
  ~StepLength();
  void initialize(dd_real alphaP, dd_real alphaD);
  void terminate();
  
  static dd_real minBlockVector(BlockVector& aVec);

  void computeStepLength(Solutions& currentPt,
			 Newton& newton,
			 WorkVariables& work,
			 ComputeTime& com);
  void MehrotraPredictor(InputData& inputData,
			 Solutions& currentPt, 
			 Phase& phase,
			 Newton& newton,
			 WorkVariables& work,
			 ComputeTime& com);
  void MehrotraCorrector(InputData& inputData,
			 Solutions& currentPt, Phase& phase,
			 Switch& reduction, Newton& newton,
			 AverageComplementarity& mu,
			 RatioInitResCurrentRes& theta,
			 WorkVariables& work,
			 Parameter& param, ComputeTime& com);
  void display(FILE* fpout = stdout);
};

class DirectionParameter
{
public:
  dd_real value;
  DirectionParameter(dd_real betaStar=0.0);
  ~DirectionParameter();
  void initialize(dd_real betaStar=0.0);
  
  void MehrotraPredictor(Phase& phase, Switch& reduction,
			 Parameter& param);
  void MehrotraCorrector(Phase& phase, StepLength& alpha,
			 Solutions& currentPt, Newton& newton,
			 AverageComplementarity& mu,
			 Parameter& param);
  void display(FILE* fpout = stdout);
};

class Switch
{
public:
  enum SwitchType {ON,OFF};
  SwitchType switchType;

  Switch(SwitchType switchType=ON);
  ~Switch();
  void initialize(SwitchType switchType=ON);
  
  void MehrotraPredictor(Phase& phase);
  void display(FILE* fpout = stdout);

};

class AverageComplementarity
{
public:
  dd_real initial;
  dd_real current;
  AverageComplementarity(dd_real lambdaStar = 0.0);
  ~AverageComplementarity();
  void initialize(dd_real lambdaStar = 0.0);
  void initialize(Solutions& initPt);
  void update(Solutions& currentPt);
  void display(FILE* fpout = stdout);
};

class RatioInitResCurrentRes
{
public:
  dd_real primal;
  dd_real dual;
  
  RatioInitResCurrentRes();
  RatioInitResCurrentRes(Parameter& param, Residuals& initRes);
  ~RatioInitResCurrentRes();

  void initialize(Parameter& param, Residuals& initRes);

  void update(Switch& reduction, StepLength& alpha);
  void update_exact(Residuals& initRes, Residuals& currentRes);
  void display(FILE* fpout = stdout);
};

class SolveInfo
{
public:
  enum phaseType { noINFO,pFEAS,dFEAS,pdFEAS,pdINF,pFEAS_dINF,
		   pINF_dFEAS,pdOPT,pUNBD,dUNBD};

  dd_real rho;
  dd_real etaPrimal;
  dd_real etaDual;
  dd_real objValPrimal;
  dd_real objValDual;

  SolveInfo();
  SolveInfo(InputData& inputData, Solutions& currentPt, 
	    dd_real mu0, dd_real omegaStar);
  ~SolveInfo();

  void initialize(InputData& inputData, Solutions& currentPt, 
		  dd_real mu0, dd_real omegaStar);

  void update(InputData& inputData,
	      DenseLinearSpace& initPt_xMat, 
	      DenseLinearSpace& initPt_zMat, 
	      Solutions& currentPt,
	      Residuals& currentRes,
	      AverageComplementarity& mu,
	      RatioInitResCurrentRes& theta,
	      Parameter& param);
  // assume   initPt = lambda * I
  void update(double& lambda, 
	      InputData& inputData,
	      Solutions& currentPt,
	      Residuals& currentRes,
	      AverageComplementarity& mu,
	      RatioInitResCurrentRes& theta,
	      Parameter& param);
  // check mu, gap, feasibility   2007/08/27
  void check(InputData& inputData,
			 Solutions& currentPt,
			 Residuals& currentRes,
			 AverageComplementarity& mu,
			 RatioInitResCurrentRes& theta,
			 Parameter& param);
  void display(FILE* fpout = stdout);
};

class Phase
{
public:
  int nDim;
  SolveInfo::phaseType value;

  Phase();
  Phase(Residuals& initRes, SolveInfo& solveInfo,
	Parameter& param, int nDim);
  ~Phase();

  bool initialize(Residuals& initRes,
		  SolveInfo& solveInfo,
		  Parameter& param, int nDim);
  bool updateCheck(Residuals& currentRes,
		   SolveInfo& solveInfo,
		   Parameter& param);
  void reverse();
  void display(FILE* fpout = stdout);
};



}

#endif // __sdpa_parts_h__
