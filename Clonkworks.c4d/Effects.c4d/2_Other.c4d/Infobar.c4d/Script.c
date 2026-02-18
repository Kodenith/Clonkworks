/*-- Neues Script --*/

#strict 2

static const IB_Grab = 1; //when an object is grabbed
static const IB_Contained = 2; //when the clonk is inside something
static const IB_Container = 4; //when the clonk holds something
static const IB_Ride = 8; //When the clonk is riding something
static const IB_Exist = 16; //if none of the above are triggered, this is triggered in the plauer's currently controlled clonk.

local iPlr;
local Value;
local Max;

func Initialize(){
	iPlr = GetOwner();
	Local(0) = 0;
	Local(1) = 0;
	SetAction("Progress");
	SetVisibility(VIS_Owner,this());
	SetPosition(200,-50);
}

func Update(){
	if(GetOwner() != iPlr || GetOwner() == -1) RemoveObject();
	
	var Current = GetCursor(iPlr);
	if(!Current){
		DebugLog("No Cursor!");
		SetVisibility(VIS_None,this());
		return(0);
	}
	
	var Selection;
	if(GetProcedure(Current) == "PUSH" && (GetActionTarget(0,Current) && (GetActionTarget(0,Current)->~InfobarTrigger() & IB_Grab)) ){
		Selection = GetActionTarget(0,Current);
	}else if(Contained(Current) && (Contained(Current)->~InfobarTrigger() & IB_Contained)){
		Selection = Contained(Current);
	}else if(Contents(0,Current) && (Contents(0,Current)->~InfobarTrigger() & IB_Container)){
		Selection = Contents(0,Current);
	}else if(GetProcedure(Current) == "ATTACH" && (GetActionTarget(0,Current)->~InfobarTrigger() & IB_Ride)){
		Selection = GetActionTarget(0,Current);
	}else if(Current->~InfobarTrigger() & IB_Exist){
		Selection = Current;
	}
	
	if(!Selection){
		SetVisibility(VIS_None,this());
		CustomMessage("",this(),GetOwner());
		return(0);
	}
	SetVisibility(VIS_Owner,this());
	
	Max = Selection->~InfobarMax();
	Value = Selection->~InfobarValue();
	var Phase;
	if(Max){
		Phase = (Value * 92) / Max;
		Phase=BoundBy(Phase,0,92);
		//DebugLog("%d/%d - Phase %d",Value,Max,Phase);
	}
	else{
		Phase = 0;
	}
	
	SetPhase(Phase);
	
	//Customization :D
	if(Selection->~InfobarColor()){
		SetColorDw(Selection->~InfobarColor());
	}else{
		SetColorDw(GetPlrColorDw(GetOwner()));
	}
	
	if(Selection->~InfobarInfo()){
		CustomMessage(Selection->~InfobarInfo(),this(),GetOwner());
	}
}