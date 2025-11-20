#strict 2
#appendto *

/* public func IsNotWeapon(){
	return(_inherited());
} */
// that can just be defined within the object ^^^

func QueryCatchBlow(pByObject){
	if(pByObject->~IsNotWeapon()){
		return(1);
	}
	
	return(_inherited(Par()));
}

func CatchBlow(int iLevel, object pBy){
	pBy->~OnBlowDealt(iLevel,this());
	return(_inherited(iLevel,pBy));
}
