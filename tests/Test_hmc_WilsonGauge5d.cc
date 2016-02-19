    /*************************************************************************************

    Grid physics library, www.github.com/paboyle/Grid 

    Source file: ./tests/Test_hmc_WilsonGauge.cc

    Copyright (C) 2015

Author: Peter Boyle <paboyle@ph.ed.ac.uk>
Author: neo <cossu@post.kek.jp>
Author: paboyle <paboyle@ph.ed.ac.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    See the full license in the file "LICENSE" in the top level distribution directory
    *************************************************************************************/
    /*  END LEGAL */
    
// Modified version by Evan Weinberg (evansweinberg@gmail.com) to test
// Nd=5. Based on Test_hmc_WilsonGauge.cc.
#include "Grid.h"

using namespace std;
using namespace Grid;
using namespace Grid::QCD;

namespace Grid { 
  namespace QCD { 


class HmcRunner : public NerscHmcRunner {
public:

  void BuildTheAction (int argc, char **argv)

  {
    typedef WilsonImplR ImplPolicy;
    typedef WilsonFermionR FermionAction;
    typedef typename FermionAction::FermionField FermionField;
    typedef std::vector<int>::iterator vintIter;

    std::vector<int> latt_size   = GridDefaultLatt();
    std::vector<int> simd_layout = GridDefaultSimd(Nd,vComplex::Nsimd());
    std::vector<int> mpi_layout  = GridDefaultMpi();
    
    // Print out the defaults, may be non-trivial with Nd=5
    std::cout << "Lattice Size (len " << latt_size.size() << "):";
    for (vintIter it = latt_size.begin(); it != latt_size.end(); ++it)
    {
      std:: cout << " " << *it;
    }
    std::cout << "\n";
    
    std::cout << " Simd Layout (len " << simd_layout.size() << "):";
    for (vintIter it = simd_layout.begin(); it != simd_layout.end(); ++it)
    {
      std:: cout << " " << *it;
    }
    std::cout << "\n";
    
    std::cout << "  MPI Layout (len " << mpi_layout.size() << "):";
    for (vintIter it = mpi_layout.begin(); it != mpi_layout.end(); ++it)
    {
      std:: cout << " " << *it;
    }
    std::cout << "\n";
    
    // The "Four" in the class name doesn't matter---it takes
    // in principle an arbitrarily sized vector, as long as
    // the args are consistent. 
    UGrid   = SpaceTimeGrid::makeFourDimGrid(latt_size, simd_layout,mpi_layout);
    UrbGrid = SpaceTimeGrid::makeFourDimRedBlackGrid(UGrid);
  
    FGrid   = UGrid;
    FrbGrid = UrbGrid;

    // temporarily need a gauge field
    LatticeGaugeField  U(UGrid);

    // Gauge action
    WilsonGaugeActionR Waction(5.6);

    //Collect actions
    ActionLevel<LatticeGaugeField> Level1(1);
    Level1.push_back(&Waction);
    TheAction.push_back(Level1);

    Run(argc,argv);
  };

};

}}

int main (int argc, char ** argv)
{
  Grid_init(&argc,&argv);

  int threads = GridThread::GetThreads();
  std::cout<<GridLogMessage << "Grid is setup to use "<<threads<<" threads"<<std::endl;

  HmcRunner TheHMC;
  
  TheHMC.BuildTheAction(argc,argv);

}

