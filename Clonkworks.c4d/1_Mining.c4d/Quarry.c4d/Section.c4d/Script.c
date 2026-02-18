/*-- Neues Script --*/

#strict
local Support;

func Initialize() {
  return(1);
}

func AttachTargetLost(){
	RemoveObject();
}

func Destruction(){
	if(Support && !OnFire(Support)) RemoveObject(Support);
	CastParticles("PxSpark",RandomX(3,7),25,0,0,25,60,RGBa(255,255,0),RGBa(255,155,0));
	if(!OnFire()) Sound("Connect");
	//CreateObject(CPIG);
}

func Incineration(){
	//burn something attached to this
	var find;
	if(find = FindObject2(Find_ActionTarget(this()),Find_Exclude(this()))){
		Incinerate(find);
	}
	
	if(Support && !OnFire(Support)) Incinerate(Support);
	
	Incinerate(CreateObject(QUA4));
	RemoveObject();
}