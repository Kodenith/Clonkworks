/*-- Neues Script --*/

#strict 2
local Grabtarg;
func MoveGrabbed(){
	var Grabbed = Grabtarg;
	if(!Grabbed || Contained(Grabbed)){
		SetAction("Moving");
		Sound("Click");
		EndAllCommands();
		return(1);
	}
	SetPosition(GetX()+GetVertex(0,0),GetY()+GetVertex(0,1),Grabbed);
	SetXDir(0,Grabbed);
	SetYDir(0,Grabbed);
}

func EndAllCommands(){
	FinishCommand(this(),0,4);
	FinishCommand(this(),0,3);
	FinishCommand(this(),0,2);
	FinishCommand(this(),0,1);
	FinishCommand(this(),0,0);
}

func Grab(){
	var iItem = Grabtarg;
	if(!iItem) return(0);
	if(GetAction() == "Idle") return(0);
	if(ObjectDistance(this(),iItem) <= 20 && !ObjectOnClaw(Grabtarg)){
		Sound("Click");
		SetAction("Grab");
		return(1);
	}else{
		Grabtarg = 0;
		EndAllCommands();
		return(0);
	}
}

func Release(){
	if(GetAction() != "Idle"){
		if(GetAction() != "Moving") Sound("Click");
		SetAction("Moving");
		if(Stuck(Grabtarg)){
		SetX(GetX(),Grabtarg);
		SetY(GetY(),Grabtarg);
		}
		Grabtarg = 0;
		return(1);
	}
}

func GrabAndDropOff(iItem,dX,dY){
	Grabtarg = iItem;
	AddCommand(this(),"Call",this(),0,0,0,38*6,"Release");
	AddCommand(this(),"MoveTo",0,dX,dY,0,38*6);
	AddCommand(this(),"Call",this(),0,0,0,0,"Grab");
	AddCommand(this(),"MoveTo",iItem,0,0,0,38*6);
}

global func ObjectOnClaw(pObj){
	for(var i in FindObjects(Find_ID(MAM2))){
		if(GetAction(i) == "Grab" && LocalN("Grabtarg",i) == pObj) return(1);
	}
}