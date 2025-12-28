/*-- Neues Script --*/

#strict 2
#include ACH1
#include FLTG

private func BasementID() { return(0); }

func DoAttachment(){
	//if(basement) RemoveObject(basement);
	if(!Filter) Filter = 1;
	SetFilterGraph();
	if(Target && !FindObject2(Find_Action("Push"), Find_ActionTarget(this())) ){
		var i,c;
		while(c = Contents(i++,Target,0)){
			if( (!FilterPath && FilterCompatibleItem(c)) || (FilterPath && !FilterCompatibleItem(c)) ){
				Exit(c);
				SetX(GetX(),c);
				SetY(GetY()+4,c);
				return(_inherited());
			}
		}
	}
	return(_inherited());
}

func ControlThrow(pObj){
	[$TxtTurn$|Image=SG01:1]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	Sound("SignalClick");
	if(!FilterPath){
		FilterPath = true;
		Message("$TxtTurnRight$",this());
	}else{
		FilterPath = false;
		Message("$TxtTurnLeft$",this());
	}
}

func ControlRight(){
	//foo but also not foo because if you delete this bad stuff happen
}