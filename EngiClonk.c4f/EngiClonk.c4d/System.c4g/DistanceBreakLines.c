//this script handles distance-based line breaks with the "Short Lines" rule enabled
#strict

#appendto SPIP
#appendto DPIP
#appendto PWRL

private func Transfer(){
	if(!ObjectCount(RSWR)) 	return(inherited());
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
