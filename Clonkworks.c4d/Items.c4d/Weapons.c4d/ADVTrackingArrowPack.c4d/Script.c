/*-- Sprengpfeilpaket --*/

#strict

#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return(ATRW); }

public func GetResearchBase() { return(TARP); }
public func IsAdvancedProduct(){ return(1); }

func Entrance(pContainer){
	SetOwner(GetOwner(pContainer));
	return(_inherited(pContainer));
}