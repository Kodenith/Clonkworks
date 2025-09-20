/*-- Neues Script --*/

#strict 2
#appendto CLNK

public func GetMaxSpecialCount(iIndex, fAmount)
{
  if(iIndex == 0) { if(fAmount) return(15); return("IsHoney"); }
  if(iIndex == 1) { if(fAmount) return(1); return("IsExtraItem"); }
}

func ControlDigDouble(){
	//disconnecting the tag from objects.
	var ovrlp = FindObject2( Find_Exclude(this()), Find_AtPoint(0,5), Find_Or( Find_Category(C4D_Structure), Find_Category(C4D_Vehicle), Find_Category(C4D_Living)), Find_Not(Find_OCF(OCF_CrewMember)), Find_NoContainer());
	if(GetOCF(ovrlp) & OCF_Container){ //foo
	}
	else{
	if(!ContentsCount() && FindObject2(Find_ID(TAG_),Find_Container(ovrlp))){
		Enter(this(), FindObject2(Find_ID(TAG_),Find_Container(ovrlp)));
		Sound("Click");
		return(1);
	}
	}
	
	// disconnecting custom lines and getting their proper kits.
	ovrlp = FindObject(0, 1,0,0,0, OCF_LineConstruct, 0,0,0, ovrlp);
	if(!ovrlp) return(_inherited());
	SetComDir(COMD_Stop);
	
	var Lines = FindObjects(Find_Action("Connect"), Find_Func("isLine"));
	for(var Line in Lines){
		var from = GetActionTarget(0, Line);
		var to = GetActionTarget(1, Line);
		
		if(from == ovrlp){
			if(ContentsCount()) return(0);
			if(GetID(to) == Line->KitType()){
				Sound("Error");
				return(1);
			}
			Sound("Connect");
			var Kit = CreateObject(Line->KitType());
			SetActionTargets(Kit, to, Line);
			return(1);
		}
		
		if(to == ovrlp){
			if(ContentsCount()) return(0);
			if(GetID(from) == Line->KitType()){
				Sound("Error");
				return(1);
			}
			Sound("Connect");
			var Kit = CreateObject(Line->KitType());
			SetActionTargets(from, Kit, Line);
			return(1);
		}
	}
	
	return(_inherited());
}
