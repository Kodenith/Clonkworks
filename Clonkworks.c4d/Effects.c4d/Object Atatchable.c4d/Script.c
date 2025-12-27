/*-- Neues Script --*/

#strict 2

local Target;

public func ObjectAttachable(){ return(1); }

func DoAttachment(){
	SetXDir(0); SetYDir(0);
	if(!Target) Detachment();
	else{
		SetObjectOrder(this(),Target,1);
		if(OnFire(Target) || GetCon(Target) < 100) Detachment();
		if(GetCategory(Target) & C4D_Vehicle){
			SetX(GetX(Target));
			SetY(GetY(Target));
			SetR(GetR(Target));
		}else if(GetCategory(Target) & C4D_Structure){
			if(GetR(Target) != 0) Detachment();
			SetX(GetX(Target)-(GetDefWidth(GetID(Target))/3));
			SetY(GetY(Target)-(GetDefOffset(GetID(Target),1)+10));
			SetR(0);
		}
	}
	
	if(Contained(Target) && !Contained()) Enter(Contained(Target),this());
	if(!Contained(Target) && Contained()) Exit();
}

func Detachment(){
	Sound("Click");
	Split2Components();
}

func CanBeDetached(){ return(1); }
func OnDetach(){ Detachment(); }

global func CreateAttach(pObj,iPipe){
	if(!DefinitionCall(iPipe,"ObjectAttachable")) return(0);
	var Pipe = CreateObject(iPipe);
	LocalN("Target",Pipe) = pObj;
	return(Pipe);
}