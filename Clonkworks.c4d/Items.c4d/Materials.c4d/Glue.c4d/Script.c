/*-- Neues Script --*/

#strict

func Initialize(){
  if(GetID(Contained()) == CHEM || GetID(Contained()) == CHM2) Enter(Contained(),CreateObject(BARL));
}

protected func Hit()
{
  Sound("WoodHit*");
  return(1);
}

func IsChemicalProduct(){ return(1); }
func GetResearchBase(){ return(BARL); }