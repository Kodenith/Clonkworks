/*-- Sprengpfeilpaket --*/

#strict

#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(TARW); }

public func GetResearchBase() { return(ARWP); }
public func IsAdvancedProduct(){ return(1); }

func Entrance(pContainer){
	SetOwner(GetController(pContainer));
	return(_inherited(pContainer));
}