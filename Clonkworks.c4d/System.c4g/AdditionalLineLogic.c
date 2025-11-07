#strict 2

#appendto SPIP
#appendto DPIP
#appendto PWRL
#appendto FNPP
#appendto HOSE
#appendto TRPW

private func Transfer(){
	if(!ObjectCount(RSWR)) 	return(inherited());
	if(this()->~CustomLengthLimit()) return(inherited());
	var from, to;
	from = GetActionTarget(0);
	to = GetActionTarget(1);
	
	var Distance;
	Distance = 500;
	//pipes have a bigger range, so they can be used without pain.
	if(GetID() != PWRL) Distance = 1200;
	
	if(ObjectDistance(from,to) > Distance){
		LineBreak(false);
		RemoveObject(this());
	}
	
	return(inherited());
}

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