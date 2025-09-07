/*-- B-Flint --*/

#strict 2
#include TFLN

public func ExplodeSize() {
  return 23;
}

protected func Hit() {
  Sound("RockHit*");
  return true;
}

func Incineration(){
  Sound("Fuse");
  SetAction("Activated");
  SetPicture(0, 12 + 64, 64, 64);
}

public func Activate()
{
	return false;
}

/* Forschung */

public func GetResearchBase() { return TFLN; }
