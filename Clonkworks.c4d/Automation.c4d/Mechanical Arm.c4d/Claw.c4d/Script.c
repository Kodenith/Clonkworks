/*-- Neues Script --*/

#strict 2
local Grabtarg;
func MoveGrabbed(){
	var Grabbed = Grabtarg;
	if(!Grabbed || Contained(Grabbed)){
		SetAction("Moving");
		Sound("Click");
		FinishCommand();
		return(1);
	}
	SetPosition(GetX()+GetVertex(0,0),GetY()+GetVertex(0,1),Grabbed);
	SetXDir(0,Grabbed);
	SetYDir(0,Grabbed);
}

func Grab(){
	var iItem = Grabtarg;
	if(!iItem) return(0);
	if(GetAction() == "Idle") return(0);
	if(ObjectDistance(this(),iItem) <= 20){
		Sound("Click");
		SetAction("Grab");
		return(1);
	}else{
		Grabtarg = 0;
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