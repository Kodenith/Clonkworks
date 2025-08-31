/*-- Neues Script --*/

#strict 2
#appendto CLNK

public func GetMaxSpecialCount(iIndex, fAmount)
{
  if(iIndex == 0) { if(fAmount) return(15); return("IsHoney"); }
}

func Activate(){
	//Detaching custom line kits
	
	//no holding stuff
	if(FindObject2(Find_Container(this()))) return(_inherited(Par()));
	
	// -Hopper Pipes-
	//find all pipes and check if youre near a thing with one attached
	for(var ln in FindObjects(Find_ID(DEPP))){
		//check from
		if(ObjectDistance(this(), LocalN("from", ln)) < 20){
			if(GetID(LocalN("to", ln)) == FNKT){
				Sound("Error");
				return(0);
			}
			var from = LocalN("from", ln);
			var NewKit = CreateObject(FNKT);
			LocalN("from", ln) = NewKit;
			Sound("Connect");
			return(0);
		}else if(ObjectDistance(this(), LocalN("to", ln)) < 20){
			//check to
			if(GetID(LocalN("from", ln)) == FNKT){
				Sound("Error");
				return(0);
			}
			var to = LocalN("to", ln);
			var NewKit = CreateObject(FNKT);
			LocalN("to", ln) = NewKit;
			Sound("Connect");
			return(0);
		}
	}
	
	return(_inherited(Par()));
}
