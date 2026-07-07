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
  for(var x = 0; x < 10; x++){
    var Cluster = CreateObject(CLUS);
    SetXDir(RandomX(-20,20),Cluster);
    SetYDir(RandomX(-20,-30),Cluster);
  }
}