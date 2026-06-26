/*-- Tera-Flint --*/

#strict

#include FLNT
/* Aufschlag */

public func ExplodeSize() { return((18/3)*2); }

/* Forschung */

public func GetResearchBase() { return(FLNT); }

protected func Hit()
{
  Explode(ExplodeSize());
  for(var x = 0; x < 12; x++){
    var Cluster = CreateObject(CLUS);
    SetXDir(RandomX(-15,15),Cluster);
    SetYDir(RandomX(-2,-30),Cluster);
  }
}