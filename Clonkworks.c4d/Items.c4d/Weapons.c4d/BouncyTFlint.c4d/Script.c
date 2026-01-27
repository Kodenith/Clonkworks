/*-- T-Flint --*/

#strict 2
#include TFLN
#include RUBB

public func MaxBounces(){ return(5); }

protected func Hit() {
  Sound("RockHit*");
  if(!ActIdle()) return(_inherited(Par(0),Par(1)));
  Sound("Fuse");
  SetAction("Activated");
  // Picture anpassen
  SetPicture(0, 12 + 64, 64, 64);
  return(_inherited(Par(0),Par(1)));
}

/* Forschung */

public func GetResearchBase() { return TFLN; }
public func IsMixerProduct(){ return(0); }
