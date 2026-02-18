/*-- Gold --*/

#strict

/* Aufschlag */

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

/* Für Goldmine */

public func Sale(int plr) {
  var obj;
  if (obj = FindObject2(Find_ID(GLDM)))
    obj->PlayerHasSold(plr, this ());
  return(GetID());
}

public func Purchase(int plr, object buyobj) {
  var obj;
  if (obj = FindObject2(Find_ID(GLDM)))
    obj->PlayerHasBought(plr, this ());
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(180); }
