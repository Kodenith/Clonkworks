/*-- Neues Script --*/

#strict 2
#appendto *

public func GetResearchBase() {
	if(!_inherited() && this()->~IsAdvancedProduct()) return(ADVW);
	return(_inherited());
}