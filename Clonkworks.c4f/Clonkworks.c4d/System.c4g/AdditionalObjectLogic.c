#strict 2
#appendto *

public func IsNotWeapon(){
	return(_inherited());
}

public func IsKit(){
	if(GetID() == LNKT || GetID() == CNKT || GetID() == FNKT) return(1);
	else return(_inherited());
}

func QueryCatchBlow(pByObject){
	if(pByObject->IsNotWeapon()){
		return(1);
	}
	
	return(_inherited(Par()));
}
