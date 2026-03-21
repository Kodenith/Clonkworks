/*-- Säurefass --*/

#strict

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Glue")); }
public func BarrelMaterialName() { return("Glue"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (iMaterial == Material("Glue")) return(GLUE);
  return(_inherited(iMaterial));
}

func IsChemicalProduct(){
  return(1);
}
func IsAnvilProduct(){
  return(0);
}
