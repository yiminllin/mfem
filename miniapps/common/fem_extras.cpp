// Copyright (c) 2010, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-443211. All Rights
// reserved. See file COPYRIGHT for details.
//
// This file is part of the MFEM library. For more information and source code
// availability see http://mfem.org.
//
// MFEM is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License (as published by the Free
// Software Foundation) version 2.1 dated February 1999.

#include "fem_extras.hpp"

using namespace std;

namespace mfem
{

namespace miniapps
{

H1_FESpace::H1_FESpace(Mesh *m,
                       const int p, const int space_dim, const int type,
                       int vdim, int order)
   : FiniteElementSpace(m, new H1_FECollection(p,space_dim,type),vdim,order)
{
   FEC_ = this->FiniteElementSpace::fec;
}

H1_FESpace::~H1_FESpace()
{
   delete FEC_;
}

ND_FESpace::ND_FESpace(Mesh *m, const int p, const int space_dim,
                       int vdim, int order)
   : FiniteElementSpace(m, new ND_FECollection(p,space_dim),vdim,order)
{
   FEC_ = this->FiniteElementSpace::fec;
}

ND_FESpace::~ND_FESpace()
{
   delete FEC_;
}

RT_FESpace::RT_FESpace(Mesh *m, const int p, const int space_dim,
                       int vdim, int order)
   : FiniteElementSpace(m, new RT_FECollection(p-1,space_dim),vdim,order)
{
   FEC_ = this->FiniteElementSpace::fec;
}

RT_FESpace::~RT_FESpace()
{
   delete FEC_;
}

L2_FESpace::L2_FESpace(Mesh *m, const int p, const int space_dim,
                       int vdim, int order)
   : FiniteElementSpace(m, new L2_FECollection(p,space_dim),vdim,order)
{
   FEC_ = this->FiniteElementSpace::fec;
}

L2_FESpace::~L2_FESpace()
{
   delete FEC_;
}

DiscreteInterpolationOperator::~DiscreteInterpolationOperator()
{}

DiscreteGradOperator::DiscreteGradOperator(FiniteElementSpace *dfes,
                                           FiniteElementSpace *rfes)
   : DiscreteInterpolationOperator(dfes, rfes)
{
   this->AddDomainInterpolator(new GradientInterpolator);
}

DiscreteCurlOperator::DiscreteCurlOperator(FiniteElementSpace *dfes,
                                           FiniteElementSpace *rfes)
   : DiscreteInterpolationOperator(dfes, rfes)
{
   this->AddDomainInterpolator(new CurlInterpolator);
}

DiscreteDivOperator::DiscreteDivOperator(FiniteElementSpace *dfes,
                                         FiniteElementSpace *rfes)
   : DiscreteInterpolationOperator(dfes, rfes)
{
   this->AddDomainInterpolator(new DivergenceInterpolator);
}

void VisualizeField(socketstream &sock, const char *vishost, int visport,
                    GridFunction &gf, const char *title,
                    int x, int y, int w, int h, bool vec)
{
   Mesh &mesh = *gf.FESpace()->GetMesh();

   bool newly_opened = false;
   int connection_failed;

   do
   {
      if (!sock.is_open() || !sock)
      {
         sock.open(vishost, visport);
         sock.precision(8);
         newly_opened = true;
      }
      sock << "solution\n";

      mesh.Print(sock);
      gf.Save(sock);

      if (newly_opened)
      {
         sock << "window_title '" << title << "'\n"
              << "window_geometry "
              << x << " " << y << " " << w << " " << h << "\n"
              << "keys maaAc";
         if ( vec ) { sock << "vvv"; }
         sock << endl;
      }

      connection_failed = !sock && !newly_opened;
   }
   while (connection_failed);
}

} // namespace miniapps

} // namespace mfem
