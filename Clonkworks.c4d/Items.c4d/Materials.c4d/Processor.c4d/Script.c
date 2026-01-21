/*-- Neues Script --*/

#strict

func Hit () {
  if(!Random(3)) Sound("MetalHit*");
}

func IsAnvilProduct(){ return(1); }
func IsAdvancedProduct(){ return(1); }
public func GetResearchBase(){ return(CNVY); }