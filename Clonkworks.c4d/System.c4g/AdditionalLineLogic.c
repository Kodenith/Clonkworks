#strict 2

#appendto SPIP
#appendto DPIP
#appendto PWRL
#appendto GERL

public func isLine(){ return(1); }
public func KitType(){
	if(GetID() == SPIP || GetID() == DPIP || GetID() == PWRL){
		return(LNKT);
	}else{
		return(_inherited());
	}
}

public func isOnLineTarget(pObj){
	var Targ = FindObject2(Find_NoContainer(),Find_OCF(OCF_LineConstruct),Find_AtPoint(AbsX(GetX(pObj)),AbsY(GetY(pObj))));
	if(!Targ) return(0);
	if(GetActionTarget(0) == Targ) return(1);
	if(GetActionTarget(1) == Targ) return(1);
	
	return(0);
}