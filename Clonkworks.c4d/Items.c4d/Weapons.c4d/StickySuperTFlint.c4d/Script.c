/*-- Super-T-Flint --*/

#strict

#include TFLS

public func ExplodeSize() { return(27); }

/* Forschung */

public func GetResearchBase() { return(STFN); }
public func IsChemicalProduct(){ return(GetID() != STFS); }