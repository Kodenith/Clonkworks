/*-- Tera-Flint --*/

#strict

#include FLNT
/* Aufschlag */

public func ExplodeSize() { return(18); }

/* Forschung */

public func GetResearchBase() { return(CFL1); }

protected func Hit()
{
  Explode(ExplodeSize());
  for(var x = 0; x < 20; x++){
    var Cluster = CreateObject(CLUS);
    SetXDir(RandomX(-20,20),Cluster);
    SetYDir(RandomX(-2,-50),Cluster);
  }
}