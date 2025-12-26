/*-- Neues Script --*/

#strict 2
#appendto *

public func GetResearchBase() {
	if(!_inherited() && DefinitionCall(GetID(),"IsAdvancedProduct")) return(ADVW);
	return(_inherited());
}